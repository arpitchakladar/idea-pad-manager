#!/bin/sh
# Run clang-tidy only on files changed since the previous commit

PREVIOUS_COMMIT=$(git rev-parse HEAD~1)

if [ -z "$PREVIOUS_COMMIT" ]; then
  echo "No previous commit found, running tidy on all files..."
  find src -type f \( -name "*.cpp" -o -name "*.hpp" \) -exec clang-tidy -fix --fix-errors --fix-notes {} +
else
  CHANGED_FILES=$(git diff --name-only $PREVIOUS_COMMIT -- src | grep -E '\.(cpp|hpp)$')
  
  if [ -z "$CHANGED_FILES" ]; then
    echo "No .cpp or .hpp files changed, skipping tidy."
  else
    echo "Running tidy on changed files:"
    echo "$CHANGED_FILES"
    echo ""
    echo "$CHANGED_FILES" | xargs clang-tidy -fix --fix-errors --fix-notes
  fi
fi

./scripts/format.sh
