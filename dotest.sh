#!/bin/sh

DoTestGCC() {
    echo "Doing test using GCC"
    gcc test.c source/mtnlog.c -iquote include -o test -std=gnu89 && ./test
}

DoTestClang() {
    echo "Doing test using Clang"
    clang test.c source/mtnlog.c -iquote include -o test && ./test
}

DoTestTCC() {
    echo "Doing test using TinyCC"
    tcc test.c source/mtnlog.c -Iinclude -o test && ./test
}

DoTestMinGW32() {
    echo "Doing test using 32-bit MinGW"
    i686-w64-mingw32-gcc test.c source/mtnlog.c -iquote include -DWIN32_LEAN_AND_MEAN -o test && wine test.exe
}

DoTestMinGW64() {
    echo "Doing test using 64-bit MinGW"
    x86_64-w64-mingw32-gcc test.c source/mtnlog.c -iquote include -DWIN32_LEAN_AND_MEAN -o test && wine64 test.exe
}

DoTestAll() {
    echo "Doing test using every compiler"
    DoTestGCC
    DoTestClang
    DoTestTCC
    DoTestMinGW32
    DoTestMinGW64
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
elif [ "$1" == "all" ]; then
    DoTestAll
else
    echo "Unknown compiler '$1'"
    echo "Valid compilers are 'gcc', 'clang', 'tcc', 'tinycc', 'all'"
    exit 1
fi
