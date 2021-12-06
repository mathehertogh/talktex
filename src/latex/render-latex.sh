#!/bin/bash

# This script builds the specified latex file and opens the resulting pdf

LATEX_BUILD_DIR="build"

set -e # Quit on error

if [[ $# -ne 1 ]]; then
	1>&2 echo "Usage: $0 [latex source file]"
	exit 1
fi

latex_path="$1"
latex_dirname=$(dirname "$latex_path")
latex_basename=$(basename "$latex_path")


cd "$latex_dirname"
latexmk -pdf -cd -output-directory="$LATEX_BUILD_DIR" "$latex_basename"
xdg-open "$LATEX_BUILD_DIR/${latex_basename%.*}.pdf"
