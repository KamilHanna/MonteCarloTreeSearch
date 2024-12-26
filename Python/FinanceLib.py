"""
File: FinanceLib.py
Author: Kamil Hanna
Date: Dec 27 2024
Description: This script contains various finance-related functions, including 
             calculating stock returns, risk, and correlation matrices. The 
             functions are designed to operate on stock data stored in pandas 
             DataFrames, with optimizations for large datasets.

Functions:
    - get_expected_returns_and_risk: Retrieves historical stock data and calculates expected returns and risk (standard deviation).
    - get_correlations: Computes the correlation matrix for a given set of stock returns.
    - get_sector_indices: Returns the start and end indices of each sector in the DataFrame.
    - get_current_price: Retrieves the current price of a stock symbol.
    - get_variance: Calculates the variance of a set of returns.
    - get_covariance: Calculates the covariance between two sets of returns.
    - get_variance2: Calculates the variance of a set of returns (alternative implementation).

Warning:
    This script includes optimizations for large datasets but may trigger 
    performance warnings when manipulating fragmented DataFrames. These warnings 
    are suppressed to avoid cluttering the output.

Version: 1.0
"""


import os
import sys
import json
import time
import numpy as np
import pandas as pd
import yfinance as yf
import matplotlib.pyplot as plt

'''
Stocks.csv functions
'''

# Function to initialize the stocks and correlations file.
def initialize_stocks(filename,_period):
    # Reading the csv file
    df = pd.read_csv(filename)
    
    df_returns = pd.DataFrame()
    #Obtaining the price, returns & risk of stocks.
    df_returns = get_expected_returns_and_risk(df,_period)
    
    get_current_price(df)

    #Save the final DataFrame.
    df.to_csv('Stocks.csv', index=False)
    stocks_info(df)

    sector_indices = get_sector_indices(df)
    df_returns.to_csv('returns.csv', index=False)

    df_corr =pd.DataFrame()
    df_corr = get_correlations(df_returns)
    df_corr.to_csv('correlation_matrix.csv', index=False)

    return df, df_corr, sector_indices

# Function to print the stocks information
def stocks_info(df):
    # Print the first few rows of the DataFrame
    print(df.head())

    # Get the number of unique sectors
    sector_counts = df['GICS Sector'].value_counts()

    # Print the counts for each sector
    print(sector_counts)

# Function to get the sector indices
def get_sector_indices(df):
    
    sector_indices = {}
    # Get unique sectors in the sorted DataFrame
    unique_sectors = df['GICS Sector'].unique()

    for sector in unique_sectors:
        sector_data = df[df['GICS Sector'] == sector]
        start_idx = sector_data.index.min()  # First index of the sector
        end_idx = sector_data.index.max()    # Last index of the sector
        sector_indices[sector] = (start_idx, end_idx)

    for sector, (start_idx, end_idx) in sector_indices.items():
        print(f"{sector}: Start Index = {start_idx}, End Index = {end_idx}")

    return sector_indices


'''
Stocks.csv functions
'''

# Function to get the current price of a stock symbol
def get_current_price(df):
    for index, row in df.iterrows(): 
        symbol = row['Symbol']
        try:
            stock = yf.Ticker(symbol)
            last_price = stock.fast_info['last_price']
            df.loc[index,'Current Price'] = last_price
        except:
            print(f"Error getting the current price for {symbol}")
            df.loc[index,'Current Price'] = None

# Function to get the returns of a stock symbol
def get_expected_returns_and_risk(df,_period='1y'):
    df_returns = pd.DataFrame()
    for index, row in df.iterrows(): 
        symbol = row['Symbol']
        try:
            stock = yf.Ticker(symbol)
            hist_data = stock.history(period=_period)  # period to be set.
            Open = hist_data['Open']    #List of opening prices
            Close = hist_data['Close']  #List of closing prices

            Returns = ((Close - Open) / Open)*100 # Calculate the returns
            
            df_returns[symbol] = Returns.reset_index(drop=True) 

            X = Returns.mean()
            df.loc[index,'Exp Return'] = X  #exp/Average of the returns
            variance = get_variance(Returns,X) #Variance of the returns
            df.loc[index,'Risk'] = np.sqrt(variance) #Standard Deviation of the returns (risk)
        except:
            print(f"Error getting the return & risk for {symbol}")
            df.loc[index,'Exp Return'] = 0.0
            df.loc[index,'Risk'] = 0.0
            df_returns[symbol] = pd.Series([0.0] * len(hist_data), index=hist_data.index).reset_index(drop=True)  # Fill with zeros
    return df_returns

# Function to get the variance of the returns
def get_variance(returns,expreturn):
    variance = 0
    for i in range(len(returns)):
        variance = variance + ((returns.iloc[i] - expreturn)**2)
    return (variance/len(returns))

'''
Correlation_matrix.csv functions
'''
           
# Function to compute correlations between stock returns
def get_correlations(df_returns):
    num_stocks = df_returns.columns.size
    print(num_stocks)
    print(f"Computing correlations for {num_stocks} stocks")

    # Initialize a NumPy array for correlations
    corr_matrix = np.zeros((num_stocks, num_stocks))

    for index1, symbol1 in enumerate(df_returns.columns):
        Returns1 = df_returns[symbol1].iloc[0:].values  # Exclude the first row which is the symbol name

        for index2 in range(index1, num_stocks):  # Only compare with stocks after symbol1
            symbol2 = df_returns.columns[index2]
            Returns2 = df_returns[symbol2].iloc[0:].values  # Exclude the first row which is the symbol name

            try:
                # Check if both return series are non-empty
                if len(Returns1) > 0 and len(Returns2) > 0:
                    X = np.mean(Returns1)
                    Y = np.mean(Returns2)
   
                    # Calculate covariance and variances
                    covariance = get_covariance(Returns1, Returns2, X, Y)

                    variance1 = get_variance2(Returns1, X)
                    variance2 = get_variance2(Returns2, Y)

                    # Calculate standard deviations
                    std1 = np.sqrt(variance1)
                    std2 = np.sqrt(variance2)

                    # Compute correlation
                    if std1 != 0 and std2 != 0:
                        correlation = covariance / (std1 * std2)
                    else:
                        correlation = 0.0

                    corr_matrix[index1, index2] = correlation
                    corr_matrix[index2, index1] = correlation
                else:
                    corr_matrix[index1, index2] = 0.0
                    corr_matrix[index2, index1] = 0.0
            
            except Exception as e:
                print(f"Error computing the correlations for {symbol1} and {symbol2}: {e}")
                corr_matrix[index1, index2] = 0.0

    # Convert the NumPy matrix back to a DataFrame at the end
    df_corr = pd.DataFrame(corr_matrix, index=df_returns.columns, columns=df_returns.columns)

    return df_corr

def get_covariance(return1,return2,expreturn1,expreturn2):
    covariance = 0
    for i in range(len(return1)):
        covariance = covariance + ((return1[i] - expreturn1)*(return2[i] - expreturn2))
    return (covariance/len(return1))

# Function to get the variance of the returns
def get_variance2(returns,expreturn):
    variance = 0
    for i in range(len(returns)):
        variance = variance + ((returns[i] - expreturn)**2)
    return (variance/len(returns))

