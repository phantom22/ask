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

JSON_DIR="$ASSETS_DIR/json"
mkdir -p "$JSON_DIR"

touch "$JSON_DIR/valid.json"
echo '{"valid_bool1":true,"valid_bool2":false,"invalid_bool1":"","invalid_bool2":5,"valid_string1":"","valid_string2":"test string","invalid_string1":3.14,"invalid_string2":[],"valid_array":[],"invalid_array":42}' > "$JSON_DIR/valid.json"

touch "$JSON_DIR/invalid.json"
echo 'gibberish 2' > "$JSON_DIR/invalid.json"

exit 0