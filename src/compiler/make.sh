#!/bin/bash

BUILD_DIR="build"

set -e # Quit on error
cd -- "$(dirname -- "$(readlink -f -- "${BASH_SOURCE[0]}")")" >/dev/null


cd "$BUILD_DIR"
ninja
