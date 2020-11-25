#!/usr/bin/env python3
import os
import sys
from datetime import datetime
import itertools

INVALID_CODE_PATH = "invalid_code_path();"
DEFAULT_CASE = INVALID_CODE_PATH

common_boilerplate = (
f"""//
// generated from {sys.argv[0]}
//

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>


#include "types.h"
#include "globals.h"
#include "utils.h"
#include "log.h"
#include "dpcc.h"

"""
)
def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)


indent_cnt = 0
def gprint(*args, **kwargs):
    if len(args) >= 1:
        lines = args[0].splitlines()
        out = ""

        for i in range(0, len(lines)):
            out += "    " * indent_cnt + lines[i] + "\n"

        new_args = list(args)
        new_args[0] = out
        args = tuple(new_args)
    print(*args, **kwargs, end='')

class scope:
    def __init__(self):
        pass
    def __enter__(self):
        global indent_cnt
        gprint("{")
        indent_cnt += 1
    def __exit__(self, type, value, traceback):
        global indent_cnt
        indent_cnt -= 1
        gprint("}")


def gen_logical_or(array):
    result = ""
    for i in range(0, len(array)):
        if (i != len(array) - 1):
            result += f"{array[i]} || "
        else:
            result += f"{array[i]}"
    return "(" + result + ")"


def gen_logical_and(array):
    result = ""
    for i in range(0, len(array)):
        if (i != len(array) - 1):
            result += f"{array[i]} && "
        else:
            result += f"{array[i]}"
    return "(" + result + ")"

def one_of(expr, array):
    result = ""
    for i in range(0, len(array)):
        if (i != len(array) - 1):
            result += f"({expr} == {array[i]}) || "
        else:
            result += f"({expr} == {array[i]})"
    return "(" + result + ")"

def switch(elem, dict, default_case = DEFAULT_CASE):
    keys = list(dict.keys())
    values = list(dict.values())

    gprint(f"switch ({elem})")
    with scope():
        if default_case == "":
            gprint("default: { /* EMPTY */ } break;")
        else:
            gprint("default:")
            with scope():
                gprint(default_case)
            gprint("break;")
        for i in range(0, len(keys)):
            gprint(f"case {keys[i]}:")
            with scope():
                gprint(values[i])
            gprint("break;")


expr_tokens = [
    "TOK_ADD",
    "TOK_SUB",
    "TOK_MUL",
    "TOK_DIV",
    "TOK_MOD",
    "TOK_EQ",
    "TOK_NEQ",
    "TOK_GT",
    "TOK_GTEQ",
    "TOK_LT",
    "TOK_LTEQ",
    "TOK_INC",
    "TOK_DEC",
    "TOK_LNOT",
    "TOK_LAND",
    "TOK_LOR",
    "TOK_BNOT",
    "TOK_BAND",
    "TOK_BOR",
    "TOK_BXOR",
    "TOK_BLSHIFT",
    "TOK_BRSHIFT",
    "TOK_POW",
]

decl_ops = [
    "TOK_KW_FN",
    "TOK_KW_LET"
]

cast_ops = [
    "TOK_KW_INT",
    "TOK_KW_FLOAT",
    "TOK_KW_BOOL",
]


def decl_map_fn(out_type, fn_name, in_type, map_dict, default_case=DEFAULT_CASE):
    gprint(f"static {out_type} {fn_name}({in_type} x)")
    with scope():
        switch("x", map_dict, default_case)


def tuple_to_comma_separated_str(t):
    return ','.join(map(str, itertools.chain.from_iterable(t)))

def gen_err(loc, fmt, args = []):
    optional_comma = "" if len(args) == 0 else ", "
    va_args = ""
    for i in range(0, len(args)):
        va_args += args[i] + (", " if i != len(args) - 1 else "")

    gprint(f"dpcc_log(DPCC_SEVERITY_ERROR, {loc}, {fmt}{optional_comma} {va_args});")
    gprint("yynerrs += 1;")


def gen_warn(loc, fmt, args = []):
    optional_comma = "" if len(args) == 0 else ", "
    va_args = ""
    for i in range(0, len(args)):
        va_args += args[i] + (", " if i != len(args) - 1 else "")

    gprint(f"dpcc_log(DPCC_SEVERITY_WARN, {loc}, {fmt}{optional_comma} {va_args});")

def gen_info(loc, fmt, args = []):
    optional_comma = "" if len(args) == 0 else ", "
    va_args = ""
    for i in range(0, len(args)):
        va_args += args[i] + (", " if i != len(args) - 1 else "")

    gprint(f"dpcc_log(DPCC_SEVERITY_INFO, {loc}, {fmt}{optional_comma} {va_args});")



