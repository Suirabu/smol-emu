#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "error.h"

bool cpu_load_program(Cpu* cpu, const char* program_data, size_t program_len) {
    if(program_len > CPU_MEM_SIZE - CPU_PROGRAM_OFFSET) {
        REPORT_ERROR("Program size exceedes amount of memory available\n");
        return false;
    }

    memcpy(cpu->data + CPU_PROGRAM_OFFSET, program_data, program_len);
    cpu->pc = CPU_PROGRAM_OFFSET;
    return true;
}

bool cpu_tick(Cpu* cpu) {
    const uint8_t op = cpu->data[cpu->pc++];

    switch(op) {
    case OP_NOP:
        break;

    default:
        REPORT_ERROR("Unknown opcode 0x%02X found at 0x%04X\n", op, cpu->pc - 1);
        return false;
    }

    return true;
}
