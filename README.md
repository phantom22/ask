# ask
a bash script that facilitates discussion with Ollama models. Note that this script also produces formatted files meant to be visualized on obsidian (or any other software that allows to visualize both markdown and latex).
The base folder will be created in ~/.ask.

## Requirements
Ollama and atleast one local ollama model. 

## Usage
```bash
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

### Additional info
Conversations can be cleared,saved,loaded and removed.
The script has other options that can be visualized with the ```-h``` flag.
