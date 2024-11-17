#include "Asset.hpp"


//Constructor 
Asset::Asset(const string& name, Real currentPrice, Real expectedReturn, Real risk, const string& assetClass) {
    setName(name);
    setcurrentPrice(currentPrice);
    setExpectedReturn(expectedReturn);
    setRisk(risk);
    setAssetClass(assetClass);
}

// Destructor
Asset::~Asset() {
    --numberOfAssets; // Decrement the static member in the destructor
}

// Getters
string Asset::getName() const {
    return name;
}

string Asset::getAssetClass() const {
    return assetClass;
}

Real Asset::getcurrentPrice() const {
    return currentPrice;
}

Real Asset::getexpectedReturn() const {
    return expectedReturn;
}

Real Asset::getRisk() const {
    return risk;
}

Real Asset::getCorrelation(size_t index) const {
    if (index >= correlations.size()) {
        throw out_of_range("Index out of range");
    }
    return correlations[index];
}

int Asset::getNumberOfAssets() {
    return numberOfAssets;
}

// Setters

void Asset::setName(const string& name) {
    this->name = name;
}   

void Asset::setAssetClass(const string& assetClass) {
    this->assetClass = assetClass;
}

void Asset::setcurrentPrice(Real currentPrice) {
    this->currentPrice = currentPrice;
}

void Asset::setExpectedReturn(Real expectedReturn) {
    this->expectedReturn = expectedReturn;
}

void Asset::setRisk(Real risk) {
    this->risk = risk;
}

void Asset::setCorrelations(const vector<Real>& correlations) {
    this->correlations = correlations;
}

// Member functions

// Returns an Asset information
void Asset::AssetInformation() const {
    cout << "Asset Name : " << getName() << endl;
    cout << "Asset Class : " << getAssetClass()<< endl;
    cout << "Current Price : " << getcurrentPrice() << endl;
    cout << "Expected Return : " << getexpectedReturn() << endl;
    cout << "Risk : " << getRisk() << endl;
}
