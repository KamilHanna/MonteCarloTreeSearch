"""
File: main.py
Author: Kamil Hanna
Date: Dec 27 2024
"""

import os
import sys
import pandas as pd
from FinanceLib import *
import warnings

# Suppress warnings
from pandas.errors import PerformanceWarning
warnings.simplefilter(action='ignore', category=PerformanceWarning)

'''
Note : This code might take a while to run as it fetches the data from Yahoo Finance API.
Also if you receive an error, please try running the code again as it might be due to the API call limit.

Execution of this file will result in generating 3 files :
1. Stocks.csv : Contains the stocks information.
2. returns.csv : Contains the returns of the stocks.
3. correlation_matrix.csv : Contains the correlation matrix of the stocks.

'''

#Options for _period : 1d, 5d, 1mo, 3mo, 6mo, 1y, 2y, 5y 10y, ytd, max

# Initialize the stocks and correlations file.
df, df_corr, sector_indices = initialize_stocks('RawInput.csv','1y')

