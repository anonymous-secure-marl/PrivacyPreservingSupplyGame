# -*- coding: utf-8 -*-

import numpy as np
from gym.spaces import Box
import torch
from enum import Enum

##################################
torch.manual_seed(0)
np.random.seed(0)
##################################

use_cuda =  torch.cuda.is_available()
device   = torch.device("cuda" if use_cuda else "cpu")


class DemandDistributionMode(Enum):
    EVEN = 0
    FCFS = 1 # first come fist serve
    PROP = 2 # inversly proportional
    
    
class Player():
    def __init__(self, in_neighbors, out_neighbors, lead_time=3, price_lim=5, 
                 quantity_lim=10, holding_cost=0.01, lost_cost=0.1, 
                 distribution_mode=DemandDistributionMode.EVEN):
#         lead_time=3
        """ Obeservation space:
            - purchasing cost C
            - demand predictor d
            - current stock x_0
            - expected stock x_1...x__{lt} (lt = lead time)
            Realized demand: 
                D_i  = N(d - kp, sigma)
                
            Actions:
            - product price p
            - purchased raw material q
            Holding cost: 
                holding cost = a_hold * inventory
            Demand unsatisfaction cost: 
                cost = a_unsat * quantity_deficit
        """ 
        
        # observation space definition
        if in_neighbors == 0:
            in_neighbors = 1
        if out_neighbors == 0:
            out_neighbors = 1
        state_lim = in_neighbors*[price_lim] + out_neighbors*[quantity_lim]+lead_time*[quantity_lim]
        self.in_num = in_neighbors
        self.out_num = out_neighbors
        self.observation_space = Box(np.zeros(len(state_lim)), np.array(state_lim))    
        self.obs = np.zeros(len(state_lim))
        self.lead_time = lead_time
        
        # distribution mode definition
        self.distribution_mode = distribution_mode
        
        # action space definition
        self.action_space = Box(np.zeros(in_neighbors + out_neighbors), 
                                np.array(in_neighbors *[quantity_lim]+ \
                                         out_neighbors *[price_lim]))
        
        # set holding cost/lost sales cost coefficients
        self.h_cost = holding_cost
        self.l_cost = lost_cost
        self.leftover_inv = 0
        # reset player    
        
#         self.reset()
        # Uncomment this while generating self.obs = np.random.rand(self.observation_space.low.shape[0])
        
#     def reset(self,f):
    def reset(self,f,ff):
        #########################################
#         self.obs = np.random.rand(self.observation_space.low.shape[0])
#         for x in self.obs:
#             f.write(str(x))
#             f.write("\n")
        
        #########################################
        obs_arr=[]
        n = self.observation_space.low.shape[0]
        for i in range(n):
            x = float(f.readline())
#             ff.write(str(x))
#             ff.write("\n")
            obs_arr.append(x)
        self.obs = np.array(obs_arr)
        ##########################################
        
        self.leftover_inv = 0
        return self.obs.copy()
    
    def stored_products(self):
        """ Return current stock level of player. """ 
        return self.leftover_inv
    
    def normalize_actions(self, action):
        normed_act = np.maximum(np.minimum(action, self.action_space.high), 
                         self.action_space.low)
        return normed_act
    
    def step_1(self, cost):
        # update cost 
        for i in range(len(cost)):
            self.obs[i] = max(min(cost[i], self.observation_space.high[i]),
                              self.observation_space.low[i])
        #states not completely updated after updating price
        
    def update_demand(self, new_demand): #TODO this function is not used
        # update demand predictor
        d_ind = self.in_num
        self.obs[d_ind] = max(min(new_demand, 
                                  self.observation_space.high[d_ind]),
                              self.observation_space.low[d_ind])
    def realize_demands(self, demands):
        # decide on the total product each player receives
        current_stock = self.obs[self.in_num + self.out_num]
        total_demand = sum(demands)
        realized_demands = []
        if current_stock >= total_demand:
            realized_demands = demands.copy()
        else:
            gap = (total_demand - current_stock)/len(demands)
            if self.distribution_mode is DemandDistributionMode.EVEN:
                [realized_demands.append(min(d, current_stock/len(demands))) for d in demands]
            elif self.distribution_mode is DemandDistributionMode.FCFS:
                order = np.argsort(self.obs[self.in_num:self.in_num+self.out_num])
                realized_demand = demands.copy()
                for ind in order:
                    realized_demand[ind] = min(gap, demands[ind])
                    gap += -realized_demand[ind]
                    if gap <= 0:
                        break 
            elif self.distribution_mode is DemandDistributionMode.PROP:
                proportions = [1/p for p in self.obs[self.in_num:self.in_num+self.out_num] ]
                prop_sum = sum(proportions)
                [realized_demands.append(
                    max(0, demands[i] - gap * proportions[i]/prop_sum)) 
                    for i in range(self.out_num)]
        return realized_demands
                   
    def step_2(self, demands, available_supply, prod_price):
        stock_ind = self.in_num + self.out_num # index of current stock
        demand_inds = slice(self.in_num, self.in_num + self.out_num)
        cur_stock = self.obs[stock_ind]
       
        # derive true product sold 
        # print(f' demand {demands}')
        prod_sold = self.realize_demands(demands) 
        # print(f' product sold {prod_sold}')
        mat_cost = self.obs[:self.in_num]
        # calculate reward
        
        reward = sum([d*p  for d, p in zip(prod_sold, prod_price)])
        # print(f'reward profit {reward}')
        reward += sum([-c*s for c, s in zip(mat_cost, available_supply)])
        # print(f'reward profit - cost {reward}')
        # add penalties
        # holding cost on left over inventory
        self.leftover_inv = cur_stock - sum(prod_sold)
        reward += -self.h_cost * self.leftover_inv
        # print(f'reward profit - cost - holding cost {reward}')
        # lost of good will cost
        reward += -self.l_cost * (sum(demands) - sum(prod_sold))
        # print(f'reward profit - cost - holding cost - good will cost {reward}')
        
        # update stocks
        # print(f'stock levels {self.obs[stock_ind:]}')
        # print(f'product sold {sum(prod_sold)}')
        self.obs[stock_ind] += -sum(prod_sold)  # left over inventory
        if self.lead_time > 0:
            self.obs[stock_ind] += self.obs[stock_ind+1]
            self.obs[stock_ind+1:-1] = self.obs[stock_ind+2:]
            self.obs[-1] = sum(available_supply)         
        else:
            self.obs[-1] += sum(available_supply)            
        # print(f'stock levels after update {self.obs[stock_ind:]}')
        # update demands
        # print(f'before forecast {self.obs[demand_inds]}')
        # print(f'original demand {self.obs[demand_inds]}')
        # print(f'new demand {demands}')
        self.obs[demand_inds] = self.forecast_demands(demands, self.obs[demand_inds])
        # print(f'after forecast {self.obs[demand_inds]}')
        # enforce state capacity
        for i in range(len(self.obs)):
            self.obs[i] = max(min(self.obs[i], self.observation_space.high[i]),
                              self.observation_space.low[i])
    
        return self.obs.copy(), reward, prod_sold
    
    
    def forecast_demands(self, new_demands, old_demands):
        # for now, do an averaging
        forecast_demands = np.array([0.8 * old + 0.2 * new 
                            for old, new in zip(old_demands, new_demands)])

        return forecast_demands
        
        
        
        
        
        
        
        
        
        
        
        
        
    