#define INVALID_CODE_PATH() assert(!"Invalid code path")

typedef struct {
    char *lexeme;
    void *buf;
    int32_t array_len;
    enum TYPE type;
} sym_t;

typedef struct {
    int32_t nsyms;
    sym_t *syms;
} scope_t;

typedef struct {
    int32_t nscopes;
    scope_t *scopes;
} symtable_t;

static symtable_t symtable;

static void _scope_begin(void)
{
    scope_t scope = { 0 };
    symtable.scopes = realloc(symtable.scopes, sizeof(scope) * (symtable.nscopes + 1));
    symtable.scopes[symtable.nscopes] = scope;
    symtable.nscopes += 1;
}

static void _scope_end(void)
{
    scope_t *scope = &symtable.scopes[symtable.nscopes - 1];

    for (int32_t i = 0; i < scope->nsyms; i++) {
        free(scope->syms[i].buf);
    }

    assert(symtable.nscopes > 0);
    symtable.scopes = realloc(symtable.scopes, sizeof(scope) * (symtable.nscopes - 1));
    symtable.nscopes -= 1;
}

static sym_t *sym_lookup(char *lexeme)
{
    for (int32_t scope_idx = symtable.nscopes - 1; scope_idx >= 0; scope_idx--) {
        scope_t *scope = &symtable.scopes[scope_idx];
        for (int32_t sym_idx = 0; sym_idx < scope->nsyms; sym_idx++) {
            sym_t *sym = &scope->syms[sym_idx];
            if (0 == strcmp(lexeme, sym->lexeme)) {
                return sym;
            }
        }
    }

    INVALID_CODE_PATH();
    return NULL;
}

static size_t sym_bufsize(enum TYPE t, int32_t array_len)
{
    assert(t == _kI32 || t == _kF32);

    size_t bufsize = 0;

    switch (t) {
    default: {
        INVALID_CODE_PATH();
    } break;
    case _kBOOL: {
        bufsize = sizeof(bool) * array_len;
    } break;
    case _kI32: {
        bufsize = sizeof(int32_t) * array_len;
    } break;
    case _kF32: {
        bufsize = sizeof(float) * array_len;
    } break;
    }

    return bufsize;
}

static void _var_decl(char *lexeme, enum TYPE t, int32_t array_len)
{
    sym_t sym = { 0 };
    sym.buf = malloc(sym_bufsize(t, array_len));
    sym.lexeme = lexeme;
    sym.type = t;

    scope_t *scope = &symtable.scopes[symtable.nscopes - 1];
    scope->syms = realloc(scope->syms, sizeof(sym_t) * (scope->nsyms + 1));
    scope->syms[scope->nsyms] = sym;
    scope->nsyms += 1;
}

static void _var_init(char *lexeme, enum TYPE t, int32_t init_list_len, void *init_list)
{
    sym_t *sym = sym_lookup(lexeme);
    memcpy(sym->buf, init_list, sym_bufsize(t, init_list_len));
}

#define GET(type, lexeme, index)                      \
    do {                                              \
        sym_t *sym = sym_lookup(lexeme);              \
        assert(index >= 0 && index < sym->array_len); \
        return ((type *)sym->buf)[index];             \
    } while (0)

static int32_t _var_get_kI32(char *lexeme, int32_t index)
{
    GET(int32_t, lexeme, index);
}

static float _var_get_kF32(char *lexeme, int32_t index)
{
    GET(float, lexeme, index);
}

static bool _var_get_kBOOL(char *lexeme, int32_t index)
{
    GET(bool, lexeme, index);
}

#define SET(type, lexeme, index, val)                 \
    do {                                              \
        sym_t *sym = sym_lookup(lexeme);              \
        assert(index >= 0 && index < sym->array_len); \
        ((type *)sym->buf)[index] = (val);            \
    } while (0)

static void _var_set_kI32(char *lexeme, int32_t index, int32_t val)
{
    SET(int32_t, lexeme, index, val);
}

static void _var_set_kF32(char *lexeme, int32_t index, float val)
{
    SET(float, lexeme, index, val);
}

static void _var_set_kBOOL(char *lexeme, int32_t index, bool val)
{
    SET(bool, lexeme, index, val);
}
