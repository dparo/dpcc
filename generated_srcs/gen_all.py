#!/usr/bin/env python3
import os
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
#include "dpcc.h"

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

    eprint("void check_and_optimize_ast(void)")
    with scope():
        eprint("ast_traversal_t att = {0};")
        eprint("ast_traversal_begin(&att);")
        eprint("ast_node_t *n = NULL;")
        eprint("while ((n = ast_traverse_next(&att)) != NULL)")
        with scope():
            eprint()
            pass




def main():

    with open("last_gen.txt", "w") as f:
        f.write(datetime.today().strftime('%Y-%m-%d-%H:%M:%S'))

    gen_list = [
        { "filepath": "src/gen.c", "fn": generate }
    ]

    for v in gen_list:
        os.makedirs(os.path.dirname(v['filepath']), exist_ok=True)
        with open(v['filepath'], 'w') as f:
            sys.stdout = f
            (v['fn'])()

main()
