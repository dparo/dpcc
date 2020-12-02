#!/usr/bin/env node


import * as FS from "fs";
import * as PROC from "process";
import * as OS from "os";
import * as PATH from "path";
import { stringify } from "querystring";


namespace Utils {
    export function as_dpcc_type(s: string): string {
        let map: Record<string, string> = {
            "int": "TYPE_I32",
            "float": "TYPE_F32",
            "bool": "TYPE_BOOL",
            "int[]": "TYPE_I32_ARRAY",
            "float[]": "TYPE_F32_ARRAY",
        };
        return map[s] || "";
    }


    export function array_as_comma_separated_string(a: string[]): string {
        return a.join();
    }

};

namespace Gen {
    let G_indentCnt = 0;
    let DEFAULT_CASE = 'invalid_code_path();';
    class Action {
        v: string | null | (() => any);

        constructor(v: string|null|(() => any)) {
            this.v = v;
        }

        do() {
            if (this.v === null) {
                return;
            }

            if (typeof this.v === "function") {
                this.v();
            } else if (typeof(this.v) === "string") {
                print(this.v);
            } else {
                throw TypeError("Wrong type expected (either string or callable)");
            }
        }
    }


    export let writeStream: FS.WriteStream|null = null;
    function write(s: string) {
        if (writeStream != null) {
            writeStream.write(s);
        } else {
            PROC.stdout.write(s);
        }
    }

    export function scope(callback: () => any) {
        Gen.print("{");
        G_indentCnt += 1;
        callback();
        G_indentCnt -= 1;
        Gen.print("}");
    }


    export function fn(decl:string, body: () => any) {
        Gen.print()
        Gen.print(decl)
        Gen.scope(body)
        Gen.print()
    }

    export function whiled(expr: string, body: () => any) {
        Gen.print(`while (${expr})`)
        Gen.scope(body)
        Gen.print()
    }

    export function print(...args: string[]) {
        if (args.length == 0 || (args.length == 1 && args[0] == "")) {
            write("\n");
        } else {
            let out = "";
            for (let v of args) {
                let lines = v.split('\n');
                for (let l of lines) {
                    let prependNewLine = l.startsWith("if") || l.startsWith("do") || l.startsWith("while") || l.startsWith("switch") || l.startsWith("for");
                    if (prependNewLine) {
                        out += "\n";
                    }
                    out += "    ".repeat(G_indentCnt) + l + "\n";
                }
            }
            write(out);
        }
    }

    export function log_or(...conds: string[]) {
        let result = "";
        for (let i = 0; i < conds.length; i++) {
            if (i < conds.length - 1) {
                result += `(${conds[i]}) || `;
            } else {
                result += `(${conds[i]})`;
            }
        }
        return '(' + result + ')';
    }

    export function log_and(...conds: string[]) {
        let result = "";
        for (let i = 0; i < conds.length; i++) {
            if (i < conds.length - 1) {
                result += `(${conds[i]}) && `;
            } else {
                result += `(${conds[i]})`;
            }
        }
        return '(' + result + ')';
    }

    export function one_of(expr: string, conds: string[]) {
        let result = "";
        for (let i = 0; i < conds.length; i++) {
            if (i < conds.length - 1) {
                result += `(${expr} == ${conds[i]}) || `;
            } else {
                result += `(${expr} == ${conds[i]})`;
            }
        }
        return '(' + result + ')';
    }


    export function ifd(d: Record<string, string|(() => any)>) {
        let i = 0;
        for (let [k, v] of Object.entries(d)) {
            if (k == '') {
                continue;
            }

            let trailing = i == 0 ? '' : 'else ';
            Gen.print(`${trailing}if (${k})`);
            Gen.scope(() => {
                (new Action(v)).do();
            });
            i++;
        }

        if ("" in d) {
            if (d[""] != '') {
                Gen.print("else");
                Gen.scope(() => {
                    (new Action(d[""] || "")).do();
                });
            }
        } else {
            Gen.print("else");
            Gen.scope(() => {
                Gen.print(DEFAULT_CASE);
            });
        }

        Gen.print()
    }


