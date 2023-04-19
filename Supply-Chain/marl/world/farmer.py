# -*- coding: utf-8 -*-
"""
Created on Thu Jul 22 13:41:31 2021

@author: t-sarahli
"""
import numpy as np
from collections import namedtuple


space = namedtuple('space', ['low', 'high'])

class Farmer():
    def __init__(self, price_capacity=7, expected_yield = 1e1):
        """ Obeservation space:
            - price forecasts: p_1, p_2, p_3
            - p_i \in [0, P_i]
            
            Price function:
                P_i  = b_i - k_iQ_i
                
            Actions:
            -(mu_1, mu_2, mu_3): amount of seeds to by.
            
            Holding cost: 
                holding cost = a_hold * inventory
            Demand unsatisfaction cost: 
                cost = a_unsat * quantity_deficit
        """
        self.b = [1, 3, 5]
        self.k = [0.1, 0.3, 0.5]

        self.demand_penalty = 3e0
        self.crop_num = 3
        self.expected_yield = expected_yield
        self.price_capacity = price_capacity
        
        # get rid of forecast_demand
        self.observation_space = np.zeros((self.crop_num))
        self.action_space = space(
                low = np.array([0] * self.crop_num), high = np.array(
                    [1] * self.crop_num))
        self.state=np.zeros((self.observation_space.shape[0]))
        
        # print(f'player has state size {self.state.shape}')
        self.reset()
        
    def reset(self, init_forecast = None):
        if init_forecast == None:
            init_forecast = np.random.rand(self.crop_num)
        self.state = init_forecast.copy() #  0 #

        return self.state.copy() 
    
    def linear_price(self, crop_ind, quantity):
        quantity = max(0, quantity)
        price = max(self.b[crop_ind] - self.k[crop_ind] * quantity, 0)
        return price
        
    def update_state(self, prices):
        self.state = 0.9 * np.array(self.state) + 0.1 * np.array(prices)
        
    def harvest(self, actions):
        yield_percentages = np.random.normal(self.expected_yield, 0.00, 3)
        actions = actions *  yield_percentages #/ np.sum(actions) 
        
        return actions.copy()
    
    def rewards(self, quantities, prices):
        profit = sum(np.multiply(quantities, prices))
        
        return profit 
    
    def step(self, actions):
        
        harvest_q = self.harvest(actions)
        prices = [self.linear_price(i, harvest_q[i]) 
                  for i in range(self.crop_num)]
        self.update_state(prices)
        
        return self.state.copy(), self.rewards(harvest_q, prices)
        
        
        
    