#ifndef UTILSS_HPP
#define UTILS_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Portfolio.hpp"


// Function to retrieve the stock data and correlations from the given files
vector<Asset> readStockAndCorrelations(const int& AssetCount,
    const string& stockFilename, const string& correlationsFilename) {
    vector<Asset> assets;
    assets.reserve(AssetCount);

    ifstream stockFile(stockFilename);
    ifstream correlationFile(correlationsFilename);

    // Check if both files opened successfully
    if (!stockFile.is_open() || !correlationFile.is_open()) {
        std::cerr << "Error: Could not open one or both files" << std::endl;
        return {};
    }

    string line;

    // Skip the header line in the stock file
    getline(stockFile, line);
    // Skip the header line in the correlation file
    getline(correlationFile, line);

    // Read both files line by line in parallel
    while (getline(stockFile, line)) {
        stringstream ss(line);
        string name, assetClass, temp;
        Real expectedReturn, risk, currentPrice;

        // Read stock data
        getline(ss, name, ',');
        getline(ss, assetClass, ',');
        getline(ss, temp, ',');
        expectedReturn = stod(temp);
        getline(ss, temp, ',');
        risk = stod(temp);
        getline(ss, temp, ',');
        currentPrice = std::stod(temp);

        // Read corresponding correlation data from correlation file
        string correlationLine;
        if (getline(correlationFile, correlationLine)) {
            stringstream correlationStream(correlationLine);
            vector<Real> correlations;

            while (getline(correlationStream, temp, ',')) {
                correlations.emplace_back(stod(temp));
            }

            // Create an Asset object and add it to the list
            assets.emplace_back(name, currentPrice, expectedReturn, risk, assetClass, move(correlations));
        }
    }

    stockFile.close();
    correlationFile.close();

    return assets;
}

#endif // UTILS_HPP