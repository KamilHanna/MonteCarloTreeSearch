import os
import sys
import pandas as pd
from FinanceLib import *


#Options for _period : 1d, 5d, 1mo, 3mo, 6mo, 1y, 2y, 5y 10y, ytd, max

'''
# Reading the csv file
df = pd.read_csv('Input.csv')
get_expected_returns_and_risk(df,'1y')
df.to_csv('Stocks.csv', index=False)
'''

df = pd.read_csv('Stocks.csv')
get_current_price(df)
df.to_csv('Stocks.csv', index=False)
