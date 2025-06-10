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
default: $(SRCDIR)/test.c
	$(CC) $(CFLAGS) $< -o $(RUNDIR)/run.out



# Compile source files
$(BUILDDIR)/list.o: $(SRCDIR)/list.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/test.o: $(SRCDIR)/test.c
	$(CC) $(CFLAGS) -c $< -o $@
