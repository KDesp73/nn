# Compiler and flags
CC = gcc

# Library directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
DIST_DIR = dist

CFLAGS = -Wall -I$(INCLUDE_DIR) -fPIC
LDFLAGS = -lm

LIBRARY_NAME = nn
SO_NAME = lib$(LIBRARY_NAME).so
A_NAME = lib$(LIBRARY_NAME).a

TARGET = $(LIBRARY_NAME)

# Repl directories
REPL_SRC_DIR = repl/src
REPL_INCLUDE_DIR = repl/include
REPL_BUILD_DIR = repl/build
REPL_TARGET = repl/nni

REPL_CFLAGS = -Wall -I$(REPL_INCLUDE_DIR) -I$(INCLUDE_DIR)
REPL_LDFLAGS = -L. -lnn -lm

# Library version
version_file = include/version.h
VERSION_MAJOR = $(shell sed -n -e 's/\#define VERSION_MAJOR \([0-9]*\)/\1/p' $(version_file))
VERSION_MINOR = $(shell sed -n -e 's/\#define VERSION_MINOR \([0-9]*\)/\1/p' $(version_file))
VERSION_PATCH = $(shell sed -n -e 's/\#define VERSION_PATCH \([0-9]*\)/\1/p' $(version_file))
VERSION = $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)

LIB_INSTALL_DIR = /usr/local/lib
HEADERS_INSTALL_DIR = /usr/local/include/nn

# Determine the build type
ifneq ($(type), RELEASE)
	CFLAGS += -DDEBUG -ggdb
	REPL_CFLAGS += -DDEBUG -ggdb
else
	CFLAGS += -O3
	REPL_CFLAGS += -O3
endif

# Library source and object files
SRC_FILES := $(shell find $(SRC_DIR) -name '*.c')
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Repl source and object files
REPL_SRC_FILES := $(shell find $(REPL_SRC_DIR) -name '*.c')
REPL_OBJ_FILES = $(patsubst $(REPL_SRC_DIR)/%.c,$(REPL_BUILD_DIR)/%.o,$(REPL_SRC_FILES))

# Default target
.DEFAULT_GOAL := help

# Total source file count
TOTAL_FILES := $(words $(SRC_FILES))
REPL_TOTAL_FILES := $(words $(REPL_SRC_FILES))

# Counter to track progress
counter = 0
repl_counter = 0

# Targets

.PHONY: all
all: check_tools $(BUILD_DIR) $(REPL_BUILD_DIR) static shared $(TARGET) $(REPL_TARGET) ## Build the project
	@echo "Build complete."

.PHONY: check_tools
check_tools: ## Check if necessary tools are available
	@command -v gcc >/dev/null 2>&1 || { echo >&2 "[ERRO] gcc is not installed."; exit 1; }
	@command -v bear >/dev/null 2>&1 || { echo >&2 "[WARN] bear is not installed. Skipping compile_commands.json target."; }

$(BUILD_DIR): ## Create the build directory if it doesn't exist
	@echo "[INFO] Creating build directory"
	mkdir -p $(BUILD_DIR)

$(REPL_BUILD_DIR): ## Create the repl build directory
	@echo "[INFO] Creating repl build directory"
	mkdir -p $(REPL_BUILD_DIR)

# Library object compilation
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(eval counter=$(shell echo $$(($(counter)+1))))
	@echo "[$(counter)/$(TOTAL_FILES)] Compiling $< -> $@"
	@$(CC) $(CFLAGS) -c -o $@ $<

# Repl object compilation
$(REPL_BUILD_DIR)/%.o: $(REPL_SRC_DIR)/%.c
	$(eval repl_counter=$(shell echo $$(($(repl_counter)+1))))
	@echo "[$(repl_counter)/$(REPL_TOTAL_FILES)] Compiling $< -> $@"
	@$(CC) $(REPL_CFLAGS) -c -o $@ $<

$(TARGET): $(BUILD_DIR) static ## Build library executable using static library
	@echo "[INFO] Building executable: $(TARGET)"
	@$(CC) src/main.c -o $(TARGET) -L. $(A_NAME) $(LDFLAGS) -I$(INCLUDE_DIR)

$(REPL_TARGET): $(REPL_BUILD_DIR) $(REPL_OBJ_FILES) static shared ## Build the REPL (nni)
	@echo "[INFO] Building REPL"
	@$(CC) -o $@ $(REPL_OBJ_FILES) $(REPL_LDFLAGS)
	@echo "[INFO] Executable created: $(REPL_TARGET)"

.PHONY: shared
shared: $(BUILD_DIR) $(OBJ_FILES) ## Build shared library
	@echo "[INFO] Building shared library: $(SO_NAME)"
	@$(CC) -shared $(CFLAGS) -o $(SO_NAME) $(OBJ_FILES)

.PHONY: static
static: $(BUILD_DIR) $(OBJ_FILES) ## Build static library
	@echo "[INFO] Building static library: $(A_NAME)"
	@$(AR) rcs $(A_NAME) $(OBJ_FILES)

.PHONY: install-repl
install-repl: all ## Install the REPL executable to /usr/bin/
	@echo "[INFO] Installing $(REPL_TARGET) to /usr/bin/"
	cp $(REPL_TARGET) /usr/bin/nni

.PHONY: uninstall-repl
uninstall-repl: ## Remove the REPL executable from /usr/bin/
	@echo "[INFO] Uninstalling nni"
	rm -f /usr/bin/nni

.PHONY: clean
clean: ## Remove all build files and executables
	@echo "[INFO] Cleaning up build directories and executables."
	rm -rf $(BUILD_DIR) $(REPL_BUILD_DIR) $(TARGET) $(REPL_TARGET) $(SO_NAME) $(A_NAME) nni

.PHONY: distclean
distclean: clean ## Perform a full clean, including backup and temporary files
	@echo "[INFO] Performing full clean, removing build directory, dist files, and editor backups."
	rm -f *~ core $(SRC_DIR)/*~ $(DIST_DIR)/*.tar.gz

.PHONY: dist
dist: $(SRC_FILES) ## Create a tarball of the project
	@echo "[INFO] Creating a tarball for version $(VERSION)"
	mkdir -p $(DIST_DIR)
	tar -czvf $(DIST_DIR)/$(TARGET)-$(VERSION).tar.gz $(SRC_DIR) $(INCLUDE_DIR) Makefile README.md

## Generate compile_commands.json
.PHONY: compile_commands.json
compile_commands.json: $(SRC_FILES) ## Generate compile_commands.json
	@echo "[INFO] Generating compile_commands.json"
	bear -- make all

## Show this help message
.PHONY: help
help: ## Show this help message
	@echo "Available commands:"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'

## Enable verbose output for debugging
.PHONY: verbose
verbose: CFLAGS += -DVERBOSE
verbose: REPL_CFLAGS += -DVERBOSE
verbose: all ## Build the project in verbose mode

.PHONY: install
install: clean all ## Install library and headers
	sudo rm -rf $(HEADERS_INSTALL_DIR)
	sudo mkdir $(HEADERS_INSTALL_DIR)
	sudo cp $(SO_NAME) $(LIB_INSTALL_DIR)
	sudo cp $(A_NAME) $(LIB_INSTALL_DIR)
	sudo cp ./include/* $(HEADERS_INSTALL_DIR)
	@echo "[INFO] Library installed successfully"

# Phony targets to avoid conflicts with file names
.PHONY: all clean distclean install uninstall dist compile_commands.json help check_tools verbose install-repl uninstall-repl
