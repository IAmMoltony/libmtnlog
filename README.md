# MtnLog

[![CodeFactor](https://www.codefactor.io/repository/github/iammoltony/libmtnlog/badge)](https://www.codefactor.io/repository/github/iammoltony/libmtnlog)

MtnLog is a really simple logging library. Originally created for [ds-craft](https://github.com/IAmMoltony/ds-craft).

## Notice

This library is now in *maintenance mode*. It will receive no new functionality, but
it will still have bug fixes, accept pull requests, review issues, add support for
more compilers and platforms and documentation updates.

## Using the library

Paste the header and source code of the library into your project somewhere. I
recommend creating a separate folder for it and configuring your build system to
add MtnLog's include files and compile its source files.

## Features

- Logging messages into stdout
- Three log levels: info, warning and error
- Logging with timestamps
- Logging with context (function, file and line)
- Change what log level will be printed to the user
- Logging into a file
- Colored log messages
- Log callback - run any code after logging a message

## Testing

Instead of using fancy testing toolkits, there is instead a script that
utilises every feature of the library and it is checked whether the output
looks about right.

### Running tests

#### Using MSVC

MSVC testing can be done using `dotest-msvc.bat` like this:

`dotest-msvc`

#### Non-MSVC

Non-MSVC tests can be run using the `dotest.sh` script:

`./dotest.sh`

By default the script tests using GCC, but it also supports other compilers. To
see the whole list, you can bring up help: `./dotest.sh help`

Please note that you need `bash` installed on your machine for the script
to work.

## Docs

MtnLog uses [Doxygen](https://doxygen.nl) for generating documentation.
All you have to do is install it and run `doxygen` in the repo folder.

## Platform compatibility

The library was tested on the following platforms:

- Linux
- Windows
- Nintendo DS (homebrew using devkitPro)
- Haiku
- BSD (tested on NetBSD)
- Nintendo GBA (homebrew using devkitPro)

However, it should work with other POSIX-compatible OSes and environments,
such as macOS.

On Windows, the library uses the Windows API for colored output. On other
platforms it uses ANSI escape sequences.

Before turning color on, make sure that the terminal supports color. You can
use the `mtnlogCheckColor()` function for that.

## Compiler compatibility

The library is known to work on the following compilers:

- GCC
- Clang
- TCC (TinyCC)
- MinGW (i686 and x86_64)
- Intel oneAPI C compiler
- Visual Studio 2010+

## Troubleshooting

### `stdbool.h` not found

MtnLog can work without `stdbool.h`. Define the `MTNLOG_NO_STDBOOL` macro
in your compiler/build system options.

This problem usually happens on really old compilers.
