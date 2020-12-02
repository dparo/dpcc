#!/usr/bin/env node


import * as FS from "fs";
import * as PROC from "process";
import * as OS from "os";
import * as PATH from "path";


const utils = {
    type_to_dpcc_type: function(s: string): string {
        let map: Record<string, string> = {
            "int": "TYPE_I32",
            "float": "TYPE_F32",
            "bool": "TYPE_BOOL",
            "int[]": "TYPE_I32_ARRAY",
            "float[]": "TYPE_F32_ARRAY",
        };
        return map[s] || "";
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
    export function scope(callback: () => any) {
        Gen.print("{");
        G_indentCnt += 1;
        callback();
        G_indentCnt -= 1;
        Gen.print("}");
    }

    export function print(...args: string[]) {
        if (args.length == 0) {
            console.log("");
        } else {
            let out = "";
            for (let v of args) {
                let lines = v.split('\n');
                for (let l of lines) {
                    let prependNewLine = l.startsWith("do") || l.startsWith("while") || l.startsWith("switch") || l.startsWith("for");
                    if (prependNewLine) {
                        out += "\n";
                    }
                    out += "    ".repeat(G_indentCnt) + l + "\n";
                }
            }
            if (writeStream == null) {
                PROC.stdout.write(out);
            } else {
                Gen.writeStream?.write(out);
            }
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


    export function ife(d: Record<string, string|(() => any)>) {
        let i = 0;
        for (let [k, v] of Object.entries(d)) {
            if (v == '') {
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
        Gen.print();
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
            }

        });
    }
}

namespace DPCC {

    class Expr {
        yytokentypes: string[];
        types_conversion_table: (string | string[])[][];

        constructor(yytokentypes: string[], types_conversion_table: (string | string[])[][]) {
            this.yytokentypes = yytokentypes;
            this.types_conversion_table = types_conversion_table;
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

        export let ALL: string[] = [
            // Initialized later
        ]
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
                ["int", ["int", "int"]]
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
                ["int", ["int", "int"]],
                ["float", ["float", "int"]],
                ["float", ["int", "float"]],
                ["float", ["float", "float"]],

                ["int", ["int"]],
                ["float", ["float"]],
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
                ["bool", ["int", "int"]],
                ["bool", ["float", "int"]],
                ["bool", ["int", "float"]],
                ["bool", ["float", "float"]],

            ]
        );

        export const LOG_EXPR = new Expr(
            [
                "TOK_LNOT",
                "TOK_LAND",
                "TOK_LOR",
            ],
            [
                ["bool", ["bool", "bool"]],
                ["bool", ["bool"]],
            ]
        );


        export const ARRAY_EXPR = new Expr (
            [
                "TOK_AR_SUBSCR",
            ],
            [
                ["int", ["int[]", "int"]],
                ["float", ["float[]", "int"]]
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
        for (let bundle of EXPRS.ALL) {
            for (let yytokenstype of bundle.yytokentypes) {
                OPS.ALL.push(yytokenstype);
            }
        }

    }

}




function generate_src_file() {
    Gen.print(DPCC.COMMON_BOILERPLATE);
    Gen.print(DPCC.COMMON_BOILERPLATE);
    Gen.print(DPCC.COMMON_BOILERPLATE);
    Gen.print(DPCC.COMMON_BOILERPLATE);
}


function generate_hdr_file() {
    Gen.print(DPCC.COMMON_BOILERPLATE);

}


function main() {
    DPCC.init();

    let fd = FS.openSync("last_gen.txt", "w");
    FS.writeSync(fd, new Date().toISOString());
    FS.closeSync(fd);


    let genList = [
        {"filepath": "js_gen/gen.c", "fn": generate_src_file},
        {"filepath": "js_gen/gen.h", "fn": generate_hdr_file},
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
