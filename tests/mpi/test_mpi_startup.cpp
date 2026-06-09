#include "Logger.h"
#include <mpi.h>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {

    bool verbose = false;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--verbose") {
            verbose = true;
        }
    }

    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Optional per-rank verbose output
    if (verbose) {
        std::cout << "Verifying processor ID: " << world_rank << std::endl;
        std::cout.flush();
    }

    // Basic sanity check
    if (world_rank < 0 || world_rank >= world_size) {
        std::cerr << "Invalid rank detected: " << world_rank << std::endl;
        std::cout.flush();
        MPI_Finalize();
        return 1;
    }

    // Barrier to synchronize all ranks
    MPI_Barrier(MPI_COMM_WORLD);

    // Gather all ranks at root (rank 0)
    int local_rank = world_rank;
    int* ranks = nullptr;
    if (world_rank == 0) {
        ranks = new int[world_size];
    }

    MPI_Gather(&local_rank, 1, MPI_INT,
               ranks, 1, MPI_INT,
               0, MPI_COMM_WORLD);

    // Root logs the final verification message
    if (world_rank == 0) {
        std::string rank_list = "[";
        for (int i = 0; i < world_size; ++i) {
            rank_list += std::to_string(ranks[i]);
            if (i < world_size - 1) rank_list += ", ";
        }
        rank_list += "]";

        log0(world_rank,
             "MPI startup validation passed. "
             "World size: " + std::to_string(world_size) +
             " Ranks registered: " + rank_list);

        delete[] ranks;
    }

    MPI_Finalize();
    return 0;
}
