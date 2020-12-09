#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <math.h>

enum TYPE {
    _kI32,
    _kF32,
    _kBOOL,
};

static void _scope_begin(void);
static void _scope_end(void);

static void _var_decl(char *lexeme, enum TYPE t, int32_t array_len);
static void _var_init(char *lexeme, enum TYPE t, int32_t init_list_len, void *init_list);

static void print_sym(char *lexeme);

static int32_t _var_get_kI32(char *lexeme, int32_t index);
static float _var_get_kF32(char *lexeme, int32_t index);
static bool _var_get_kBOOL(char *lexeme, int32_t index);

static int32_t _var_set_kI32(char *lexeme, int32_t index, int32_t val);
static float _var_set_kF32(char *lexeme, int32_t index, float val);
static bool _var_set_kBOOL(char *lexeme, int32_t index, bool val);







int main()
{

// Special variable used to implemenent INC (x++) and dec (x--)
// It is used to temporary hold the result of the INC/DEC in order to perform the side effect
int32_t _vspcIncDec;
// Special variable used to the negation for control statements (if, for, ...)
// For example the for loop needs to negate the user provided condition
bool    _vspcNeg;

// 3AC Var decls
int32_t _vi0 = 0;
int32_t _vi1 = 0;
int32_t _vi2 = 0;
bool    _vb0 = false;

_scope_begin();
    printf("%s", "\n\nIteration example\n");
    _scope_begin();
        _var_decl("a", _kI32, 10);
        _scope_begin();
            _var_decl("i", _kI32, 1);
            _var_init("i", _kI32, 1, (int32_t[]) {0});
            _lbl0:
            _vb0 = _var_get_kI32("i", 0) < 10;
            _vspcNeg = !_vb0;
            if (_vspcNeg) goto _lbl1;
            _scope_begin();
                _vi0 = _var_get("a", _kI32, _var_get_kI32("i", 0));
                _vi1 = _var_set_kI32("a", _var_get_kI32("i", 0), _var_get_kI32("i", 0));
            _scope_end();
            _vi2 = _var_get_kI32("i", 0);
            _vspcIncDec = _var_get_kI32("i", 0) + 1;
            _var_set_kI32("i", 0, _vspcIncDec);
            goto _lbl0;
            _lbl1:
        _scope_end();
        print_sym("a");
    _scope_end();
_scope_end();
}







#define INVALID_CODE_PATH() assert(!"Invalid code path")

typedef struct sym {
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
    symtable.scopes = realloc(symtable.scopes, sizeof(*symtable.scopes) * (symtable.nscopes + 1));
    assert(symtable.scopes);
    symtable.scopes[symtable.nscopes] = scope;
    symtable.nscopes += 1;
}

static void _scope_end(void)
{
    assert(symtable.nscopes > 0);

    scope_t *scope = &symtable.scopes[symtable.nscopes - 1];

    for (int32_t i = 0; i < scope->nsyms; i++) {
        free(scope->syms[i].buf);
    }

    free(scope->syms);

    memset(&symtable.scopes[symtable.nscopes - 1], 0, sizeof(symtable.scopes[symtable.nscopes - 1]));
    if ((symtable.nscopes - 1) == 0) {
        free(symtable.scopes);
        symtable.scopes = NULL;
    } else {
        symtable.scopes = realloc(symtable.scopes, sizeof(*symtable.scopes) * (symtable.nscopes - 1));
        assert(symtable.scopes);
    }
    symtable.nscopes -= 1;
}

static sym_t *sym_lookup(char *lexeme)
{
    assert(symtable.nscopes > 0);

    for (int32_t scope_idx = symtable.nscopes - 1; scope_idx >= 0; scope_idx--) {
        scope_t *scope = &symtable.scopes[scope_idx];
        for (int32_t sym_idx = 0; sym_idx < scope->nsyms; sym_idx++) {
            if (scope->syms == NULL) {
                continue;
            }
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
    sym.array_len = array_len;

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

static void _index_check(sym_t *sym, int32_t index)
{
    if (!(index >= 0 && index < sym->array_len)) {
        fprintf(stderr, "Index out of bounds error on variable `%s`\n", sym->lexeme);
        fprintf(stderr, "Variable array len is %d\n", sym->array_len);
        fprintf(stderr, "But got index %d instead\n", index);
        abort();
    }
}

static void print_sym(char *lexeme)
{
    sym_t *sym = sym_lookup(lexeme);
    assert(sym);

    printf("%s = ", lexeme);
    if (sym->array_len > 1) {
        printf("{ ");
    }

    for (int32_t i = 0; i < sym->array_len; i++) {
        char *comma = i == sym->array_len - 1 ? "" : ", ";
        switch (sym->type) {
        default: { INVALID_CODE_PATH(); } break;
        case _kBOOL: { printf("%d%s", ((bool *) sym->buf)[i], comma); } break;
        case _kI32:  { printf("%d%s", ((int32_t *) sym->buf)[i], comma); } break;
        case _kF32:  { printf("%f%s", ((float *) sym->buf)[i], comma); } break;
        }
    }

    if (sym->array_len > 1) {
        printf(" }");
    }

    printf("\n");
}

static int32_t _var_get_kI32(char *lexeme, int32_t index)
{
    sym_t *sym = sym_lookup(lexeme);
    assert(sym);
    _index_check(sym, index);
    return ((int32_t *)sym->buf)[index];
}

static float _var_get_kF32(char *lexeme, int32_t index)
{
    sym_t *sym = sym_lookup(lexeme);
    assert(sym);
    _index_check(sym, index);
    return ((float *)sym->buf)[index];
}

static bool _var_get_kBOOL(char *lexeme, int32_t index)
{
    sym_t *sym = sym_lookup(lexeme);
    assert(sym);
    _index_check(sym, index);
    return ((bool *)sym->buf)[index];
}

static int32_t _var_set_kI32(char *lexeme, int32_t index, int32_t val)
{
    sym_t *sym = sym_lookup(lexeme);
    assert(sym);
    _index_check(sym, index);
    ((int32_t *)sym->buf)[index] = (val);
    return val;
}

static float _var_set_kF32(char *lexeme, int32_t index, float val)
{
    sym_t *sym = sym_lookup(lexeme);
    assert(sym);
    _index_check(sym, index);
    ((float *)sym->buf)[index] = (val);
    return val;
}

static bool _var_set_kBOOL(char *lexeme, int32_t index, bool val)
{
    sym_t *sym = sym_lookup(lexeme);
    assert(sym);
    _index_check(sym, index);
    ((bool *)sym->buf)[index] = (val);
    return val;
}
