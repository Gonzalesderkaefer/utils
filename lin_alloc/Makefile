# C Compiler
CC := gcc

# Compiler Flags
CFLAGS := -Werror -Wall -g

# Source directory
SRCDIR := src

# Builds go here
BUILDDIR := build

# Get all source files
SOURCES := $(shell find $(SRCDIR) -type f -name *.c)

# Derive Object files from source files
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.c=.o))

# Derive Header files from source files
HEADERS := $(SOURCES:.c=.h)

# Subprojects
SUBPROCS := subprojects

default: $(OBJECTS)

test: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o run/test.out

$(BUILDDIR)/test.o: $(SRCDIR)/test.c
	$(CC) $(CFLAGS) -c $< -o $@

# Default target for source files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h
	@echo "Building $(shell basename $@)"
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@
