# Enable strict mode and stop on errors
$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest

# Trap errors with a friendly message
trap {
    Write-Host "`n❌ Error on line $($_.InvocationInfo.ScriptLineNumber). Pausing..."
    Read-Host "Press Enter to exit"
    exit 1
}

# Move to the directory where this script is located
Set-Location -Path (Split-Path -Parent $MyInvocation.MyCommand.Definition)

# Base URL for game assets
$GITHUB_OWNER = "humbertodias"
$GITHUB_TAG = "v0.1"
$ASSETS_URL = "https://github.com/$GITHUB_OWNER/openfight/releases/download/$GITHUB_TAG/data.zip"
$ASSETS_FILE = "data.zip"
$ASSETS_DIR = "data"

# Download and extract data if not already present
if (Test-Path -Path $ASSETS_DIR -PathType Container) {
    Write-Host "✓ '$ASSETS_DIR' directory already exists. Skipping download."
} else {
    Write-Host "↓ Downloading $ASSETS_FILE..."
    Invoke-WebRequest -Uri $ASSETS_URL -OutFile $ASSETS_FILE -UseBasicParsing

    Write-Host "📦 Extracting $ASSETS_FILE..."
    Expand-Archive -Path $ASSETS_FILE -DestinationPath "."

    Write-Host "🧹 Cleaning up..."
    Remove-Item -Path $ASSETS_FILE -Force

    Write-Host "✅ Assets ready."
}

# Run the game binary
Write-Host "🚀 Launching openfight..."
Start-Process -FilePath ".\openfight.exe" -NoNewWindow -Wait
