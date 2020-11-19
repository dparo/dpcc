#!/usr/bin/env bash

cd "$(dirname "$0")"
cd ../

mkdir -p report

bison --graph=report/parser.dot src/parser.y -o ./scripts/bison_out
bison --xml=report/parser.xml src/parser.y -o ./scripts/bison_out

dot -Tpdf report/parser.dot -o report/parser.pdf
xsltproc /usr/share/bison/xslt/xml2xhtml.xsl report/parser.xml >report/parser.html

rm report/parser.xml

rm ./scripts/bison_out*
