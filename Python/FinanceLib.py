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

# Function to initialize the stocks DataFrame
def initialize_stocks(filename,_period):
    # Reading the csv file
    df = pd.read_csv(filename)

    #Obtaining the price, returns & risk of stocks.
    get_current_price(df)
    get_expected_returns_and_risk(df,_period)
    #Save the final DataFrame.
    df.to_csv('Stocks.csv', index=False)

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
    for index, row in df.iterrows(): 
        symbol = row['Symbol']
        try:
            stock = yf.Ticker(symbol)
            hist_data = stock.history(period=_period)  # period to be set.
            Open = hist_data['Open']    #List of opening prices
            Close = hist_data['Close']  #List of closing prices
            Returns = ((Close - Open) / Open)*100 # Calculate the returns
            X = Returns.mean()
            df.loc[index,'Exp Return'] = X  #exp/Average of the returns
            variance = get_variance(Returns,X) #Variance of the returns
            df.loc[index,'Risk'] = np.sqrt(variance) #Standard Deviation of the returns (risk)
        except:
            print(f"Error getting the return & risk for {symbol}")
            df.loc[index,'Exp Return'] = 0.0
            df.loc[index,'Risk'] = 0.0

# Function to get the variance of the returns
def get_variance(returns,expreturn):
    variance = 0
    for i in range(len(returns)):
        variance = variance + ((returns.iloc[i] - expreturn)**2)
    return (variance/len(returns))

'''
Correlations.csv functions
'''

# Function to get the returns of a stock symbol
def get_correlations(df,df_corr,_period='1y'):
    
    for index1, row1 in df.iterrows():
    
        for index2, row2 in df.iterrows(): 
            symbol1 = row1['Symbol']
            symbol2 = row2['Symbol']
            
            try:
                stock1 = yf.Ticker(symbol1)
                stock2 = yf.Ticker(symbol2)
                hist_data1 = stock1.history(period=_period)  
                hist_data2 = stock2.history(period=_period)  


                Open1 = hist_data1['Open']    
                Close1 = hist_data1['Close']

                Open2 = hist_data2['Open']
                Close2 = hist_data2['Close']

                Returns1 = ((Close1 - Open1) / Open1)*100
                Returns2 = ((Close2 - Open2) / Open2)*100

                X = Returns1.mean()
                Y = Returns2.mean()

                covariance = get_covariance(Returns1,Returns2,X,Y)
                variance1 = get_variance(Returns1,X)
                variance2 = get_variance(Returns2,Y)
                std1 = np.sqrt(variance1)
                std2 = np.sqrt(variance2)

                correlation = covariance/(std1*std2)

                df_corr.loc[index1,index2] = correlation
            except:
                print(f"Error computing the correlations for {symbol1,symbol2}")
                df_corr.loc[index1, index2] = 0.0

def get_covariance(return1,return2,expreturn1,expreturn2):
    covariance = 0
    for i in range(len(return1)):
        covariance = covariance + ((return1.iloc[i] - expreturn1)*(return2.iloc[i] - expreturn2))
    return (covariance/len(return1))

'''
def generate_nodes(num_nodes, grid_size):
    # Randomly generate node coordinates within the grid size
    nodes = np.random.rand(num_nodes, 2) * grid_size
    return nodes

def save_nodes_to_file(nodes, filename):
    # Save node coordinates to a JSON file
    with open(filename, 'w') as file:
        json.dump(nodes.tolist(), file)

def plot_nodes(nodes, filename):
    
    # Get the current working directory
    pwd = os.getcwd()
    
    # Create the full path for saving the image
    full_path = os.path.join(pwd, filename)
    
    # Plot nodes and their connections
    plt.figure(figsize=(8, 8))
    
    # Scatter plot for the nodes
    plt.scatter(nodes[:, 0], nodes[:, 1], c='blue', marker='o')

    # Draw lines connecting each pair of nodes
    num_nodes = len(nodes)
    for i in range(num_nodes):
        for j in range(i + 1, num_nodes):
            plt.plot([nodes[i, 0], nodes[j, 0]], [nodes[i, 1], nodes[j, 1]], 'k--', linewidth=0.5)
    
    plt.title('Node Grid with Connections')
    plt.xlabel('X Coordinate')
    plt.ylabel('Y Coordinate')
    plt.grid(True)
    plt.savefig(filename)
    plt.show()

def plot_nodes_unconnected(nodes, filename):
    
    # Get the current working directory
    pwd = os.getcwd()
    
    # Create the full path for saving the image
    full_path = os.path.join(pwd, filename)
    
    # Plot nodes without any connections
    plt.figure(figsize=(8, 8))
    
    # Scatter plot for the nodes
    plt.scatter(nodes[:, 0], nodes[:, 1], c='blue', marker='o')
    
    plt.title('Node Grid without Connections')
    plt.xlabel('X Coordinate')
    plt.ylabel('Y Coordinate')
    plt.grid(True)
    plt.savefig(filename)
    plt.show()
    '''