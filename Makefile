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
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.c=.o)) $(AOBJECTS)

# Derive Header files from source files
HEADERS := $(SOURCES:.c=.h)

# Subprojects
SUBPROCS := subprojects

# Export dir
TARDIR := target

export: $(OBJECTS)
	@echo "Linking $^"
	@mkdir -p target
	@ar rcs $(TARDIR)/utils.a
	@for h in $(HEADERS) ; do \
		cp $$h target/ ; \
	done



# Default target for source files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h
	@echo "Building $(shell basename $@)"
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@
