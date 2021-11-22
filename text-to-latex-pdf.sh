#!/bin/bash

# This script converts lines of text on stdin to latex, builds the pdf and opens it

BUILD_DIR="build"

set -e # Quit on error

# Go to this script's directory
cd -- "$(dirname -- "$(readlink -f -- "${BASH_SOURCE[0]}")")" >/dev/null


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


"./run-latex-generator.sh" -d -i "$input" > "latex/talktex-output/output.tex"
"latex/render-latex.sh" "latex/talktex-output/output.tex"
