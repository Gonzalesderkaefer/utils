# Project name
PRJNAME := utils

# Compiler
CC := cc

# Compile flags
CFLAGS := -Werror -Wall -std=c99 -g

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

# All source files
SOURCES := $(shell find $(SRCDIR) -type f -name *.c)

# Derive Object files from source files
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.c=.o)) $(AOBJECTS)

# Derive Header files from source files
HEADERS := $(SOURCES:.c=.h)

# Export dir
TARDIR := target

test: export
	@$(CC) $(CFLAGS) -c test/test.c -o $(BUILDDIR)/test.o
	@$(CC) $(CFLAGS) $(OBJECTS) $(BUILDDIR)/test.o $(LFLAGS) -o run.out


export: $(OBJECTS) $(TARDIR)/list.h $(TARDIR)/temp_list.h
	@echo "Linking $^"
	@mkdir -p target
	@ar rcs $(LIBDIR)/libutils.a $(OBJECTS)
	@cp $(LIBDIR)/libutils.a $(TARDIR)/
	@for h in $(HEADERS) ; do \
		cp $$h target/ ; \
	done
	@cp $(SRCDIR)/list/list.h $(TARDIR)/list.h

$(TARDIR)/list.h: $(SRCDIR)/list/list.h

$(TARDIR)/temp_list.h: $(SRCDIR)/list/temp_list.h
	cp $< $@

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
	rm run.out
	rm -rf target/*
