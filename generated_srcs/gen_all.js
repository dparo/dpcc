"use strict";
const FS = require("fs")
const PROC = require("process");
const OS = require("os")

class Gen {
    static G_indentCnt = 0;
    static DEFAULT_CASE = 'invalid_code_path();';

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
            process.stdout.write(out)
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
            default_case = d[""]
        }

        this.print(`switch (${elem})`)
        this.scope(() => {
            if (default_case == "") {
                this.print("default: { /* EMPTY */ } break;");
            } else {
                this.print("default:")
                this.scope(() => {
                    (new this.Action(default_case)).do()
                })
                this.print("break;")
            }

            for (let [k, v] of Object.entries(d)) {
                if (k == "") {
                    continue;
                }

                this.print(`case ${k}:`)
                this.scope(() => {
                    (new this.Action(v)).do()
                })
            }

        })
    }
}

