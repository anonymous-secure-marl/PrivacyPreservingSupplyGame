# -*- coding: utf-8 -*-
"""
Created on Tue Aug 31 13:47:05 2021


https://machinelearningmastery.com/arima-for-time-series-forecasting-with-python/

@author: t-sarahli
Rolling forecast class
"""
from statsmodels.tsa.arima.model import ARIMA
import armax.armax as armax
import numpy as np

class RollingARMAX():
    def __init__(self, file_name, ar, ma, diff):
        self.ar = ar 
        self.ma = ma 
        self.diff = diff  
        data_volume, data_price = armax.clean_strawberry_data(file_name)
        rss, res = armax.arima_fit(data_volume, data_price, ar, ma, diff)
        self.model = res
        self.model_rss = rss
        self.data = data_volume
        self.exog = data_price

    # def predict(self, exog_value):
    #     return self.model.forecast(exog=exog_value)
    def forecast_demand(self, exog_value):
        demand = self.model.forecast(len(exog_value),  exog=exog_value)
        demand = np.squeeze(demand.to_numpy())
        if demand.ndim > 0:
            demand = demand[-1]
        return demand

    # use this: 
    # self.model.forecast(15, exog=5*np.random.rand(15)) # predict 15 steps past the end
    def add_data(self, new_vol, new_price):
        self.data.append(new_vol)
        self.exog.append(new_price)

    def update_fit(self):
        rss, res = armax.arima_fit(
            self.data, self.exog, self.ar, self.ma, self.diff)
        self.model = res 
        self.model_rss = rss 

class SimpleARMAX():
    def __init__(self):    
        self.slope = -0.2
        self.coef = 20
        return 0

    def forecast_demand(self, exog_value):
        return max(self.slope * exog_value[-1] + self.coef, 0)
