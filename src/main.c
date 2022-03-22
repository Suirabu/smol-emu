#include <assert.h>
#include <stdio.h>

#include "cpu.h"
#include "error.h"
#include "util.h"

void print_usage(FILE* stream) {
    fprintf(stream,
        "SmolEmu - Smol emulator\n\n"
        "Usage:\n"
        "    ./smol-emu <ROM path>\n"
    );
}

int main(int argc, char** argv) {
    if(argc != 2) {
        print_usage(stderr);
        REPORT_ERROR("Invalid usage\n");
        return 1;
    }

    // TODO: Implement proper argument parser
    const char* executable_path = argv[0];
    const char* rom_path = argv[1];

    // Make sure that specified ROM is small enough to fit in our CPU's memory
    const size_t program_len = util_get_file_len(rom_path);
    if(program_len > CPU_MEM_SIZE - CPU_PROGRAM_OFFSET) {
        REPORT_ERROR("ROM '%s' is too large. ROMs must be ~3.2KB or less in size.\n", rom_path);
        return 1;
    }
    
    Cpu cpu = { 0 };

    char* program_data = util_get_file_contents(rom_path);
    cpu_load_program(&cpu, program_data, program_len);
    free(program_data);

    while(cpu_tick(&cpu));

    return 0;
}
