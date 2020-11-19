#!/usr/bin/env python3
import sys


def generate():
    print(
"""
int main(int argc, char **argv) {
    for (int32_t i = 0; i < ARRAY_LEN(i); i++) {
        return false;
    }
    return 0;
}
""")

def main():
    output_file_path = sys.argv[1]
    output_file = open(output_file_path, 'w')
    sys.stdout = output_file
    generate()
    output_file.close()

main()
