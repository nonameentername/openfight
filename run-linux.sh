#!/bin/bash

set -euo pipefail
trap 'echo -e "\n❌ Error on line $LINENO. Pausing..."; read -p "Press Enter to exit." dummy' ERR

# Move to the script's directory
cd "$(dirname "$(realpath "$0")")"

# Base URL for game assets
GITHUB_OWNER="humbertodias"
GITHUB_TAG="v0.1"
ASSETS_URL="https://github.com/$GITHUB_OWNER/openfight/releases/download/$GITHUB_TAG/data.zip"
ASSETS_FILE="data.zip"
ASSETS_DIR="data"

# Download and extract data if not already present
if [[ -d "$ASSETS_DIR" ]]; then
    echo "✓ '$ASSETS_DIR' directory already exists. Skipping download."
else
    echo "↓ Downloading $ASSETS_FILE..."
    curl -fsSL -o "$ASSETS_FILE" "$ASSETS_URL"

    echo "📦 Extracting $ASSETS_FILE..."
    unzip -q "$ASSETS_FILE"

    echo "🧹 Cleaning up..."
    rm -f "$ASSETS_FILE"

    echo "✅ Assets ready."
fi

# Run the game binary
echo "🚀 Launching openfight..."
exec ./openfight
