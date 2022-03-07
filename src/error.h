#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>

#define REPORT_ERROR(...) \
    fprintf(stderr, "\033[31;1mError:\033[0m " __VA_ARGS__);

#endif // ERROR_H
