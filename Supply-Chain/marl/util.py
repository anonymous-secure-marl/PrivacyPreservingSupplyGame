# -*- coding: utf-8 -*-

import numpy as np
import random 
import torch

use_cuda = torch.cuda.is_available()
device   = torch.device("cuda" if use_cuda else "cpu")

##################################
torch.manual_seed(0)
np.random.seed(0)
random.seed(0)
##################################

class ReplayBuffer:
    def __init__(self, capacity):
        self.capacity = capacity
        self.buffer = []
        self.position = 0
    
    def push(self, state, action, reward, next_state):
        if len(self.buffer) < self.capacity:
            self.buffer.append(None)
        self.buffer[self.position] = (state, action, reward, next_state)
        self.position = (self.position + 1) % self.capacity
    
    def sample(self, SampleBatch, batch_size, agent_ind = None):
        
#     def sample(self, SampleBatch, SampleBatch_c, batch_size, agent_ind = None):
#         batch = random.sample(self.buffer, batch_size)
#         state, action, reward, next_state = map(np.stack, zip(*batch))
        
        ####################################################################
#         l=len(self.buffer)
#         prev_len = 396000
#         if l-prev_len < 100:
#             batch = random.sample(self.buffer, batch_size)
#         else:
#             batch = random.sample(self.buffer[:prev_len], 48)
#             batch2 = random.sample(self.buffer[prev_len:], batch_size-48)
#             batch = batch + batch2
#         state, action, reward, next_state = map(np.stack, zip(*batch))
        #####################################################################
#         l=len(self.buffer)
#         prev_len = 396000
#         if l-prev_len < 100:
#             sample_arr = random.sample(range(0, l), batch_size)
#         else:
#             sample_arr = random.sample(range(0, prev_len), 48)
#             sample_arr2 = random.sample(range(prev_len, l), batch_size - 48)
#             sample_arr = sample_arr + sample_arr2
# #         for x in sample_arr:
# #             SampleBatch.write(str(x))
# #             SampleBatch.write("\n")
            

        sample_arr = []
        for i in range(batch_size):
            x = int(SampleBatch.readline())
#             SampleBatch_c.write(str(x))
#             SampleBatch_c.write("\n")
            sample_arr.append(x)
            
        batch = []
        for i in sample_arr:
            batch.append(self.buffer[i])
        state, action, reward, next_state = map(np.stack, zip(*batch))
        ###################################################################
        return state, action, reward, next_state
    
    def __len__(self):
        return len(self.buffer)
    
class Buffer:
    def __init__(self, capacity):
        self.capacity = capacity
        self.buffer = []
        self.position = 0
    
    def push(self, state, action, reward, next_state):
        if len(self.buffer) < self.capacity:
            self.buffer.append(None)
        self.buffer[self.position] = (state, action, reward, next_state)
        self.position = (self.position + 1) % self.capacity
    
    def retrieve(self, agent_ind = None):
        state, action, reward, next_state = zip(*self.buffer)
        state = torch.FloatTensor(state).to(device)
        reward = torch.FloatTensor(reward).to(device)
        next_state = torch.FloatTensor(next_state).to(device)
        action = torch.cat(action, dim=0)
        
        return state, action, reward, next_state
    
    def __len__(self):
        return len(self.buffer)

    def clear(self):
        self.buffer.clear()
        self.__init__(self.capacity)

        
class OUNoise(object):
    """Ornstein-Uhlenbeck process. 
    
    https://github.com/vitchyr/rlkit/blob/master/rlkit/exploration_strategies/ou_strategy.py
    https://en.wikipedia.org/wiki/Ornstein%E2%80%93Uhlenbeck_process
    """
    def __init__(self, action_space, mu=0.0, theta=0.3, max_sigma=0.15, 
                  min_sigma=0.15, decay_period=10000):
    # def __init__(self, action_space, mu=0.0, theta=0.15, max_sigma=0.3, 
    #               min_sigma=0.3, decay_period=100000):
        # print(f'min sigma received {min_sigma}')
        self.mu           = mu
        self.theta        = theta
        self.sigma        = max_sigma
        self.max_sigma    = max_sigma
        self.min_sigma    = min_sigma
        self.decay_period = decay_period
        self.action_dim   = action_space.low.shape[0]
        # print(f'action dimsion = {self.action_dim}')
        self.low          = action_space.low
        self.high         = action_space.high
        
        self.reset()
        
    def reset(self):
        self.state = np.ones(self.action_dim) * self.mu
        
#     def evolve_state(self,g):
    def evolve_state(self,g, gg):
        x  = self.state
        
        #######################################################
#         rand = np.random.randn(self.action_dim)
#         for x in rand:
#             g.write(str(x))
#             g.write("\n")
            

        rand_arr=[]
        n = self.action_dim
        for i in range(n):
            x = float(g.readline())
#             gg.write(str(x))
#             gg.write("\n")
            rand_arr.append(x)
        rand = np.array(rand_arr)
        ########################################################
#         dx = (self.theta * (self.mu - x) 
#               + self.sigma * np.random.randn(self.action_dim))

        dx = (self.theta * (self.mu - x) 
              + self.sigma * rand)
        self.state = x + dx
        
        return self.state
    
#     def get_action(self, g, action, t=0):
#         ou_state = self.evolve_state(g)
#         self.sigma = (self.max_sigma - (self.max_sigma - self.min_sigma) * 
#                       min(1.0, t / self.decay_period))
#         return np.clip(action + ou_state, self.low, self.high)
    
    def get_action(self, g, gg, action, t=0):
        ou_state = self.evolve_state(g,gg)
        self.sigma = (self.max_sigma - (self.max_sigma - self.min_sigma) * 
                      min(1.0, t / self.decay_period))
        return np.clip(action + ou_state, self.low, self.high)
           

def concatenate_spaces(individual_spaces):
    stacked_size = []
    head, end = 0, 0
    for shape in individual_spaces:
        end = end + shape
        # end_a = end_a + action_shape 
        range_next = (head, end)
        # range_a = (head_a, end_a)
        stacked_size.append(range_next)
        # action_size.append(range_a)
        head = end
        # head_a = end_a 
    return stacked_size

def player_slices(players, p_state_shapes=None):
    # state slices
    if p_state_shapes is None:
        obs_shapes = [p.obs.shape[0] for p in players]
    else:
        obs_shapes = p_state_shapes
    obs_s = concatenate_spaces(obs_shapes) # concatenated state space of each player
    obs_slices = [slice(obs_s[p][0], obs_s[p][1]) for p in range(len(players))]
    
    # action slices
    action_shapes = [p.action_space.high.shape[0] for p in players]
    act_s = concatenate_spaces(action_shapes) # concatenated action space of each player
    act_slices = [slice(act_s[p][0], act_s[p][1]) for p in range(len(players))] 
    
    # reward slices
    r_slices = [p for p in range(len(players))]

    return obs_slices, act_slices, r_slices
    
    
    
    
    