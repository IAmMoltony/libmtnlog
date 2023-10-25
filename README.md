# libmtnlog

`libmtnlog` is my logging library. Originally created for [ds-craft](https://github.com/IAmMoltony/ds-craft).

## Using the library

Paste the header and source code of the library into your project somewhere. I recommend creating a separate folder for it and configuring your build system to add `libmtnlog`'s include files and compile its source files.

## Features

- Logging messages
- Three log levels: info, warning and error
- Ability to change what log level will be printed to the user
- Logging into a file

## Platform compatibility

The library has been tested and works on POSIX environments (Linux, Mac, BSD, ...). I'm not sure if this works on Windows.
