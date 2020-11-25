#!/usr/bin/env python3
import os
import sys
from datetime import datetime
import itertools


DEFAULT_CASE = "fprintf(stderr, \"COMPILER INTERNAL ERROR --- Invalid code path\");\nfflush(stderr);\nassert(0);"

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
#include "parser.h"

extern int yynerrs;

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


def all_of(array):
    result = ""
    for i in range(0, len(array)):
        if (i != len(array) - 1):
            result += f"{array[i]} && "
        else:
            result += f"{array[i]}"
    return "(" + result + ")"

def one_of(array):
    result = ""
    for i in range(0, len(array)):
        if (i != len(array) - 1):
            result += f"{array[i]} || "
        else:
            result += f"{array[i]}"
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



def generate():
    gprint(common_boilerplate)

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
                gen_info("&expected_type->tok->loc", '"But got `%s`\"', ["type_as_str(got_type->md.type)"])



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

                gprint("// Type deduce casting")
                gprint(f"if (n->kind == {one_of(cast_ops)} && n->parent != {one_of(decl_ops)})")
                with scope():
                    switch("n->kind", {
                        "TOK_KW_INT": "n->md.type = TYPE_I32;",
                        "TOK_KW_FLOAT": "n->md.type = TYPE_F32;",
                        "TOK_KW_BOOL": "n->md.type = TYPE_BOOL;",

                    })

                gprint("// Type deduce var delcs")
                gprint(f"if (n->kind == {one_of(decl_ops)})")
                with scope():
                    gprint("ast_node_t *expected_type = n->childs[0];")
                    gprint("ast_node_t *child_type = n->childs[2];")
                    gprint("if (expected_type != NULL && child_type != NULL)")
                    with scope():
                        gprint("// We might have type mismatch")
                        gprint("if (expected_type->md.type != child_type->md.type)")
                        with scope():
                            gprint("dpcc_log(DPCC_SEVERITY_ERROR, &n->tok->loc, \"\");");
                    gprint("else")
                    with scope():
                        gprint("// Direct type deduction")
                        gprint("n->md.type = n->childs[1]->md.type;")




def main():

    with open("last_gen.txt", "w") as f:
        f.write(datetime.today().strftime('%Y-%m-%d-%H:%M:%S'))

    gen_list = [
        { "filepath": "src/gen.c", "fn": generate }
    ]

    for v in gen_list:
        os.makedirs(os.path.dirname(v['filepath']), exist_ok=True)
        with open(v['filepath'], 'w') as f:
            sys.stdout = f
            (v['fn'])()

main()
