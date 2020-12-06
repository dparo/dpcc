#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

enum TYPE {
    _kI32,
    _kF32,
    _kBOOL,
    _kI32_ARRAY,
    _kF32_ARRAY,
};

static void _scope_begin(void);
static void _scope_end(void);

static void _var_decl(char *lexeme, enum TYPE t, int32_t array_len);
static void _var_init(char *lexeme, enum TYPE t, int32_t init_list_len, void *init_list);

static int32_t _var_get_kI32(char *lexeme, int32_t index);
static float _var_get_kF32(char *lexeme, int32_t index);
static bool _var_get_kBOOL(char *lexeme, int32_t index);

static int32_t _var_set_kI32(char *lexeme, int32_t index, int32_t val);
static float _var_set_kF32(char *lexeme, int32_t index, float val);
static bool _var_set_kBOOL(char *lexeme, int32_t index, bool val);
