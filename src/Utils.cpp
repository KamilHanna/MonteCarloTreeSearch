#include "Utils.hpp"

// Function to retrieve the stock data and correlations from the given files
Portfolio<Real> readPortfolioData(const int& AssetCount,
    const string& stockFilename, const string& correlationsFilename) {
    
    vector<Asset> assets;
    assets.reserve(AssetCount);
    vector<Real> weights;
    weights.reserve(AssetCount);

    ifstream stockFile(stockFilename);
    ifstream correlationFile(correlationsFilename);

    // Check if both files opened successfully
    if (!stockFile.is_open() || !correlationFile.is_open()) {
        std::cerr << "Error: Could not open one or both files" << std::endl;
        return Portfolio<Real>(vector<Asset>(), vector<Real>());
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
        Real expectedReturn, risk, currentPrice, weight;
    
        // Read stock data
        getline(ss, name, ',');
        getline(ss, assetClass, ',');
        getline(ss, temp, ',');
        expectedReturn = stod(temp);
        getline(ss, temp, ',');
        risk = stod(temp);
        getline(ss, temp, ',');
        currentPrice = std::stod(temp);
        getline(ss, temp, ',');
        weight = stod(temp);

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
            weights.emplace_back(weight);
        }
    }

    stockFile.close();
    correlationFile.close();

    Portfolio<Real> myPortfolio(move(assets), move(weights));

    return myPortfolio;
}

// Function to Merge portfolio weights based on sector order.
vector<Real> getMergedPortfolioWeights(vector<vector<Real>>& weights){
    
    if (weights.empty()) {
        throw std::invalid_argument("Weights vector is empty.");
    }

    vector<Real> mergedWeights;

    auto iportfolio = weights.begin(); 
    int portfolioIndex = 0;
    // Loop through each sector and take the weights of each sector from a different portfolio (in order)
    for (const auto& sector : Constraints::sectors) {
            for (int i = sector.start_index; i <= sector.end_index && iportfolio != weights.end(); ++i) {
                mergedWeights.emplace_back(iportfolio->at(i));

            }
            ++iportfolio;
    }
    return mergedWeights;
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
    std::sort(adjustments.begin(), adjustments.end(), std::greater<Real>());
    return adjustments;
}

