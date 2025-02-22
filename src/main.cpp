#include <iostream>

#include "Node.hpp"
#include "Portfolio.hpp"
#include "MCTS.hpp"
#include "Asset.hpp"
#include "Utils.hpp"
#include "mpi.h"

using namespace std;
 
int main(int argc, char *argv[]) {

    /*

    We should emphasize the importance of the input data in the main function.
    the number of assets is predefined and is 503).
    The stock data and correlation data are read from the given files.
    (automatically, produced from the python code. the user can use the python code to generate data
    fo different time frames).
    
    In the main , we should have inputs regarding :
    n_cores.
    Scalability factors regarding (number of gradients/timesteps for weight adjustments).
    
    (number of simulations, number of iterations, number of threads)
    
    use chrono to compute time
    */

    // Initialize MPI
    MPI_Init(&argc, &argv);

    MCTS_setup();
    
    MPI_Finalize();

    return 0;
}