    export function switchd(elem: string, d: Record<string, string|(()=> any)>) {
        let default_case = new Action(d[""] || DEFAULT_CASE);

        Gen.print(`switch (${elem})`);
        Gen.scope(() => {
            Gen.print("default: ")
            Gen.scope(() => {
                default_case.do()
            })
            Gen.print("break;");

            for (let [k, v] of Object.entries(d)) {
                if (k == "") {
                    continue;
                }

                Gen.print(`case ${k}:`);
                Gen.scope(() => {
                    (new Action(v)).do();
                });
                Gen.print("break;");
            }
        });

        Gen.print()
    }

    export function map(elem: string, output: string, d: Record<string, string>) {
        let default_case = d[""] || DEFAULT_CASE;

        Gen.print(`switch (${elem})`);
        Gen.scope(() => {
            if ("" in d) {
                Gen.print("default: ")
                Gen.scope(() => {
                    Gen.print(`${output} = ${default_case}`)
                })
                Gen.print("break;");
            } else {
                Gen.print("default: ")
                Gen.scope(() => {
                    Gen.print(`invalid_code_path();`)
                })
                Gen.print("break;");
            }

            for (let [k, v] of Object.entries(d)) {
                if (k == "") {
                    continue;
                }

                Gen.print(`case ${k}:`);
                Gen.scope(() => {
                    Gen.print(`${output} = ${v};`)
                });
                Gen.print("break; ")
            }

        });
        Gen.print()
    }
}

namespace DPCC {

    class Expr {
        yytokentypes: string[];
        types_conversion_table: ExprTypeRule[];

        constructor(yytokentypes: string[], types_conversion_table: ExprTypeRule[]) {
            this.yytokentypes = yytokentypes;
            this.types_conversion_table = types_conversion_table;
        }
    }

    class ExprTypeRule {
        outType: string;
        inTypes: string[];

        constructor (outType: string, inTypes: string[]) {
            this.outType = outType;
            this.inTypes = inTypes
        }
    }


    export const COMMON_BOILERPLATE = (
`//
// Generated from ${PROC.argv[1]}
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

`
    )

    export namespace TYPES {
        export const INTEGRAL = [
            "TYPE_I32",
            "TYPE_F32",
            "TYPE_BOOL",
        ];

        export const ARRAY = [
            "TYPE_I32_ARRAY",
            "TYPE_F32_ARRAY",
        ];

        export const ALL = [
            ...INTEGRAL,
            ...ARRAY,
        ];
    }


    export namespace OPS {
        export const DECL = [
            "TOK_KW_FN",
            "TOK_KW_LET",
        ];

        export const CAST = [
            "TOK_KW_INT",
            "TOK_KW_FLOAT",
            "TOK_KW_BOOL",
        ];

        export const CONTROL_FLOW = [
            "TOK_KW_IF",
            "TOK_KW_DO",
            "TOK_KW_WHILE",
            "TOK_KW_FOR",
        ]

        export let ALL: string[] = [
            // Initialized later
        ];
    }

    export namespace EXPRS {
        export const INTEGER_EXPR = new Expr(
            [
                "TOK_MOD",
                "TOK_BNOT",
                "TOK_BAND",
                "TOK_BOR",
                "TOK_BXOR",
                "TOK_BLSHIFT",
                "TOK_BRSHIFT",
            ],
            [
                new ExprTypeRule("int", ["int", "int"])
            ]
        );


        export const MATH_EXPR = new Expr (
            [
                "TOK_ASSIGN",
                "TOK_ADD",
                "TOK_SUB",
                "TOK_MUL",
                "TOK_DIV",
                "TOK_POW",
                "TOK_INC",
                "TOK_DEC",
                "TOK_POS",
                "TOK_NEG",
            ],
            [
                new ExprTypeRule("int", ["int", "int"]),
                new ExprTypeRule("float", ["float", "int"]),
                new ExprTypeRule("float", ["int", "float"]),
                new ExprTypeRule("float", ["float", "float"]),

                new ExprTypeRule("int", ["int"]),
                new ExprTypeRule("float", ["float"]),
            ]
        );

