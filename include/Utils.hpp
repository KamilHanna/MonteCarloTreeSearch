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

constexpr int NumberOfAssets = 503; // Number of assets in the portfolio

// Function to retrieve the stock data and correlations from the given files
vector<Asset> readStockAndCorrelations(const int& AssetCount,
    const string& stockFilename, const string& correlationsFilename);
 
//Function to generate N random adjustments for the weights of the assets
vector<Real> generate_adjustment_values(const int& N);

// Function to print "MCTS" using special symbols
void print_mcts_banner();

// Function to setup the MCTS
void MCTS_setup();

#endif // UTILS_HPP