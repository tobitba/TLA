#! /bin/bash

set -euxo pipefail

DIR="$(dirname "$(readlink -f "$0")")"
BASE_PATH="$DIR/../.."
cd "$BASE_PATH"

rm --force --recursive "$BASE_PATH/build"
rm --force "$BASE_PATH/src/main/c/frontend/lexical-analysis/FlexScanner.c"
rm --force "$BASE_PATH/src/main/c/frontend/syntactic-analysis/BisonParser.c"
rm --force "$BASE_PATH/src/main/c/frontend/syntactic-analysis/BisonParser.h"

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -S . -B build
cd build
make
cd ..

echo "All done."
