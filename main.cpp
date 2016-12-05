#include <stdio.h>
#include <unistd.h>
#include <mpi.h>

#include <array>
#include <iostream>

#include "map.h"


static const unsigned int num_symbols = 4;
static const unsigned int num_msgs = 3;
static const unsigned int num_rows = 6;

static const bool set_left_cols = true;

static const unsigned int num_cols = 64; // num_symbols ^ num_msgs

int main(int argc, char *argv[]) {
    int rank,size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    Map<num_symbols, num_msgs, num_rows, num_cols, set_left_cols> map;
    map.init();

    static const unsigned int cell_start = num_msgs * num_cols + (set_left_cols ? num_symbols : 0);

    bool res = map.run<cell_start + 8>(rank, size, cell_start);
    if (res) {
        std::cout << "Found solution!" << std::endl;
        map.print();
    } else {
        std::cout << "No solution found" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
