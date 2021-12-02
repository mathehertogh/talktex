#!/bin/bash

BUILD_DIR="build"
MODEL_DIR="deepspeech-models"

set -e # Quit on error

# Go to this script's directory
cd -- "$(dirname -- "$(readlink -f -- "${BASH_SOURCE[0]}")")" >/dev/null


sudo apt install flex bison alsa-utils portaudio19-dev
pip3 install meson ninja deepspeech pyaudio webrtcvad halo --user

mkdir -p "$MODEL_DIR"
wget https://github.com/mozilla/DeepSpeech/releases/download/v0.9.3/deepspeech-0.9.3-models.pbmm -O "$MODEL_DIR/deepspeech-0.9.3-models.pbmm"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"
meson ..
