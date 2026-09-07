#ifndef UTILS_PRETTY_PRINTERS_H
#define UTILS_PRETTY_PRINTERS_H

#include "../backend/assembly.h"
#include "../midend/ir.h"
#include <memory>
#include <ostream>
#include <string>
#include <vector>

/**
 * Class containing static pretty-printer methods for IR and assembly programs.
 */
class PrettyPrinters {
  public:
    /**
     * Print the IR program to stdout.
     *
     * @param irProgram The IR program to print.
     * @param irStaticVariables The list of IR static variables.
     */
    static void
    printIRProgram(const IR::Program &irProgram,
                   const std::vector<std::unique_ptr<IR::StaticVariable>>
                       &irStaticVariables);

    /**
     * Emit the assembly program to the given output stream.
     *
     * This is the unified implementation of assembly formatting: the compiler
     * driver emits to an assembly file stream, while `printAssemblyProgram`
     * emits to stdout.
     *
     * @param assemblyProgram The assembly program to emit.
     * @param out The output stream to emit to.
     */
    static void emitAssemblyProgram(const Assembly::Program &assemblyProgram,
                                    std::ostream &out);

    /**
     * Print the assembly program to stdout.
     *
     * @param assemblyProgram The assembly program to print.
     */
    static void printAssemblyProgram(const Assembly::Program &assemblyProgram);

  private:
    /**
     * Print an IR function definition to stdout.
     *
     * @param functionDefinition The IR function definition to print.
     */
    static void
    printIRFunctionDefinition(const IR::FunctionDefinition &functionDefinition);

    /**
     * Print an IR static variable to stdout.
     *
     * @param staticVariable The IR static variable to print.
     */
    static void printIRStaticVariable(const IR::StaticVariable &staticVariable);

    /**
     * Print an IR instruction to stdout.
     *
     * @param instruction The IR instruction to print.
     */
    static void printIRInstruction(const IR::Instruction &instruction);

    /**
     * Print an IR return instruction to stdout.
     *
     * @param returnInstruction The IR return instruction to print.
     */
    static void
    printIRReturnInstruction(const IR::ReturnInstruction &returnInstruction);

    /**
     * Print an IR sign-extend instruction to stdout.
     *
     * @param signExtendInstruction The IR sign-extend instruction to print.
     */
    static void printIRSignExtendInstruction(
        const IR::SignExtendInstruction &signExtendInstruction);

    /**
     * Print an IR truncate instruction to stdout.
     *
     * @param truncateInstruction The IR truncate instruction to print.
     */
    static void printIRTruncateInstruction(
        const IR::TruncateInstruction &truncateInstruction);

    /**
     * Print an IR zero-extend instruction to stdout.
     *
     * @param zeroExtendInstruction The IR zero-extend instruction to print.
     */
    static void printIRZeroExtendInstruction(
        const IR::ZeroExtendInstruction &zeroExtendInstruction);

    /**
     * Print an IR unary instruction to stdout.
     *
     * @param unaryInstruction The IR unary instruction to print.
     */
    static void
    printIRUnaryInstruction(const IR::UnaryInstruction &unaryInstruction);

    /**
     * Print an IR binary instruction to stdout.
     *
     * @param binaryInstruction The IR binary instruction to print.
     */
    static void
    printIRBinaryInstruction(const IR::BinaryInstruction &binaryInstruction);

    /**
     * Print an IR copy instruction to stdout.
     *
     * @param copyInstruction The IR copy instruction to print.
     */
    static void
    printIRCopyInstruction(const IR::CopyInstruction &copyInstruction);

    /**
     * Print an IR jump instruction to stdout.
     *
     * @param jumpInstruction The IR jump instruction to print.
     */
    static void
    printIRJumpInstruction(const IR::JumpInstruction &jumpInstruction);

    /**
     * Print an IR jump-if-zero instruction to stdout.
     *
     * @param jumpIfZeroInstruction The IR jump-if-zero instruction to print.
     */
    static void printIRJumpIfZeroInstruction(
        const IR::JumpIfZeroInstruction &jumpIfZeroInstruction);

    /**
     * Print an IR jump-if-not-zero instruction to stdout.
     *
     * @param jumpIfNotZeroInstruction The IR jump-if-not-zero instruction to
     * print.
     */
    static void printIRJumpIfNotZeroInstruction(
        const IR::JumpIfNotZeroInstruction &jumpIfNotZeroInstruction);

    /**
     * Print an IR label instruction to stdout.
     *
     * @param labelInstruction The IR label instruction to print.
     */
    static void
    printIRLabelInstruction(const IR::LabelInstruction &labelInstruction);

    /**
     * Print an IR function call instruction to stdout.
     *
     * @param functionCallInstruction The IR function call instruction to print.
     */
    static void printIRFunctionCallInstruction(
        const IR::FunctionCallInstruction &functionCallInstruction);

    /**
     * Emit the assembly code for a function definition.
     *
     * @param functionDefinition The function definition to emit.
     * @param out The output stream to emit to.
     */
    static void emitAssyFunctionDefinition(
        const Assembly::FunctionDefinition &functionDefinition,
        std::ostream &out);

