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

// Adds an asset to the portfolio
template <typename T>
void Portfolio<T>::addAsset(const Asset& asset, const T& weight) {
    assets.emplace_back(asset);
    weights.emplace_back(weight);
}    

// Computes and returns value of the portfolio
template <typename T>
Real Portfolio<T>::computePortfolioValue() const {
    Real portfolioValue = 0;
    for (int i = 0; i < assets.size(); i++) {
        portfolioValue += assets[i].getCurrentPrice() * weights[i];
    }
    return portfolioValue;
}

// Computes and returns expected return of the portfolio
template <typename T>
Real Portfolio<T>::computeExpectedReturn() const {
    Real expectedReturn = 0;
    for (int i = 0; i < assets.size(); i++) {
        expectedReturn += assets[i].getExpectedReturn() * weights[i];
    }
    return expectedReturn;
}

// Computes and returns risk (Variance) of the portfolio
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
    return (computeExpectedReturn() - Constants::RiskFreeRate) / computeVolatility(); // 0.02 is the risk free rate
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

