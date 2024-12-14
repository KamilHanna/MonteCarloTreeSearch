import os
import sys
import pandas as pd
from FinanceLib import *


df = pd.read_csv('Input.csv')
#symbols = df['Symbol'].tolist()  
#print(symbols)

df_corr =pd.DataFrame()
get_correlations(df,df_corr,_period='1y')
df_corr.to_csv('correlation_matrix.csv', index=False)
#correlation_matrix = get_correlation_matrix(symbols, _period='1y')

# Saving the correlation matrix to a CSV file 
#correlation_matrix.to_csv('correlation_matrix.csv')

