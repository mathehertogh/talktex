#!/bin/bash

set -e # Quit on error
cd -- "$(dirname -- "$(readlink -f -- "${BASH_SOURCE[0]}")")" >/dev/null

# We just need to build the compiler.
# This could currently just be a symlink, but it general that may not suffice.
src/compiler/make.sh
