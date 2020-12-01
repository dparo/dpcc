#!/usr/bin/env node

"use strict";

const FS = require("fs")
const PROC = require("process");
const OS = require("os")
const PATH = require("path")


const utils = {
    type_to_dpcc_type: function(s) {
        return {
            "int": "TYPE_I32",
            "float": "TYPE_F32",
            "bool": "TYPE_BOOL",
            "int[]": "TYPE_I32_ARRAY",
            "float[]": "TYPE_F32_ARRAY",
        }[s]
    }
}

class Gen {
    static G_indentCnt = 0;
    static DEFAULT_CASE = 'invalid_code_path();';

    static writeStream = process.stdout;

    static Action = class {
        constructor(v) {
            this.v = v
        }

        do() {
            if (typeof this.v === "function") {
                this.v();
            } else if (typeof(this.v) === "string") {
                Gen.print(this.v)
            } else {
                throw TypeError("Wrong type expected (either string or callable)")
            }
        }

    }

    static scope(callback) {
        this.print("{")
        this.G_indentCnt += 1;
        callback();
        this.G_indentCnt -= 1;
        this.print("}")
    }

    static print(...args) {
        if (args.length == 0) {
            console.log("")
        } else {
            let out = ""
            for (let v of args) {
                let lines = v.split('\n')
                for (let l of lines) {
                    let prependNewLine = l.startsWith("do") || l.startsWith("while") || l.startsWith("switch") || l.startsWith("for");
                    if (prependNewLine) {
                        out += "\n"
                    }
                    out += "    ".repeat(this.G_indentCnt) + l + "\n"
                }
            }

            Gen.writeStream.write(out)
        }
    }

    static log_or(...conds) {
        let result = "";
        for (let i in conds) {
            if (i < conds.length - 1) {
                result += `(${conds[i]}) || `;
            } else {
                result += `(${conds[i]})`;
            }
        }
        return '(' + result + ')';
    }

    static log_and(...conds) {
        let result = "";
        for (let i in conds) {
            if (i < conds.length - 1) {
                result += `(${conds[i]}) && `;
            } else {
                result += `(${conds[i]})`;
            }
        }
        return '(' + result + ')';
    }

    static one_of(expr, conds) {
        let result = "";
        for (let i in conds) {
            if (i < conds.length - 1) {
                result += `(${expr} == ${conds[i]}) || `
            } else {
                result += `(${expr} == ${conds[i]})`
            }
        }
        return '(' + result + ')'
    }


    static if(d) {
        let i = 0;
        for (let [k, v] of Object.entries(d)) {
            if (v == '') {
                continue;
            }

            let trailing = i == 0 ? '' : 'else '
            this.print(`${trailing}if (${k})`)
            this.scope(() => {
                (new this.Action(v)).do()
            })
            i++;
        }

        if ("" in d) {
            if (d[""] != '') {
                this.print("else")
                this.scope(() => {
                    (new this.Action(d[""])).do()
                })
            }
        } else {
            this.print("else")
            this.scope(() => {
                this.print(this.DEFAULT_CASE)
            })
        }
        this.print()
    }


    static switch(elem, d) {
        let default_case = this.DEFAULT_CASE;
        if ("" in d) {
            default_case = d[""];
        }

        this.print(`switch (${elem})`)
        this.scope(() => {
            if (default_case == "") {
                this.print("default: { /* EMPTY */ } break;");
            } else {
                this.print("default:")
                this.scope(() => {
                    (new this.Action(default_case)).do();
                });
                this.print("break;");
            }

            for (let [k, v] of Object.entries(d)) {
                if (k == "") {
                    continue;
                }

                this.print(`case ${k}:`)
                this.scope(() => {
                    (new this.Action(v)).do();
                });
            }

        })
    }
}

class DPCC {
    static OpsBundle = class {
        constructor(yytokentypes, types_conversion_table) {
            this.yytokentypes = yytokentypes;
            this.types_conversion_table = types_conversion_table;
        }
    }

    static COMMON_BOILERPLATE = (
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

    static init() {
        DPCC.BUNDLES.ALL = [
            DPCC.BUNDLES.INTEGER_OPS,
            DPCC.BUNDLES.MATH_OPS,
            DPCC.BUNDLES.LOGICAL_COMPARISONS,
            DPCC.BUNDLES.LOGICAL_OPERATORS,
            DPCC.BUNDLES.ARRAY_OPERATORS
        ];

        for (let bundle of DPCC.BUNDLES.ALL) {
            for (let yytokenstype of bundle.yytokentypes) {
                DPCC.OPS.push(yytokenstype);
            }
        }

    }

    static OPS = [
        // Initialized later
    ]
    static BUNDLES = {

        DECL_OPS: [
            "TOK_KW_FN",
            "TOK_KW_LET",
        ],

        CAST_OPS: [
            "TOK_KW_INT",
            "TOK_KW_FLOAT",
            "TOK_KW_BOOL",
        ],

        INTEGER_OPS: new DPCC.OpsBundle(
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
        ),

        MATH_OPS: new DPCC.OpsBundle(
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
        ),

        LOGICAL_COMPARISONS: new DPCC.OpsBundle(
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
        ),

        LOGICAL_OPERATORS: new DPCC.OpsBundle(
            [
                "TOK_LNOT",
                "TOK_LAND",
                "TOK_LOR",
            ],
            [
                ["bool", ["bool", "bool"]],
                ["bool", ["bool"]],
            ]
        ),


        ARRAY_OPERATORS: new DPCC.OpsBundle (
            [
                "TOK_AR_SUBSCR",
            ],
            [
                ["int", ["int[]", "int"]],
                ["float", ["float[]", "int"]]
            ],
        ),

        ALL: [
            // Initialized later
        ],

    }
}





function generate_src_file() {
    Gen.print(DPCC.COMMON_BOILERPLATE)
}


function generate_hdr_file() {

}


function main() {
    DPCC.init();

    let fd = FS.openSync("last_gen.txt", "w");
    FS.writeSync(fd, new Date().toISOString())
    FS.closeSync(fd);


    let genList = [
        {"filepath": "js_gen/gen.c", "fn": generate_src_file},
        {"filepath": "js_gen/gen.h", "fn": generate_hdr_file},
    ]

    for (let [k, v] of Object.entries(genList)) {
        try {
            FS.mkdirSync(PATH.dirname(v.filepath), {recursive: true})
            Gen.writeStream = new FS.createWriteStream(v.filepath);
            v.fn();
        } finally {

        }
    }
}

main();
