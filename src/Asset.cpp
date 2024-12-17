#include "Asset.hpp"


//Constructor 
Asset::Asset(const string& name, const Real& currentPrice, 
const Real& expectedReturn, const Real& risk, 
const string& assetClass,vector<Real>&& correlations) 
: name(name), currentPrice(currentPrice), expectedReturn(expectedReturn), 
risk(risk), assetClass(assetClass),correlations(move(correlations))
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

Real Asset::getCurrentPrice() const {
    return currentPrice;
}

Real Asset::getExpectedReturn() const {
    return expectedReturn;
}

Real Asset::getRisk() const {
    return risk;
}

const vector<Real>& Asset::getCorrelations() const{
    return correlations;
}

int Asset::getNumberOfAssets(){
    return numberOfAssets;
}

// Setters

void Asset::setName(const string& name) {
    this->name = name;
}   

void Asset::setAssetClass(const string& assetClass) {
    this->assetClass = assetClass;
}

void Asset::setCurrentPrice(const Real& currentPrice) {
    if (currentPrice < 0) {
        throw std::invalid_argument("Current price cannot be negative.");
    }
    this->currentPrice = currentPrice;
}

void Asset::setExpectedReturn(const Real& expectedReturn) {
    this->expectedReturn = expectedReturn;
}

void Asset::setRisk(const Real& risk) {
    if (risk < 0) {
        throw std::invalid_argument("Risk cannot be negative.");
    }
    this->risk = risk;
}

void Asset::setCorrelations(const vector<Real>&& correlations) {
    for (auto i = correlations.begin(); i!= correlations.end(); ++i) {
        if (*i < -1.0 || *i> 1.0) {
            throw invalid_argument("Correlations must be between -1 and 1.");
        }
    }
    this->correlations = move(correlations);
}

// Member functions

Real Asset::getCorrelation(const int& index) const {
    if (index < 0 || index >= static_cast<int>(correlations.size())) {
        throw std::out_of_range("Index out of range");
    }
    return correlations[index];
}

// Returns an Asset information
void Asset::AssetInformation() const {
    cout << "Asset Name : " << getName() << endl;
    cout << "Asset Class : " << getAssetClass()<< endl;
    cout << "Current Price : " << getCurrentPrice() << endl;
    cout << "Expected Return : " << getExpectedReturn() << endl;
    cout << "Risk : " << getRisk() << endl;
}