# libmtnlog

`libmtnlog` is my really simple logging library. Originally created for [ds-craft](https://github.com/IAmMoltony/ds-craft).

## Using the library

Paste the header and source code of the library into your project somewhere. I recommend creating a separate folder for it and configuring your build system to add `libmtnlog`'s include files and compile its source files.

## Features

- Logging messages into stdout
- Three log levels: info, warning and error
- Change what log level will be printed to the user
- Logging into a file
- Colored log messages

## Docs

`libmtnlog` uses [Doxygen](https://doxygen.nl) for generating documentation. All you have to do is install it and run `doxygen` in the repo
folder.

## Platform compatibility

The library has been tested and works on POSIX environments (Linux, Mac, BSD, ...) and on Windows. Colored output uses ANSI escape sequences, so make sure you check if the terminal supports color before turning color on.

On Windows, `libmtnlog` uses the Windows API for colored output.

## Compiler compatibility

The library is known to work on the following compilers:

- GCC
- Clang
- TCC (TinyCC)
- MinGW (i686 and x86_64)

**Note:** MSVC (Visual C++) *might* work but I couldn't get it working on Wine and couldn't test it. Use at your own risk.

