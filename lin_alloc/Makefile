# C Compiler
CC := gcc

# Compiler Flags
CFLAGS := -Werror -Wall

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

# Export
export: $(OBJECTS)
	mkdir target
	cp $(SRCDIR)/lin_alloc.h target/lin_alloc.h
	cp $(BUILDDIR)/lin_alloc.o target/lin_alloc.o


test: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o run/test.out

# Cleanup all built files
clean:
	rm -rf build
	rm -rf target


$(BUILDDIR)/test.o: $(SRCDIR)/test.c
	$(CC) $(CFLAGS) -c $< -o $@

# Default target for source files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h
	@echo "Building $(shell basename $@)"
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@
