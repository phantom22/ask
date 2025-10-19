#!/bin/bash
if [[ -z "$1" ]]; then
    echo "Passed an empty argument!"
    exit 1
fi

if [[ ! -e "$1/Makefile" || ! -e "$1/ask" || ! -e "$1/tests/prepare_assets.sh" ]]; then
    echo "Invalid project dir!"
    exit 1
fi

ASSETS_DIR="$1/tests/assets"

rm -rf "$ASSETS_DIR"
mkdir -p "$ASSETS_DIR"

OPEN_DIR="$ASSETS_DIR/open"

mkdir -p "$OPEN_DIR"
touch "$OPEN_DIR/existing_empty_file.txt"
touch "$OPEN_DIR/existing_not_empty_file.txt"
echo "gibberish" > "$OPEN_DIR/existing_not_empty_file.txt"

exit 0