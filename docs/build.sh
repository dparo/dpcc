#!/usr/bin/env bash


cd $(dirname "$0")

latexmk -pdf -interaction=nonstopmode -recorder -file-line-error -pvc main.tex
exit $?
