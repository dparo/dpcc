#!/usr/bin/env bash

cd "$(dirname "$0")"

mkdir -p report

echo "Calling Bison (Generate verbose output, XML report, and GRAPH representation)..."
bison -Wall -Wother -Wcounterexamples -Wdangling-alias \
      --color=auto \
      -fcaret -ffixit \
      --report=itemset --report=lookahead --report=solved --report=counterexamples \
      --verbose \
      --graph=report/parser.dot \
      --xml=report/parser.xml \
      ../src/parser.y



mv parser.output                     report

# echo "Generating graph PDF..."
# dot -Tpdf report/parser.dot -o report/parser.pdf

echo "Generating HTML from the xml report..."
xsltproc /usr/share/bison/xslt/xml2xhtml.xsl report/parser.xml >report/parser.html

echo "Finishing, removing temp files."
rm report/parser.xml *.c
rm -rf ../report
mv report ../

echo "Done!"
