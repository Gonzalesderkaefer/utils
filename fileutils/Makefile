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

# Object files
OBJFILES := $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRCFILES)) $(BUILDDIR)/path.o

# Addional Object files
AOBJFILES :=

# This is the header file that will be provided provided
HEADER :=

export: $(OBJFILES)
	ld -r $^ -o $(TARGETDIR)/fileutils.o
	cp $(HEADDIR)/fileutils.h $(TARGETDIR)/fileutils.h


test: $(BUILDDIR)/test.o export
	gcc $(TARGETDIR)/fileutils.o $< -o $(RUNDIR)/test.out
	$(RUNDIR)/test.out


# Target for Build files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


# Target for test file
$(BUILDDIR)/test.o: $(TESTDIR)/test.c
	$(CC) $(CFLAGS) -c $< -o $@
