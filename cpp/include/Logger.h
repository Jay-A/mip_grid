#pragma once

#include <iostream>
#include <string>

/**
 * @file log0.h
 * @brief Lightweight MPI-safe logging utility (rank-0 logger).
 *
 * Provides a minimal logging helper that ensures only MPI rank 0
 * produces console output. Useful for avoiding duplicate logs in
 * parallel execution contexts.
 *
 * This is a header-only utility and introduces no runtime state.
 *
 * @section usage Example
 * @code
 * log0(rank, "Grid generation complete");
 * @endcode
 */

/**
 * @brief Print a message only from MPI rank 0.
 *
 * @param rank MPI rank identifier
 * @param msg  Message to print
 */
inline void log0(int rank, const std::string& msg) {
    if (rank == 0) {
        std::cout << msg << std::endl;
    }
}