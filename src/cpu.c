#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "error.h"

#define DEBUG

#define ASSERT_MIN_STACK_CAPACITY(min) \
    if(cpu->sp < min) { \
        REPORT_ERROR("Expected minimum of %u value(s) on stack. Found %u instead.\n", min, cpu->sp); \
        return false; \
    }

static bool cpu_addr_in_bounds(uint16_t addr) {
    return addr < CPU_MEM_SIZE;
}

bool cpu_write(Cpu* cpu, uint16_t addr, size_t n, const void* data) {
    if(!cpu_addr_in_bounds(addr + n - 1)) {
        REPORT_ERROR("Attempted to write to out-of-bounds memory address 0x%X\n", addr);
        return false;
    }

    memcpy(cpu->data + addr, data, n);
    return true;
}

bool cpu_read(Cpu* cpu, uint16_t addr, size_t n, void* data) {
    if(!cpu_addr_in_bounds(addr + n - 1)) {
        REPORT_ERROR("Attempted to read to out-of-bounds memory address 0x%X\n", addr);
        return false;
    }

    memcpy(data, cpu->data + addr, n);
    return true;
}

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
    // Meta operations
    case OP_NOP:
        break;

    // Stack operations
    case OP_PUSH_8: {
        const uint8_t val = cpu->data[cpu->pc++];
        cpu->stack[cpu->sp++] = val;
        break;
    }
    case OP_PUSH_16: {
        const uint16_t val = (cpu->data[cpu->pc] << 8) + cpu->data[cpu->pc + 1];
        cpu->pc += 2;
        cpu->stack[cpu->sp++] = val;
        break;
    }
    case OP_DROP: {
        ASSERT_MIN_STACK_CAPACITY(1);
        --cpu->sp;
#ifdef DEBUG
        printf("Dropped value %u...\n", cpu->stack[cpu->sp]);
#endif // DEBUG
        break;
    }
    case OP_DUP: {
        ASSERT_MIN_STACK_CAPACITY(1);
        const uint16_t val = cpu->stack[cpu->sp - 1];
        cpu->stack[cpu->sp++] = val;
        break;
    }
    case OP_SWAP: {
        ASSERT_MIN_STACK_CAPACITY(2);
        const uint16_t a = cpu->stack[cpu->sp - 1];
        const uint16_t b = cpu->stack[cpu->sp - 2];
        cpu->stack[cpu->sp - 1] = b;
        cpu->stack[cpu->sp - 2] = a;
        break;
    }
    case OP_OVER: {
        ASSERT_MIN_STACK_CAPACITY(2);
        const uint16_t val = cpu->stack[cpu->sp - 2];
        cpu->stack[cpu->sp++] = val;
        break;
    }
    case OP_ROT: {
        ASSERT_MIN_STACK_CAPACITY(3);
        const uint16_t a = cpu->stack[cpu->sp - 3];
        const uint16_t b = cpu->stack[cpu->sp - 2];
        const uint16_t c = cpu->stack[cpu->sp - 1];
        cpu->stack[cpu->sp - 3] = b;
        cpu->stack[cpu->sp - 2] = c;
        cpu->stack[cpu->sp - 1] = a;
        break;
     }
    case OP_STORE_8: {
        ASSERT_MIN_STACK_CAPACITY(2);
        const uint16_t val = cpu->stack[cpu->sp - 2];
        const uint16_t addr = cpu->stack[cpu->sp - 1];
        cpu_write(cpu, addr, 1, &val);
        cpu->sp -= 2;
        break;
     }
    case OP_STORE_16: {
        ASSERT_MIN_STACK_CAPACITY(2);
        const uint16_t val = cpu->stack[cpu->sp - 2];
        const uint16_t addr = cpu->stack[cpu->sp - 1];
        cpu_write(cpu, addr, 2, &val);
        cpu->sp -= 2;
        break;
     }
    case OP_LOAD_8: {
        ASSERT_MIN_STACK_CAPACITY(1);
        const uint16_t addr = cpu->stack[cpu->sp - 1];
        uint8_t val;
        cpu_read(cpu, addr, 1, &val);
        cpu->stack[cpu->sp - 1] = val;  // Overwrite addr, effectively dropping it and pushing our new value
        break;
     }
    case OP_LOAD_16: {
        ASSERT_MIN_STACK_CAPACITY(1);
        const uint16_t addr = cpu->stack[cpu->sp - 1];
        uint16_t val;
        cpu_read(cpu, addr, 2, &val);
        cpu->stack[cpu->sp - 1] = val;  // Overwrite addr, effectively dropping it and pushing our new value
        break;
     }

    case OP_ADD: {
        ASSERT_MIN_STACK_CAPACITY(2);
        const uint16_t a = cpu->stack[cpu->sp - 2];
        const uint16_t b = cpu->stack[cpu->sp - 1];
        --cpu->sp;
        cpu->stack[cpu->sp - 1] = a + b;
        break;
    }
    case OP_SUB: {
        ASSERT_MIN_STACK_CAPACITY(2);
        const uint16_t a = cpu->stack[cpu->sp - 2];
        const uint16_t b = cpu->stack[cpu->sp - 1];
        --cpu->sp;
        cpu->stack[cpu->sp - 1] = a - b;
        break;
    }
    case OP_MULT: {
        ASSERT_MIN_STACK_CAPACITY(2);
        const uint16_t a = cpu->stack[cpu->sp - 2];
        const uint16_t b = cpu->stack[cpu->sp - 1];
        --cpu->sp;
        cpu->stack[cpu->sp - 1] = a * b;
        break;
    }
    case OP_DIV: {
        ASSERT_MIN_STACK_CAPACITY(2);
        const uint16_t a = cpu->stack[cpu->sp - 2];
        const uint16_t b = cpu->stack[cpu->sp - 1];
        --cpu->sp;
        cpu->stack[cpu->sp - 1] = a / b;
        break;
    }
    case OP_MOD: {
        ASSERT_MIN_STACK_CAPACITY(2);
        const uint16_t a = cpu->stack[cpu->sp - 2];
        const uint16_t b = cpu->stack[cpu->sp - 1];
        --cpu->sp;
        cpu->stack[cpu->sp - 1] = a % b;
        break;
    }

    default:
        REPORT_ERROR("Unknown opcode 0x%02X found at 0x%04X\n", op, cpu->pc - 1);
        return false;
    }

    return true;
}
