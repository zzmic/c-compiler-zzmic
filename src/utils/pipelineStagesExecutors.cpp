#include "pipelineStagesExecutors.h"
#include "../backend/assembly.h"
#include "../backend/assemblyGenerator.h"
#include "../backend/backendSymbolTable.h"
#include "../backend/fixupPass.h"
#include "../backend/pseudoToStackPass.h"
#include "../frontend/frontendSymbolTable.h"
#include "../frontend/lexer.h"
#include "../frontend/parser.h"
#include "../frontend/printVisitor.h"
#include "../frontend/program.h"
#include "../frontend/semanticAnalysisPasses.h"
#include "../midend/ir.h"
#include "../midend/irGenerator.h"
#include "../midend/irOptimizationPasses.h"
#include "../utils/prettyPrinters.h"
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

std::vector<Token>
PipelineStagesExecutors::lexerExecutor(std::string_view sourceFileName) {
    std::ifstream sourceFileInputStream(std::string{sourceFileName});
    if (sourceFileInputStream.fail()) {
        std::stringstream msg;
        msg << "Unable to open source file: " << sourceFileName;
        throw std::ios_base::failure(msg.str());
    }

    // Read the entire source file into a string.
    // The first iterator is an iterator that reads characters from the input
    // stream.
    // The second iterator is the (default-constructed) end-of-stream iterator.
    // The string range-constructor reads characters from the input stream until
    // the end-of-stream iterator is reached.
    auto input =
        std::string((std::istreambuf_iterator<char>(sourceFileInputStream)),
                    std::istreambuf_iterator<char>());
    sourceFileInputStream.close();

    std::vector<Token> tokens;
    try {
        tokens = lexer(input);
        printTokens(tokens);
    } catch (const std::exception &e) {
        std::stringstream msg;
        msg << "Lexical error in lexerExecutor in PipelineStagesExecutors: "
            << e.what();
        throw std::runtime_error(msg.str());
    }

    return tokens;
}

std::unique_ptr<AST::Program>
PipelineStagesExecutors::parserExecutor(const std::vector<Token> &tokens) {
    std::unique_ptr<AST::Program> program;
    try {
        AST::Parser parser(tokens);
        // Parse the tokens to generate the AST program.
        program = parser.parse();
        AST::PrintVisitor printVisitor;
        std::cout << "\n";
        // Visit and print the AST program after parsing.
        program->accept(printVisitor);
    } catch (const std::exception &e) {
        std::stringstream msg;
        msg << "Parsing error in parserExecutor in PipelineStagesExecutors: "
            << e.what();
        throw std::runtime_error(msg.str());
    }

    return program;
}

int PipelineStagesExecutors::semanticAnalysisExecutor(
    AST::Program &astProgram, AST::FrontendSymbolTable &frontendSymbolTable) {
    AST::IdentifierResolutionPass IdentifierResolutionPass;
    AST::TypeCheckingPass typeCheckingPass(frontendSymbolTable);
    AST::LoopLabelingPass loopLabelingPass;
    auto variableResolutionCounter = 0;

    try {
        // Perform the identifier-resolution pass.
        variableResolutionCounter =
            IdentifierResolutionPass.resolveProgram(astProgram);
    } catch (const std::exception &e) {
        std::stringstream msg;
        msg << "Identifier resolution error in semanticAnalysisExecutor in "
               "PipelineStagesExecutors: "
            << e.what();
        throw std::runtime_error(msg.str());
    }
    try {
        // Perform the type-checking pass.
        typeCheckingPass.typeCheckProgram(astProgram);
    } catch (const std::exception &e) {
        std::stringstream msg;
        msg << "Type-checking error in semanticAnalysisExecutor in "
               "PipelineStagesExecutors: "
            << e.what();
        throw std::runtime_error(msg.str());
    }
    try {
        // Perform the loop-labeling pass.
        loopLabelingPass.labelLoops(astProgram);
    } catch (const std::exception &e) {
        std::stringstream msg;
        msg << "Loop-labeling error in semanticAnalysisExecutor in "
               "PipelineStagesExecutors: "
            << e.what();
        throw std::runtime_error(msg.str());
    }
    try {
        // Visit and print the AST program after semantic analysis.
        AST::PrintVisitor printVisitor;
        std::cout << "\n";
        astProgram.accept(printVisitor);
    } catch (const std::exception &e) {
        std::stringstream msg;
        msg << "Printing AST error (in semantic analysis) in "
               "semanticAnalysisExecutor in PipelineStagesExecutors: "
            << e.what();
        throw std::runtime_error(msg.str());
    }

    return variableResolutionCounter;
}

