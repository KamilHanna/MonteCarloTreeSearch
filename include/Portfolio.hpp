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
    Portfolio(vector<Asset>& assets, vector<T>& weights);

    //Getters
    vector<Asset> getAssets() const;
    vector<T> getWeights() const;

    //Setters
    void setAssets(vector<Asset>& assets);
    void setWeights(vector<T>& weights);
    void addAsset(Asset& asset, T& weight);
  
    //Member functions
    Real computePortfolioValue() const;
    Real computeExpectedReturn() const;
    Real computeRisk()  const;
    Real computeVolatility() const;
    Real computeSharpeRatio()  const;

    /*
    Functionalities to print info about a portfolio (even more stuff like VaR...)
    Also print all assets with weights & prices?
    void PortfolioInformation() const {
        cout << "Portfolio Information" << endl;
        cout << "---------------------" << endl;
        cout << "Expected Return: " << computeExpectedReturn() << endl;
        cout << "Risk: " << computeRisk() << endl;
        cout << "Sharpe Ratio: " << computeSharpeRatio() << endl;
        cout << "Portfolio Value: " << computePortfolioValue() << endl;
    }
    
    */
};

#endif //PORTFOLIO_HPP

