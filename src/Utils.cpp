#include "Utils.hpp"

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

//Function to generate N random adjustments for the weights of the assets
vector<Real> generate_adjustment_values(const int& N) {
    vector<Real> adjustments(N);

    // Seed random number generator
    std::srand(static_cast<unsigned int>(std::time(0)));

    //Generating random adjustments in range(min_adjustment, max_adjustment)
    for (int i = 0; i < N; ++i) {
        //rand nbr in range(0,1); scaled to range(min_adjustment, max_adjustment)
        Real random_value = Constraints::min_adjustment + 
        static_cast<Real>(rand()) / RAND_MAX * 
        (Constraints::max_adjustment - Constraints::min_adjustment);
        adjustments[i] = random_value;
    }
    std::sort(adjustments.begin(), adjustments.end());
    return adjustments;
}
