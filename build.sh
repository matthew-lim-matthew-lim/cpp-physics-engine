#!/usr/bin/env bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Source Conan build environment (compiler/linker flags via pkg-config)
source "$SCRIPT_DIR/build/conanbuild.sh"

# Build
make -C "$SCRIPT_DIR/src"

# Source Conan run environment (LD_LIBRARY_PATH for shared SDL3 libs)
source "$SCRIPT_DIR/build/conanrun.sh"

# Run
exec "$SCRIPT_DIR/src/program"
