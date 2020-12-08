#!/usr/bin/env python3

from subprocess import Popen, PIPE
import sys

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)


def strip_comments(text):
    lines = text.splitlines()
    code = []
    for l in lines:
        if not l.strip().startswith('//'):
            code.append(l)
    return "\n".join(code)

def extract_expected_out(text):
    beg = "//@"
    lines = text.splitlines()

    comments = []
    for l in lines:
        if l.startswith(beg):
            comments.append((l[len(beg):]).strip())
    return comments;

def check_test(input, exit_code, output, expected_output):
    output = output.strip()
    if len(output) == 0 or output.isspace():
        output = None

    if (exit_code == 0 and expected_output is None):
        eprint(f"(exit_code = {exit_code}) :: Expected this test to fail but successed instead")
        eprint("-------- INPUT:")
        eprint(input)
        eprint("-------- OUTPUT:")
        eprint(output)
        eprint("-------- EXPECTED OUTPUT:")
        eprint(expected_output)
        eprint("--------")
        exit(1)
    elif (output != expected_output):
        eprint(f"(exit_code = {exit_code}) :: Wrong output obtained from the following test")
        eprint("-------- INPUT:")
        eprint(input)
        eprint("-------- OUTPUT:")
        eprint(output)
        eprint("-------- EXPECTED OUTPUT:")
        eprint(expected_output)
        eprint("--------")
        exit(1)



def test(input, expected_output):
    if len(expected_output) == 0 or expected_output == "\n":
        expected_output = None

    out_filepath = "tmp/code.txt"

    out_fh = open(out_filepath, "w")

    out_fh.write(input)
    out_fh.close()

    process = Popen(["run_tree/bin/dpcc", "run", out_filepath], stdout=PIPE)
    (output, err) = process.communicate()
    exit_code = process.wait()

    output = output.decode("utf-8")

    output = output.strip()

    check_test(input, exit_code, output, expected_output)




with open("tests/progs.txt", "r") as progs:
    import re
    contents = progs.read()

    chunks = re.split(r'^/{8}/*$', contents, flags=re.MULTILINE)
    chunks = list(map(lambda c: c.strip(), chunks))

    for c in chunks:
        input = strip_comments(c).strip()
        expected_output = "\n".join(extract_expected_out(c))

        if len(input) > 0 and not input.isspace():
            test(input, expected_output)
