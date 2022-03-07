#include <assert.h>
#include <stdio.h>

#include "cpu.h"
#include "error.h"

void print_usage(FILE* stream) {
    fprintf(stream,
        "SmolEmu - Smol emulator\n\n"
        "Usage:\n"
        "    ./smol-emu <ROM path>\n"
    );
}

int main(int argc, char** argv) {
    // if(argc != 2) {
    //     print_usage(stderr);
    //     REPORT_ERROR("Invalid usage\n");
    //     return 1;
    // }

    // TODO: Implement proper argument parser
    const char* executable_path = argv[0];
    const char* rom_path = argv[1];

    Cpu cpu = { 0 };

    const char program_data[] = { OP_NOP, 0xFF };
    const char program_len = sizeof(program_data) / sizeof(*program_data);
    cpu_load_program(&cpu, program_data, program_len);

    assert(cpu_tick(&cpu) && "No-op");
    assert(!cpu_tick(&cpu) && "Invalid instruction");

    return 0;
}
