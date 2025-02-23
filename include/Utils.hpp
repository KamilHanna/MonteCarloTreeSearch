#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime> 
#include <stdexcept>
#include <chrono>

#include "MCTS.hpp"
#include "Node.hpp"
#include "Asset.hpp"
#include "Portfolio.hpp"
#include "Logger.hpp"
#include "Constraints.hpp"

struct MCTSParams {
    bool initialization;
    int simulations;
    int horizontal_scaling;
    bool finetuning;
    int finetuning_iterations;
    bool early_stopping;
    Real early_stopping_return;
    Real early_stopping_risk;
    int TreeCut;
    Real TreeCutReductionValue;
};

// Function to retrieve the stock data and correlations from the given files
Portfolio<Real> readPortfolioData(const int& AssetCount,
    const string& stockFilename, const string& correlationsFilename);
 
//Function to generate N random adjustments for the weights of the assets
vector<Real> generate_adjustment_values(const int& N);

// Function to setup the MCTS
void MCTS_setup();

#endif // UTILS_HPP