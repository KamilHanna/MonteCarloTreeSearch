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

// Computes and returns value of the Portfolio Weighted Average Price (WAP)
template <typename T>
Real Portfolio<T>::computePortfolioWAP() const {
    Real portfolioWAP = 0;
    for (auto asseti = assets.begin(), weighti = weights.begin(); 
        asseti != assets.end() && weighti != weights.end(); 
        ++asseti, ++weighti) {
        portfolioWAP += asseti->getCurrentPrice() * (*weighti);
    }
    return portfolioWAP;
}

// Computes and returns expected return of the portfolio
template <typename T>
Real Portfolio<T>::computeExpectedReturn() const {
    Real expectedReturn = 0;
    for(auto asseti = assets.begin(), weighti = weights.begin(); 
        asseti != assets.end() && weighti != weights.end(); 
        ++asseti, ++weighti) {
        expectedReturn += asseti->getExpectedReturn() * (*weighti);
    }
    return expectedReturn;
}

// Computes and returns risk of the portfolio (Variance)
template <typename T> 
Real Portfolio<T>::computeVarianceRisk() const{
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

//The real "RISK" basically.
// Computes and returns volatility of the portfolio (Standard Deviation)
template <typename T>
Real Portfolio<T>::computeVolatilityRisk() const {
    return sqrt(computeVarianceRisk());
}

// Computes and returns Sharpe ratio of the portfolio
template <typename T>
Real Portfolio<T>::computeSharpeRatio() const{
    return (computeExpectedReturn() - Constants::RiskFreeRate) / computeVolatilityRisk(); 
}

template <typename T>
Real Portfolio<T>::computeAnnualizedReturn() const {
    // convert daily return from % to decimal
    Real dailyReturnDecimal = computeExpectedReturn() / 100.0;
    Real annualizedReturn = std::pow(1 + dailyReturnDecimal, Constants::tradingDays) - 1;
    // Convert back to percentage (optional, depending on your use case)
    return annualizedReturn * 100;
}

template <typename T>
Real Portfolio<T>::computeAnnualizedVolatility() const {
    return computeVolatilityRisk() * sqrt(Constants::tradingDays);
}

template <typename T>
Real Portfolio<T>::computeAverageReturn(const int &start_idx, const int &end_idx) const {
    Real averageReturn = 0;
    int count = 0;
    for (int i = start_idx; i <= end_idx && i < assets.size(); ++i) {
        averageReturn += assets[i].getExpectedReturn();
        ++count;
    }
    return count > 0 ? averageReturn / count : 0;
}

template <typename T>
Real Portfolio<T>::computeAverageRisk(const int &start_idx, const int &end_idx) const {
    Real averageRisk = 0;
    int count = 0;
    for (int i = start_idx; i <= end_idx && i < assets.size(); ++i) {
        averageRisk += assets[i].getRisk();
        ++count;
    }
    return count > 0 ? averageRisk / count : 0;
}

template <typename T>
void Portfolio<T>::printWeights() const {
    std::cout << "Weights: ";
       for(auto weighti = weights.begin(); weighti != weights.end(); ++weighti) {
        std::cout << *weighti << " ";
    } 
        std::cout << std::endl;
}

template <typename T>
Real Portfolio<T>::simulatePerformance() const {
    Real expectedReturn = computeExpectedReturn();
    Real risk = computeVolatilityRisk();
    Real sharpeRatio = computeSharpeRatio();

    // A weighted combination of Sharpe ratio and risk-return tradeoff
    Real performanceScore = sharpeRatio + (expectedReturn - Constants::risk_aversion * risk);
    
    return performanceScore;
} 

template <typename T>
void Portfolio<T>::PortfolioInformation() const {
    cout << "$$Portfolio Information$$" << endl;
    cout << "*************************" << endl;
    cout << "Expected Return (Daily): " << computeExpectedReturn() << " %"<< endl;
    cout << "Portfolio Risk/Volatility (Daily) : " << computeVolatilityRisk() << " %"<< endl;
    cout << "Sharpe Ratio (Daily) : " << computeSharpeRatio() << endl;
    cout << "Portfolio Weighted Average Price: " << computePortfolioWAP() << " $" << endl;
    cout << "Portfolio Annual Return : " << computeAnnualizedReturn() << " %" << endl;
    cout << "Portfolio Annual Risk : " << computeAnnualizedVolatility() << " %" << endl;
    cout << "*************************" << endl;
}


// Assigning weights based on sector constraints.
template<typename T>
void Portfolio<T>::initializeWeights(){
    
    // Set all weights to 1
    for(auto weighti = weights.begin(); weighti != weights.end(); ++weighti) {
        *weighti = 1.0;
    }

    //Normalizing weights so they match Sector total weight constraints
    for (const auto& sector : Constraints::sectors) {
        Real sectorWeight = 0;
        for (int i = sector.start_index; i <= sector.end_index; ++i) {
            sectorWeight += weights[i];
        }
        Real normfactor = sector.max_weight/sectorWeight;

        for (int i = sector.start_index; i <= sector.end_index; ++i) {
            weights[i] = weights[i] * normfactor;
        }
    }
}

// Normalize weights so they sum to 1
template<typename T>
void Portfolio<T>::normalizeWeights(){
    Real totalWeight = 0;
    for(auto weighti = weights.begin(); weighti != weights.end(); ++weighti) {
        totalWeight += *weighti;
    }

    for(auto weighti = weights.begin(); weighti != weights.end(); ++weighti) {
        *weighti = *weighti / totalWeight;
    }
}


//Action Space :
//Action 1: Adjust the asset weights gradually based on returns of the assets.

template <typename T>
void Portfolio<T>::Action1(const Real &adjustment_value,const Constraints::Sector& sector){

    //# Compute the average return
    Real average_return = computeAverageReturn(sector.start_index, sector.end_index);

    //# Compute the total adjustment distance from the average return
    Real total_distance = 0;
    for (int i = sector.start_index; i <= sector.end_index && i < assets.size(); ++i) {
        total_distance += std::abs(assets[i].getExpectedReturn() - average_return);
    }

    //# If total distance is 0, no adjustment can be made
    if (total_distance == 0) {
        return;
    }
    else {
        for (int i = sector.start_index; i <= sector.end_index && i < assets.size(); ++i) {
            Real return_distance = assets[i].getExpectedReturn() - average_return;

            //# Scale the adjustment value based on distances
            Real scaled_adjustment = (std::abs(return_distance) / total_distance) * adjustment_value;

            //# Adjust weights while respecting the max and min constraints
            if (return_distance > 0) {
                if (weights[i] + scaled_adjustment <= Constraints::MaxAssetWeight) {
                    weights[i] += scaled_adjustment;
                } else {
                    weights[i] = Constraints::MaxAssetWeight;
                }
            } else {
                if (weights[i] - scaled_adjustment >= Constraints::MinAssetWeight) {
                    weights[i] -= scaled_adjustment;
                } else {
                    weights[i] = Constraints::MinAssetWeight;
                }
            }
        }

    }

    //Normalizing weights so they match Sector total weight constraints
    Real sectorWeight = 0;
    for (int i =  sector.start_index ; i <= sector.end_index && i < assets.size(); ++i) {
        sectorWeight += weights[i];
    }
    Real normfactor = sector.max_weight/sectorWeight;

    for (int i = sector.start_index; i <= sector.end_index; ++i) {
            weights[i] = weights[i] * normfactor;
    }
}

//Action 2: Adjust the asset weights gradually based on returns of the risk of the assets.

template <typename T>
void Portfolio<T>::Action2(const Real &adjustment_value,const Constraints::Sector& sector){

    //# Compute the average return
    Real average_risk = computeAverageRisk(sector.start_index, sector.end_index);

    //# Compute the total adjustment distance from the average risk
    Real total_distance = 0;
    for (int i = sector.start_index; i <= sector.end_index && i < assets.size(); ++i) {
        total_distance += std::abs(assets[i].getRisk() - average_risk);
    }

    //# If total distance is 0, no adjustment can be made
    if (total_distance == 0) {
        return;
    }
    else {
        for (int i = sector.start_index; i <= sector.end_index && i < assets.size(); ++i) {
            Real risk_distance = assets[i].getRisk() - average_risk;

            //# Scale the adjustment value based on distances
            Real scaled_adjustment = (std::abs(risk_distance) / total_distance) * adjustment_value;

            //# Adjust weights while respecting the max and min constraints
            if (risk_distance < 0) {
                if (weights[i] + scaled_adjustment <= Constraints::MaxAssetWeight) {
                    weights[i] += scaled_adjustment;
                } else {
                    weights[i] = Constraints::MaxAssetWeight;
                }
            } else {
                if (weights[i] - scaled_adjustment >= Constraints::MinAssetWeight) {
                    weights[i] -= scaled_adjustment;
                } else {
                    weights[i] = Constraints::MinAssetWeight;
                }
            }
        }

    }

    //Normalizing weights so they match Sector total weight constraints
    Real sectorWeight = 0;
    for (int i =  sector.start_index ; i <= sector.end_index && i < assets.size(); ++i) {
        sectorWeight += weights[i];
    }
    Real normfactor = sector.max_weight/sectorWeight;

    for (int i = sector.start_index; i <= sector.end_index; ++i) {
            weights[i] = weights[i] * normfactor;
    }
}

//Fine tuning
//Action 3: Adjust the portfolio overall based on asset correlations.
template <typename T>
void Portfolio<T>::Action3(const Real &adjustment_value){

    for (int i = 0; i < assets.size(); ++i) {
        for (int j = 0; j < assets.size(); ++j) {
            if (i != j) {
                Real correlation = assets[i].getCorrelation(j);
                Real correlation_distance = correlation - 0.5;

                Real scaled_adjustment = std::abs(correlation_distance) * adjustment_value;

                if (correlation_distance < 0) {
                    if (weights[i] + scaled_adjustment <= Constraints::MaxAssetWeight) {
                        weights[i] += scaled_adjustment;
                    } else {
                        weights[i] = Constraints::MaxAssetWeight;
                    }
                } else {
                    if (weights[i] - scaled_adjustment >= Constraints::MinAssetWeight) {
                        weights[i] -= scaled_adjustment;
                    } else {
                        weights[i] = Constraints::MinAssetWeight;
                    }
                }
            }
        }
    }


        //Normalizing weights so they match Sector total weight constraints
    for (const auto& sector : Constraints::sectors) {
        Real sectorWeight = 0;
        for (int i = sector.start_index; i <= sector.end_index; ++i) {
            sectorWeight += weights[i];
        }
        Real normfactor = sector.max_weight/sectorWeight;

        for (int i = sector.start_index; i <= sector.end_index; ++i) {
            weights[i] = weights[i] * normfactor;
        }
    }
    
}

template class Portfolio<Real>;

