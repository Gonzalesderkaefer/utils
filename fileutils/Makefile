# C Compiler
CC := gcc

# Compiler Flags
CFLAGS :=

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

export: $(OBJECTS)
	@echo "Linking $^"
	@mkdir -p target
	@ld -r $^ -o target/fileutils.o
	@for h in $(HEADERS) ; do \
		cp $$h target/ ; \
	done



# Default target for source files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h
	@echo "Building $(shell basename $@)"
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@
