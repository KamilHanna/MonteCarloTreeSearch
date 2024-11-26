#ifndef ASSET_HPP
#define ASSET_HPP

#include <iostream>
#include <cmath>
#include <vector>
#include <Types.hpp>

using namespace std;

class Asset {

private:
    string name;                                 // Name of the asset
    string assetClass;                           // Asset class of the asset
    Real currentPrice;                           // Current price of the asset
    Real expectedReturn;                         // Expected return of the asset
    Real risk;                                   // Standard deviation of returns [Risk]
    vector<Real> correlations;                   // Correlations with other assets
    inline static int numberOfAssets = 0;        // Number of nodes created

public:
    // Constructor
    Asset(const string& name, Real& currentPrice, Real& expectedReturn, Real& risk, const string& assetClass);
    //Destructor
    ~Asset();

    // Getters
    string getName() const ;
    string getAssetClass() const;
    Real getcurrentPrice() const;
    Real getexpectedReturn() const;
    Real getRisk() const;
    Real getCorrelation(size_t& index) const;
    static int getNumberOfAssets();

   // Setters
    void setName(const string& name);
    void setAssetClass(const string& category);
    void setcurrentPrice(Real& currentPrice);
    void setExpectedReturn(Real& expectedReturn);
    void setRisk(Real& risk);
    void setCorrelations(const vector<Real>& correlations);



    // Member functions
    void AssetInformation() const;
    /*
    void setCovariance(const std::string& otherAsset, double cov) { covarianceWithOtherAssets[otherAsset] = cov; }
    void setHistoricalReturns(const std::vector<double>& returns) { historicalReturns = returns; }
    void setDividendYield(double yield) { dividendYield = yield; }
    void setTransactionCost(double cost) { transactionCost = cost; }

        
    double getDividendYield() const { return dividendYield; }
    double getTransactionCost() const { return transactionCost; }
*/
};

#endif // ASSET_HPP