        export const LOG_COMPS_EXPR = new Expr (
            [

                "TOK_EQ",
                "TOK_NEQ",
                "TOK_LT",
                "TOK_GT",
                "TOK_GTEQ",
                "TOK_LTEQ",
            ],
            [
                new ExprTypeRule("bool", ["int", "int"]),
                new ExprTypeRule("bool", ["float", "int"]),
                new ExprTypeRule("bool", ["int", "float"]),
                new ExprTypeRule("bool", ["float", "float"]),
            ]
        );

        export const LOG_EXPR = new Expr(
            [
                "TOK_LNOT",
                "TOK_LAND",
                "TOK_LOR",
            ],
            [
                new ExprTypeRule("bool", ["bool", "bool"]),
                new ExprTypeRule("bool", ["bool"]),
            ]
        );


        export const ARRAY_EXPR = new Expr (
            [
                "TOK_AR_SUBSCR",
            ],
            [
                new ExprTypeRule("int", ["int[]", "int"]),
                new ExprTypeRule("float", ["float[]", "int"]),
            ],
        );

        export const ALL = [
            INTEGER_EXPR,
            MATH_EXPR,
            LOG_COMPS_EXPR,
            LOG_EXPR,
            ARRAY_EXPR,
        ];

    }

    export function init() {
        for (let expr of EXPRS.ALL) {
            for (let yytokenstype of expr.yytokentypes) {
                OPS.ALL.push(yytokenstype);
            }
        }

    }
}


function log(severity: string, node: string, fmt: string, ...args: string[]) {
    fmt = "\"" + fmt + "\"";
    let optional_comma = args.length >= 1 ? ", " : "";

    Gen.print(`dpcc_log(${severity}, &((${node})->tok->loc), ${fmt}${optional_comma}${args.join()});`)
}

function err(node: string, fmt: string, ...args: string[]) {
    log('DPCC_SEVERITY_ERROR', node, fmt, ...args);
    Gen.print('yynerrs += 1;')
}

function warn(node: string, fmt: string, ...args: string[]) {
    log('DPCC_SEVERITY_WARNING', node, fmt, ...args);
}

function info(node: string, fmt: string, ...args: string[]) {
    log('DPCC_SEVERITY_INFO', node, fmt, ...args);
}




namespace DPCC_Gen {

    export function vars() {
        Gen.print('static str_t S_str = {0};');
    }
    export function cat(fmt: string, ...args: string[]) {
        fmt = "\"" + fmt + "\"";
        let optional_comma = args.length >= 1 ? ", " : "";
        Gen.print(`sfcat(&G_allctx, &S_str, ${fmt}${optional_comma}${args.join()});`)
    }
    export function new_tmp_var() {
        Gen.fn('static char *new_tmp_var(enum DPCC_TYPE type)', () => {
            Gen.print('str_t s = {0};')
            Gen.switchd('type', {
                "TYPE_I32": 'sfcat(&G_allctx, &s, "__i%d", G_codegen_i32_cnt++);',
                "TYPE_F32": 'sfcat(&G_allctx, &s, "__f%d", G_codegen_f32_cnt++);',
                "TYPE_BOOL": 'sfcat(&G_allctx, &s, "__b%d", G_codegen_bool_cnt++);',
            })
            Gen.print('return s.cstr;')
        })
    }

    export function new_tmp_label() {
        Gen.fn('static char *new_tmp_label()', () => {
            Gen.print('str_t s = {0};')
            Gen.print('sfcat(&G_allctx, &s, "__l%d", G_codegen_jmp_cnt++);')
            Gen.print('return s.cstr;')
        })
    }

    export function is_expr_node() {
        Gen.fn('static inline bool is_expr_node(ast_node_t *n)', () => {
            let token_types: string[] = [];
            DPCC.EXPRS.ALL.map((expr) => expr.yytokentypes.map((s) => token_types.push(s)))
            Gen.print(`return (${Gen.one_of("n->kind", token_types)});`);
        })
    }

    export function typemismatch_check() {
        Gen.fn('static inline void typemismatch_check(ast_node_t *expected_type, ast_node_t *got_type)', () => {
            Gen.ifd({
                '': '',
                '(expected_type != NULL && got_type != NULL) && (expected_type->md.type != got_type->md.type)': () => {
                    err("expected_type", "Type Mismatch");
                    info("expected_type", "Expected `%s`", "dpcc_type_as_str(expected_type->md.type)");
                    info("got_type", "But got `%s`", "dpcc_type_as_str(got_type->md.type)")
                }
            })
        })
    }

