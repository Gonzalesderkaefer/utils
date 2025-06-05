#!/usr/bin/sh

# Name of the project
PROJNAME="fileutils"

# Compiler
CC="gcc";

# C-Compiler flags
CFLAGS="-Wall -Werror";

# Source directory
SRCDIR="src";

# This is for static libraries and other binaries
BUILDDIR="build";

# Here are the used header files
HEADDIR="src/headers";

# This is where the header file and compiled libraries will land
TARGETDIR="target";

# This is where executables will land
RUNDIR="run";

# Test directory
TESTDIR="test";

# Other libraries we intend to use in our code
LIBRARIES="lib";

# Source files
SRCFILES="src/*.c";

# Object files
OBJFILES="$(echo ${SRCFILES} | sed 's/src\(.*\).c/build\1.o/g') ${BUILDDIR}/path.o";


# Function to build all the source files into object files
build_obj() {
    for src in ${SRCFILES}; do
        # get obj file name
        obj=$(echo ${src} | sed 's/src\(.*\).c/build\1.o/g');
        # compile
        ${CC} ${CFLAGS} -c ${src} -o ${obj}
    done
}

# Create test object file
test_obj() {
    ${CC} ${CFLAGS} -c test/test.c -o build/test.o
}


# Build library for export as
exp() {
    build_obj;
    if [ -e ${BUILDDIR}/test.o ]; then # Delete test object since it exports main
        /usr/bin/rm ${BUILDDIR}/test.o;
    fi
    ld -r ${OBJFILES} -o ${TARGETDIR}/${PROJNAME}.o; # Paritial linking
    cp ${HEADDIR}/${PROJNAME}.h ${TARGETDIR}/${PROJNAME}.h;
}


# Compile test and run it
test() {
    exp;
    test_obj;
    ${CC} ${CFLAGS} ${BUILDDIR}/test.o ${TARGETDIR}/${PROJNAME}.o -o ${RUNDIR}/test.out
    exec ${RUNDIR}/test.out
}

default() {
    test;
}



init() {
    # Create directories
    mkdir build;
    mkdir lib;
    mkdir run;
    mkdir src;
    mkdir target;
    mkdir test;
}




case $1 in
    "init")
        init;
        ;;

    "test")
        test;
        ;;

    "exp")
        exp;
        ;;

    *)
        default;
        ;;
esac


