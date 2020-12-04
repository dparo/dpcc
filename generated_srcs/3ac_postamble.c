
typedef struct {
    char *lexeme;
    void *buf;
    int32_t array_len;
} sym_t;

typedef struct {
    int32_t nsyms;
    sym_t   **syms;
} scope_t;


typedef struct {
    int32_t nscopes;
    scope_t *scopes;
} symtable_t;

static symtable_t symtable;

static void _scope_begin(void)
{
    scope_t scope = {0};
    symtable.scopes = realloc(symtable.scopes, sizeof(scope) * (symtable.nscopes + 1));
    symtable.scopes[symtable.nscopes] = scope;
    symtable.nscopes += 1;
}

static void _scope_end(void)
{
    scope_t *s = symtable.scopes[symtable.nscopes - 1];

    for (int32_t i = 0; i < s->nsyms; i++) {
        free(s->syms[i].buf);
    }

    assert(symtable.nscopes > 0);
    symtable.scopes = realloc(symtable.scopes, sizeof(scope) * (symtable.nscopes - 1));
    symtable.nscopes -= 1;
}

static void _var_decl(char *lexeme, enum TYPE t, int32_t array_len)
{
    assert(t == _kI32 || t == _kF32);

    sym_t sym = {0};
    // Initialize the symbol here
    size_t bufsize = 0;
    switch (t) {
    case _kI32: {
        bufsize = sizeof(int32_t) * array_len;
    } break;
    case _kF32: {
        bufsize = sizeof(float) * array_len;
    } break;
    }

    sym.buf = malloc(bufsize);

    scope_t *s = &symtable.scopes[sytamble.nscopes - 1];
    s.syms = realloc(s.syms, sizeof(sym_t) * (s.nsyms + 1));
    s.syms[s.nsyms] = sym;
    s.nsyms += 1;
}

static sym_t *sym_lookup(char *lexeme)
{
    for (int32_t scope_idx = symtable.nscopes - 1; scope_idx >= 0; scope_idx -- ) {
        scope_t *scope = symtable.scopes[i];
        for (int32_t sym_idx = 0; sym_idx < scope->nsyms; sym_idx++) {
            sym_t sym = &scope->syms[sym_idx];
            if (0 == strcmp(lexeme, s->lexeme)) {
                return sym;
            }
        }
    }

    assert(!"Invalid code path");
    return NULL;
}

static size_t sym_bufsize(enum TYPE t, int32_t array_len)
{
    assert(t == _kI32 || t == _kF32);

    sym_t sym = {0};
    // Initialize the symbol here
    size_t bufsize = 0;
    switch (t) {
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
    sym.buf = malloc(sym_buf_size(t, array_len));

    scope_t *s = &symtable.scopes[sytamble.nscopes - 1];
    s.syms = realloc(s.syms, sizeof(sym_t) * (s.nsyms + 1));
    s.syms[s.nsyms] = sym;
    s.nsyms += 1;
}

static void    _var_init(char *lexeme, enum TYPE t, int32_t init_list_len, void *init_list)
{
    sym_t *sym = sym_lookup(lexeme);
    memcpy(sym->buf, init_list, sym_bufsize(t, init_list_len));
}