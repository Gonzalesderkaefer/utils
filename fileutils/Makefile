# Compiler
CC := gcc

# C-Compiler flags
CFLAGS := -Wall -Werror

# Source directory
SRCDIR := src

# This is for static libraries and other binaries
BUILDDIR := build

# Here are the used header files
HEADDIR := src/headers

# This is where the header file and compiled libraries will land
TARGETDIR := target

# This is where executables will land
RUNDIR := run

# Test directory
TESTDIR := test

# Other libraries we intend to use in our code
LIBRARIES := lib

# Source files
SRCFILES := $(wildcard src/*.c)

# Addional Object files
AOBJFILES := $(BUILDDIR)/path.o

# Object files
OBJFILES := $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRCFILES)) $(AOBJFILES)

# This is the header file that will be provided provided
HEADER :=

export: $(OBJFILES)
	ld -r $^ -o $(TARGETDIR)/fileutils.o
	cp $(HEADDIR)/fileutils.h $(TARGETDIR)/fileutils.h

$(BUILDDIR)/path.o: ../path/Makefile
	(cd $(shell dirname $<); make export)
	@cp $(shell dirname $<)/target/path.o $@

test: $(BUILDDIR)/test.o export
	gcc $(TARGETDIR)/fileutils.o $< -o $(RUNDIR)/test.out
	$(RUNDIR)/test.out


# Target for Build files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


# Target for test file
$(BUILDDIR)/test.o: $(TESTDIR)/test.c
	$(CC) $(CFLAGS) -c $< -o $@
