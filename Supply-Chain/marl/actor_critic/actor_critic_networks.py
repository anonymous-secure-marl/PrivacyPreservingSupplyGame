# -*- coding: utf-8 -*-

import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import os as os
import time
from gym.spaces import Box

from collections import namedtuple


##################################
torch.manual_seed(0)
np.random.seed(0)
##################################

use_cuda = torch.cuda.is_available()
device   = torch.device("cuda" if use_cuda else "cpu")

model = namedtuple('model', ['actor', 'critic', 'actor_targ', 'critic_targ'])
optimizer = namedtuple('optimizer', ['actor', 'critic'])

   
class CriticNet(nn.Module):
    def __init__(self, num_inputs, num_actions, num_output=1, hidden_size=256, 
                 init_w=3e-3):
        super().__init__()
        self.linear1 = nn.Linear(num_inputs + num_actions, hidden_size)
        self.linear2 = nn.Linear(hidden_size, hidden_size)
        self.linear3 = nn.Linear(hidden_size, num_output)
        
        self.linear3.weight.data.uniform_(-init_w, init_w)
        self.linear3.bias.data.uniform_(-init_w, init_w)
        
    def forward(self, state, action):  
        x = torch.cat([state, action], 1)
        x = F.relu(self.linear1(x))
        x = F.relu(self.linear2(x))
        x = self.linear3(x)
        # x = (torch.tanh(x) + 1)/2 
        return x
    
#     ##############################################
#     def forward2(self, state, action):  
#         x = torch.cat([state, action], 1)
#         x = F.relu(self.linear1(x))
#         x = F.relu(self.linear2(x))
#         x.retain_grad()
#         y = self.linear3(x)
#         # x = (torch.tanh(x) + 1)/2 
#         return y, x
#     ##############################################

class ActorNet(nn.Module):
    def __init__(self, action_lim, num_inputs, num_actions, device, 
                 hidden_size=256, init_w=3e-3):
        super().__init__()
        self.action_lim_high = torch.Tensor(action_lim.high).to(device)
        self.device = device
        self.num_inputs = num_inputs
        self.num_actions = num_actions
        self.linear1 = nn.Linear(num_inputs, hidden_size).to(device)
        self.linear2 = nn.Linear(hidden_size, hidden_size).to(device)
        self.linear3 = nn.Linear(hidden_size, num_actions).to(device)
        
        self.linear3.weight.data.uniform_(-init_w, init_w)
        self.linear3.bias.data.uniform_(-init_w, init_w)
        
    def forward(self, state, get_model=False):
        x = F.relu(self.linear1(state))
        x = F.relu(self.linear2(x))
        model = self.linear3(x)
        #########################
#         model.retain_grad()
        #########################
        x = (torch.sigmoid(model) * self.action_lim_high)
        if get_model:
            return x, model 
        else:
            return x
    
    def get_action(self, state, get_model=False):
        state = torch.FloatTensor(state).to(self.device)  
        if get_model:
            action, model = self.forward(state, get_model)
            return action.detach().cpu().numpy(), model
        else:
            action = self.forward(state, get_model)
            return action #.detach().cpu().numpy()   

def deep_copy(ac_nn, tao=1): 
    """do a deep copy of the given actor_critic_network tuple.
    tao = 0: deep copy from target nn to nn
    tau = 1: deep copy from nn to target nn
    default: tau = 1
    """
    for target_param, param in zip(ac_nn.actor_targ.parameters(), 
                                   ac_nn.actor.parameters()):
        target_param.data.copy_(tao * param.data + (1 - tao) * target_param.data)
    for target_param, param in zip(ac_nn.critic_targ.parameters(), 
                                   ac_nn.critic.parameters()):
        target_param.data.copy_(tao * param.data + (1 - tao) * target_param.data)

def initiate_ac(old_actor):
    new_model = model(
        actor=ActorNet(old_actor.action_lim,  old_actor.num_inputs, 
                    old_actor.num_actions, old_actor.device).to(
                        old_actor.device),
        critic=CriticNet(old_actor.num_inputs, old_actor.num_actions).to(
            old_actor.device), 
        actor_targ=ActorNet(old_actor.action_lim,  old_actor.num_inputs, 
                         old_actor.num_actions, old_actor.device).to(
                            old_actor.device),
        critic_targ=CriticNet(old_actor.num_inputs, old_actor.num_actions).to(
            old_actor.device))
        
    for target_param, param in zip(new_model.actor_targ.parameters(), 
                                   new_model.actor.parameters()):
        target_param.data.copy_(param.data)
        
    deep_copy(new_model) 
        
    return new_model

def get_trainers(world, a_in, a_out, c_state, c_act, device, lr_a, lr_c, nn_size=64):
    ac_nns = []
    optimizers = []
    print('in get trainers')
    for i in range(len(c_state)):
        if hasattr(world, 'entities'):
            a_lim = world.entities[i].chain.action_space
        else:
            a_lim = world.players[i].action_space
            
        if len(a_in) == 1 and len(a_out) == 1:
            # this is a actor  Mode = UNIT scenario
            total_low = np.concatenate([p.action_space.low for p in world.players])
            total_high = np.concatenate([p.action_space.high for p in world.players])
            
            a_lim = Box(low=total_low, high=total_high)
            print(f'total action limit space {a_lim}')
            
        
        player_model = model( 
            actor=ActorNet(a_lim, a_in[i], a_out[i], device, 
                           hidden_size=nn_size).to(device),
            critic=CriticNet(c_state[i], c_act[i], hidden_size=nn_size).to(device), 
            actor_targ=ActorNet(a_lim, a_in[i], a_out[i], device, 
                                hidden_size=nn_size).to(device),
            critic_targ=CriticNet(c_state[i], c_act[i], 
                                  hidden_size=nn_size).to(device))
        ac_nns.append(player_model)
        optimizers.append(optimizer(**{
            'actor': optim.Adam(player_model.actor.parameters(), lr_a),
#             'actor': optim.SGD(player_model.actor.parameters(), lr_a),
            'critic': optim.Adam(player_model.critic.parameters(), lr_c)
#             'critic': optim.SGD(player_model.critic.parameters(), lr_c)
            }))
        deep_copy(player_model)

    return ac_nns, optimizers

def save_models(nns, directory=''):
    cur_time = time.strftime('%y_%m_%d%H%M')
    path_name = os.path.join(directory, '')

    if not os.path.exists(path_name):
        os.mkdir(path_name)
    ind = 0
    networks = ['actor', 'actor_targ', 'critic', 'critic_targ']
    
    for player in nns: 
        for net in networks:
            torch.save(getattr(player, net), 
                       os.path.join(path_name, '{}_{}_{}.pt'.format(
                           ind, net, cur_time)))
        ind +=1
        
def load_models(nns, player_ind, directory=''):
    ind = 0
    for ind in range(player_ind):
        nns.append(model(**{
            'actor': torch.load(
                os.path.join(directory, 'actor_{}.pt'.format(ind))), 
            'critic': torch.load(
                os.path.join(directory, 'actor_targ_{}.pt'.format(ind))),
            'actor_targ': torch.load(
                os.path.join(directory, 'critic_{}.pt'.format(ind))),
            'critic_targ': torch.load(
                os.path.join(directory, 'critic_targ_{}.pt'.format(ind))),
        }))
        