// Function to setup and initiate the MCTS
void MCTS_setup() {  
    //Input MCTS parameters
    MCTSParams Params;
    cout <<" " << std::endl;
    cout <<"MCTS Portfolio Optimization" << std::endl;
    cout <<"Suggestion, In Phase I, there is no limit for simulations, the more the better." << std::endl;   
    cout <<" "<< std::endl;
    cout <<"Weight intialization types : " << std::endl;
    cout <<"-Equal initialization based on sector constraints-" << std::endl;
    cout <<"-Marketcap based initialization-" << std::endl;
    cout <<"Enter 0 for equal initialization, otherwise 1 : ";
    cin >> Params.initialization;
    cout <<" " << std::endl;
    cout <<" MCTS Phase I : Exploration {Sector Based + Merging Optimization}" << std::endl;
    cout <<" Action space : 1 & 2 ; return & risk weight gradual adjustments"<< std::endl;
    cout <<"Enter the number of simulations : ";
    cin >> Params.simulations;
    cout <<"Note [Horizontal scaling expands each child node from the 11 sectors,by duplicating them and applying an action on each with different weight adjustmentvalue.Horizontal Scaling = size of adjutment values vector ] "<< std::endl;
    cout <<"Enter the horizontal scaling factor : ";
    cin >> Params.horizontal_scaling;
    cout <<" " << std::endl;
    cout <<"TreeCut Optimization divides simulations by a Treecut value, which then adjustments are by multiplied by a reductionvalue factor, which decreases as the tree goes deeper." << std::endl;
    cout <<"Enter the TreeCut value : ";
    cin >> Params.TreeCut;
    cout <<"Enter the TreeCutReductionValue (0> & <1) : ";
    cin >> Params.TreeCutReductionValue;

    cout <<" " << std::endl;
    cout << "MCTS Phase II : Finetuning {Overall}" << std::endl;
    cout <<" Action space : 3 ; correlations based weight gradual adjustments."<< std::endl;
    cout <<"Enter 1 for Finetuning, 0 otherwise : ";
    cin >> Params.finetuning;
    cout <<"Enter the number of finetuning iterations (Suggestion, no more than 3.) : ";
    cin >> Params.finetuning_iterations;

    cout <<" " << std::endl;
    cout << "Early stopping conditions allow you to set a return and risk threshold for the portfolio." << std::endl;
    cout << "If the portfolio meets these conditions, the MCTS will stop early." << std::endl;
    cout <<"Enter 1 for Early Stopping, 0 otherwise : ";
    cin >> Params.early_stopping;
    cout <<"Enter the Early Stopping Return in % : ";
    cin >> Params.early_stopping_return;
    cout <<"Enter the Early Stopping Risk in % : ";
    cin >> Params.early_stopping_risk;
    cout <<" " << std::endl;

    try{
    Portfolio<Real> myPortfolio = (move(readPortfolioData(Constraints::NumberOfAssets,
        "../Python/Stocks.csv", 
        "../Python/correlation_matrix.csv")));
    
        if(Params.initialization == 0){
            myPortfolio.initializeWeights();
        }else{
            myPortfolio.normalizeWeights();
        }
    
        Node<Portfolio<Real>> myNode(move(myPortfolio));

        MCTS myMCTS(move(myNode), Params.simulations, Params.horizontal_scaling,
        Params.finetuning, Params.finetuning_iterations);
        
        //Set early stopping conditions
        if(!Params.early_stopping){
            myMCTS.setEarlyStopping(false);
        }else{
            myMCTS.setEarlyStopping(true);
            myMCTS.setEarlyStoppingReturn(Params.early_stopping_return);
            myMCTS.setEarlyStoppingRisk(Params.early_stopping_risk);
        }

        //Set TreeCut values
        if(Params.TreeCut > 0 & Params.TreeCutReductionValue > 0 & Params.TreeCutReductionValue < 1 & Params.TreeCut < Params.simulations){
            myMCTS.setTreeCut(Params.TreeCut);
            myMCTS.setTreeCutReductionValue(Params.TreeCutReductionValue);
        }else{
            myMCTS.setTreeCut(0);
            myMCTS.setTreeCutReductionValue(1.0);
        }
    
    // Displaying the problem setup
        // Init logger
        Logger logger(80);
        logger.setHeader("MCTS");

        // Set MCTS parameters details
        vector<pair<string, vector<pair<string, VariantType>>>> DataLine = {
            {"Portfolio parameters", {
                {"Weights Initialization : ", Params.initialization == 0 ? "Equal" : "Marketcap based"},
                {"Number of assets : ", Constraints::NumberOfAssets},
                {"Number of sectors : ", Constants::treeWidth},
            }},
            {"MCTS parameters ", {
                {"Number of simulations : ", Params.simulations},
                {"Horizontal scaling : ", Params.horizontal_scaling},
                {"Children per simulation : ", Params.horizontal_scaling * Constants::treeWidth},
                {"Finetuning : ", (Params.finetuning != 0) ? "Yes" : "No"},
                {"Finetuning iterations : ", Params.finetuning_iterations > 0 && (Params.finetuning != 0) ? to_string(Params.finetuning_iterations) : "None"},
                {"Early stopping (return) : ", Params.early_stopping_return > 0 ? to_string(Params.early_stopping_return) : "None"},
                {"Early stopping (risk) : ", Params.early_stopping_risk > 0 ? to_string(Params.early_stopping_risk) : "None"}
            }},
            {"MCTS Setup Complete ", {}},
            {"Starting MCTS.. ", {}}
        };
        logger.setDataLine(DataLine);

        // Print display box
        logger.printDisplayBox();

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


