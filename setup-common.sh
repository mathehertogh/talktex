#!/bin/bash

COMPILER_DIR="src/compiler"
COMPILER_BUILD_DIR="build"

TALKTEX_DIR="src/talktex"
TALKTEX_MODEL_DIR="deepspeech/models"

####################################################################################################

set -e # Quit on error
script_dir=$(cd -- "$(dirname -- "$(readlink -f -- "${BASH_SOURCE[0]}")")" >/dev/null && pwd)

####################################################################################################

echo "$(tput bold)TalkTeX: Starting common setup...$(tput sgr0)"
echo ""

# Setup compiler
cd "$script_dir/$COMPILER_DIR"
	mkdir -p "$COMPILER_BUILD_DIR"
	cd "$COMPILER_BUILD_DIR"
	meson

# Setup talktex
cd "$script_dir/$TALKTEX_DIR"
	mkdir -p "$TALKTEX_MODEL_DIR"
	wget "https://github.com/mozilla/DeepSpeech/releases/download/v0.9.3/deepspeech-0.9.3-models.pbmm" -O "$TALKTEX_MODEL_DIR/deepspeech-0.9.3-models.pbmm"

echo ""
echo "$(tput bold; tput setaf 2)TalkTeX: Setup succesful!$(tput sgr0)"
