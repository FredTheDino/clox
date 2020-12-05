#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define RED        "\033[0;31m"
#define BRED       "\033[1;31m"
#define GREEN      "\033[0;32m"
#define BGREEN     "\033[1;32m"
#define YELLOW     "\033[0;33m"
#define BYELLOW    "\033[01;33m"
#define BLUE       "\033[0;34m"
#define BBLUE      "\033[1;34m"
#define MAGENTA    "\033[0;35m"
#define BMAGENTA   "\033[1;35m"
#define CYAN       "\033[0;36m"
#define BCYAN      "\033[1;36m"
#define RESET      "\033[0m"

#define _STR(s) #s
#define STR(s) _STR(s)

void __assert_failed();

#define ASSERT(expr, ...) \
    if ((expr) == 0) { \
        printf(RED "A " RESET "%s", __func__); \
        printf(" <" BLUE __FILE__ RESET ":" BLUE STR(__LINE__) RESET "> "); \
        printf(__VA_ARGS__); \
        printf("\n"); \
        __assert_failed(); \
        exit(1); \
    }

// #define DEBUG_TRACE_EXECUTION
// #define DEBUG_PRINT_CODE

#define UINT8_COUNT (UINT8_MAX + 1)
