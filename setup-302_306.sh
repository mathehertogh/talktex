#!/bin/bash

PKG_DIR="packages"
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


# Install required packages
mkdir -p "$script_dir/$PKG_DIR/tmp"
cd "$script_dir/$PKG_DIR/tmp"
apt download libportaudio2
apt download portaudio19-dev

for file in *.deb; do
	dpkg -x $file ../
done
cd "$script_dir"
rm -rf "$script_dir/$PKG_DIR/tmp"


# Export package binaries and libraries to environment variables
export "C_INCLUDE_PATH=$script_dir/$PKG_DIR/usr/include:$C_INCLUDE_PATH"
export "LIBRARY_PATH=$script_dir/$PKG_DIR/usr/lib:$LIBRARY_PATH"
export "LIBRARY_PATH=$script_dir/$PKG_DIR/usr/lib/x86_64-linux-gnu:$LIBRARY_PATH"
export "LD_LIBRARY_PATH=$script_dir/$PKG_DIR/usr/lib:$LD_LIBRARY_PATH"
export "LD_LIBRARY_PATH=$script_dir/$PKG_DIR/usr/lib/x86_64-linux-gnu:$LD_LIBRARY_PATH"
export "PATH=$script_dir/$PKG_DIR/usr/bin:$PATH"


# Install python modules
python3 -m pip install meson ninja pyaudio deepspeech webrtcvad halo numpy scipy --user
export "PATH=$HOME/.local/bin:$PATH"


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
echo ""


# compile talktex
cd "$script_dir"
./make-all.sh

echo ""
echo "$(tput bold; tput setaf 2)TalkTeX: Compilation successul!$(tput sgr0)"
echo ""