std::pair<std::unique_ptr<IR::Program>,
          std::unique_ptr<std::vector<std::unique_ptr<IR::StaticVariable>>>>
PipelineStagesExecutors::irGeneratorExecutor(
    const AST::Program &astProgram, int variableResolutionCounter,
    AST::FrontendSymbolTable &frontendSymbolTable) {
    std::cout << "\n";
    std::pair<std::unique_ptr<IR::Program>,
              std::unique_ptr<std::vector<std::unique_ptr<IR::StaticVariable>>>>
        irProgramAndIRStaticVariables;
    try {
        IR::IRGenerator irGenerator(variableResolutionCounter,
                                    frontendSymbolTable);
        // Generate the IR program from the AST program.
        irProgramAndIRStaticVariables = irGenerator.generateIR(astProgram);
    } catch (const std::exception &e) {
        std::stringstream msg;
        msg << "IR generation error in irGeneratorExecutor in "
               "PipelineStagesExecutors: "
            << e.what();
        throw std::runtime_error(msg.str());
    }
    return irProgramAndIRStaticVariables;
}

void PipelineStagesExecutors::irOptimizationExecutor(
    IR::Program &irProgram, bool foldConstantsPass, bool propagateCopiesPass,
    bool eliminateUnreachableCodePass, bool eliminateDeadStoresPass) {
    for (auto &topLevel : irProgram.getTopLevels()) {
        if (auto *functionDefinition =
                dynamic_cast<IR::FunctionDefinition *>(topLevel.get())) {
            // Extract the function body from the function definition, optimize
            // the function body, and set the optimized function body back to
            // the function definition.
            const auto &functionBody = functionDefinition->getFunctionBody();
            auto optimizedFunctionBody = IR::IROptimizer::irOptimize(
                functionBody, foldConstantsPass, propagateCopiesPass,
                eliminateUnreachableCodePass, eliminateDeadStoresPass);
            functionDefinition->setFunctionBody(
                std::move(optimizedFunctionBody));
        }
    }
}

std::unique_ptr<Assembly::Program> PipelineStagesExecutors::codegenExecutor(
    const IR::Program &irProgram,
    const std::vector<std::unique_ptr<IR::StaticVariable>> &irStaticVariables,
    const AST::FrontendSymbolTable &frontendSymbolTable) {
    std::unique_ptr<Assembly::Program> assemblyProgram;
    try {
        // Convert the frontend symbol table to backend symbol table before
        // assembly generation so all variables (including temporaries) are
        // available.
        Assembly::BackendSymbolTable backendSymbolTable;
        Assembly::convertFrontendToBackendSymbolTable(frontendSymbolTable,
                                                      backendSymbolTable);

        Assembly::AssemblyGenerator assemblyGenerator(irStaticVariables,
                                                      frontendSymbolTable);
        // Generate the assembly program from the IR program.
        assemblyProgram = assemblyGenerator.generateAssembly(irProgram);

        Assembly::PseudoToStackPass pseudoToStackPass;
        // Associate the stack size with each top-level element.
        auto &topLevels = assemblyProgram->getTopLevels();
        pseudoToStackPass.replacePseudoWithStackAndAssociateStackSize(
            topLevels, backendSymbolTable);

        // Fix up the assembly program.
        Assembly::FixupPass::fixup(topLevels);
    } catch (const std::exception &e) {
        std::stringstream msg;
        msg << "Code generation error in codegenExecutor in "
               "PipelineStagesExecutors: "
            << e.what();
        throw std::runtime_error(msg.str());
    }

    // Finally, return the assembly program.
    return assemblyProgram;
}

void PipelineStagesExecutors::codeEmissionExecutor(
    const Assembly::Program &assemblyProgram,
    std::string_view assemblyFileName) {
    std::ofstream assemblyFileStream(std::string{assemblyFileName});
    if (!assemblyFileStream.is_open()) {
        std::stringstream msg;
        msg << "Error: Unable to open output file " << assemblyFileName
            << " in codeEmissionExecutor in PipelineStagesExecutors";
        throw std::ios_base::failure(msg.str());
    }

    // Emit the assembly program to the assembly file stream. The emission
    // itself lives in `PrettyPrinters` so that emitting to a file and printing
    // to stdout share one implementation.
    PrettyPrinters::emitAssemblyProgram(assemblyProgram, assemblyFileStream);

    assemblyFileStream.close();
}
