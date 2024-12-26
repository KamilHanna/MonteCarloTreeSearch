#include "Portfolio.hpp"

//Constructor
template <typename T>
Portfolio<T>::Portfolio( vector<Asset>&& assets, vector<T>&& weights) : 
    assets(move(assets)),weights(move(weights)){}

//Getters
template <typename T>
const vector<Asset>& Portfolio<T>::getAssets() const {
    return assets;
}

template <typename T>
const vector<T>& Portfolio<T>::getWeights() const{
    return weights;
}

//Setters
template <typename T>
void Portfolio<T>::setAssets(vector<Asset>&& assets) {
    this->assets = move(assets);
}

template <typename T>
void Portfolio<T>::setWeights(vector<T>&& weights) {
    this->weights = move(weights);
}


//Member functions

// Computes and returns value of the portfolio
template <typename T>
Real Portfolio<T>::computePortfolioValue() const {
    Real portfolioValue = 0;
    for (auto asseti = assets.begin(), weighti = weights.begin(); 
        asseti != assets.end() && weighti != weights.end(); 
        ++asseti, ++weighti) {
        portfolioValue += asseti->getCurrentPrice() * (*weighti);
    }
    return portfolioValue;
}

// Computes and returns expected return of the portfolio
template <typename T>
Real Portfolio<T>::computeExpectedReturn() const {
    Real expectedReturn = 0;
    for(auto asseti = assets.begin(), weighti = weights.begin(); 
        asseti != assets.end() && weighti != weights.end(); 
        ++asseti, ++weighti) {
        expectedReturn += asseti->getExpectedReturn() * (*weighti);
    }
    return expectedReturn;
}

// Computes and returns risk of the portfolio
template <typename T> 
Real Portfolio<T>::computeRisk() const{
    Real risk = 0;
    for (int i = 0; i < assets.size(); i++) {
        for (int j = 0; j < assets.size(); j++) {
            if(i == j) {
            risk += weights[i] * weights[j] * assets[i].getRisk() * assets[j].getRisk();
            } else {
            risk += weights[i] * weights[j] * assets[i].getRisk() * assets[j].getRisk() * assets[i].getCorrelation(j);
            }
        }
    }
    return risk;
}

// Computes and returns volatility of the portfolio
template <typename T>
Real Portfolio<T>::computeVolatility() const {
    return sqrt(computeRisk());
}

// Computes and returns Sharpe ratio of the portfolio
template <typename T>
Real Portfolio<T>::computeSharpeRatio() const{
    return (computeExpectedReturn() - Constants::RiskFreeRate) / computeVolatility(); 
}

// Randomly assign weights, ensuring they sum to 1 and respect constraints
template <typename T>
void Portfolio<T>::initializeWeights() {
    T totalWeight = 0;
    
    for(auto weighti = weights.begin(); weighti != weights.end(); ++weighti) {
        T randomWeight = static_cast<T>(rand()) / RAND_MAX;  // Random between 0 and 1
        *weighti = randomWeight;
        totalWeight += randomWeight;
    }

    // Normalizing weights so they sum to 1
    for (auto it = weights.begin(); it != weights.end(); ++it) {
        *it /= totalWeight;
    }
}

template <typename T>
void Portfolio<T>::rebalanceSectorWeights(const vector<SectorConstraint>& sectorConstraints) {
       constexpr std::array<Real, num_sectors> max_weights = {

        Constraints::max_weights[]
  
    for (const auto& constraint : sectorConstraints) {
        T sectorWeight = computeSectorWeight(constraint.sectorName); // Calculate sector weight
        for (size_t i = 0; i < assets.size(); ++i) {
            if (assets[i].sector == constraint.sectorName) {
                if (sectorWeight > constraint.maxWeight) {
                    // Reduce sector weight
                    weights[i] = max(weights[i] - 0.01, MinWeight);
                } else if (sectorWeight < constraint.minWeight) {
                    // Increase sector weight
                    weights[i] = min(weights[i] + 0.01, MaxWeight);
                }
            }
        }
    }
    normalizeWeights(); // Ensure weights sum to 1
}

template <typename T>
void Portfolio<T>::printWeights() const {
    std::cout << "Weights: ";
       for(auto weighti = weights.begin(); weighti != weights.end(); ++weighti) {
        std::cout << *weighti << " ";
    } 
        std::cout << std::endl;
}

template <typename T>
Real Portfolio<T>::simulatePerformance() const {
    Real expectedReturn = computeExpectedReturn();
    Real risk = computeRisk();
    Real sharpeRatio = computeSharpeRatio();

    // A weighted combination of Sharpe ratio and risk-return tradeoff
    Real performanceScore = sharpeRatio + (expectedReturn - Constants::risk_aversion * risk);
    
    return performanceScore;
} 

template <typename T>
void Portfolio<T>::PortfolioInformation() const {
    cout << "$$Portfolio Information$$" << endl;
    cout << "*************************" << endl;
    cout << "Expected Return: " << computeExpectedReturn() << endl;
    cout << "Risk: " << computeRisk() << endl;
    cout << "Sharpe Ratio: " << computeSharpeRatio() << endl;
    cout << "Portfolio Value: " << computePortfolioValue() << " $" << endl;
    cout << "Portfolio Volatility: " << computeVolatility() << endl;
    cout << "*************************" << endl;
    
    for (int i = 0; i < assets.size(); i++) {
        assets[i].AssetInformation();
        cout << "Asset Weight: " << weights[i] << endl;
        cout << "*************************" << endl;
    }
}

template class Portfolio<Real>;

