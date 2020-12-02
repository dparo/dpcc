#!/usr/bin/env python3

import os
import sys
from datetime import datetime
import itertools




class TypeDeduceRule:
    def __init__(self, rule: str, block: callable):
        self.rule = rule
        self.block = block

class TypeDeduceRules:
    def __init__(self, rules: list, default_case: str):
        self.rules = rules
        self.default_case = default_case

    def gen(self):
        d = {}
        for i, r in enumerate(self.rules):
            d[r.rule] = r.block
        gif(d)


def array_decl_type_deduce():
    gprint('n->md.array_len = 6969;')

TYPE_DEDUCE_RULES = {
    # If none of below: not possible. Invalid code path
    '': lambda: gprint('invalid_code_path();'),

    # Base cases for type deduction
    'n->kind == TOK_CHAR_LIT': lambda: gprint('n->md.type = TYPE_I32;'),
    'n->kind == TOK_I32_LIT': lambda: gprint('n->md.type = TYPE_I32;'),
    'n->kind == TOK_F32_LIT': lambda: gprint('n->md.type = TYPE_F32;'),
    'n->kind == TOK_ID': lambda: gprint('if (n->decl) { n->md.type = n->decl.md.type; }'),

    # Deduce types for casting operators
    f'{one_of("n->kind", CAST_OPS)}': lambda: (
        gswitch('n->kind', {
            'TOK_KW_INT': 'n->md.type = TYPE_I32;',
            'TOK_KW_FLOAT': 'n->md.type = TYPE_F32;',
            'TOK_KW_BOOL': 'n->md.type = TYPE_BOOL;',
        }),
    ),

    # Assign correct type for user listed integral var decl
    f'c0 && c0->kind != TOK_OPEN_BRACKET && ({one_of("n->kind", DECL_OPS)})': lambda: (
        gswitch('c0->kind', {
            'TOK_KW_INT': 'c0->md.type = TYPE_I32',
            'TOK_KW_FLOAT': 'c0->md.type = TYPE_F32',
            'TOK_KW_BOOL': 'c0->md.type = TYPE_BOOL',
        }),
        gprint('n->md.type = c0->md.type;'),
        gprint('n->md.array_len = 1;'),
        gprint('if (c2)'),
        with scope():
            gprint('// If the user has provided a RHS for the variable declaration, we must check for typemismatches'),
            gprint('typemismatch_check(c0, c2);'),
    ),

    # Assign correct type for user listed array var decl
    f'c0 && c0->kind == TOK_OPEN_BRACKET && ({one_of("n->kind", DECL_OPS)})': lambda: (
        gswitch('c0->kind', {
            'TOK_KW_INT': 'c0->md.type = TYPE_I32_ARRAY',
            'TOK_KW_FLOAT': 'c0->md.type = TYPE_F32_ARRAY',
        }),
        gprint('n->md.type = c0->md.type;'),
        array_decl_type_deduce(),
    ),

    # No user provided type, must deduce it.
    f'{one_of("n->kind", DECL_OPS)}': lambda: (
        gprint("assert(n->num_childs == 3 && c2 != NULL);"),
        gif({
            'c2->kind == OPEN_BRACKET': lambda: gen_err("&c2->tok->loc", "Type deduction for array initializer list is ambiguous")
            'c2->md.type == TYPE_NONE': lambda: gen_err("&c2->tok->loc", "Cannot deduce type from the given RHS value")
            '': "n->md.type = c2->md.type"
        })
    ),



    f'{one_of("n->kind", ALL_OPS)}', lambda: (
        gen_type_deduce_expr_and_operators()
    ),
}


