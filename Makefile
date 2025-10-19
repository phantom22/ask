ifneq ($(shell uname -s),Linux)
$(error This Makefile only supports linux.)
endif

.PHONY: setup clean cleantests all rebuildhash

C_COMPILER = gcc
CPP_COMPILER = g++
OPTIMIZATION = -O3 -s -DNDEBUG

# For now using c2x instead of c23
CFLAGS_MAIN = -std=c2x $(OPTIMIZATION) -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L
# Note that -DSHELL_SCRIPT_SHA256SUM is also part of $(CFLAGS_TEST) but it's calculated on demand
# aka only when the tests target is specified.
CFLAGS_TEST = -std=c++20 $(OPTIMIZATION) -DHOME_DIR=\"$(HOME)\" -DPROJECT_DIR=\"$(CURDIR)\"
LDFLAGS =
LIBS_MAIN = -lncurses -ljansson -lcrypto
LIBS_TEST = $(LIBS_MAIN) -lgtest -lgtest_main -pthread

D_SRC = ask
D_BUILD = build
D_BIN = bin
D_TEST = tests

HASH_FILE = .script_hash

EXE_MAIN = $(D_BIN)/ask
EXE_TEST = $(D_TEST)/test

# Recursively find all .c files (Unix/Linux/macOS)
# the windows part also converts backslashes to forward slashes for consistency
SOURCES_MAIN = $(shell find $(D_SRC) -name '*.c')
SOURCES_TEST = $(D_TEST)/tests.cpp

# Generate flat object file paths, no preserving folder structure in build/
OBJECTS_MAIN = $(addprefix $(D_BUILD)/, $(notdir $(SOURCES_MAIN:.c=.o)))

$(shell mkdir -p bin build)

# Default target
all: $(EXE_MAIN)

# Link all objects into executable
$(EXE_MAIN): $(OBJECTS_MAIN)
	@echo Linking
	@$(C_COMPILER) $(LDFLAGS) $(LIBDIRS) $^ -o $@ $(LIBS_MAIN)
	@echo Build complete: $(EXE_MAIN)

# For the c files that are in %D_SRC%
$(D_BUILD)/%.o: $(D_SRC)/%.c
	@echo Compiling $<
	@$(C_COMPILER) $(CFLAGS_MAIN) $(INCLUDEDIRS) -c $< -o $@

# For the c files that are in %D_SRC% subfolders
$(D_BUILD)/%.o: $(D_SRC)/*/%.c
	@echo compiling $<
	@$(C_COMPILER) $(CFLAGS_MAIN) $(INCLUDEDIRS) -c $< -o $@

# Clean build artifacts, suppress errors where there are no artefacts to clean then call make with silent flag
clean:
	@rm -f $(D_BUILD)/*.o 2>&1
	@rm -f $(D_BIN)/ask 2>&1
	@make -s

tests: $(EXE_TEST)

$(EXE_TEST): $(SOURCES_TEST)
	@touch "$(HASH_FILE)"
	$(eval HASH := $(shell cat $(HASH_FILE)))
	$(eval VALID_HASH := $(shell sha256sum tests/prepare_assets.sh | cut -d ' ' -f 1))
	$(eval IS_INVALID := false)
# validate hash length and hash symbols
	$(if $(filter 64,$(shell echo -n "$(HASH)" | wc -c)),,$(eval IS_INVALID := true))
	$(if $(shell echo "$(HASH)" | grep -E '^[0-9a-fA-F]{64}$$'),,$(eval IS_INVALID := true))
# replace invalid hash with valid one
	$(if $(filter true,$(IS_INVALID)),$(info rebuilding contents of $(HASH_FILE).)$(eval HASH := $(VALID_HASH)),)
# update .script_hash
	$(if $(HASH),@echo "$(HASH)" > "$(HASH_FILE)",)
	@echo Compiling and linking $(SOURCES_TEST)
	@$(CPP_COMPILER) $(CFLAGS_TEST) -DSHELL_SCRIPT_SHA256SUM=\"$(HASH)\" $< -o $@ $(LIBS_TEST) $(LDFLAGS)
	@echo Build complete: $(EXE_TEST)


# if rebuildhash is specified without cleantests, this recipe throws an error
ifneq (,$(filter cleantests,$(MAKECMDGOALS)))
# this recipe is defined like this to prevent messages like "nothing to do"
rebuildhash:
	@:
else
rebuildhash:
	$(error rebuildhash is meant to be used along with cleantests.)
endif

# cleantests clears the contents of $(HASH_FILE) if used with
# rebuildhash target
ifneq (,$(filter rebuildhash,$(MAKECMDGOALS)))
cleantests:
	@echo "" > "$(HASH_FILE)"
else
cleantests:
endif
	@rm -f $(D_TEST)/test 2>&1
	@rm -rf $(D_TEST)/assets 2>&1
	@make -s tests

setup:
	@chmod +x "$(CURDIR)/bash/ask"
	@chmod +x "$(CURDIR)/tests/prepare_assets.sh"
	@echo Settings \"bash/ask\" and \"tests/prepare_assets.sh\" as executable scripts.