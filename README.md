# ask
a bash script that facilitates discussion with Ollama models (especially for Deepseek-R1 which allows to toggle reasoning). Note that this script also produces formatted files meant to be visualized on obsidian (or any other software that allows to visualize both markdown and latex). The base folder will be created in ~/.ask.

## Important
The shell script, for now, is feature complete and working, while the C implementation is at its beginning stage.

To correctly setup the project you must do this first.
```shell
# Chmods the relevant scripts
# - bash/ask
# - tests/prepare_assets.sh
make setup
```

## Shell requirements
- Ollama and at least one local ollama model.
- jq (probably already installed on your system)

## Shell usage
```shell
# For a concise answer
ask -C "What are snaps on Linux?"
# For a follow-up question
ask -z  "And flatpaks and AppImages?"
# For a question that considers the full conversation
ask -H "Which is considered more easy to maintain?"
# For a question that shouldn't be added to the JSON history of the conversation
#    while still formatted markdown/latex output.
ask -N "Where do flatpaks store data?"
# For a question that should only be answered on the terminal, without formatting
#    and saving
ask -O "Why use Linux instead of Windows?"
# For a question that answers the prompt contained in ~/.ask/prompt.md
ask -F
```

## Additional Shell info
Conversations can be cleared,saved,loaded and removed. The script has other options that can be visualized with the ```-h``` flag.

## Building
```shell
# to build bin/ask
make
# to force rebuild bin/ask
make clean
```

## C Dependencies
- OpenSSL
- ncurses
- jansson
- GTest (only needed for test targets)

Also note that Linux is the only system supported by the Makefile because of POSIX calls.

## Building tests
```shell
# to build tests/test
make tests
# to force rebuild tests/test
make cleantests
# needed after modifying tests/prepare_assets.sh or the tests wont run
make rebuildhash cleantests 
```
