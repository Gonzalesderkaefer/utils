#!/usr/bin/sh
# Color variables for output

black="\e[0;30m"
red="\e[0;31m"
green="\e[0;32m"
yellow="\e[0;33m"
blue="\e[0;34m"
purple="\e[0;35m"
cyan="\e[0;36m"
white="\e[0;37m"
end="\e[0m"




# The name of the project
PROJECT_NAME="hash_table";

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

# These are all directories
DIRS="${SRCDIR} ${BUILDDIR} ${HEADDIR} ${TARGETDIR} ${RUNDIR} ${TESTDIR} ${LIBRARIES}";

# Source files
SRCFILES="src/*.c";

# Addional Object files
AOBJFILES="";

# Object files
OBJFILES="$(echo "${SRCFILES}" | sed 's/src\(.*\).c/build\1.o/g') ${AOBJFILES}";

# This is the header file that will be provided provided
HEADER="";


# Build every source file to its object file
obj_build() {
    for src in ${SRCFILES}; do
        # Get object file from
        obj="$(echo "${src}" | sed 's/src\(.*\).c/build\1.o/g')";

        # Compile object file
        if ! [ -f ${obj} ] || [ ${obj} -ot ${src} ]; then
            printf "${green}Building ${src} ${end}\n"
            ${CC} ${CFLAGS} -c "${src}" -o "${obj}"
        fi
    done
}


# create all directories
init() {
    for dir in ${DIRS}; do
        mkdir -p "$dir"
    done
}

# Build test object file
test_build() {
    # If test.c does not exist return;
    if ! [ -f ${TESTDIR}/test.c ]; then
        printf "${red} ${TESTDIR}/test.c not found ${end}";
        return;
    fi
    printf "${green}Building ${TESTDIR}/test.c ${end}\n"
    ${CC} ${CFLAGS} -c "${TESTDIR}/test.c" -o "${BUILDDIR}/test.o"
}

# This function creates the release (Specific to project)
release() {
    echo "undefined";
}

# This function runs and builds necessary files for the test
test() {
    test_build;
    ${CC} ${CFLAGS} ${BUILDDIR}/*.o -o run/test.out;
    exec run/test.out;
}

# The function  that is run when this scripts is invoked without args
default() {
    obj_build;
}

case $1 in
    "test")
        test;
        ;;
    "release")
        release;
        ;;
    "init")
        init;
        ;;
    *)
        default;
        ;;
esac

