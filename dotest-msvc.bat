@echo off
echo "Doing test using MSVC"
cl /I include /D MTNLOG_NO_STDBOOL test.c source/mtnlog.c && test
