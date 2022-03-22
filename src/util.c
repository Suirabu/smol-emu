#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "util.h"

char* util_get_file_contents(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if(!file) {
        REPORT_ERROR("Failed to open file '%s' for reading\n", file_path);
        return NULL;
    }

    const size_t file_len = util_get_file_len(file_path);
    
    char* buffer = malloc(file_len + 1);
    if(!buffer) {
        REPORT_ERROR("Failed to allocate memory for file buffer\n");
        fclose(file);
        return NULL;
    }

    if(fread(buffer, file_len, 1, file) != 1) {
        REPORT_ERROR("Failed to read contents of '%s' into content buffer\n", file_path);
        fclose(file);
        free(buffer);
        return NULL;
    }

    fclose(file);
    buffer[file_len] = '\0';
    return buffer;
}

size_t util_get_file_len(const char* file_path) {
    FILE* file = fopen(file_path, "r");
    if(!file) {
        REPORT_ERROR("Failed to open file '%s' for reading\n", file_path);
        return (size_t)-1;
    }

    fseek(file, 0, SEEK_END);
    const size_t file_len = ftell(file);
    rewind(file);

    fclose(file);
    return file_len;
}
