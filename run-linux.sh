#!/bin/bash

set -euo pipefail
trap 'echo -e "\n❌ Error on line $LINENO. Pausing..."; read -p "Press Enter to exit." dummy' ERR

# Move to the script's directory
cd "$(dirname "$(realpath "$0")")"

sudo apt install -y build-essential libxml2-dev libsdl2-dev libsdl2-image-dev libglu1-mesa-dev libglew-dev

cmake -Bbuild
cmake --build build

# Run the game binary
echo "🚀 Launching openfight..."
exec ./openfight
