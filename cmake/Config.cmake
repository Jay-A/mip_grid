# -------------------------------------------------------
# mip_grid project policy configuration
# -------------------------------------------------------

# Preferred MPI implementation (soft preference only)
set(MIP_GRID_PREFERRED_MPI "OpenMPI")

# Require MPI (hard requirement)
set(MIP_GRID_REQUIRE_MPI ON)

# C++ standard policy
set(MIP_GRID_CXX_STANDARD 20)

# Enable warnings aggressively (good for HPC correctness)
set(MIP_GRID_ENABLE_WARNINGS ON)
