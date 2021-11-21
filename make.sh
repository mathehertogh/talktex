#!/bin/bash

BUILD_DIR="build"

set -e # Quit on error

# Go to this script's directory
cd -- "$(dirname -- "$(readlink -f -- "${BASH_SOURCE[0]}")")" >/dev/null


cd "$BUILD_DIR"
ninja
