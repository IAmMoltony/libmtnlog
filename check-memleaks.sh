#!/usr/bin/env bash
if ! command -v valgrind &> /dev/null; then
    echo "Valgrind not found. Please install it. (\`valgrind')"
    exit 1
fi

valgrind --leak-check=full --show-leak-kinds=all ./test
