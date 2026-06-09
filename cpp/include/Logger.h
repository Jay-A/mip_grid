#pragma once
#include <iostream>

inline void log0(int rank, const std::string& msg) {
    if (rank == 0) {
        std::cout << msg << std::endl;
    }
}
