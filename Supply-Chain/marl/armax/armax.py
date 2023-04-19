# -*- coding: utf-8 -*-
"""
Created on Tue Aug 31 13:47:05 2021


https://machinelearningmastery.com/arima-for-time-series-forecasting-with-python/

@author: t-sarahli
Generate strawberry armax demand forecast
"""
from pandas import read_csv, DataFrame
import matplotlib.pyplot as plt
from pandas.plotting import autocorrelation_plot
# from statsmodels.tsa.arima_model import ARIMA
from statsmodels.tsa.arima.model import ARIMA
import numpy as np
import os
import statsmodels.api as sm


os.environ["KMP_DUPLICATE_LIB_OK"]="TRUE"

# data = read_csv('strawberry_data_clean.csv')
# volume_name = 'Volume ( 1e7 Pounds)'
# price_name = 'USA strawberry retail price'
# data[price_name] = data[price_name].apply(clean_currency).astype('float')
# autocorrelation_plot(data)
# print(data.head())
# data.plot()
# plt.show()

def clean_currency(x):
    """ If the value is a string, then remove currency symbol and delimiters
    otherwise, the value is numeric and can be converted
    """
    if isinstance(x, str):
        return(x.replace('$', '').replace(',', ''))
    return(x)
def clean_strawberry_data(file_name): #'strawberry_data_clean.csv'
    data = read_csv(file_name)
    volume_name = 'Volume ( 1e7 Pounds)'
    price_name = 'USA strawberry retail price'
    data[price_name] = data[price_name].apply(clean_currency).astype('float')
    # autocorrelation_plot(data)
    data_volume = DataFrame({volume_name: data[volume_name]})
    data_price = DataFrame({price_name: data[price_name]})
    return data_volume, data_price

def arima_fit(data, exog, ar, ma, diff, file_name=None):
    data_arr = np.squeeze(data.to_numpy())
    model =  ARIMA(data, exog=exog, order=(ar, diff, ma))
    res = model.fit()
    print(res.summary())
    residuals = DataFrame(res.resid)
    print(residuals.describe())
    rss  = sum((res.fittedvalues - data_arr)**2)
    if file_name is not None:
        res.save(f'{ar}-{diff}-{ma}_quantity_model.pickle')
    # to call, use sm.load('2-0-1_quantity_model.pickle')
    rss  = sum((res.fittedvalues - data_arr)**2)
    return rss, res

def iterative_arima_fit(data, exog):
    fit_results = {}
    best_fit = [99999, 0]
    best_params = None
    data_arr = np.squeeze(data.to_numpy())
    for ar in [2]:# range(4):
        for ma in [1]:# range(4):
            for diff in [0]: #range(1):
                rss, res = arima_fit(data, exog, ar, ma, diff, f'{ar}-{diff}-{ma}_quantity_model.pickle')
                if rss > 0:
                    print(f'{rss}')
                    fit_results[f'{ar}-{diff}-{ma}'] = [rss, res]
                    if rss < best_fit[0]:
                        best_fit = [rss, res]
                        best_params = f'{ar}-{diff}-{ma}'
    print(f'best parameters are {best_params}')
    
    return fit_results, best_fit

# data_volume, data_price = clean_strawberry_data('strawberry_data_clean.csv')
# fit_res, best_fit = iterative_arima_fit(data_volume, data_price)

# print('-------- best model is --------------- ')
# print(best_fit[1].summary())

# plt.figure()
# plt.plot([fit[0] for fit in fit_res.values()])
# plt.title('rss value for all fits')
# plt.grid()
# plt.show()

# # summary of fit model
# print(res.summary())
# # line plot of residuals
# residuals = DataFrame(res.resid)
# residuals.plot()
# plt.show()
# # density plot of residuals
# residuals.plot(kind='kde')
# plt.show()
# # summary stats of residuals
# print(residuals.describe())


# # make prediction
# y_hat = res.predict(exog=data[price_name])
# plt.figure()

# plt.plot(data[volume_name])
# plt.plot(res.fittedvalues, '-.')
# # plt.plot(y_hat[1:], '-.')
# plt.show()
# # plt.figure()
# # plt.scatter(data[price_name], data[volume_name])
# # plt.show()

