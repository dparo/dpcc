#!/usr/bin/env bash

cd "$(dirname "$0")"

mkdir -p report

echo "Generating textual verbose representation..."
bison --verbose --color=auto ../src/parser.y \
      -Wcounterexamples -Wempty-rule -Wmidrule-values \

mv *.output                     report

echo "Generating graph representation..."
bison --graph=report/parser.dot ../src/parser.y           1> /dev/null 2> /dev/null

echo "Generating XML report..."
bison --xml=report/parser.xml ../src/parser.y             1> /dev/null 2> /dev/null

echo "Generating graph PDF..."
dot -Tpdf report/parser.dot -o report/parser.pdf

echo "Generating HTML from the xml report..."
xsltproc /usr/share/bison/xslt/xml2xhtml.xsl report/parser.xml >report/parser.html

echo "Finishing, removing temp files."
rm report/parser.xml bison_out *.c
rm -rf ../report
mv report ../

echo "Done!"
