#! /bin/bash

set -euo pipefail

DIR="$(dirname "$(readlink -f "$0")")"
BASE_PATH="$DIR/../.."
cd "$BASE_PATH"

INPUT="$1"
shift 1
build/Compiler "$@" <"$INPUT"
