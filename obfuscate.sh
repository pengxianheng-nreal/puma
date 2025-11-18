#!/bin/bash
set -e
BIN=${OBFUSCATE_BIN}
find puma -name "*.cc" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp"|xargs $BIN > obfuscate.log