    export function typecheck_expr_and_operators() {
        Gen.fn('static void typecheck_expr_and_operators(ast_node_t *n)', () => {
            Gen.print(`if (${Gen.one_of("n->kind", DPCC.OPS.ALL)})`)
            Gen.scope(() => {
                for (let expr of DPCC.EXPRS.ALL) {
                    let all_yytokenstypes = []
                    for (let yytokenstype of expr.yytokentypes) {
                        all_yytokenstypes.push(yytokenstype)
                    }
                    Gen.print(`if (${Gen.one_of(`n->kind`, all_yytokenstypes)})`)
                    Gen.scope( () => {
                        for (let tct_idx = 0; tct_idx < expr.types_conversion_table.length; tct_idx++) {
                            let tct = expr.types_conversion_table[tct_idx]!

                            let out_type = Utils.as_dpcc_type(tct.outType)
                            let in_types = tct.inTypes.map((item) => Utils.as_dpcc_type(item))
                            let num_in_types = in_types.length

                            let maybe_else = tct_idx == 0 ? "" : "else "

                            let check_type_list: string[] = []
                            for (let i = 0; i < in_types.length; i++) {
                                check_type_list.push(`n->childs[${i}]->md.type == ${in_types[i]}`)
                            }

                            Gen.print(`${maybe_else}if ((n->num_childs == ${num_in_types}) && ${Gen.log_and(...check_type_list)})`)
                            Gen.scope(() => {
                                Gen.print(`n->md.type = ${out_type};`)
                            })
                        }
                        Gen.print("else")
                        Gen.scope(() => {
                            err(`n`, "Types composing this expression cannot be broadcasted");
                        })
                    })
                }
            })
        })

    }


    export function check_array_initializer_list() {
        Gen.fn('static void check_initializer_list(ast_node_t *n, enum DPCC_TYPE expected_type, int32_t array_type_len, int32_t init_list_len)', () => {
            Gen.print("ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;")
            Gen.print("ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;")

            Gen.ifd({
                '(c0->childs[1] && init_list_len != array_type_len)': () => {
                    err("c2", "Number of elements in initializer list do not match")
                    info("c0->childs[1]", "Expected number of elements: `%d`", 'array_type_len')
                    info("c2", "Number of elements got: `%d`", 'init_list_len')
                },
                '(c2 && init_list_len <= 0)': () => {
                    err("c2", "Number of elements in initializer list is invalid")
                    info("c2", "Cannot create zero or negative sized array")
                },
                '': ''
            })

            // Now make sure that each type in the initializer list is correct
            Gen.print("for (int32_t i = 0; i < c2->num_childs; i++)")
            Gen.scope(() => {
                Gen.ifd({
                    '(c2->childs[i] && c2->childs[i]->md.type != expected_type)': () => {
                        err("c2->childs[i]", "Type mismatch in array initializer list")
                        info("c0->childs[0]", "Expected `%s`", "dpcc_type_as_str(c0->childs[0]->md.type)")
                        info("c2->childs[i]", "Got `%s`", "dpcc_type_as_str(c2->childs[i]->md.type)")
                    },
                    '': ''
                })
            })
        })
    }

