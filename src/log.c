#include "log.h"
#include "globals.h"
#include <stdarg.h>
#include <unistd.h>

void dpcc_set_log_color(FILE *stream, enum DPCC_LOG_COLOR color)
{
    bool tty = isatty(fileno(stream));
    if (tty) {
        int32_t color_table[] = {
            0,
            31,
            33,
            32,
        };

        fprintf(stream, "\x1b[%dm", color_table[color]);
    }
}

void dpcc_log(enum DPCC_LOG_SEVERITY severity, loc_t *loc, char *fmt, ...)
{

    int32_t color_table[] = {
        DPCC_LOG_COLOR_GREEN,
        DPCC_LOG_COLOR_YELLOW,
        DPCC_LOG_COLOR_RED,
    };

    char *severity_string[] = {
        "info",
        "warning",
        "error",
    };

    dpcc_set_log_color(stderr, color_table[severity]);

    char *filepath = G_filepath;
    int32_t line = loc->first_line;
    int32_t column = loc->first_column;

    va_list ap;
    va_start(ap, fmt);
    char *msg = "";
    int msg_required_len = vsnprintf(NULL, 0, fmt, ap);
    msg = calloc(1, msg_required_len + 1);
    va_start(ap, fmt);
    vsnprintf(msg, msg_required_len + 1, fmt, ap);
    va_end(ap);

    fprintf(stderr, "%s:%d:%d: %s: %s\n", filepath, line, column, severity_string[severity], msg);

    dpcc_set_log_color(stderr, DPCC_LOG_COLOR_RESET);

    free(msg);
}
