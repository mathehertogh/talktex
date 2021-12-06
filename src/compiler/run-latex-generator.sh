#!/bin/bash

BUILD_DIR="build"

set -e # Quit on error
cd -- "$(dirname -- "$(readlink -f -- "${BASH_SOURCE[0]}")")" >/dev/null


"./make.sh" | grep -Fxv "ninja: no work to do." >&2 || true
"$BUILD_DIR/src/latex-generator/compiler_latex_generator" "$@"
