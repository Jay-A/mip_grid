cmake_minimum_required(VERSION 3.20)

set(JSON_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external)
set(JSON_HEADER "${JSON_INCLUDE_DIR}/json.hpp")

if(NOT EXISTS "${JSON_HEADER}")
    message(STATUS "-------------------------------------------------")
    message(STATUS "Missing dependency: nlohmann/json.hpp")
    message(STATUS "-------------------------------------------------")
    message(STATUS "This project requires json.hpp (header-only).")
    message(STATUS "")
    message(STATUS "Install it with:")
    message(STATUS "  mkdir -p external && wget https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp -O external/json.hpp")
    message(STATUS " "      )
    message(STATUS "-------------------------------------------------")

    message(FATAL_ERROR "Missing required dependency: external/json.hpp")
endif()

project(mip_grid LANGUAGES CXX)

# -------------------------------------------------------
# Project policy
# -------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(Config)

# -------------------------------------------------------
# C++ standard
# -------------------------------------------------------
set(CMAKE_CXX_STANDARD ${MIP_GRID_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${JSON_INCLUDE_DIR})

# -------------------------------------------------------
# MPI (core dependency)
# -------------------------------------------------------
find_package(MPI REQUIRED)

message(STATUS "-------------------------------------------------")
message(STATUS "MPI CXX Compiler: ${MPI_CXX_COMPILER}")
message(STATUS "MPI Version: ${MPI_CXX_VERSION}")
message(STATUS "-------------------------------------------------")

# -------------------------------------------------------
# MPI preference warning (non-fatal)
# -------------------------------------------------------
if (MPI_CXX_COMPILER MATCHES "mvapich")
    message(WARNING "MVAPICH detected. OpenMPI is preferred for this project.")
elseif (MPI_CXX_COMPILER MATCHES "openmpi")
    message(STATUS "OpenMPI detected (preferred).")
endif()

# -------------------------------------------------------
# HDF5 (optional dependency controlled by environment)
# -------------------------------------------------------

find_package(HDF5 REQUIRED COMPONENTS C CXX)

message(STATUS "-------------------------------------------------")
message(STATUS "HDF5 found")
message(STATUS "HDF5 include dirs: ${HDF5_INCLUDE_DIRS}")
message(STATUS "HDF5 libraries: ${HDF5_LIBRARIES}")
message(STATUS "-------------------------------------------------")

# -------------------------------------------------------
# Global output directories (ADD HERE)
# -------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# -------------------------------------------------------
# Subdirectories
# -------------------------------------------------------
add_subdirectory(cpp)

# -------------------------------------------------------
# Testing (future CI hook)
# -------------------------------------------------------
enable_testing()
add_subdirectory(tests)

add_subdirectory(examples)