def gen_type_deduce():
    gprint()
    gprint("static void type_deduce(ast_node_t *n)")
    with scope():
        gprint("// Utilities vars t oeasily access childs")
        gprint("ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;")
        gprint("ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;")

        gprint("// Base cases for type deduction")
        gswitch("n->kind", {
            '': '',
            "TOK_CHAR_LIT": "n->md.type = TYPE_I32;",
            "TOK_I32_LIT": "n->md.type = TYPE_I32;",
            "TOK_F32_LIT": "n->md.type = TYPE_F32;",
            "TOK_BOOL_LIT": "n->md.type = TYPE_BOOL;",
            "TOK_ID": "if (n->decl) n->md.type = n->decl->md.type;",
        })

        gprint("// Assign the correct type to each casting operator")
        gprint(f'if ({one_of("n->kind", CAST_OPS)} && !{one_of("n->parent->kind", DECL_OPS)})')
        with scope():
            gswitch("n->kind", {
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
                gswitch('c0->kind', {
                    'TOK_KW_INT': 'c0->md.type = TYPE_I32;',
                    'TOK_KW_FLOAT': 'c0->md.type = TYPE_F32;',
                    'TOK_KW_BOOL': 'c0->md.type = TYPE_BOOL;',
                })
            gprint("else if (c0->md.type == TYPE_NONE && c0->kind == TOK_OPEN_BRACKET)")
            with scope():
                gprint("// Handle array types")
                gswitch("c0->childs[0]->kind", {
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
            gprint('if (n->childs[0]->md.type != TYPE_I32_ARRAY && n->childs[0]->md.type != TYPE_F32_ARRAY)')
            with scope():
                gen_err("&n->childs[0]->tok->loc", '"Identifier is not an array"')
                gen_info("&n->childs[0]->decl->tok->loc", '"Previous declaration was here (type: %s)"', ["dpcc_type_as_str(n->childs[0]->md.type)"])
            gprint('else if (subscript_idx < 0 || (subscript_idx >= array_len))')
            with scope():
                gen_err("&n->childs[1]->tok->loc", '"Invalid subscript constant"')
                gen_info("&n->childs[0]->decl->tok->loc", '"As specified from declaration index should be in [%d, %d)"', ["0", "array_len"])
                gen_info("&n->childs[1]->tok->loc", '"Got `%d` instead"', ["subscript_idx"])


def gen_setup_addrs_and_jmp_table():
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
            gprint(f'if (n->md.type != TYPE_NONE && {one_of("n->kind", ALL_OPS)})')
            with scope():
                gprint('assert(n->md.type != TYPE_I32_ARRAY);')
                gprint('assert(n->md.type != TYPE_F32_ARRAY);')
                gprint('n->md.addr = new_tmp_var(n->md.type);')

        gprint(f'if ({one_of("n->kind", CONTROL_FLOW_OPS)})')
        with scope():
            gprint('if (n->kind == TOK_KW_WHILE && n->md.jmp_bot == NULL)')
            with scope():
                gprint('n->md.jmp_bot = new_tmp_label();')
            gprint('else if (n->kind == TOK_KW_DO && n->md.jmp_top == NULL)')
            with scope():
                gprint('n->md.jmp_top = new_tmp_label();')
            gprint('else if (n->kind == TOK_KW_FOR && n->md.jmp_top == NULL)')
            with scope():
                gprint('n->md.jmp_top = new_tmp_label();')
            gprint('else if ((n->kind == TOK_KW_IF) && (n->md.jmp_next == NULL || n->md.jmp_bot == NULL))')
            with scope():
                gprint('n->md.jmp_next = new_tmp_label();')
                gprint('n->md.jmp_bot = new_tmp_label();')
            gprint('else')
            with scope():
                gprint('invalid_code_path();')


def gen_check_and_optimize_ast():
    gprint()
    gprint("void check_and_optimize_ast(void)")
    with scope():
        gprint("ast_traversal_t att = {0};")
        gprint("ast_traversal_begin(&att, &G_root_node, false, true);")
        with scope():
            gprint("ast_node_t *n = NULL;")
            gprint("while ((n = ast_traverse_next(&att, NULL)) != NULL)")
            with scope():
                gprint('if (n->md.type == TYPE_NONE)')
                with scope():
                    gprint("type_deduce(n);")
                gprint("setup_addrs_and_jmp_tables(n);")


def gen_codegen_expr():
    gprint()
    gprint('void codegen_expr(str_t *str, ast_node_t *root)')
    with scope():
        gprint('(void) str, (void) root;')
        gprint(f'assert(is_expr_node(root));')
        gprint('ast_traversal_t att = {0};')
        gprint('ast_traversal_begin(&att, root, false, true);')

        gprint('ast_node_t *n = NULL;')
        gprint("while ((n = ast_traverse_next(&att, NULL)) != NULL)")
        with scope():
            gprint('if (n->kind == TOK_ASSIGN)')
            with scope():
                gprint('// Extra care here, we need to generate the proper code to assign to user declared identifiers')
            gprint('else if (n->kind == TOK_OPEN_BRACKET)')
            with scope():
                pass
            gprint('else')
            with scope():
                pass

def gen_codegen():
    gprint()
    gprint('char *codegen(void)')
    with scope():
        gprint("str_t str = {0};")
        gprint('ast_traversal_t att = {0};')
        gprint('ast_traversal_begin(&att, &G_root_node, true, true);')

        gprint('sfcat(&G_allctx, &str, "\\n");')
        gprint('ast_node_t *n = NULL;')
        gprint('bool is_top_down_encounter = false;')
        gprint("while ((n = ast_traverse_next(&att, &is_top_down_encounter)) != NULL)")
        with scope():
            gprint('ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;')
            gprint('ast_node_t *c1 = (n->num_childs >= 2) ? n->childs[1] : NULL;')
            gprint('ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;')
            gprint('ast_node_t *c3 = (n->num_childs >= 4) ? n->childs[3] : NULL;')
            gprint('(void) c0; (void) c1; (void) c2; (void) c3;')
            gprint('if (n->kind == TOK_KW_MAIN && n->childs[0]->kind == TOK_OPEN_BRACE)')
            with scope():
                gprint('// Don\'t need to do anything, can just skip')
            gprint('else if (n->kind == TOK_SEMICOLON && n->childs[0]->kind == TOK_KW_IF)')
            with scope():
                pass
            gprint('else if (n->kind == TOK_SEMICOLON && n->childs[0]->kind == TOK_KW_WHILE)')
            with scope():
                pass
            gprint('else if (n->kind == TOK_SEMICOLON && n->childs[0]->kind == TOK_KW_DO)')
            with scope():
                pass
            gprint('else if (n->kind == TOK_SEMICOLON && n->childs[0]->kind == TOK_KW_FOR)')
            with scope():
                pass
            gprint('else if (n->kind == TOK_SEMICOLON && n->childs[0]->kind == TOK_KW_LET)')
            with scope():
                gprint('if (is_top_down_encounter)')
                with scope():
                    gprint('ast_node_t *self = n->childs[0];')
                    gprint('ast_node_t *c0 = (self->num_childs >= 1) ? self->childs[0] : NULL;')
                    gprint('ast_node_t *c1 = (self->num_childs >= 2) ? self->childs[1] : NULL;')
                    gprint('ast_node_t *c2 = (self->num_childs >= 3) ? self->childs[2] : NULL;')
                    gprint('ast_node_t *c3 = (self->num_childs >= 4) ? self->childs[3] : NULL;')
                    gprint('(void) c0; (void) c1; (void) c2; (void) c3;')

                    gprint('sfcat(&G_allctx, &str,"decl_var(%s, \\"%s\\", %d, ", dpcc_type_as_enum_str(self->md.type), c1->tok->lexeme, c1->md.array_len);')

                    gswitch("self->md.type", {
                        "TYPE_I32": 'sfcat(&G_allctx, &str, "(int32_t[]) {");',
                        "TYPE_I32_ARRAY": 'sfcat(&G_allctx, &str, "(int32_t[]) {");',
                        "TYPE_F32": 'sfcat(&G_allctx, &str, "(float[]) {");',
                        "TYPE_F32_ARRAY": 'sfcat(&G_allctx, &str, "(float[]) {");',
                        "TYPE_BOOL": 'sfcat(&G_allctx, &str, "(bool[]) {");',
                        })

                    gprint('sfcat(&G_allctx, &str, "});\\n");')

            gprint('else if (n->kind == TOK_SEMICOLON && n->childs[0]->kind == TOK_KW_PRINT)')
            with scope():
                pass
            gprint('else if (n->kind == TOK_SEMICOLON)')
            with scope():
                pass
            gprint('else if (n->kind == TOK_OPEN_BRACE)')
            with scope():
                gprint('if (is_top_down_encounter)')
                with scope():
                    gprint('sfcat(&G_allctx, &str, "push();\\n");')
                gprint('else')
                with scope():
                    gprint('sfcat(&G_allctx, &str, "pop();\\n");')

            gprint(f'else if (is_expr_node(n))')
            with scope():
                gprint('codegen_expr(&str, n);')
                gprint('ast_traversal_pop(&att);')
                pass
            gprint('else')
            with scope():
                gprint('// invalid_code_path();')


        gprint('sfcat(&G_allctx, &str, "\\n");')
        gprint("return str.cstr;")


def generate_src_file():
    gprint(common_boilerplate)

    gprint('#include "parser.h"')
    gprint('extern int yynerrs;')

    gen_typemismatch_check()
    gen_deduce_array_type()

    gen_is_expr_node()
    gen_type_deduce_expr_and_operators("n")
    gen_type_deduce()
    gen_new_tmp_var()
    gen_new_tmp_label()
    gen_setup_addrs_and_jmp_table()
    gen_check_and_optimize_ast()
    gen_codegen_expr()
    gen_codegen()





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
