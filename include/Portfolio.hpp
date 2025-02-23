#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP


#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm> 
#include <numeric> 


#include "Constants.hpp"
#include "Types.hpp"
#include "Asset.hpp"
#include "Constraints.hpp"

using namespace std;

template <typename T>
class Portfolio {

private:
    vector<Asset> assets;       // Assets in the portfolio
    vector<T> weights;          // Weights of the Assets in the portfolio

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
    Real computePortfolioWAP() const;
    Real computeExpectedReturn() const;
    Real computeVarianceRisk() const;
    Real computeVolatilityRisk() const;
    Real computeSharpeRatio() const;
    Real simulatePerformance() const;
    Real computeAnnualizedReturn() const;
    Real computeAnnualizedVolatility() const;

    Real computeAverageReturn(const int &start_idx, const int &end_idx) const;
    Real computeAverageRisk(const int &start_idx, const int &end_idx) const;

    void initializeWeights();
    void normalizeWeights();
    void printWeights() const;
    void printWeightsToFile(const string& filename) const;
    void PortfolioInformation() const;

    //Actions
    //Action 1: Adjust the asset weights gradually based on returns of the assets.
    void Action1(const Real &adjustment_value,const Constraints::Sector& sector);
    //Action 2: Adjust the asset weights gradually based on risk of the assets. 
    void Action2(const Real &adjustment_value,const Constraints::Sector& sector);
    //Action 3: Adjust the portfolio overall based on asset correlations.
    void Action3(const Real &adjustment_value);

};

#endif //PORTFOLIO_HPP