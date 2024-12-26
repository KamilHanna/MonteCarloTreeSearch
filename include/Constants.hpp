#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP


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
 */

// Constants
namespace Constants {

    constexpr Tiny RiskFreeRate = 0.0443; 
    // Tune this parameter based on the risk preference
    constexpr Tiny risk_aversion = 0.4;   
      
}

#endif // CONSTANTS_HPP