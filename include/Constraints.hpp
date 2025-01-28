#ifndef CONSTRAINTS_HPP
#define CONSTRAINTS_HPP

#include <array>
#include <string_view>
#include "Types.hpp"

// Portfolio Constraints
namespace Constraints {

    // Number of sectors in the S&P 500
    constexpr size_t num_sectors = 11;
    struct Sector {
        std::string_view name;
        Real max_weight;
        int start_index;
        int end_index;
    };

    // Array of sectors with names, max weights, and index boundaries
    constexpr std::array<Sector, num_sectors> sectors = {{
        {"Information Technology", 0.3274, 0, 68},
        {"Financials", 0.1279, 69, 140},
        {"Health Care", 0.1108, 141, 202},
        {"Consumer Discretionary", 0.1012, 203, 252},
        {"Communication Services", 0.0983, 253, 274},
        {"Industrials", 0.0749, 275, 352},
        {"Consumer Staples", 0.0571, 353, 390},
        {"Energy", 0.0334, 391, 412},
        {"Utilities", 0.0268, 413, 443},
        {"Real Estate", 0.0226, 444, 474},
        {"Materials", 0.0197, 475, 502}
    }};
    
    constexpr Tiny MinAssetWeight = 0.000001;   // 0.0001% / 10e-6
    constexpr Tiny MaxAssetWeight = 0.025;    // 2.5% / 2.5e-2

    constexpr Real min_adjustment = 1e-5;
    constexpr Real max_adjustment = 5e-4;    
}

#endif // CONSTRAINTS_HPP