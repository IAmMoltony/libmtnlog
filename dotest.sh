#!/bin/sh
gcc test.c source/mtnlog.c -iquote include -o test && ./test
