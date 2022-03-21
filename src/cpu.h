#ifndef CPU_H
#define CPU_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define CPU_MEM_SIZE        40000  // 40KB
#define CPU_PROGRAM_OFFSET  0x2000

#define CPU_STACK_SIZE      0xFF

typedef enum {
    OP_NOP      = 0x00,

    OP_PUSH_8   = 0x10,
    OP_PUSH_16  = 0x11,
    OP_DROP     = 0x12,
    OP_DUP      = 0x13,
    OP_SWAP     = 0x14,
    OP_OVER     = 0x15,
    OP_ROT      = 0x16,
    OP_STORE_8  = 0x17,
    OP_STORE_16 = 0x18,
    OP_LOAD_8   = 0x19,
    OP_LOAD_16  = 0x1A,

    OP_ADD      = 0x20,
    OP_SUB      = 0x21,
    OP_MULT     = 0x22,
    OP_DIV      = 0x23,
    OP_MOD      = 0x24,

    OP_AND      = 0x30, 
    OP_OR       = 0x31, 
    OP_NOT      = 0x32, 
    OP_XOR      = 0x33, 
    OP_SHIFTL   = 0x34, 
    OP_SHIFTR   = 0x35, 

    OP_CMP      = 0x40, 
    OP_JMP      = 0x41, 
    OP_JEQ      = 0x42, 
    OP_JNE      = 0x43, 
    OP_JLT      = 0x44, 
    OP_JLE      = 0x45, 
    OP_JGT      = 0x46, 
    OP_JGE      = 0x47, 
} CpuOpcode;

typedef struct {
    uint8_t data[CPU_MEM_SIZE];
    uint16_t pc;    // Program counter

    uint16_t stack[CPU_STACK_SIZE];
    uint8_t sp;     // Stack pointer
} Cpu;

bool cpu_load_program(Cpu* cpu, const char* program_data, size_t program_len);
bool cpu_tick(Cpu* cpu);

bool cpu_write(Cpu* cpu, uint16_t addr, size_t n, const void* data);
bool cpu_read(Cpu* cpu, uint16_t addr, size_t n, void* data);

#endif // CPU_H