def generate_src_file():
    gprint(common_boilerplate)

    gprint('#include "parser.h"')
    gprint('extern int yynerrs;')

    gprint('static void invalid_code_path(void)')
    with scope():
        gprint('fprintf(stderr, "COMPILER INTERNAL ERROR --- Invalid code path");')
        gprint('fflush(stderr);')
        gprint('assert(0);')

    decl_map_fn("char*", "type_as_str", "int", {
        "TYPE_I32": "return \"int\";",
        "TYPE_F32": "return \"float\";",
        "TYPE_BOOL": "return \"bool\";",
    })

    gprint("void typemismatch_check(ast_node_t *expected_type, ast_node_t *got_type)")
    with scope():
        gprint("if (expected_type != NULL && got_type != NULL)")
        with scope():
            gprint("if (expected_type->md.type != got_type->md.type)")
            with scope():
                gen_err("&expected_type->tok->loc", '"Type mismatch"')
                gen_info("&expected_type->tok->loc", '"Expected `%s`\"', ["type_as_str(expected_type->md.type)"])
                gen_info("&got_type->tok->loc", '"But got `%s`\"', ["type_as_str(got_type->md.type)"])



    gprint("void check_and_optimize_ast(void)")
    with scope():
        gprint("ast_traversal_t att = {0};")
        gprint("ast_traversal_begin(&att);")
        with scope():
            gprint("ast_node_t *n = NULL;")
            gprint("while ((n = ast_traverse_next(&att)) != NULL)")
            with scope():
                gprint("// Base cases for type deduction")
                switch("n->kind", {
                    "TOK_CHAR_LIT": "n->md.type = TYPE_I32;",
                    "TOK_I32_LIT": "n->md.type = TYPE_I32;",
                    "TOK_F32_LIT": "n->md.type = TYPE_F32;",
                    "TOK_BOOL_LIT": "n->md.type = TYPE_BOOL;",
                }, default_case='')
                pass

                gprint("// Assign the correct type to each casting operator")
                gprint(f'if ({one_of("n->kind", cast_ops)} && !{one_of("n->parent->kind", decl_ops)})')
                with scope():
                    switch("n->kind", {
                        "TOK_KW_INT": "n->md.type = TYPE_I32;",
                        "TOK_KW_FLOAT": "n->md.type = TYPE_F32;",
                        "TOK_KW_BOOL": "n->md.type = TYPE_BOOL;",

                    })

                gprint("// Assign the correct type if the var declaration has a user listed type")
                gprint(f'if {one_of("n->kind", decl_ops)}')
                with scope():
                    gprint('if (n->childs[0] && n->childs[0]->md.type == TYPE_NONE)')
                    with scope():
                        switch('n->childs[0]->kind', {
                            'TOK_KW_INT': 'n->childs[0]->md.type = TYPE_I32;',
                            'TOK_KW_FLOAT': 'n->childs[0]->md.type = TYPE_F32;',
                            'TOK_KW_BOOL': 'n->childs[0]->md.type = TYPE_BOOL;',
                        })
                        pass

                gprint("// Deduce type of the variable declarations")
                gprint(f'if {one_of("n->kind", decl_ops)}')
                with scope():
                    gprint("ast_node_t *var_decl_type = n->childs[0];")
                    gprint("ast_node_t *child_type = n->childs[2];")
                    gprint("if (var_decl_type != NULL && child_type != NULL)")
                    with scope():
                        gprint("// We might have type mismatch")
                        gprint("typemismatch_check(var_decl_type, child_type);")
                    gprint("else if (child_type != NULL)")
                    with scope():
                        gprint("// Type deduce from child")
                        gprint("n->md.type = n->childs[1]->md.type;")
                    gprint("else")
                    with scope():
                        gprint("// Assume integer")
                        gprint("n->md.type = TYPE_I32;")



def generate_hdr_file():
    gprint(common_boilerplate)
    gprint("void check_and_optimize_ast(void);")
    gprint("void codegen(void);")

def main():

    with open("last_gen.txt", "w") as f:
        f.write(datetime.today().strftime('%Y-%m-%d-%H:%M:%S'))

    gen_list = [
        { "filepath": "src/gen.c", "fn": generate_src_file },
        { "filepath": "src/gen.h", "fn": generate_hdr_file },
    ]

    for v in gen_list:
        os.makedirs(os.path.dirname(v['filepath']), exist_ok=True)
        with open(v['filepath'], 'w') as f:
            sys.stdout = f
            (v['fn'])()

main()
