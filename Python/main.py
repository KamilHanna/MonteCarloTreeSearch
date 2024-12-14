import os
import sys
import pandas as pd
from FinanceLib import *



# Reading the csv file
df = pd.read_csv('Input.csv')
get_expected_returns_and_risk(df,'1y')
df.to_csv('Stocks.csv', index=False)

#threshold = 1e-7
#df.loc[df['Exp Return'] < threshold, 'Exp Return'] = 0  


'''
df = pd.read_csv('Stocks.csv')
get_current_price(df)
df.to_csv('Stocks.csv', index=False)
'''