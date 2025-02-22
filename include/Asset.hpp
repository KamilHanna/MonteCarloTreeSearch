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
    Asset(const string& name, 
        const Real& currentPrice, 
        const Real& expectedReturn, 
        const Real& risk, 
        const string& assetClass,
        vector<Real>&& correlations);

    //Destructor
    ~Asset();

    // Getters
    string getName() const ;
    string getAssetClass() const;
    Real getCurrentPrice() const;
    Real getExpectedReturn() const;
    Real getRisk() const;
    const vector<Real>& getCorrelations() const;
    static int getNumberOfAssets();

   // Setters
    void setName(const string& name);
    void setAssetClass(const string& category);
    void setCurrentPrice(const Real& currentPrice);
    void setExpectedReturn(const Real& expectedReturn);
    void setRisk(const Real& risk);
    void setCorrelations(const vector<Real>&& correlations);

    // Member functions
    Real getCorrelation(const int& index) const;
    void AssetInformation() const;
    
};

#endif // ASSET_HPP