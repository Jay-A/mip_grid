#!/bin/bash
set -euo pipefail

# HARD PURGE
module --force purge
module reset 2>/dev/null || true

# -------------------------------------------------------
# mip_grid - C++ / MPI build script
# -------------------------------------------------------

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# -------------------------------------------------------
# Usage function
# -------------------------------------------------------
usage() {
    cat <<EOF
Usage: $(basename "$0") [MODE] [-b BUILD_TYPE]

Modes:
  build        Configure (if needed) + build (DEFAULT)
  configure    Only run CMake configure step
  clean        Remove build directory only
  rebuild      Clean + configure + build

Options:
  -b, --build-type <type>    Debug, Release, RelWithDebInfo, MinSizeRel
  -h, --help                 Show help

Examples:
  $(basename "$0")                     # build (default Release)
  $(basename "$0") build -b Debug
  $(basename "$0") rebuild -b Release
  $(basename "$0") configure -b Debug
  $(basename "$0") clean
EOF
    exit 0
}

# -------------------------------------------------------
# 0. Defaults
# -------------------------------------------------------
MODE="build"
BUILD_TYPE="Release"

# -------------------------------------------------------
# 1. Parse arguments
# -------------------------------------------------------
while [[ $# -gt 0 ]]; do
    case "$1" in
        build|configure|clean|rebuild)
            MODE="$1"
            shift
            ;;
        -b|--build-type)
            if [[ -n "${2-}" ]]; then
                BUILD_TYPE="$2"
                shift 2
            else
                echo "ERROR: -b|--build-type requires an argument"
                usage
            fi
            ;;
        -h|--help)
            usage
            ;;
        *)
            echo "ERROR: Unknown argument: $1"
            usage
            ;;
    esac
done

# -------------------------------------------------------
# 2. Validate build type
# -------------------------------------------------------
VALID_BUILD_TYPES=("Debug" "Release" "RelWithDebInfo" "MinSizeRel")

if [[ ! " ${VALID_BUILD_TYPES[*]} " =~ " ${BUILD_TYPE} " ]]; then
    echo "ERROR: Invalid build type: $BUILD_TYPE"
    exit 1
fi

# -------------------------------------------------------
# 3. Load environment
# -------------------------------------------------------
echo "[mip_grid] Loading HPC environment..."

if [ -f "$SCRIPT_DIR/env.sh" ]; then
    source "$SCRIPT_DIR/env.sh"
fi

if [ -f "$SCRIPT_DIR/env.sh.local" ]; then
    source "$SCRIPT_DIR/env.sh.local"
fi

echo "--------------------------------------------------"
echo "[mip_grid] Mode: $MODE"
echo "[mip_grid] Build type: $BUILD_TYPE"
echo "--------------------------------------------------"

BUILD_DIR="build"

# -------------------------------------------------------
# 4. CLEAN MODE
# -------------------------------------------------------
if [ "$MODE" = "clean" ]; then
    echo "[mip_grid] Cleaning build directory..."
    rm -rf "$BUILD_DIR"
    echo "[mip_grid] Clean complete"
    exit 0
fi

# -------------------------------------------------------
# 5. REBUILD MODE
# -------------------------------------------------------
if [ "$MODE" = "rebuild" ]; then
    echo "[mip_grid] Rebuilding from scratch..."
    rm -rf "$BUILD_DIR"
fi

# -------------------------------------------------------
# 6. CONFIGURE (for build/rebuild/configure)
# -------------------------------------------------------
if [ "$MODE" = "configure" ] || [ "$MODE" = "rebuild" ] || [ "$MODE" = "build" ]; then

    mkdir -p "$BUILD_DIR"

    echo "[mip_grid] Configuring project..."
    cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE"
fi

# -------------------------------------------------------
# 7. BUILD (default + rebuild)
# -------------------------------------------------------
if [ "$MODE" = "build" ] || [ "$MODE" = "rebuild" ]; then

    echo "[mip_grid] Building project..."
    cmake --build "$BUILD_DIR" -j $(nproc 2>/dev/null || echo 4)
fi

# -------------------------------------------------------
# 8. Completion
# -------------------------------------------------------
echo "=================================================="
echo "[mip_grid] Done ($MODE)"
echo "=================================================="
