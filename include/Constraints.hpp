#ifndef CONSTRAINTS_HPP
#define CONSTRAINTS_HPP

#include "Types.hpp"

// Portfolio Constraints
namespace Constraints {
    
    constexpr Tiny MinWeight = 0.0;
    constexpr Tiny MaxWeight = 0.1;
    constexpr Tiny max_portfolio_volatility = 0.5;
    constexpr Tiny lambda_penalty = 0.0001; 
    constexpr short int MinAssets = 2;
}

#endif // CONSTRAINTS_HPP