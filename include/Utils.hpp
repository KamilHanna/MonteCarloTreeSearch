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

#include "Portfolio.hpp"

// Function to retrieve the stock data and correlations from the given files
vector<Asset> readStockAndCorrelations(const int& AssetCount,
    const string& stockFilename, const string& correlationsFilename);
 
//Function to generate N random adjustments for the weights of the assets
vector<Real> generate_adjustment_values(const int& N);

#endif // UTILS_HPP