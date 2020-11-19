#!/usr/bin/env python3
import sys
from datetime import datetime

def generate():
    print(
"""
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "globals.h"
#include "utils.h"

#include <stdarg.h>
#include "log.h"
""")

def main():

    with open("last_gen.txt", "w") as f:
        f.write(datetime.today().strftime('%Y-%m-%d-%H:%M:%S')) 
    
    output_file = open("src/gen.c", 'w')
    sys.stdout = output_file
    generate()
    output_file.close()

main()