    export function typecheck_array() {
        Gen.fn("static void typecheck_array(ast_node_t *n)", () => {
            Gen.print("ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;")
            Gen.print("ast_node_t *c1 = (n->num_childs >= 2) ? n->childs[1] : NULL;")
            Gen.print("ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;")

            Gen.print("// Deduce type of array variable declarations")
            Gen.print("assert((c0->childs[1] == NULL) || (c0->childs[1]->kind == TOK_I32_LIT && c0->childs[1]->md.type == TYPE_I32));")

            Gen.print("int32_t array_type_len = c0->childs[1] ? c0->childs[1]->val.as_i32 : 0;")
            Gen.print("int32_t init_list_len = c2 != NULL ? c2->num_childs : 0;")


            Gen.ifd({
                'c0->childs[1] && array_type_len <= 0': () => {
                    err("c0->childs[1]", "The number of elements in an array must be a positive integer")
                    info("c0->childs[1]", "Got `%d`", 'array_type_len')
                },

                // Initializer list is provided
                'c2': () => {
                    Gen.print("enum DPCC_TYPE expected_type = TYPE_NONE;")
                    Gen.map('c0->md.type', 'expected_type', {
                        'TYPE_I32_ARRAY': "TYPE_I32",
                        'TYPE_F32_ARRAY': "TYPE_F32",
                    })

                    Gen.print("check_initializer_list(n, expected_type, array_type_len, init_list_len);");
                },

                // We don't have an initializer list, so make sure that at least the type array is sized
                '(c0 == NULL || c0->childs[1] == NULL || c0->childs[1]->kind != TOK_I32_LIT || c0->childs[1]->md.type != TYPE_I32)': () => {
                    err("c0", "Size of the array must be specified")
                    info("c0", "Either specify the size inside the square brackets, or provide an initializer list")
                },

                '': '',
            })
        })
    }


