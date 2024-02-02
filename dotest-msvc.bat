@echo off
echo "Doing test using MSVC"
cl /I include /D WIN32 test.c source/mtnlog.c && test