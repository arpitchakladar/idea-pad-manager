#!/bin/sh
find src -type f -name "*.cpp" -exec clang-tidy -fix --fix-errors --fix-notes {} +
