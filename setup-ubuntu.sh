#!/bin/bash

BUILD_DIR="build"

set -e # Quit on error

# Go to this script's directory
cd -- "$(dirname -- "$(readlink -f -- "${BASH_SOURCE[0]}")")" >/dev/null


sudo apt install flex bison
pip3 install meson ninja --user

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"
meson ..
