#!/bin/bash
# -------------------------------------------------------
# mip_grid environment setup (portable layer)
# -------------------------------------------------------
#
# This file defines the expected toolchain structure.
# It does NOT force system-specific modules.
#
# For local builds:
#   copy env.sh.example -> env.sh.local
#
# -------------------------------------------------------

echo "[mip_grid] Loading portable environment policy..."

# Minimal safe baseline
module purge
module load StdEnv 2>/dev/null || true

# -------------------------------------------------------
# Toolchain policy (NO system coupling here)
# -------------------------------------------------------

export MIP_GRID_HDF5_MODE="serial"   # serial | parallel | off
export MIP_GRID_MPI_ENABLED="true"

echo "[mip_grid] Policy:"
echo "  HDF5 mode: ${MIP_GRID_HDF5_MODE}"
echo "  MPI enabled: ${MIP_GRID_MPI_ENABLED}"

echo "[mip_grid] NOTE: No compiler or MPI modules selected here."
echo "[mip_grid] Use env.sh.local for system-specific toolchain."
