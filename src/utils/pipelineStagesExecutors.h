#ifndef UTILS_PIPELINE_STAGES_EXECUTORS_H
#define UTILS_PIPELINE_STAGES_EXECUTORS_H

#include "../backend/assembly.h"
#include "../frontend/frontendSymbolTable.h"
#include "../frontend/lexer.h"
#include "../frontend/program.h"
#include "../midend/ir.h"
#include <memory>
#include <string_view>
#include <utility>
#include <vector>

/**
 * Class containing static executor methods for each pipeline stage.
 */
class PipelineStagesExecutors {
  public:
    /**
     * Perform lexical analysis on the source file and generate the list of
     * tokens.
     *
     * @param sourceFile The input source file.
     * @return The list of tokens generated from lexing.
     */
    [[nodiscard]] static std::vector<Token>
    lexerExecutor(std::string_view sourceFile);

    /**
     * Perform syntactic analysis on the list of tokens and generate the AST
     * program.
     *
     * @param tokens The list of tokens to parse.
     * @return The AST program generated from parsing.
     */
    [[nodiscard]] static std::unique_ptr<AST::Program>
    parserExecutor(const std::vector<Token> &tokens);

    /**
     * Perform semantic-analysis passes on the AST program.
     *
     * @param astProgram The AST program to analyze.
     * @param frontendSymbolTable The frontend symbol table.
     * @return An integer counter for variable resolution.
     */
    [[nodiscard]] static int
    semanticAnalysisExecutor(AST::Program &astProgram,
                             AST::FrontendSymbolTable &frontendSymbolTable);

    /**
     * Generate (but not yet emit) the IR from the AST program.
     *
     * @param astProgram The AST program to convert to IR.
     * @param variableResolutionCounter An integer counter for variable
     * resolution.
     * @param frontendSymbolTable The frontend symbol table.
     * @return A pair consisting of the IR program and the vector of static
     * variables in IR.
     */
    [[nodiscard]] static std::pair<
        std::unique_ptr<IR::Program>,
        std::unique_ptr<std::vector<std::unique_ptr<IR::StaticVariable>>>>
    irGeneratorExecutor(const AST::Program &astProgram,
                        int variableResolutionCounter,
                        AST::FrontendSymbolTable &frontendSymbolTable);

    /**
     * Perform optimization passes on the IR program.
     *
     * @param irProgram The IR program to optimize.
     * @param foldConstantsPass Whether to perform the constant-folding pass.
     * @param propagateCopiesPass Whether to perform the copy-propagation pass.
     * @param eliminateUnreachableCodePass Whether to perform the
     * unreachable-code elimination pass.
     * @param eliminateDeadStoresPass Whether to perform the dead-store
     * elimination pass.
     */
    static void irOptimizationExecutor(IR::Program &irProgram,
                                       bool foldConstantsPass,
                                       bool propagateCopiesPass,
                                       bool eliminateUnreachableCodePass,
                                       bool eliminateDeadStoresPass);

    /**
     * Generate (but not yet emit) the assembly program from the IR program.
     *
     * @param irProgram The IR program to convert to assembly.
     * @param irStaticVariables A vector of static variables in IR.
     * @param frontendSymbolTable The frontend symbol table.
     * @return The assembly program generated from the IR.
     */
    [[nodiscard]] static std::unique_ptr<Assembly::Program>
    codegenExecutor(const IR::Program &irProgram,
                    const std::vector<std::unique_ptr<IR::StaticVariable>>
                        &irStaticVariables,
                    const AST::FrontendSymbolTable &frontendSymbolTable);

    /**
     * Emit the generated assembly code to the assembly file.
     *
     * @param assemblyProgram The assembly program to emit.
     * @param assemblyFile The output assembly file.
     */
    static void codeEmissionExecutor(const Assembly::Program &assemblyProgram,
                                     std::string_view assemblyFile);
};

#endif // UTILS_PIPELINE_STAGES_EXECUTORS_H
