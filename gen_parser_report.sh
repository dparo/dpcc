#!/usr/bin/env bash

cd "$(dirname "$0")"

mkdir -p report

bison --graph=report/parser.dot src/parser.y -o /dev/null     1> /dev/null 2> /dev/null
bison --xml=report/parser.xml src/parser.y -o /dev/null       1> /dev/null 2> /dev/null

dot -Tpdf report/parser.dot -o report/parser.pdf
xsltproc /usr/share/bison/xslt/xml2xhtml.xsl report/parser.xml >report/parser.html

rm report/parser.xml
