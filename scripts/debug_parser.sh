#!/usr/bin/env bash

cd "$(dirname "$0")"
cd ../

bison --color=auto \
      -Wcounterexamples -Wempty-rule -Wmidrule-values \
      src/parser.y -o ./scripts/bison_out

rm ./scripts/bison_out*