    export function typecheck() {
        Gen.fn('static void typecheck(ast_node_t *n)', () => {
            Gen.print("ast_node_t *c0 = (n->num_childs >= 1) ? n->childs[0] : NULL;")
            Gen.print("ast_node_t *c1 = (n->num_childs >= 2) ? n->childs[1] : NULL;")
            Gen.print("ast_node_t *c2 = (n->num_childs >= 3) ? n->childs[2] : NULL;")

            Gen.print(`bool is_casting_operator = ${Gen.one_of("n->kind", DPCC.OPS.CAST)} && ! ${Gen.one_of("n->parent->kind", DPCC.OPS.DECL)};`)
            Gen.print(`bool var_decl = (${Gen.one_of("n->kind", DPCC.OPS.DECL)});`)
            Gen.print(`bool var_decl_with_user_listed_type = var_decl && c0;`)
            Gen.print(`bool var_decl_no_user_listed_type = var_decl && !c0;`)
            Gen.print(`bool integral_var_decl = var_decl && (!c0 || c0->kind != TOK_OPEN_BRACKET);`)
            Gen.print('bool var_decl_with_rhs = var_decl && c2;')
            Gen.print(`bool array_var_decl = var_decl && c0 && c0->kind == TOK_OPEN_BRACKET;`)
            Gen.print(`bool array_var_decl_num_elems_provided = array_var_decl && c1;`)
            Gen.print(`bool is_typable_identifier = n->kind == TOK_ID && n->parent && !${Gen.one_of("n->parent->kind", DPCC.OPS.DECL)};`)
            Gen.print(`bool is_array_subscript = n->kind == TOK_AR_SUBSCR && n->childs[1]->kind == TOK_I32_LIT && n->childs[1]->md.type == TYPE_I32;`)

            Gen.ifd({
                // If not matched do not do anything
                '': '',
                // Base cases for type deduction
                "n->kind == TOK_CHAR_LIT": "n->md.type = TYPE_I32;",
                "n->kind == TOK_I32_LIT": "n->md.type = TYPE_I32;",
                "n->kind == TOK_F32_LIT": "n->md.type = TYPE_F32;",
                "n->kind == TOK_BOOL_LIT": "n->md.type = TYPE_BOOL;",
                "n->kind == TOK_ID": "if (n->decl) n->md.type = n->decl->md.type;",

                // For every other ast node process only if its type is not yet set
                'n->md.type == TYPE_NONE': () => Gen.ifd({
                    // Casting operators
                    'is_casting_operator': () => Gen.switchd("n->kind", {
                        "TOK_KW_INT": "n->md.type = TYPE_I32;",
                        "TOK_KW_FLOAT": "n->md.type = TYPE_F32;",
                        "TOK_KW_BOOL": "n->md.type = TYPE_BOOL;",
                    }),

                    'var_decl': () => {

                        // Extract init_val_len from the initializer list
                        // - Integral types are assumed to be an array of lenght 1,
                        //   which does not support subscript operator

                        Gen.print('ast_node_t *type = c0 ? c0 : NULL;')
                        Gen.print('ast_node_t *rhs = c2 ? c2 : NULL;')
                        Gen.print('int32_t init_val_len = 1;')
                        Gen.ifd({
                            'array_var_decl && c2 && c2->kind == TOK_OPEN_BRACE': 'init_val_len = c2->num_childs;',
                            'array_var_decl && !c2': () => err("n", "Expected valid initializer list for array var decls"),
                            '': ''
                        })

                        Gen.ifd({
                            'array_var_decl': () => {
                                Gen.ifd({
                                    '': 'invalid_code_path();',

                                    'var_decl_with_user_listed_type': () => {
                                        Gen.map("c0->childs[0]->kind", "c0->md.type", {
                                            'TOK_KW_INT': 'TYPE_I32_ARRAY;',
                                            'TOK_KW_FLOAT': 'TYPE_F32_ARRAY;',
                                        })
                                        Gen.print('typecheck_array(n);')
                                    },
                                    'var_decl_no_user_listed_type': () => {
                                        err("n", "Expected valid initializer list for array var decls")
                                    },
                                })
                            },
                            'integral_var_decl': () => {
                                Gen.ifd({
                                    '': 'invalid_code_path();',
                                    'var_decl_no_user_listed_type && !var_decl_with_rhs': () => {
                                        // Assume integer
                                        Gen.print('n->md.type = TYPE_I32;')
                                    },
                                    // Integral var decl with user listed type
                                    'var_decl_with_user_listed_type': () => Gen.map("c0->kind", "c0->md.type", {
                                        'TOK_KW_INT': 'TYPE_I32',
                                        'TOK_KW_FLOAT': 'TYPE_F32',
                                        'TOK_KW_BOOL': 'TYPE_BOOL',
                                    }),
                                    // User listed type and right hand side: Type check
                                    'var_decl_with_user_listed_type && var_decl_with_rhs': () => {
                                        Gen.print('typemismatch_check(type, rhs);')
                                    },
                                    // Type deduce from RHS
                                    'var_decl_no_user_listed_type && var_decl_with_rhs': () => {
                                        Gen.print('n->md.type = rhs->md.type;')
                                    },
                                })
                            },
                        })

                        // For every var decl forward the computed from the child
                        Gen.print('n->md.array_len = init_val_len;')
                    },

                    'is_typable_identifier': () => {
                        Gen.print('assert(n->decl != NULL);')
                        Gen.print('assert(n->decl->md.type != TYPE_NONE);')
                        Gen.print('n->md.type = n->decl->md.type;')
                    },

                    'is_array_subscript': () => {
                        Gen.print('int32_t subscript_idx = n->childs[1]->val.as_i32;')
                        Gen.print('int32_t array_len = n->childs[0]->decl->md.array_len;')
                        Gen.ifd({
                            '': '',
                            'n->childs[0]->md.type != TYPE_I32_ARRAY && n->childs[0]->md.type != TYPE_F32_ARRAY': () => {
                                err("n->childs[0]", "Identifier is not an array")
                                info("n->childs[0]->decl", "Previous declaration was here (type: %s)", "dpcc_type_as_str(n->childs[0]->md.type)")
                            },
                            'subscript_idx < 0 || (subscript_idx >= array_len)': () => {
                                err("n->childs[1]", "Invalid subscript constant")
                                info("n->childs[0]->decl", "As specified from declaration index should be in [%d, %d)", "0", "array_len")
                                info("n->childs[1]", "Got `%d` instead", "subscript_idx")
                            }

                        })
                    },
                    '': () => {
                        Gen.print('typecheck_expr_and_operators(n);')
                    }
                })


            })
        })
    }

    export function setup_addrs_and_jmp_tables() {
        Gen.fn('static void setup_addrs_and_jmp_tables(ast_node_t *n)', () => {
            Gen.ifd({
                // Generate addr tmp_var name for each expr node
                '!n->md.addr && (n->md.type != TYPE_NONE && is_expr_node(n))': () => {
                    Gen.print('assert(n->decl->md.type == n->md.type);')
                    Gen.print('assert(n->md.type != TYPE_I32_ARRAY);')
                    Gen.print('assert(n->md.type != TYPE_F32_ARRAY);')
                    Gen.print('n->md.addr = new_tmp_var(n->md.type);')
                },
                // Labels for while
                'n->kind == TOK_KW_WHILE && n->md.jmp_bot == NULL': 'n->md.jmp_bot = new_tmp_label();',
                // Labels for do / while
                'n->kind == TOK_KW_DO && n->md.jmp_top == NULL': 'n->md.jmp_top = new_tmp_label();',
                // Labels for for
                'n->kind == TOK_KW_FOR && n->md.jmp_top == NULL': 'n->md.jmp_top = new_tmp_label();',
                // Labels for if
                '(n->kind == TOK_KW_IF) && (n->md.jmp_next == NULL || n->md.jmp_bot == NULL)': () => {
                    Gen.print('n->md.jmp_next = new_tmp_label();')
                    Gen.print('n->md.jmp_bot = new_tmp_label();')
                },

            })

        })
    }

