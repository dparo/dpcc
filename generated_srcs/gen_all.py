#!/usr/bin/env python3
import sys
from datetime import datetime

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


#include "types.h"
#include "globals.h"
#include "utils.h"
#include "log.h"
"""
)

indent_cnt = 0
def eprint(*args, **kwargs):
    print("    " * indent_cnt, end='')
    print(*args, **kwargs)

class scope:
    def __init__(self):
        pass
    def __enter__(self):
        global indent_cnt
        eprint("{")
        indent_cnt += 1
    def __exit__(self, type, value, traceback):
        global indent_cnt
        indent_cnt -= 1
        eprint("}")


def generate():
    eprint(common_boilerplate)
    eprint("int main(int argc, char**argv)")
    with scope(): 
        eprint("return 0;")
        with scope(): 
            eprint("int a = 10;")  


def main():

    with open("last_gen.txt", "w") as f:
        f.write(datetime.today().strftime('%Y-%m-%d-%H:%M:%S')) 

    gen_list = [
        { "filepath": "src/gen.c", "fn": generate }
    ]

    for v in gen_list:
        with open(v['filepath'], 'w') as f:
            sys.stdout = f
            (v['fn'])()

main()
