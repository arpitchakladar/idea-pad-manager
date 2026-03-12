#!/bin/sh
find src -type f \( -name "*.cpp" -o -name "*.hpp" \) -exec clang-tidy -fix --fix-errors --fix-notes {} +
./scripts/format.sh
