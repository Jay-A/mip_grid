#!/usr/bin/env bash
set -euo pipefail

EXEC="$1"
shift

# --- fixed test limits ---
MAX_NODES=1
MAX_TASKS=6

# --- detect environment ---
SLURM_NNODES=${SLURM_NNODES:-1}
SLURM_NTASKS=${SLURM_NTASKS:-6}

# --- override logic ---
# If Slurm gives more than allowed, clamp it
NODES=$(( SLURM_NNODES < MAX_NODES ? SLURM_NNODES : MAX_NODES ))
TASKS=$(( SLURM_NTASKS < MAX_TASKS ? SLURM_NTASKS : MAX_TASKS ))

# --- CI / non-Slurm fallback ---
if [ -z "${SLURM_NTASKS:-}" ]; then
    NODES=1
    TASKS=6
fi

echo "[mpi_test] running with nodes=$NODES tasks=$TASKS"
echo "[mpi_test] exec=$EXEC"

srun -N"$NODES" -n"$TASKS" "$EXEC" "$@"
