#!/bin/bash

set -e # Quit on error
cd -- "$(dirname -- "$(readlink -f -- "${BASH_SOURCE[0]}")")" >/dev/null

####################################################################################################

echo "$(tput bold)TalkTeX: Starting Ubuntu-specific setup...$(tput sgr0)"
echo ""

sudo apt install wget flex bison python3 python3-pip alsa-utils portaudio19-dev latexmk

echo ""
echo "$(tput bold; tput setaf 2)TalkTeX: Ubuntu-specific setup succesful!$(tput sgr0)"

./setup-common.sh
