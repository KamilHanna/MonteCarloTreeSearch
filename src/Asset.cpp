#include "Asset.hpp"


//Constructor 
Asset::Asset(const string& name, Real& currentPrice, Real& expectedReturn, Real& risk, const string& assetClass) 
: name(name), currentPrice(currentPrice), expectedReturn(expectedReturn), risk(risk), assetClass(assetClass)
{
    ++numberOfAssets; // Increment the static member in the constructor
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

Real Asset::getCorrelation(int& index) const {
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

void Asset::setcurrentPrice(Real& currentPrice) {
    if (currentPrice < 0) {
        throw std::invalid_argument("Current price cannot be negative.");
    }
    this->currentPrice = currentPrice;
}

void Asset::setExpectedReturn(Real& expectedReturn) {
    this->expectedReturn = expectedReturn;
}

void Asset::setRisk(Real& risk) {
    if (risk < 0) {
        throw std::invalid_argument("Risk cannot be negative.");
    }
    this->risk = risk;
}

void Asset::setCorrelations(const vector<Real>& correlations) {
    for (const Real& correlation : correlations) {
        if (correlation < -1.0 || correlation > 1.0) {
            throw std::invalid_argument("Correlations must be between -1 and 1.");
        }
    }
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