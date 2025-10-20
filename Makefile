ifneq ($(shell uname -s),Linux)
$(error This Makefile only supports linux.)
endif

.PHONY: setup clean cleantests all rebuildhash resetdir

C_COMPILER = gcc
CPP_COMPILER = g++
OPTIMIZATION = -O3

MAIN_ENTRY_POINT = main.c
TEST_ENTRY_POINT = tests.cpp

# For now using c2x instead of c23
CFLAGS_MAIN = -std=c2x $(OPTIMIZATION) -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L
# Note that -DSHELL_SCRIPT_SHA256SUM is also part of $(CFLAGS_TEST) but it's calculated on demand
# aka only when the tests target is specified.
CFLAGS_TEST = -std=c++20 $(OPTIMIZATION) -DHOME_DIR=\"$(HOME)\" -DPROJECT_DIR=\"$(CURDIR)\"

LIBS_MAIN = -lncurses -ljansson -lcrypto
LDFLAGS_MAIN = -s $(LIBS_MAIN)

LIBS_TEST = -lgtest -lgtest_main -pthread
LDFLAGS_TEST = -s $(LIBS_MAIN) $(LIBS_TEST)

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
SOURCES_TEST = $(filter-out $(D_SRC)/$(MAIN_ENTRY_POINT), $(SOURCES_MAIN))

# Generate flat object file paths, no preserving folder structure in build/
OBJECTS_MAIN = $(addprefix $(D_BUILD)/, $(notdir $(SOURCES_MAIN:.c=.o)))
OBJECTS_TEST = $(addprefix $(D_BUILD)/test/, $(notdir $(SOURCES_TEST:.c=.o)))

# Default target
all: $(EXE_MAIN)

# Link all objects into executable
$(EXE_MAIN): $(OBJECTS_MAIN)
	@echo Linking
	@$(C_COMPILER) $^ $(LDFLAGS_MAIN) -o $@ && \
	echo 'Build complete: $(EXE_MAIN)'

# For the c files that are in %D_SRC%
$(D_BUILD)/%.o: $(D_SRC)/%.c
	@echo Compiling $<
	@$(C_COMPILER) $(CFLAGS_MAIN) -c $< -o $@

# For the c files that are in %D_SRC% subfolders
$(D_BUILD)/%.o: $(D_SRC)/*/%.c
	@echo Compiling $<
	@$(C_COMPILER) $(CFLAGS_MAIN) -c $< -o $@

# Duplicate for the target test
$(D_BUILD)/test/%.o : $(D_SRC)/%.c
	@echo Compiling $<
	@$(C_COMPILER) $(CFLAGS_MAIN) -DASK_TEST_MODE -c $< -o $@

# Duplicate for the target test
$(D_BUILD)/test/%.o: $(D_SRC)/*/%.c
	@echo Compiling $<
	@$(C_COMPILER) $(CFLAGS_MAIN) -DASK_TEST_MODE -c $< -o $@

# Remove previous build artifacts to force a clean make
clean:
	@rm -f $(CURDIR)/$(D_BUILD)/*.o 2>&1
	@rm -f "$(CURDIR)/$(D_BIN)/ask" 2>&1
	@make -s

tests: $(OBJECTS_TEST) $(EXE_TEST)

$(EXE_TEST): $(D_TEST)/$(TEST_ENTRY_POINT)
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
	@echo Compiling and linking $(D_TEST)/$(TEST_ENTRY_POINT)
	@$(CPP_COMPILER) $(CFLAGS_TEST) -DSHELL_SCRIPT_SHA256SUM=\"$(HASH)\" $< $(OBJECTS_TEST) -o $@ $(LDFLAGS_TEST) && \
	echo 'Build complete: $(EXE_TEST)'

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
	@rm -f "$(CURDIR)/$(D_TEST)/test" 2>&1
	@rm -rf "$(CURDIR)/$(D_TEST)/assets" 2>&1
	@rm -f $(CURDIR)/$(D_BUILD)/test/*.o 2>&1
	@make -s tests

setup:
	@touch "$(HASH_FILE)"
	@if ! test -d "$(CURDIR)/$(D_BIN)"; then \
		mkdir "$(CURDIR)/$(D_BIN)" && \
		echo 'Created $(D_BIN)/.' || \
		echo 'Failed to create $(D_BIN)/.'; \
	fi

	@if ! test -d "$(CURDIR)/$(D_BUILD)"; then \
		mkdir "$(CURDIR)/$(D_BUILD)" "$(CURDIR)/$(D_BUILD)/test" && \
		echo 'Created $(D_BUILD)/.' || \
		echo 'Failed to create $(D_BUILD)/.'; \
	fi

	@if ! test -x "$(CURDIR)/bash/ask"; then \
		chmod +x "$(CURDIR)/bash/ask" && \
		echo 'Chmodded bash/ask as executable script.' || \
		echo 'Failed to chmod bash/ask.'; \
	fi

	@if ! test -x "$(CURDIR)/$(D_TEST)/prepare_assets.sh"; then \
		chmod +x "$(CURDIR)/$(D_TEST)/prepare_assets.sh" && \
		echo 'Chmodded tests/prepare_assets.sh as executable script.' || \
		echo 'Failed to chmod tests/prepare_assets.sh.'; \
	fi

resetdir:
# resetdir won't delete .script_hash, this is intended behavior
	@if test -d "$(CURDIR)/$(D_BIN)"; then \
		rm -rf "$(CURDIR)/$(D_BIN)" && \
		echo 'Removed recursively $(D_BIN)/.' || \
		echo 'Failed to remove recursively $(D_BIN)/.'; \
	fi

	@if test -d "$(CURDIR)/$(D_BUILD)"; then \
		rm -rf "$(CURDIR)/$(D_BUILD)" && \
		echo 'Removed recursively $(D_BUILD)/.' || \
		echo 'Failed to remove recursively $(D_BUILD)/.'; \
	fi

	@if test -f "$(CURDIR)/tests/test"; then \
		rm -f "$(CURDIR)/tests/test" && \
		echo 'Removed tests/test executable.' || \
		echo 'Failed to remove tests/test.'; \
	fi

	@if test -x "$(CURDIR)/bash/ask"; then \
		chmod -x "$(CURDIR)/bash/ask" && \
		echo 'Unchmodded bash/ask.' || \
		echo 'Failed to unchmod bash/ask.'; \
	fi

	@if test -x "$(CURDIR)/$(D_TEST)/prepare_assets.sh"; then \
		chmod -x "$(CURDIR)/$(D_TEST)/prepare_assets.sh" && \
		echo 'Unchmodded tests/prepare_assets.sh.' || \
		echo 'Failed to unchmod tests/prepare_assets.sh.'; \
	fi