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


// Function to print "MCTS" using special symbols
void print_mcts_banner() {
    std::cout << "#########################################" << std::endl;
    std::cout << "#  MM     MM   @@@@@@   TTTTT  $$$$$$   #" << std::endl;
    std::cout << "#  MMM   MMM  @           T    $        #" << std::endl;
    std::cout << "#  MM M M MM  @           T     $$$$    #" << std::endl;
    std::cout << "#  MM  M  MM  @           T         $   #" << std::endl;
    std::cout << "#  MM     MM   @@@@@@     T    $$$$$$   #" << std::endl;
    std::cout << "#########################################\n" << std::endl;
}

// Function to setup the MCTS
void MCTS_setup() {

    void print_mcts_banner();
    int simulations, horizontalScaling,EarlyStopping;
    bool finetuning;
    std::cout << "**Enter the number of simulations: ";
    std::cin >> simulations;

    std::cout << "Note [Horizontal scaling expands each child node from the 11 sectors," <<
    "by duplicating them and applying an action on each with different weight adjustment" <<
    "value.Horizontal Scaling = size of adjutment values vector ] " << std::endl;
    
    std::cout << "**Enter the horizontal scaling factor: ";
    std::cin >> horizontalScaling;

    std::cout << "**Enter the early stopping factor: ";
    std::cin >> EarlyStopping;

    std::cout << "Note [Fine tuning applies the third Action from the action space,"<<
    "which adjusts the portfolio overall based on asset correlations instead of sector based adjustments." << std::endl;
    

    std::cout << "**Enter 1 for finetuning, 0 for no finetuning: ";
    std::cin >> finetuning;

    try{
        vector<Asset> assets = readStockAndCorrelations(NumberOfAssets,
        "../Python/Stocks.csv", 
        "../Python/correlation_matrix.csv");

        vector<Real> weights(Asset::getNumberOfAssets());

        Portfolio<Real> myPortfolio(move(assets), move(weights));
        myPortfolio.initializeWeights();

        Node<Portfolio<Real>> myNode(move(myPortfolio));
    

        myNode.getPortfolio().initializeWeights();
        cout << myNode.getPortfolio().computeSharpeRatio() << endl;
        MCTS myMCTS(move(myNode), simulations, horizontalScaling, finetuning, EarlyStopping);
        
        myMCTS.MCTSInformation();
        
        //Computing Execution time.
        auto start = chrono::high_resolution_clock::now();
        myMCTS.startMCTS();
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
        cout << "Execution time : " << duration.count() << " milliseconds" << endl;


    }catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

}

