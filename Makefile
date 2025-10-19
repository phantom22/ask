ifneq ($(shell uname -s),Linux)
$(error This Makefile only supports linux.)
endif

.PHONY: setup clean cleantests all rebuildhash

EXECUTABLE = ask
TESTEXECUTABLE = test

CC = g++
CFLAGS = -xc -std=c23 -O3 -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L # -s -DNDEBUG
LDFLAGS = -lc
LIBS = -lncurses -ljansson -lcrypto
HASH_FILE := .script_hash

SRCDIR = ask
BUILDDIR = build
BINDIR = bin

TESTSDIR = tests
TEST = $(TESTSDIR)/tests.cpp
TESTCFLAGS := -std=c++20 -O3 -DHOME_DIR=\"$(HOME)\" -DPROJECT_DIR=\"$(CURDIR)\"
TESTLIBS = $(LIBS) -lgtest -lgtest_main -pthread

# Recursively find all .c files (Unix/Linux/macOS)
# the windows part also converts backslashes to forward slashes for consistency
SOURCES := $(shell find $(SRCDIR) -name '*.c')

# Generate flat object file paths, no preserving folder structure in build/
OBJECTS = $(addprefix $(BUILDDIR)/, $(notdir $(SOURCES:.c=.o)))

$(shell mkdir -p bin build)

# Default target
all: $(BINDIR)/$(EXECUTABLE)

# Link all objects into executable
$(BINDIR)/$(EXECUTABLE): $(OBJECTS)
	@echo linking
	@$(CC) $(LDFLAGS) $(LIBDIRS) $^ -o $@ $(LIBS)
	@echo compiled executable at \"$(BINDIR)/$(EXECUTABLE)\"

# For the c files that are in %SRCDIR%
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@echo compiling $<
	@$(CC) $(CFLAGS) $(INCLUDEDIRS) -c $< -o $@

# For the c files that are in %SRCDIR% subfolders
$(BUILDDIR)/%.o: $(SRCDIR)/*/%.c
	@echo compiling $<
	@$(CC) $(CFLAGS) $(INCLUDEDIRS) -c $< -o $@

# Clean build artifacts, suppress errors where there are no artefacts to clean then call make with silent flag
clean:
	@rm -f $(BUILDDIR)/*.o 2>&1
	@rm -f $(BINDIR)/$(EXECUTABLE) 2>&1
	@make -s

tests: $(TESTSDIR)/$(TESTEXECUTABLE)

$(TESTSDIR)/$(TESTEXECUTABLE): $(TEST)
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
	@$(CC) $(TESTCFLAGS) -DSHELL_SCRIPT_SHA256SUM=\"$(HASH)\" $< -o $@ $(TESTLIBS)
	@echo compiled executable at \"$(TESTSDIR)/$(TESTEXECUTABLE)\"


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
	@rm -f $(TESTSDIR)/$(TESTEXECUTABLE) 2>&1
	@rm -rf $(TESTSDIR)/assets 2>&1
	@make -s tests

setup:
	@chmod +x "$(CURDIR)/bash/ask"
	@chmod +x "$(CURDIR)/tests/prepare_assets.sh"
	@echo Settings \"./bash/ask\" and \"./tests/prepare_assets.sh\" as executables.