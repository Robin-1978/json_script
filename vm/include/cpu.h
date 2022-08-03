#include <cstdint>

namespace vm::cpu
{

namespace x86
{
    enum Register
    {
        EAX,
        EBX,
        ECX,
        EDX,
        ESI,
        EDI,
        EBP,
        ESP,

        NUM_REGISTERS

    }; // enum Register


} // namespace x86

namespace riscv
{
    enum Register
    {
        ZERO,
        RA,
        SP,
        GP,
        TP,
        T0,T1,T2,
        FP,
        S1,
        A0, A1, A2, A3, A4, A5, A6, A7,
        S2, S3, S4, S5, S6, S7, S8, S9, S10, S11,
        T3,T4,T5,T6,
        NUM_REGISTERS
    };
    
    enum OpCode
    {
        NUM_OPCODES
    };

} // namespace riscv


struct Vm
{
    uint32_t registers[x86::NUM_REGISTERS];
    char memory[1024 * 1024 * 64];
};


} // namespace vm::cpu