    // Setup, type checking, type deduction, and genError reporting
    export function first_ast_pass() {
        Gen.fn('static void first_ast_pass(void)', () => {
            Gen.print("ast_traversal_t att = {0};")
            Gen.print("ast_traversal_begin(&att, &G_root_node, false, true);")
            Gen.print("ast_node_t *n = NULL;")
            Gen.whiled("(n = ast_traverse_next(&att, NULL)) != NULL", () => {
                Gen.ifd({
                    'n->md.type == TYPE_NONE': 'typecheck(n);',
                    '': 'setup_addrs_and_jmp_tables(n);'
                })
            })
        })
    }

    // Actually produces the output code
    export function second_ast_pass() {
        Gen.fn('static char *second_ast_pass(void)', () => {
            Gen.print('memset(&S_str, 0, sizeof(S_str));')
            cat("\\n");
            Gen.print()

            Gen.print('ast_traversal_t att = {0};')
            Gen.print('ast_traversal_begin(&att, &G_root_node, true, true);')
            Gen.print('ast_node_t *n = NULL;')
            Gen.print('bool is_top_down_encounter = false;')

            Gen.whiled("(n = ast_traverse_next(&att, &is_top_down_encounter)) != NULL", () => {

            })


            Gen.ifd({
                'yynerrs == 0': 'return S_str.cstr;',
                '': 'return NULL;'
            })
        })
    }


    export function codegen() {
        Gen.fn('char *codegen(void)', () => {
            Gen.print('first_ast_pass();')
            Gen.ifd({
                'yynerrs == 0': 'return second_ast_pass();',
                '': '',
            })
            Gen.print('return NULL;')
        })
    }

}



function generate_src_file() {
    Gen.print(DPCC.COMMON_BOILERPLATE);

    Gen.print('#include "gen.h"')
    Gen.print('#include "parser.h"')

    Gen.print('\n\n')
    Gen.print('extern int yynerrs;')
    DPCC_Gen.vars()
    Gen.print('\n\n')

    DPCC_Gen.is_expr_node()
    DPCC_Gen.new_tmp_var()
    DPCC_Gen.new_tmp_label()
    DPCC_Gen.typemismatch_check()
    DPCC_Gen.check_array_initializer_list()
    DPCC_Gen.typecheck_expr_and_operators()

    DPCC_Gen.typecheck_array()
    DPCC_Gen.typecheck();
    DPCC_Gen.setup_addrs_and_jmp_tables()

    DPCC_Gen.first_ast_pass()
    DPCC_Gen.second_ast_pass()
    DPCC_Gen.codegen()
}


function generate_hdr_file() {
    Gen.print(DPCC.COMMON_BOILERPLATE);
    Gen.print("char *codegen(void);")
}

function main() {
    DPCC.init();

    let fd = FS.openSync("last_gen.txt", "w");
    FS.writeSync(fd, new Date().toISOString());
    FS.closeSync(fd);


    let genList = [
        {"filepath": "src/gen.c", "fn": generate_src_file},
        {"filepath": "src/gen.h", "fn": generate_hdr_file},
    ];

    for (let [_, v] of Object.entries(genList)) {
        let ws: FS.WriteStream|null = null;

        try {
            FS.mkdirSync(PATH.dirname(v.filepath), {recursive: true});
            ws = FS.createWriteStream(v.filepath);
            Gen.writeStream = ws;
            v.fn()
        } finally {
            if (ws != null) {
                ws.end();
            }
        }
    }
}

main();
