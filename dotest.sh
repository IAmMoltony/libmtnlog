#!/bin/sh

function DoTestGCC() {
    echo "Doing test using GCC"
    gcc test.c source/mtnlog.c -iquote include -o test && ./test
}

function DoTestClang() {
    echo "Doing test using Clang"
    clang test.c source/mtnlog.c -iquote include -o test && ./test
}

function DoTestTCC() {
    echo "Doing test using TinyCC"
    tcc test.c source/mtnlog.c -Iinclude -o test && ./test
}

function DoTestAll() {
    echo "Doing test using every compiler"
    DoTestGCC
    DoTestClang
    DoTestTCC
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
elif [ "$1" == "all" ]; then
    DoTestAll
else
    echo "Unknown compiler '$1'"
    echo "Valid compilers are 'gcc', 'clang', 'tcc', 'tinycc', 'all'"
    exit 1
fi
