# libmtnlog

[![CodeFactor](https://www.codefactor.io/repository/github/iammoltony/libmtnlog/badge)](https://www.codefactor.io/repository/github/iammoltony/libmtnlog)

`libmtnlog` is my really simple logging library. Originally created for [ds-craft](https://github.com/IAmMoltony/ds-craft).

## Using the library

Paste the header and source code of the library into your project somewhere. I
recommend creating a separate folder for it and configuring your build system to
add `libmtnlog`'s include files and compile its source files.

## Features

- Logging messages into stdout
- Three log levels: info, warning and error
- Logging with timestamps
- Logging with context (function, file and line)
- Change what log level will be printed to the user
- Logging into a file
- Colored log messages

## Testing

Instead of using fancy testing toolkits, there is instead a program that
utilises every feature of the library and it is checked whether the output
looks right.

### Running tests

Tests can be run using the `dotest.sh` script:

`./dotest.sh`

By default the script tests using GCC, but it also supports other compilers. To
see the whole list, you can bring up help: `./dotest.sh help`

## Docs

`libmtnlog` uses [Doxygen](https://doxygen.nl) for generating documentation.
All you have to do is install it and run `doxygen` in the repo folder.

## Platform compatibility

The library has been tested and works on POSIX environments (Linux, Mac, BSD,
...) and on Windows.

On Windows, `libmtnlog` uses the Windows API for colored output. On other
platforms it uses ANSI escape sequences.

Before turning color on, make sure that the terminal supports color. You can
use the `mtnlogCheckColor()` function for that.

## Compiler compatibility

The library is known to work on the following compilers:

- GCC
- Clang
- TCC (TinyCC)
- MinGW (i686 and x86_64)

**Note:** MSVC (Visual C++) might work but I couldn't get it working on Wine and
couldn't test it. Use at your own risk.
