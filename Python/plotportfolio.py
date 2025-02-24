import numpy as np
import pandas as pd
import yfinance as yf
import matplotlib.pyplot as plt

# Step 1: Load weights and symbols
# Assuming weights are in a text file called 'weights.txt' and symbols are in a DataFrame
with open('weights.txt', 'r') as f:
    weights = np.array([float(line.strip()) for line in f])

# Example DataFrame with symbols (replace with your actual DataFrame)
dfstocks= pd.read_csv("Stocks.csv")
symbols = dfstocks['Symbol'].tolist()
df_symbols = pd.DataFrame({'Symbol': symbols})

# Step 2: Fetch historical data
# Define the time period for historical data
start_date = '2020-01-01'
end_date = '2023-01-01'

# Fetch S&P 500 data
sp500 = yf.download('^GSPC', start=start_date, end=end_date)['Adj Close']

# Fetch data for your improved index
data = yf.download(list(df_symbols['Symbol']), start=start_date, end=end_date)['Adj Close']

# Step 3: Calculate portfolio value
# Normalize weights to ensure they sum to 1
weights = weights / np.sum(weights)

# Calculate daily returns for your improved index
portfolio_returns = data.pct_change().dropna()
portfolio_value = (portfolio_returns * weights).sum(axis=1)

# Calculate cumulative returns
initial_investment = 10000
portfolio_cumulative = (1 + portfolio_value).cumprod() * initial_investment
sp500_cumulative = (1 + sp500.pct_change().dropna()).cumprod() * initial_investment

# Step 4: Plot the results
plt.figure(figsize=(10, 6))
plt.plot(portfolio_cumulative, label='Improved S&P 500 Index')
plt.plot(sp500_cumulative, label='Original S&P 500 Index')
plt.title('Improved S&P 500 Index vs Original S&P 500')
plt.xlabel('Date')
plt.ylabel('Portfolio Value ($)')
plt.legend()
plt.grid(True)
plt.savefig('originalvsgenerated.png', dpi=300, bbox_inches='tight')  # Save as PNG file

plt.show()