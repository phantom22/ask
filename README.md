# ask
a bash script that facilitates discussion with Ollama models (especially for Deepseek-R1 which allows toggling reasoning). 

The script also can produce formatted files meant to be visualized on Obsidian (or any other software that allows to visualize both markdown and LaTeX).

**The shell script is currently feature complete and working, while the C implementation is at its beginning stage.**

# Setup
First of all ```make setup``` chmods the relevant scripts and creates the necessary file system for the C project.

While using using the program itself will generate the ~/.ask directory, used to manage the conversations.

**For correct functionality ```ollama``` and at least one local model must be installed.**

# Installing the dependencies
Shell requirements: ```jq```.

C requirements: ```OpenSSL```, ```ncurses```, ```jansson```, ```GTest``` (only for test targets)
.
### Ubuntu
```shell
# To compile the C project
sudo apt install libssl-dev libncurses5-dev libncursesw5-dev libjansson-dev libgtest-dev

# For the shell script
sudo apt install jq
```
### Fedora
```shell
# To compile the C project
sudo dnf install openssl-devel ncurses-devel jansson-devel gtest-devel

# For the shell script
sudo dnf install jq
```

# Building
For the main program:
```shell
# to build bin/ask
make
# to force rebuild bin/ask
make clean
```

For the tests:
```shell
# to build tests/test
make tests
# to force rebuild tests/test
make cleantests
# needed after modifying tests/prepare_assets.sh or the tests won't run
make rebuildhash cleantests 
```

## Usage
```shell
# For a concise answer
ask -C "What are snaps on Linux?"
# For a follow-up question
ask -z  "And flatpaks and AppImages?"
# For a question that considers the full conversation
ask -H "Which is considered more easy to maintain?"
# For a question that shouldn't be added to the JSON history (still formats output)
ask -N "Where do flatpaks store data?"
# For a terminal-only answer without formatting or saving
ask -O "Why use Linux instead of Windows?"
# For a question that answers the prompt in ~/.ask/prompt.md
ask -F
```

# Additional Features
Conversations can be cleared, saved, loaded, and removed. The script has other options that can be viewed with the ```-h``` flag.