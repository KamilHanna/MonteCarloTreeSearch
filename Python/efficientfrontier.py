import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import minimize

# Hypothetical data for asset classes (expected returns, standard deviations, and correlations)
asset_classes = ['Cash', 'Bonds', 'Large-Cap Stocks', 'Small-Cap Stocks', 'International Stocks', 'Real Estate', 'Commodities']
expected_returns = np.array([0.01, 0.03, 0.07, 0.09, 0.06, 0.05, 0.04])  # Annual returns
stdevs = np.array([0.01, 0.05, 0.15, 0.20, 0.18, 0.10, 0.12])  # Annual standard deviations

# Correlation matrix (hypothetical)
corr_matrix = np.array([
    [1.00, 0.10, 0.00, 0.00, 0.00, 0.00, 0.00],  # Cash
    [0.10, 1.00, 0.30, 0.20, 0.25, 0.15, 0.10],  # Bonds
    [0.00, 0.30, 1.00, 0.70, 0.60, 0.40, 0.20],  # Large-Cap Stocks
    [0.00, 0.20, 0.70, 1.00, 0.50, 0.30, 0.25],  # Small-Cap Stocks
    [0.00, 0.25, 0.60, 0.50, 1.00, 0.35, 0.30],  # International Stocks
    [0.00, 0.15, 0.40, 0.30, 0.35, 1.00, 0.20],  # Real Estate
    [0.00, 0.10, 0.20, 0.25, 0.30, 0.20, 1.00]   # Commodities
])

# Covariance matrix
cov_matrix = np.outer(stdevs, stdevs) * corr_matrix

# Function to calculate portfolio return
def portfolio_return(weights):
    return np.sum(expected_returns * weights)

# Function to calculate portfolio risk
def portfolio_risk(weights):
    return np.sqrt(weights.T @ cov_matrix @ weights)

# Function to minimize (portfolio risk for a given return)
def minimize_risk(weights, target_return):
    return portfolio_risk(weights)

# Constraints and bounds
constraints = ({'type': 'eq', 'fun': lambda weights: np.sum(weights) - 1},  # Weights sum to 1
               {'type': 'eq', 'fun': lambda weights: portfolio_return(weights) - target_return})  # Target return
bounds = tuple((0, 1) for _ in range(len(asset_classes)))  # Weights between 0 and 1

# Generate efficient frontier
target_returns = np.linspace(expected_returns.min(), expected_returns.max(), 100)
efficient_risks = []

for target_return in target_returns:
    result = minimize(minimize_risk, x0=np.ones(len(asset_classes)) / len(asset_classes),  # Initial guess
                     args=(target_return), method='SLSQP', bounds=bounds, constraints=constraints)
    efficient_risks.append(result.fun)

# Plotting the efficient frontier
plt.figure(figsize=(10, 6))
plt.plot(efficient_risks, target_returns, 'b-', label='Efficient Frontier')

# Add your portfolio(s) as points
your_portfolios = [
    {'return': 0.0099426, 'risk': 0.096039, 'label': 'Portfolio 1'},
    {'return': 0.0302683, 'risk': 0.0928715, 'label': 'Portfolio 2'},
    {'return': 0.0769928, 'risk': 0.0878488, 'label': 'Portfolio 3'},
    {'return': 0.082367, 'risk': 0.08817, 'label': 'Portfolio 4'}  # Add more portfolios as needed
]

for portfolio in your_portfolios:
    plt.plot(portfolio['risk'], portfolio['return'], 'ro')  # Plot as red dots
    plt.text(portfolio['risk'], portfolio['return'], portfolio['label'], fontsize=9, ha='right')

# Labels and title
plt.xlabel('Portfolio Risk (Standard Deviation)')
plt.ylabel('Portfolio Return')
plt.title('Efficient Frontier with MCT Generated Portfolio(s)')
plt.legend()
plt.grid(True)

# Save the plot as an image
plt.savefig('efficient_frontier.png', dpi=300, bbox_inches='tight')  # Save as PNG file
plt.show()  # Display the plot