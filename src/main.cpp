#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <string>
#include <chrono>

#include "Node.hpp"
#include "Portfolio.hpp"
#include "MCTS.hpp"
#include "Asset.hpp"
#include "Utils.hpp"

using namespace std;
 
constexpr int NumberOfAssets = 503; // Number of assets in the portfolio

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
try{
    vector<Asset> assets = readStockAndCorrelations(NumberOfAssets,
     "/home/viken/Desktop/MCTS_AMSC/MonteCarloTreeSearch/Python/Stocks.csv", 
     "/home/viken/Desktop/MCTS_AMSC/MonteCarloTreeSearch/Python/correlation_matrix.csv");

    vector<Real> weights(Asset::getNumberOfAssets());

    Portfolio<Real> myPortfolio(move(assets), move(weights));
    myPortfolio.initializeWeights();

    Node<Portfolio<Real>> myNode(move(myPortfolio));
   

    myNode.getPortfolio().initializeWeights();
    cout << myNode.getPortfolio().computeSharpeRatio() << endl;
    MCTS myMCTS(move(myNode), 100, 11, 10, true, 10);

    myMCTS.startMCTS();

    


    //Node<Portfolio<Real>> myNode2(move(myMCTS.getRoot()));

    //myNode2.getPortfolio().printWeights();


}catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
}
    return 0;
}

