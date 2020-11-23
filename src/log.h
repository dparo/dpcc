#pragma once

#include "types.h"
#include <stdio.h>
#include <stdlib.h>

enum DPCC_LOG_COLOR {
    DPCC_LOG_COLOR_RESET,
    DPCC_LOG_COLOR_RED,
    DPCC_LOG_COLOR_YELLOW,
    DPCC_LOG_COLOR_GREEN,
};

enum DPCC_LOG_SEVERITY {
    DPCC_SEVERITY_INFO,
    DPCC_SEVERITY_WARNING,
    DPCC_SEVERITY_ERROR,
};

void dpcc_set_log_color(FILE *stream, enum DPCC_LOG_COLOR out_color);
void dpcc_log(enum DPCC_LOG_SEVERITY severity, tokloc_t *loc, char *fmt, ...)
    __attribute__((format(printf, 3, 4)));
