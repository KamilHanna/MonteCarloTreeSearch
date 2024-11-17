#include "Portfolio.hpp"

//Constructor
template <typename T>
Portfolio<T>::Portfolio(vector<Asset> assets, vector<T> weights) {
    setAssets(assets);
    setWeights(weights);
}

//Getters
template <typename T>
vector<Asset> Portfolio<T>::getAssets() const {
    return assets;
}

template <typename T>
vector<T> Portfolio<T>::getWeights() const{
    return weights;
}

//Setters
template <typename T>
void Portfolio<T>::setAssets(vector<Asset> assets) {
    this->assets = assets;
}

template <typename T>
void Portfolio<T>::setWeights(vector<T> weights) {
    this->weights = weights;
}

template <typename T>
void Portfolio<T>::addAsset(Asset asset, T weight) {
    assets.push_back(asset);
    weights.push_back(weight);
}    

//Member functions

// Computes and returns value of the portfolio
template <typename T>
Real Portfolio<T>::computePortfolioValue() const {
    Real portfolioValue = 0;
    for (int i = 0; i < assets.size(); i++) {
        portfolioValue += assets[i].getcurrentPrice() * weights[i];
    }
    return portfolioValue;
}

// Computes and returns expected return of the portfolio
template <typename T>
Real Portfolio<T>::computeExpectedReturn() const {
    Real expectedReturn = 0;
    for (int i = 0; i < assets.size(); i++) {
        expectedReturn += assets[i].getexpectedReturn() * weights[i];
    }
    return expectedReturn;
}

// Computes and returns risk of the portfolio
template <typename T> 
Real Portfolio<T>::computeRisk() const{
    Real risk = 0;
    for (int i = 0; i < assets.size(); i++) {
        risk += pow(assets[i].getRisk(), 2) * pow(weights[i], 2);
    }
    return sqrt(risk);
}

// Computes and returns Sharpe ratio of the portfolio
template <typename T>
Real Portfolio<T>::computeSharpeRatio() const{
    return (computeExpectedReturn() - 0.02) / computeRisk(); // 0.02 is the risk free rate
}

template <typename T>
Real Portfolio<T>::computeVaR() const{

}
