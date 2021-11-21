#!/bin/bash

BUILD_DIR="build"

set -e # Quit on error

# Go to this script's directory
cd -- "$(dirname -- "$(readlink -f -- "${BASH_SOURCE[0]}")")" >/dev/null


"./make.sh" | grep -Fxv "ninja: no work to do." || true
"$BUILD_DIR/src/latex-generator/compiler_latex_generator" "$@"
