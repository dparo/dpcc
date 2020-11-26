#!/usr/bin/env python3
import os
import sys
from datetime import datetime
import itertools

from utils import bin2header

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



decl_ops = [
    "TOK_KW_FN",
    "TOK_KW_LET"
]

cast_ops = [
    "TOK_KW_INT",
    "TOK_KW_FLOAT",
    "TOK_KW_BOOL",
]



def decl_map_fn(out_type, fn_name, in_type, map_dict, default_case=DEFAULT_CASE, default_return_value="0"):
    gprint(f"static {out_type} {fn_name}({in_type} x)")
    with scope():
        switch("x", map_dict, default_case)
        if default_return_value is not None and len(default_return_value) > 0:
            gprint(f"return ({out_type}) {default_return_value};")


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


def debug_print(string):
    gprint(f'printf("--------------- %s\\n", "{string}");')



def generate_src_file():
    gprint(common_boilerplate)

    gprint('#include "parser.h"')
    gprint('extern int yynerrs;')

    gprint("void typemismatch_check(ast_node_t *expected_type, ast_node_t *got_type)")
    with scope():
        gprint("if (expected_type != NULL && got_type != NULL)")
        with scope():
            gprint("if (expected_type->md.type != got_type->md.type)")
            with scope():
                gen_err("&expected_type->tok->loc", '"Type mismatch"')
                gen_info("&expected_type->tok->loc", '"Expected `%s`\"', ["dpcc_type_as_str(expected_type->md.type)"])
                gen_info("&got_type->tok->loc", '"But got `%s`\"', ["dpcc_type_as_str(got_type->md.type)"])



    gprint("static void type_deduce(ast_node_t *n)")
    with scope():
        gprint("// Utilities vars t oeasily access childs")
        gprint("ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;")
        gprint("ast_node_t *c1 = (n->num_childs >= 2) ? n->childs[1] : NULL;")
        gprint("ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;")
        gprint("ast_node_t *c3 = (n->num_childs >= 4) ? n->childs[3] : NULL;")

        gprint("// Base cases for type deduction")
        switch("n->kind", {
            "TOK_CHAR_LIT": "n->md.type = TYPE_I32;",
            "TOK_I32_LIT": "n->md.type = TYPE_I32;",
            "TOK_F32_LIT": "n->md.type = TYPE_F32;",
            "TOK_BOOL_LIT": "n->md.type = TYPE_BOOL;",
            "TOK_ID": "if (n->decl) n->md.type = n->decl->md.type;",
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
        gprint(f'if (c0 && ({one_of("n->kind", decl_ops)}))')
        with scope():
            gprint('if (c0->md.type == TYPE_NONE && c0->kind != TOK_OPEN_BRACKET)')
            with scope():
                gprint("// Handle integral types")
                switch('c0->kind', {
                    'TOK_KW_INT': 'c0->md.type = TYPE_I32;',
                    'TOK_KW_FLOAT': 'c0->md.type = TYPE_F32;',
                    'TOK_KW_BOOL': 'c0->md.type = TYPE_BOOL;',
                })
            gprint("else if (c0->md.type == TYPE_NONE && c0->kind == TOK_OPEN_BRACKET)")
            with scope():
                gprint("// Handle array types")
                switch("c0->childs[0]->kind", {
                    "TOK_KW_INT": "c0->md.type = TYPE_I32_ARRAY;",
                    "TOK_KW_FLOAT": "c0->md.type = TYPE_F32_ARRAY;",
                })
            gprint("// Forward the same type to the keyword let")
            gprint("n->md.type = c0->md.type;")

        gprint("// Deduce type of variable declarations")
        gprint(f'if {one_of("n->kind", decl_ops)}')
        with scope():
            gprint('// Deduce type of integral variable declarations')
            gprint(f'if ((c0 == NULL) || (c0->kind != TOK_OPEN_BRACKET))')
            with scope():
                gprint("ast_node_t *var_decl_type = c0;")
                gprint("ast_node_t *child_type = c2;")
                gprint("if (var_decl_type != NULL && child_type != NULL)")
                with scope():
                    gprint("// We might have type mismatch")
                    gprint("typemismatch_check(var_decl_type, child_type);")
                gprint("else if (child_type != NULL)")
                with scope():
                    gprint("// Type deduce from child")
                    gprint("n->md.type = child_type->md.type;")
                gprint("else")
                with scope():
                    gprint("// Assume integer")
                    gprint("n->md.type = TYPE_I32;")
            gprint("else")
            with scope():
                gprint("// Deduce type of array variable declarations")

                gprint("assert((c0->childs[1] == NULL) || (c0->childs[1]->kind == TOK_I32_LIT && c0->childs[1]->md.type == TYPE_I32));")

                gprint("int32_t array_type_len = c0->childs[1] ? c0->childs[1]->val.as_i32 : 0;")
                gprint("int32_t init_list_len = c2 != NULL ? c2->num_childs : 0;")

                gprint('if(c0->childs[1] && array_type_len <= 0)')
                with scope():
                    gen_err("&c0->childs[1]->tok->loc", '"The number of elements in an array must be a positive integer"')
                    gen_info("&c0->childs[1]->tok->loc", '"Got `%d`"', ['array_type_len'])
                gprint("else")
                with scope():
                    gprint("if (c2)")
                    with scope():
                        gprint("// Initializer list is provided")
                        gprint("enum DPCC_TYPE expected_type;")
                        switch("c0->md.type", {
                            "TYPE_I32_ARRAY": "expected_type = TYPE_I32;",
                            "TYPE_F32_ARRAY": "expected_type = TYPE_F32;",
                        })

                        gprint("if (c0->childs[1] && init_list_len != array_type_len)")
                        with scope():
                            gen_err("&c2->tok->loc", '"Number of elements in initializer list do not match"')
                            gen_info("&c0->childs[1]->tok->loc", '"Expected number of elements: `%d`"', ['array_type_len'])
                            gen_info("&c2->tok->loc", '"Number of elements got: `%d`"', ['init_list_len'])

                        gprint("// Now make sure that each type in the initializer list is correct")
                        gprint("for (int32_t i = 0; i < c2->num_childs; i++)")
                        with scope():
                            gprint("if (c2->childs[i] && c2->childs[i]->md.type != expected_type)")
                            with scope():
                                gen_err("&c2->childs[i]->tok->loc", '"Type mismatch in array initializer list"')
                                gen_info("&c0->childs[0]->tok->loc", '"Expected `%s`"', ["dpcc_type_as_str(c0->childs[0]->md.type)"])
                                gen_info("&c2->childs[i]->tok->loc", '"Got `%s`"', ["dpcc_type_as_str(c2->childs[i]->md.type)"])
                    gprint("if (c0->childs[1] == NULL)")
                    with scope():
                        gprint("// Number of elements are not specified we need to deduce them")

                    gprint("else")
                    with scope():
                        gprint("// Number of elements are specified. Make sure that they match with the initializer list")




                gprint("// Forward the same type to the keyword let")
                gprint("n->md.type = c0->md.type;")
                gprint("n->md.array_len = init_list_len;")


        gprint("// Deduce type of expression and operators: This is the most demanding and difficult part")







    gprint("void check_and_optimize_ast(void)")
    with scope():
        gprint("ast_traversal_t att = {0};")
        gprint("ast_traversal_begin(&att);")
        with scope():
            gprint("ast_node_t *n = NULL;")
            gprint("while ((n = ast_traverse_next(&att)) != NULL)")
            with scope():
                gprint('if (n->md.type == TYPE_NONE)')
                with scope():
                    gprint("type_deduce(n);")




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
