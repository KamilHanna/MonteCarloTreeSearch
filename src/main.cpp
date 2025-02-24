#include <iostream>

#include "MCTS.hpp"
#include "Utils.hpp"
#include "omp.h"

using namespace std;
 
int main(int argc, char *argv[]) {

    int num_threads = omp_get_max_threads();
    std::cout << "Number of threads: " << num_threads << std::endl;
    
    MCTS_setup();

    return 0;
}

