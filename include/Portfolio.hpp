#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP


#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "Constants.hpp"
#include "Types.hpp"
#include "Asset.hpp"
#include "Constraints.hpp"
using namespace std;

template <typename T>
class Portfolio {

private:
    vector<Asset> assets;       // Assets in the portfolio
    vector<T> weights;          // Weights of rhe Assets in the portfolio

public :
    //Constructor
    Portfolio(vector<Asset>&& assets, vector<T>&& weights);

    //Getters
    const vector<Asset>& getAssets() const;
    const vector<T>& getWeights() const;

    //Setters
    void setAssets(vector<Asset>&& assets);
    void setWeights(vector<T>&& weights);
  
    //Member functions
    void addAsset(const Asset& asset, const T& weight);
    Real computePortfolioValue() const;
    Real computeExpectedReturn() const;
    Real computeRisk()  const;
    Real computeVolatility() const;
    Real computeSharpeRatio()  const;
    void PortfolioInformation() const;

};

#endif //PORTFOLIO_HPP