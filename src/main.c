#include <stdio.h>

#include "error.h"

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

    return 0;
}
