"use strict";
const FS = require("fs")
const PROC = require("process");
const OS = require("os")

function gprint(...args) {
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
                out += "    ".repeat(G_indentCnt) + l + "\n"
            }
        }
    }
}