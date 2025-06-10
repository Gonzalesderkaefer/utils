# C-Compiler
CC := gcc

# Default Compile flags
CFLAGS := -Wall -Werror

# Source dir
SRCDIR := src

# Build dir
BUILDDIR := build

# Run dir (for executables)
RUNDIR := run



# Compile test
default: $(SRCDIR)/main.c
	$(CC) $(CFLAGS) $< -o $(RUNDIR)/run.out



$(BUILDDIR)/main.o: $(SRCDIR)/main.c
	$(CC) $(CFLAGS) -c $< -o $@
