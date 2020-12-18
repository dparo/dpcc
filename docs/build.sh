#!/usr/bin/env bash


cd $(dirname "$0")

latexmk -pdfxe -interaction=nonstopmode -recorder -file-line-error main.tex \
    | egrep "^((l\\..*)|(.*:.*))$"

exit $?
