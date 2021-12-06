#!/bin/bash

# This script converts lines of text given in the first argument or via standard in to latex,
# builds a pdf and opens it

####################################################################################################

LATEX_OUTPUT_DIR="latex-output"

####################################################################################################

set -e # Quit on error
cd -- "$(dirname -- "$(readlink -f -- "${BASH_SOURCE[0]}")")" >/dev/null

####################################################################################################

input=""
if [[ $# -eq 0 ]]; then
	input="$(</dev/stdin)"
elif [[ $# -eq 1 ]]; then
	input="$1"
else
	1>&2 echo "Usage: $0 [input text]"
	1>&2 echo "If [input text] is omitted, input is read from standard in."
	exit 1
fi

mkdir -p "$LATEX_OUTPUT_DIR"
cp "src/latex/talktex.sty" "$LATEX_OUTPUT_DIR"
src/compiler/run-latex-generator.sh -d -i "$input" > "$LATEX_OUTPUT_DIR/output.tex"
src/latex/render-latex.sh "$LATEX_OUTPUT_DIR/output.tex"