    /**
     * Emit the assembly code for a static variable.
     *
     * @param staticVariable The static variable to emit.
     * @param out The output stream to emit to.
     */
    static void
    emitAssyStaticVariable(const Assembly::StaticVariable &staticVariable,
                           std::ostream &out);

    /**
     * Emit the assembly code for an instruction.
     *
     * @param instruction The instruction to emit.
     * @param out The output stream to emit to.
     */
    static void emitAssyInstruction(const Assembly::Instruction &instruction,
                                    std::ostream &out);

    /**
     * Emit the assembly code for a move instruction.
     *
     * @param movInstruction The move instruction to emit.
     * @param out The output stream to emit to.
     */
    static void
    emitAssyMovInstruction(const Assembly::MovInstruction &movInstruction,
                           std::ostream &out);

    /**
     * Emit the assembly code for a move-with-sign-extend instruction.
     *
     * @param movsxInstruction The move-with-sign-extend instruction to emit.
     * @param out The output stream to emit to.
     */
    static void
    emitAssyMovsxInstruction(const Assembly::MovsxInstruction &movsxInstruction,
                             std::ostream &out);

    /**
     * Emit the assembly code for a return instruction.
     *
     * @param out The output stream to emit to.
     */
    static void emitAssyRetInstruction(std::ostream &out);

    /**
     * Emit the assembly code for a push instruction.
     *
     * @param pushInstruction The push instruction to emit.
     * @param out The output stream to emit to.
     */
    static void
    emitAssyPushInstruction(const Assembly::PushInstruction &pushInstruction,
                            std::ostream &out);

    /**
     * Emit the assembly code for a call instruction.
     *
     * @param callInstruction The call instruction to emit.
     * @param out The output stream to emit to.
     */
    static void
    emitAssyCallInstruction(const Assembly::CallInstruction &callInstruction,
                            std::ostream &out);

    /**
     * Emit the assembly code for a unary instruction.
     *
     * @param unaryInstruction The unary instruction to emit.
     * @param out The output stream to emit to.
     */
    static void
    emitAssyUnaryInstruction(const Assembly::UnaryInstruction &unaryInstruction,
                             std::ostream &out);

    /**
     * Emit the assembly code for a binary instruction.
     *
     * @param binaryInstruction The binary instruction to emit.
     * @param out The output stream to emit to.
     */
    static void emitAssyBinaryInstruction(
        const Assembly::BinaryInstruction &binaryInstruction,
        std::ostream &out);

    /**
     * Emit the assembly code for a compare instruction.
     *
     * @param cmpInstruction The compare instruction to emit.
     * @param out The output stream to emit to.
     */
    static void
    emitAssyCmpInstruction(const Assembly::CmpInstruction &cmpInstruction,
                           std::ostream &out);

    /**
     * Emit the assembly code for a signed-integer-division instruction.
     *
     * @param idivInstruction The signed-integer-division instruction to emit.
     * @param out The output stream to emit to.
     */
    static void
    emitAssyIdivInstruction(const Assembly::IdivInstruction &idivInstruction,
                            std::ostream &out);

    /**
     * Emit the assembly code for an unsigned-integer-division instruction.
     *
     * @param divInstruction The unsigned-integer-division instruction to emit.
     * @param out The output stream to emit to.
     */
    static void
    emitAssyDivInstruction(const Assembly::DivInstruction &divInstruction,
                           std::ostream &out);

    /**
     * Emit the assembly code for a covert-doubleword-to-quadword instruction.
     *
     * @param cdqInstruction The covert-doubleword-to-quadword instruction to
     * emit.
     * @param out The output stream to emit to.
     */
    static void
    emitAssyCdqInstruction(const Assembly::CdqInstruction &cdqInstruction,
                           std::ostream &out);

    /**
     * Emit the assembly code for a jump instruction.
     *
     * @param jmpInstruction The jump instruction to emit.
     * @param out The output stream to emit to.
     */
    static void
    emitAssyJmpInstruction(const Assembly::JmpInstruction &jmpInstruction,
                           std::ostream &out);

    /**
     * Emit the assembly code for a conditional jump instruction.
     *
     * @param jmpCCInstruction The conditional jump instruction to emit.
     * @param out The output stream to emit to.
     */
    static void
    emitAssyJmpCCInstruction(const Assembly::JmpCCInstruction &jmpCCInstruction,
                             std::ostream &out);

    /**
     * Emit the assembly code for a set-byte-on-condition instruction.
     *
     * @param setCCInstruction The set-byte-on-condition instruction to emit.
     * @param out The output stream to emit to.
     */
    static void
    emitAssySetCCInstruction(const Assembly::SetCCInstruction &setCCInstruction,
                             std::ostream &out);

    /**
     * Emit the assembly code for a label instruction.
     *
     * @param labelInstruction The label instruction to emit.
     * @param out The output stream to emit to.
     */
    static void
    emitAssyLabelInstruction(const Assembly::LabelInstruction &labelInstruction,
                             std::ostream &out);

    /**
     * Prepend an underscore to the identifier if the underlying OS is macOS.
     *
     * @param identifier The identifier to modify.
     */
    static void prependUnderscoreToIdentifierIfMacOS(
        [[maybe_unused]] std::string &identifier);
};

#endif // UTILS_PRETTY_PRINTERS_H
