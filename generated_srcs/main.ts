import * as FS from "fs";
import * as PROC from "process";
import * as PATH from "path";


namespace Utils {
    export function as_dpcc_type(s: string): string {
        let map: Record<string, string> = {
            "int": "TYPE_I32",
            "float": "TYPE_F32",
            "bool": "TYPE_BOOL",
            "bool[]": "TYPE_BOOL_ARRAY",
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
            Gen.print("default:")
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
                Gen.print("default:")
                Gen.scope(() => {
                    Gen.print(`${output} = ${default_case}`)
                })
                Gen.print("break;");
            } else {
                Gen.print("default:")
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
                Gen.print("break;")
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

        export let ALL: string[] = [
            // Initialized later
        ];

        export let PREFIX: string[] = [
            "ExprPos",
            "ExprNeg",
            "ExprBNot",
            "ExprLNot"
        ];

        export let POSTFIX: string[] = [
            "ExprInc",
            'ExprDec',
        ]
    }

    export namespace EXPRS {
        export const INTEGER_EXPR = new Expr(
            [
                "ExprMod",
                "ExprBNot",
                "ExprBAnd",
                "ExprBOr",
                "ExprBXor",
                "ExprBLShift",
                "ExprBRShift",
            ],
            [
                new ExprTypeRule("int", ["int"]),
                new ExprTypeRule("int", ["int", "int"]),
            ]
        );

        export const ASSIGN_EXPR = new Expr (
            [
                "ExprAssign",
            ],
            [
                new ExprTypeRule("int", ["int", "int"]),
                new ExprTypeRule("float", ["float", "float"]),
                new ExprTypeRule("bool", ["bool", "bool"]),
            ]
        );

        export const MATH_EXPR = new Expr (
            [
                "ExprAdd",
                "ExprSub",
                "ExprMul",
                "ExprDiv",
                "ExprPow",
                "ExprInc",
                "ExprDec",
                "ExprPos",
                "ExprNeg",
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


        export const EQ_EQ_EXPR = new Expr (
            [

                "ExprEq",
            ],
            [
                new ExprTypeRule("bool", ["int", "int"]),
            ]
        );

        export const LOG_COMPS_EXPR = new Expr (
            [

                "ExprNeq",
                "ExprLt",
                "ExprGt",
                "ExprGtEq",
                "ExprLtEq",
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
                "ExprLNot",
                "ExprLAnd",
                "ExprLOr",
            ],
            [
                new ExprTypeRule("bool", ["bool"]),
                new ExprTypeRule("bool", ["bool", "bool"]),
            ]
        );


        export const ARRAY_EXPR = new Expr (
            [
                "ExprArraySubscript",
            ],
            [
                new ExprTypeRule("bool", ["bool[]", "int"]),
                new ExprTypeRule("int", ["int[]", "int"]),
                new ExprTypeRule("float", ["float[]", "int"]),
            ],
        );

        export const ALL = [
            INTEGER_EXPR,
            ASSIGN_EXPR,
            MATH_EXPR,
            EQ_EQ_EXPR,
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

function printf(fmt: string, ...args: string[]) {
    fmt = "\"" + fmt + "\"";
    let optional_comma = args.length >= 1 ? ", " : "";
    Gen.print(`printf(${fmt}${optional_comma}${args.join()});`)
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

    export function is_expr_node() {
        Gen.fn('bool is_expr_node(ast_node_t *n)', () => {
            let token_types: string[] = [];
            DPCC.EXPRS.ALL.map((expr) => expr.yytokentypes.map((s) => token_types.push(s)))
            Gen.print(`return (${Gen.one_of("n->kind", token_types)});`);
        })
    }

    export function is_prefix_op() {
        Gen.fn('bool is_prefix_op(ast_node_t *n)', () => {
            Gen.print(`return (${Gen.one_of("n->kind", DPCC.OPS.PREFIX)});`);
        })
    }

    export function is_postfix_op() {
        Gen.fn('bool is_postfix_op(ast_node_t *n)', () => {
            Gen.print(`return (${Gen.one_of("n->kind", DPCC.OPS.POSTFIX)});`);
        })
    }

    export function typecheck_expr_and_operators() {
        Gen.fn('void typecheck_expr_and_operators(ast_node_t *n)', () => {
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


}



function generate_src_file() {
    Gen.print(DPCC.COMMON_BOILERPLATE);

    Gen.print('#include "__gen.h"')

    Gen.print('\n\n')
    Gen.print('extern int yynerrs;')
    Gen.print('\n\n')


    DPCC_Gen.is_prefix_op();
    DPCC_Gen.is_postfix_op();
    DPCC_Gen.is_expr_node();

    DPCC_Gen.typecheck_expr_and_operators()
}


function generate_hdr_file() {
    Gen.print(DPCC.COMMON_BOILERPLATE);
    Gen.print()
    Gen.print("bool is_prefix_op(ast_node_t *n);")
    Gen.print("bool is_postfix_op(ast_node_t *n);")
    Gen.print("bool is_expr_node(ast_node_t *n);")
    Gen.print()
    Gen.print('void typecheck_expr_and_operators(ast_node_t *n);')
}

function CHexConvert(name: string, fileContents: string): string {
    let result = "";

    result += "#include <stddef.h>\n"
    result += "#include <stdint.h>\n"
    result += "\n"
    result += `static int32_t ${name}_len = ${fileContents.length};\n`;

    result += `static uint8_t ${name}[${fileContents.length}] = {\n    `

    for (let i = 0; i < fileContents.length; i++) {
        let char = Number(fileContents.charCodeAt(i)).toString(16);
        result += `0x${char}, `;
        if ((i + 1) % 32 == 0) {
            result += '\n    ';
        }
    }

    result += '\n};'

    return result;
}



function main() {
    DPCC.init();

    let fd = FS.openSync("last_gen.txt", "w");
    FS.writeSync(fd, new Date().toISOString());
    FS.closeSync(fd);


    let genList = [
        {"filepath": "src/__gen.c", "fn": generate_src_file},
        {"filepath": "src/__gen.h", "fn": generate_hdr_file},
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

    {
        let contents = FS.readFileSync("3ac_preamble.c", { encoding: "utf-8"});
        let converted: string = CHexConvert("THREEAC_PREAMBLE",  contents);
        let out = FS.openSync("src/__3ac_preamble.h", "w");
        FS.writeSync(out, converted)
        FS.closeSync(out)
    }
    {
        let contents = FS.readFileSync("3ac_postamble.c", { encoding: "utf-8"});
        let converted: string = CHexConvert("THREEAC_POSTAMBLE",  contents);
        let out = FS.openSync("src/__3ac_postamble.h", "w");
        FS.writeSync(out, converted)
        FS.closeSync(out)
    }
}

main();
