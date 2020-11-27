#!/usr/bin/env python3
import os
import sys
from datetime import datetime
import itertools

from utils import bin2header
from ops import ALL_BUNDLES, ALL_OPS, CONTROL_FLOW_OPS, type_to_dpcc_type

INVALID_CODE_PATH = "invalid_code_path();"
DEFAULT_CASE = INVALID_CODE_PATH

common_boilerplate = (
"""//
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

        for _, l in enumerate(lines):
            should_prepend_newline = l.startswith("do") or l.startswith("while") or l.startswith("switch") or l.startswith("for")
            if should_prepend_newline:
                out += "\n"
            out += "    " * indent_cnt + l + "\n"

        new_args = list(args)
        new_args[0] = out
        args = tuple(new_args)
    if len(args) == 0 or args[0] == "":
        print("")
    else:
        print(*args, **kwargs, end='')


class scope:
    def __init__(self):
        pass

    def __enter__(self):
        global indent_cnt
        gprint("{")
        indent_cnt += 1

    def __exit__(self, _type, _value, _traceback):
        global indent_cnt
        indent_cnt -= 1
        gprint("}")


def gen_logical_or(array):
    result = ""
    for i, _ in enumerate(array):
        if i != len(array) - 1:
            result += f"({array[i]}) || "
        else:
            result += f"({array[i]})"
    return "(" + result + ")"


def gen_logical_and(array):
    result = ""
    for i, _ in enumerate(array):
        if i != len(array) - 1:
            result += f"({array[i]}) && "
        else:
            result += f"({array[i]})"
    return "(" + result + ")"


def one_of(expr, array):
    result = ""
    for i, _ in enumerate(array):
        if i != len(array) - 1:
            result += f"({expr} == {array[i]}) || "
        else:
            result += f"({expr} == {array[i]})"
    return "(" + result + ")"


def switch(elem, d, default_case=DEFAULT_CASE):
    keys = list(d.keys())
    values = list(d.values())

    gprint(f"switch ({elem})")
    with scope():
        if default_case == "":
            gprint("default: { /* EMPTY */ } break;")
        else:
            gprint("default:")
            with scope():
                gprint(default_case)
            gprint("break;")
        for i, _ in enumerate(keys):
            gprint(f"case {keys[i]}:")
            with scope():
                gprint(values[i])
            gprint("break;")
    gprint()


DECL_OPS = [
    "TOK_KW_FN",
    "TOK_KW_LET"
]


CAST_OPS = [
    "TOK_KW_INT",
    "TOK_KW_FLOAT",
    "TOK_KW_BOOL",
]


def decl_map_fn(out_type, fn_name, in_type, map_dict, default_case=DEFAULT_CASE, default_return_value="0"):
    gprint('')
    gprint(f"static {out_type} {fn_name}({in_type} x)")
    with scope():
        switch("x", map_dict, default_case)
        if default_return_value is not None and len(default_return_value) > 0:
            gprint(f"return ({out_type}) {default_return_value};")


def tuple_to_comma_separated_str(t):
    return ','.join(map(str, itertools.chain.from_iterable(t)))


def gen_err(loc, fmt, args=None):
    va_args = ""
    optional_comma = ""
    if args is not None:
        optional_comma = "" if len(args) == 0 else ", "
        va_args = ""
        for i, _ in enumerate(args):
            va_args += args[i] + (", " if i != len(args) - 1 else "")

    gprint(f"dpcc_log(DPCC_SEVERITY_ERROR, {loc}, {fmt}{optional_comma} {va_args});")
    gprint("yynerrs += 1;")


def gen_warn(loc, fmt, args=None):
    va_args = ""
    optional_comma = ""
    if args is not None:
        optional_comma = "" if len(args) == 0 else ", "
        va_args = ""
        for i, _ in enumerate(args):
            va_args += args[i] + (", " if i != len(args) - 1 else "")

    gprint(f"dpcc_log(DPCC_SEVERITY_WARNING, {loc}, {fmt}{optional_comma} {va_args});")


def gen_info(loc, fmt, args=None):
    va_args = ""
    optional_comma = ""
    if args is not None:
        optional_comma = "" if len(args) == 0 else ", "
        va_args = ""
        for i, _ in enumerate(args):
            va_args += args[i] + (", " if i != len(args) - 1 else "")

    gprint(f"dpcc_log(DPCC_SEVERITY_INFO, {loc}, {fmt}{optional_comma} {va_args});")


def debug_print(string):
    gprint(f'printf("--------------- %s\\n", "{string}");')


def gen_ops_type_deduction_code(n):
    gprint(f'if ({one_of("n->kind", ALL_OPS)})')
    with scope():
        for bundle in ALL_BUNDLES:
            all_yytokenstypes = []
            for yytokenstype in bundle.yytokentypes:
                all_yytokenstypes.append(yytokenstype)
            gprint(f'if ({one_of("n->kind", all_yytokenstypes)})')
            with scope():
                for tct_idx in range(0, len(bundle.types_conversion_table)):
                    tct = bundle.types_conversion_table[tct_idx]
                    out_type = type_to_dpcc_type(tct[0])
                    in_types = [type_to_dpcc_type(item) for item in tct[1]]
                    num_in_types = len(in_types)

                    maybe_else = "else " if tct_idx != 0 else ""

                    check_type_list = []
                    for i, _ in enumerate(in_types):
                        check_type_list.append(f"{n}->childs[{i}]->md.type == {in_types[i]}")

                    gprint(f'{maybe_else }if (({n}->num_childs == {num_in_types}) && {gen_logical_and(check_type_list)})')
                    with scope():
                        gprint(f'{n}->md.type = {out_type};')
                gprint("else")
                with scope():
                    gen_err(f"&{n}->tok->loc", '"Types composing this expression cannot be broadcasted."')


def generate_src_file():
    gprint(common_boilerplate)

    gprint('#include "parser.h"')
    gprint('extern int yynerrs;')

    gprint()
    gprint("void typemismatch_check(ast_node_t *expected_type, ast_node_t *got_type)")
    with scope():
        gprint("if (expected_type != NULL && got_type != NULL)")
        with scope():
            gprint("if (expected_type->md.type != got_type->md.type)")
            with scope():
                gen_err("&expected_type->tok->loc", '"Type mismatch"')
                gen_info("&expected_type->tok->loc", '"Expected `%s`\"', ["dpcc_type_as_str(expected_type->md.type)"])
                gen_info("&got_type->tok->loc", '"But got `%s`\"', ["dpcc_type_as_str(got_type->md.type)"])

    gprint()
    gprint('static void deduce_array_type(ast_node_t *n)')
    with scope():
        gprint("ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;")
        gprint("ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;")
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
                gprint('else if (c2 && init_list_len <= 0)')
                with scope():
                    gen_err("&c2->tok->loc", '"Number of elements in initializer list is invalid"')
                    gen_info("&c2->tok->loc", '"Cannot create zero or negative sized array"')

                gprint("// Now make sure that each type in the initializer list is correct")
                gprint("for (int32_t i = 0; i < c2->num_childs; i++)")
                with scope():
                    gprint("if (c2->childs[i] && c2->childs[i]->md.type != expected_type)")
                    with scope():
                        gen_err("&c2->childs[i]->tok->loc", '"Type mismatch in array initializer list"')
                        gen_info("&c0->childs[0]->tok->loc", '"Expected `%s`"', ["dpcc_type_as_str(c0->childs[0]->md.type)"])
                        gen_info("&c2->childs[i]->tok->loc", '"Got `%s`"', ["dpcc_type_as_str(c2->childs[i]->md.type)"])
            gprint("else")
            with scope():
                gprint("// We don't have an initializer list, so make sure that at least the type array is sized")
                gprint("if (c0 == NULL || c0->childs[1] == NULL || c0->childs[1]->kind != TOK_I32_LIT || c0->childs[1]->md.type != TYPE_I32)")
                with scope():
                    gen_err("&c0->tok->loc", '"Size of the array must be specified"')
                    gen_info("&c0->tok->loc", '"Either specify the size inside the square brackets, or provide an initializer list"')

        gprint("// Forward the same type to the keyword let")
        gprint("n->md.type = c0->md.type;")
        gprint("n->md.array_len = init_list_len;")

    gprint()
    gprint('static void type_deduce_expr_and_operators(ast_node_t *n)')
    with scope():
        gen_ops_type_deduction_code("n")

    gprint()
    gprint("static void type_deduce(ast_node_t *n)")
    with scope():
        gprint("// Utilities vars t oeasily access childs")
        gprint("ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;")
        gprint("ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;")

        gprint("// Base cases for type deduction")
        switch("n->kind", {
            "TOK_CHAR_LIT": "n->md.type = TYPE_I32;",
            "TOK_I32_LIT": "n->md.type = TYPE_I32;",
            "TOK_F32_LIT": "n->md.type = TYPE_F32;",
            "TOK_BOOL_LIT": "n->md.type = TYPE_BOOL;",
            "TOK_ID": "if (n->decl) n->md.type = n->decl->md.type;",
        }, default_case='')

        gprint("// Assign the correct type to each casting operator")
        gprint(f'if ({one_of("n->kind", CAST_OPS)} && !{one_of("n->parent->kind", DECL_OPS)})')
        with scope():
            switch("n->kind", {
                "TOK_KW_INT": "n->md.type = TYPE_I32;",
                "TOK_KW_FLOAT": "n->md.type = TYPE_F32;",
                "TOK_KW_BOOL": "n->md.type = TYPE_BOOL;",

            })

        gprint("// Assign the correct type if the var declaration has a user listed type")
        gprint(f'if (c0 && ({one_of("n->kind", DECL_OPS)}))')
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
        gprint(f'if {one_of("n->kind", DECL_OPS)}')
        with scope():
            gprint('// Deduce type of integral variable declarations')
            gprint('if ((c0 == NULL) || (c0->kind != TOK_OPEN_BRACKET))')
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
                gprint('deduce_array_type(n);')

        gprint('// Deduce the type of each identifier used')
        gprint(f'if (n->kind == TOK_ID && n->parent && !{one_of("n->parent->kind", DECL_OPS)})')
        with scope():
            gprint('assert(n->decl != NULL);')
            gprint('assert(n->decl->md.type != TYPE_NONE);')
            gprint('n->md.type = n->decl->md.type;')

        gprint("// Deduce type of expression and operators: This is the most demanding and difficult part")
        gprint('if (n->md.type == 0)')
        with scope():
            gprint('type_deduce_expr_and_operators(n);')

        gprint('// Check sensisble array subscript bounds')
        gprint('if (n->kind == TOK_AR_SUBSCR && n->childs[1]->kind == TOK_I32_LIT && n->childs[1]->md.type == TYPE_I32)')
        with scope():
            gprint('int32_t subscript_idx = n->childs[1]->val.as_i32;')
            gprint('int32_t array_len = n->childs[0]->decl->md.array_len;')
            gprint('if (subscript_idx < 0 || (subscript_idx >= array_len))')
            with scope():
                gen_err("&n->childs[1]->tok->loc", '"Invalid subscript constant"')
                gen_info("&n->childs[0]->decl->tok->loc", '"As specified from declaration index should be in [%d, %d)"', ["0", "array_len"])
                gen_info("&n->childs[1]->tok->loc", '"Got `%d` instead"', ["subscript_idx"])

    gprint()
    gprint("static char *gen_tmp_var(enum DPCC_TYPE type)")
    with scope():

        switch("type", {
            "TYPE_I32": 'return dallfmt(&G_allctx, "__i%d", G_codegen_i32_cnt++);',
            "TYPE_F32": 'return dallfmt(&G_allctx, "__f%d", G_codegen_f32_cnt++);',
            "TYPE_BOOL": 'return dallfmt(&G_allctx, "__b%d", G_codegen_bool_cnt++);',
        })

        gprint('return NULL;')

    gprint()
    gprint('static char *gen_tmp_label()')
    with scope():
        gprint('return dallfmt(&G_allctx, "__l%d", G_codegen_jmp_cnt++);')

    gprint()
    gprint("static void setup_addrs_and_jmp_tables(ast_node_t *n)")
    with scope():
        # gprint("ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;")
        # gprint("ast_node_t *c1 = (n->num_childs >= 2) ? n->childs[1] : NULL;")
        # gprint("ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;")
        # gprint("ast_node_t *c3 = (n->num_childs >= 4) ? n->childs[3] : NULL;")

        gprint('if (!n->md.addr)')
        with scope():
            gprint('// Generate address for all temporary computations performed by operators')
            gprint(f'if ({one_of("n->kind", ALL_OPS)})')
            with scope():
                gprint('assert(n->md.type != TYPE_NONE);')
                gprint('assert(n->md.type != TYPE_I32_ARRAY);')
                gprint('assert(n->md.type != TYPE_F32_ARRAY);')
                gprint('n->md.addr = gen_tmp_var(n->md.type);')

        gprint(f'if ({one_of("n->kind", CONTROL_FLOW_OPS)})')
        with scope():
            gprint('if (n->kind == TOK_KW_WHILE && n->md.jmp_bot == NULL)')
            with scope():
                gprint('n->md.jmp_bot = gen_tmp_label();')
            gprint('else if (n->kind == TOK_KW_DO && n->md.jmp_top == NULL)')
            with scope():
                gprint('n->md.jmp_top = gen_tmp_label();')
            gprint('else if (n->kind == TOK_KW_FOR && n->md.jmp_top == NULL)')
            with scope():
                gprint('n->md.jmp_top = gen_tmp_label();')
            gprint('else if ((n->kind == TOK_KW_IF) && (n->md.jmp_next == NULL || n->md.jmp_bot == NULL))')
            with scope():
                gprint('n->md.jmp_next = gen_tmp_label();')
                gprint('n->md.jmp_bot = gen_tmp_label();')
            gprint('else')
            with scope():
                gprint('invalid_code_path();')

    gprint()
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
                gprint("setup_addrs_and_jmp_tables(n);")

    gprint()
    gprint("static ast_node_t *codegen_traverse_next(ast_traversal_t *t, bool *downside_traversal)")
    with scope():
        gprint('*downside_traversal = false;')
        gprint('if (t->stack_cnt == 0)')
        with scope():
            gprint('return NULL;')

        gprint('while (t->stack_childs[t->stack_cnt - 1] < t->stack_nodes[t->stack_cnt - 1]->num_childs)')
        with scope():
            gprint('int32_t ci = t->stack_childs[t->stack_cnt - 1];')
            gprint('ast_node_t *child = NULL;')

            gprint('while ((ci < t->stack_nodes[t->stack_cnt - 1]->num_childs) && ((child = t->stack_nodes[t->stack_cnt - 1]->childs[ci]) == NULL))')
            with scope():
                gprint('ci++;')
            gprint('if (child)')
            with scope():
                gprint('// Assert that the parent backpointer of the child is indeed correct')
                gprint('assert(child->parent == t->stack_nodes[t->stack_cnt - 1]);')

                gprint('t->stack_childs[t->stack_cnt - 1] = ci + 1;')
                gprint('ast_traversal_push(t, child, 0);')
                gprint('*downside_traversal = true;')
                gprint('return t->stack_nodes[t->stack_cnt - 1];')
            gprint('else')
            with scope():
                gprint('break;')

        gprint('ast_node_t *nvcs = t->stack_nodes[t->stack_cnt - 1];')
        gprint('ast_traversal_pop(t);')

        gprint('if (nvcs->kind == TOK_YYEOF)')
        with scope():
            gprint('return NULL;')

        gprint('return nvcs;')

    gprint()
    gprint('char *codegen(void)')
    with scope():
        gprint("str_t str = {0};")
        gprint('ast_traversal_t att = {0};')
        gprint('ast_traversal_begin(&att);')

        gprint("return str.cstr;")


def generate_hdr_file():
    gprint(common_boilerplate)
    gprint("void check_and_optimize_ast(void);")
    gprint("char *codegen(void);")


def main():

    with open("last_gen.txt", "w") as f:
        f.write(datetime.today().strftime('%Y-%m-%d-%H:%M:%S'))

    gen_list = [
        {"filepath": "src/gen.c", "fn": generate_src_file},
        {"filepath": "src/gen.h", "fn": generate_hdr_file},
    ]

    for v in gen_list:
        os.makedirs(os.path.dirname(v['filepath']), exist_ok=True)
        with open(v['filepath'], 'w') as f:
            sys.stdout = f
            (v['fn'])()


main()
