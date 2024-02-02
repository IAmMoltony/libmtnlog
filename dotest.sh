#!/usr/bin/env bash

DoTestGCC() {
    if ! command -v gcc &> /dev/null; then
        echo "GCC not found (command \`gcc')"
    else
        echo "Doing test using GCC"
        gcc test.c source/mtnlog.c -iquote include -g -o test -std=gnu89 -Wall -Wextra && ./test
    fi
}

DoTestClang() {
    if ! command -v clang &> /dev/null; then
        echo "Clang not found (command \`clang')"
    else
        echo "Doing test using Clang"
        clang test.c source/mtnlog.c -iquote include -g -o test && ./test
    fi
}

DoTestTCC() {
    if ! command -v tcc &> /dev/null; then
        echo "TCC not found (command \`tcc')"
    else
        echo "Doing test using TinyCC"
        tcc test.c source/mtnlog.c -Iinclude -g -o test && ./test
    fi
}

DoTestMinGW32() {
    if ! command -v i686-w64-mingw32-gcc &> /dev/null; then
        echo "32-bit MinGW not found (command \`i686-w64-mingw32-gcc')"
    else
        echo "Doing test using 32-bit MinGW"
        if ! command -v wine &> /dev/null; then
            echo "Wine not found (command \`wine')"
        else
            i686-w64-mingw32-gcc test.c source/mtnlog.c -iquote include -DWIN32_LEAN_AND_MEAN -g -o test && wine test.exe
        fi
    fi
}

DoTestMinGW64() {
    if ! command -v x86_64-w64-mingw32-gcc &> /dev/null; then
        echo "64-bit MinGW not found (command \`x86_64-w64-mingw32-gcc')"
    else
        echo "Doing test using 64-bit MinGW"
        if ! command -v wine &> /dev/null; then
            echo "Wine not found (command \`wine')"
        else
            x86_64-w64-mingw32-gcc test.c source/mtnlog.c -iquote include -DWIN32_LEAN_AND_MEAN -g -o test && wine64 test.exe
        fi
    fi
}

DoTestICX() {
    if ! command -v icx &> /dev/null; then
        echo "Intel compiler not found (command \`icx')"
    else
        echo "Doing test using Intel compiler"
        icx -Iinclude test.c source/mtnlog.c -Rno-debug-disables-optimization -g -o test && ./test
    fi
}

DoTestAll() {
    echo "Doing test using every compiler"
    DoTestGCC
    DoTestClang
    DoTestTCC
    DoTestMinGW32
    DoTestMinGW64
    DoTestICX
}

PrintHelp() {
    echo "Usage: $0 <compiler>"
    echo "       $0 help"
    echo "Valid compilers are 'gcc', 'clang', 'tcc', 'tinycc' (alias of 'tcc'), 'mingw' (test using both 'mingw32' and 'mingw64'), 'mingw32', 'mingw64', 'intel', 'icx' (alias of 'intel'), 'all'"
    echo "'all' tests using all compilers"
}

if [ "$1" == "" ]; then
    DoTestGCC
elif [ "$1" == "gcc" ]; then
    DoTestGCC
elif [ "$1" == "clang" ]; then
    DoTestClang
elif [ "$1" == "tcc" ]; then
    DoTestTCC
elif [ "$1" == "tinycc" ]; then
    DoTestTCC
elif [ "$1" == "mingw" ]; then
    DoTestMinGW32
    DoTestMinGW64
elif [ "$1" == "mingw32" ]; then
    DoTestMinGW32
elif [ "$1" == "mingw64" ]; then
    DoTestMinGW64
elif [ "$1" == "intel" ]; then
    DoTestICX
elif [ "$1" == "icx" ]; then
    DoTestICX
elif [ "$1" == "all" ]; then
    DoTestAll
elif [ "$1" == "help" ]; then
    PrintHelp
else
    echo "Unknown compiler '$1'"
    PrintHelp
    exit 1
fi
