# C Compiler
CC := gcc

# Compiler Flags
CFLAGS := -g

# Source directory
SRCDIR := src

# Builds go here
BUILDDIR := build

# This is the export
TARGETDIR := target

# Get all source files
SOURCES := $(shell find $(SRCDIR) -type f -name *.c)

# Addional Object files
AOBJECTS :=

# Derive Object files from source files
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.c=.o)) $(AOBJECTS)

# Derive Header files from source files
HEADERS := $(SOURCES:.c=.h)

# Subprojects
SUBPROCS := subprojects

default: $(OBJECTS)
	@echo "Linking jazzy.out"
	$(CC) $^ -o run.out

export: $(OBJECTS)
	@echo "Exporting .."
	@mv $< $(TARGETDIR)/list.o
	@cp $(SRCDIR)/list/list.h $(TARGETDIR)/list.h


# Custom recipe for main.c
$(BUILDDIR)/main.o: $(SRCDIR)/main.c
	@echo "Building $(shell basename $@)"
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@


# Default target for source files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h
	@echo "Building $(shell basename $@)"
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@
