# -*- coding: utf-8 -*-

import numpy as np
from collections import namedtuple
import statsmodels.api as sm
import armax.rolling_forecast as armax

space = namedtuple('space', ['low', 'high'])

class SupplyChain():
    def __init__(self, price_capacity=7, storage_capacity=1, num_inputs=1,
                 price_penalty=1, quantity_penalty = 1, preferred_price = 0,
                 preferred_quantity=0, use_armax=False):
        """ Obeservation space:
            - [0]: current inventory
            - [1]: current downstream price estimate
            - [2]: current upstream demand estimate
            
            Exogenous signals:
            - cost per item - P_in
            - forecast_demand - D_1
            - forecast_demand - D_2
            Wrap this into the transition itself - generates stochasticity.
        
            
            Demand function:
                Q_D = forecast_demand_1 - a_demand * P_out + epsilon
                a_demand = coefficient
                epsilon = Gaussian variable, mean = d, variance = sigma
                
            Actions:
            -0: Price = nonnegative price of outgoing products.
            -i: Quantity  = nonnegative quantity of products bought from f[i]
            
            Holding cost: 
                holding cost = a_hold * inventory
            Demand unsatisfaction cost: 
                cost = a_unsat * quantity_deficit
        """
        self.demand_sigma = 1e-3
        # self.a_demand = -3.5701#2 #1e0
        # self.c_demand = 14.17 # 1
        self.c_demand = 3.3878
        self.a_demand = -0.1326
        self.sigma_demand = 0.1
        # ARMAX demand parameters
        # self.a_demand = -0.2964
        # self.c_demand = 3.5883
        self.a_price = 0 #1e0
        self.c_price = 0.9038 
        self.constant_price = 1e-1
        self.a_hold = 1e1
        self.a_unsatisfied = 5e-2
        self.reward_coeff = 1e1
        
        self.input_prices = num_inputs
        self.storage_capacity = storage_capacity
        self.price_capacity = price_capacity
        self.price_penalty  = price_penalty
        self.preferred_price = preferred_price
        self.quantity_penalty  = quantity_penalty
        self.preferred_quantity = preferred_quantity
        # get rid of forecast_demand
        self.observation_space = np.zeros((num_inputs+1)) 
        # last one is demand forecast
        
        # price is at most 5, quantity is at most 2.
        space_dim = max(1, num_inputs)
        self.action_space = space(
            low = np.array([0]*(space_dim+1)), high = np.array(
                [price_capacity] + [storage_capacity]* space_dim))    
        self.state=np.zeros((self.observation_space.shape[0]))
        self.use_armax = use_armax
        if self.use_armax:
            # (1, 1, 1), (0, 1, 1) both worked surprisingly well
            self.armax_model = armax.RollingARMAX('armax/strawberry_data_clean.csv', 1, 1, 1)
            self.prev_prices = []
        self.reset()
    
    def linear_demand(self, price):
        price = max(price, 0)
        # forecast = min(np.random.normal(self.state[1], 0), self.c_demand)
        demand = self.c_demand  + self.a_demand * price 
        demand += np.random.normal(0, self.sigma_demand) # add noise
        # print(f'demand requested at price {price} = {demand}')
        return max(demand, 0)
    
    def linear_supply(self, quantity):
        quantity = max(quantity, 0)
        # forecast = min(np.random.normal(self.state[0], 0), self.c_price)
        price = self.a_price * quantity + self.c_price # max(, 0) #
        return price
    
    def holding_cost(self, quantity):
        return self.a_hold * quantity**2
    
    def demand_unsatisfaction(self, quantity_sold, cur_demand):
        return self.a_unsatisfied * max(cur_demand - quantity_sold, 0)
    
    def reset(self):
        # print(f'storage capacity is {self.storage_capacity}')
        for i in range(self.input_prices):
            # random initial price estimate
            self.state[i] = np.random.uniform(0, self.price_capacity)

        self.state[-1] = np.random.uniform(0, self.c_demand)
        self.prev_prices = []
        return self.state.copy()
    
    def update_buying_price(self, price, player_ind):
        self.state[player_ind] = price
        
    def update_inventory(self, quantity_bought, quantity_sold):
        
        if type(quantity_bought) is list:
            sum_bought = sum(quantity_bought)
        else:
            sum_bought = quantity_bought
        raise AssertionError('inventory code is broken - no inventory here')
        # self.state[0] += -quantity_sold + sum_bought
        # self.state[0] = min(self.storage_capacity, max(self.state[0], 0))

    def forecast_demand(self, price, demand, alpha = 0.5):
        self.prev_prices.append(price)
        
        if self.use_armax:
            new_demand = self.armax_model.model.forecast(len(self.prev_prices), exog=self.prev_prices)
            next_demand = np.squeeze(new_demand.to_numpy())
            if next_demand.ndim > 0:
                next_demand = next_demand[-1] 
        else:
            next_demand = (alpha * demand + (1 - alpha) * self.state[-1])

        self.state[-1] = next_demand
        

    def forecast_price(self, price_bought, alpha=0.5):
        # price forecast update
        for i in range(len(price_bought)):
            self.state[i] = (alpha * price_bought[i] 
                               + (1 - alpha) * self.state[i])

    def step_no_inventory(self, action, price_in, demand_in, alpha = 0.5):
        # ensure no extra products are created
        price_out = action[0]
        quantity_in = [action[i+1] for i in range(len(action) - 1)]
        quantity_out = min(sum(quantity_in), demand_in)
        # self.debug_code(price_in, quantity_in, price_out, quantity_out)

        # calculate reward
        reward = self.reward_coeff * (
            quantity_out * price_out
            - sum([price_in[i] * quantity_in[i] for i in range(len(price_in))]))
        # print(f'reward is {reward}')

        # update forecast states
        self.forecast_demand(price_out, quantity_out)
        self.forecast_price(price_in)

        # self.update_forecasts(price_in, demand_in, alpha)
        reward += -self.price_penalty *(
                price_out - self.preferred_price)**2 #9.009
        reward += - self.quantity_penalty * (
                sum(quantity_in) - self.preferred_quantity)**2 # 1.8
        # print(f'reward is {reward}')
        return self.state.copy(), reward, max(demand_in - quantity_out, 0)
    
    def normalize_actions(self, action):
        for i in range(len(action)):
            action[i] = min(max(action[i], self.action_space.low[i]),
                            self.action_space.high[i])

    def debug_code(self, price_in, quantity_in, price_out, quantity_out):
        print('-----------------start ---------------------')
        print(f'price_in {price_in}, quantity_in {quantity_in}')
        print(f'price_out {price_out}, quantity_out {quantity_out}')
        print(f'price_penalty = {self.price_penalty}')
        print(f'preferred price = {self.preferred_price}')

        print(f'quantity_penalty = {self.quantity_penalty}')
        print(f'preferred quantity = {self.preferred_quantity}')