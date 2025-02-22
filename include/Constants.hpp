#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "Types.hpp"
/*
 *
 * @constant : {Risk-Free Rate} :
 * 
 * The return on an investment considered free of risk,
 * often represented by government bonds, serving as a benchmark for comparing
 * risky assets. It reflects the minimum return expected without taking on additional risk.
 *
 * @constant : {Risk Aversion} : 
 * 
 * A measure of an investor's preference for avoiding risk, 
 * indicating how much uncertainty they are willing to accept in exchange 
 * for potential returns. It influences the trade-off between risk and 
 * return in portfolio optimization.
 * 
 * 
 * @constant : {Exploration Constant} :
 * 
 * A parameter used in the Upper Confidence Bound 1 (UCB1) algorithm,
 * which balances the trade-off between exploration and exploitation in
 * Monte Carlo Tree Search (MCTS). It determines the level of exploration
 * in the search tree.
 */

// Constants
namespace Constants {

    //Portfolio Class Constants
    
    constexpr Tiny RiskFreeRate = 0.0443; 
    // Tune this parameter based on the risk preference (lower value for higher risk aversion)
    constexpr Tiny risk_aversion = 1.0;
    constexpr int tradingDays = 252;
    
    //MCTS Class Constants

    // Tune this parameter based on your preference
    // Exploration constant for UCB1 (MCTS)
    constexpr Tiny explorationConstant = 1.41;  
    // Tree width for MCTS
    constexpr int treeWidth = 11;

      
}

#endif // CONSTANTS_HPP