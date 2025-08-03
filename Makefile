# Project name
PRJNAME := utils

# Compiler
CC := cc

# Compile flags
CFLAGS := -Werror -Wall -pedantic -std=c99 -g -Wno-unused-function

# Source directory
SRCDIR := src

# Precomipiled libraries that have to stay
LIBDIR := lib

# Static libraries
STATLIB := $(shell find $(LIBDIR)  -depth -maxdepth 1 -type f -name *.a)

# Combined static library
COMBLIB := $(LIBDIR)/lib$(PRJNAME).a

# Linking flags
LFLAGS := -L./$(LIBDIR) -l$(PRJNAME)

# Build files go here
BUILDDIR := build

# Export dir
TARDIR := target

# Unity tardir
UNITY_TARDIR := target/unity

# All source files
SOURCES := $(shell find $(SRCDIR) -type f -name *.c)

# Derive source files for unity build from sources
UNITYSRC := $(patsubst $(SRCDIR)/%,$(UNITY_TARDIR)/%,$(SOURCES))

# Derive Object files from source files
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.c=.o)) $(AOBJECTS)

# Derive Header files from source files
HEADERS := $(SOURCES:.c=.h)


export: $(OBJECTS)
	@echo "Linking..."
	@mkdir -p target
	@ar rcs $(LIBDIR)/libutils.a $(OBJECTS)
	@cp $(LIBDIR)/libutils.a $(TARDIR)/
	@for h in $(HEADERS) ; do \
		cp $$h target/ ; \
	done


unity: $(UNITYSRC)

# Default target for source files
$(UNITY_TARDIR)/%.c: $(SRCDIR)/%.c
	mkdir -p $(shell dirname $@)
	cp $< $@


test: export
	@$(CC) $(CFLAGS) -c test/test.c -o $(BUILDDIR)/test.o
	@$(CC) $(CFLAGS) $(OBJECTS) $(BUILDDIR)/test.o $(LFLAGS) -o run.out


# Default target to create a combined static library
$(COMBLIB):
	@cd $(LIBDIR) && \
	for file in ./*.a ; do \
		ar -x $$file  ; \
	done 
	@cd ../
	@ar -rcs $@ $(LIBDIR)/*.o
	@/usr/bin/rm -r $(LIBDIR)/*.o


# Default target for source files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h
	@echo "Building $(shell basename $@)"
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -rf build/*
	rm -rf target/*
	$(shell test -e run.out && rm run.out)
