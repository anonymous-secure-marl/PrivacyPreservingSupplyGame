# -*- coding: utf-8 -*-
"""
Created on Thu Aug  5 11:57:06 2021

@author: t-sarahli
"""
import numpy as np
import time

import torch.nn as nn
import torch.optim as optim

import ddpg
import util as ut
import models as nns
import armax.rolling_forecast as armax


def log_info(epoch, episode_reward, step, rews, actions, states, player_num):
    if player_num > 1:
        print(f'\r episode: {epoch} '
              # f'{np.round(episode_reward/(step+1), 1)} '
              f' r1 = {np.round(rews[0])}, r2 = {np.round(rews[1])} '
               # f'state = {np.round(states, 2)}' 
              f'p0:{np.round(actions[0][0], 2)} '
              f'{np.round(actions[0][1], 2)} '
              f'p1:{np.round(actions[1][0], 2)} '
              f'{np.round(actions[1][1], 2)} ', end=' ')
    else:
        print(f'\r episode: {epoch} '
              f'{np.round(episode_reward/(step+1), 1)} '
              f'state = {np.round(states, 2)} ' 
              f'actions = {np.round(actions, 2)} ', end=' ') 
        
def azure_log(azure_run, rewards, sample_policies, action_slices, max_steps, 
              player_num, is_test=False):
    
    log_targs = ['reward', 'price', 'quantity']
    if is_test:
        log_targs = ['test_reward', 'test_price', 'test_quantity']
    for i in range(player_num):
        azure_run.log(
            f'{log_targs[0]} {i}', 
            np.mean([r[i] for r in rewards[-max_steps:]]))
        acts = [s[action_slices[i]] 
                for s in sample_policies[-max_steps:]]
        azure_run.log(f'{log_targs[1]} {i}', 
                      np.mean([a[0] for a in acts]))
        azure_run.log(f'{log_targs[2]} {i}', 
                      np.mean([a[1] for a in acts]))
        for ind in range(2, len(acts[0])):
            azure_run.log(f'{log_targs[2]} {i}:{ind}', 
                          np.mean([a[ind] for a in acts]))

      
def train_marl(world, ac_nns, optimizers, memories, max_epochs, max_steps, 
               batch_size, device, tau=1e-2, azure_run=None, 
               critic_mode=nns.CriticMode.INDIVIDUAL, target_update_int=10):
    rewards = []
    critic_criterion = nn.MSELoss()

    obs_shapes = [p.chain.observation_space.shape[0] 
                  for p in world.entities]
    action_shapes = [p.chain.action_space.low.shape[0] 
                     for p in world.entities]
    obs_s = ut.concatenate_spaces(obs_shapes) # concatenated state space of each player
    action_s = ut.concatenate_spaces(action_shapes) # concatenated action space of each player
    obs_slices = [slice(obs_s[p][0], obs_s[p][1]) 
                  for p in range(world.player_num)]
    action_slices = [slice(action_s[p][0], action_s[p][1]) 
                      for p in range(world.player_num)]
    for epoch in range(max_epochs):
        states = world.reset()
        episode_reward = 0
        actions = [[0,0] for _ in range(world.player_num)]
        next_states = [np.zeros(states[i].shape) for i in range(world.player_num)]
        rews = [0,0]
        # print('------------------eps begin -------------')
        for step in range(max_steps):
            if step % 20 == 3:
                log_info(epoch, episode_reward, step, rews, actions, states, 
                         world.player_num)

            if world.player_num == 1:
                next_states, actions, rews, demand_step = world.step(
                    ac_nns, step)
            else:
                total_states = critic_mode is nns.CriticMode.TOTAL_STATES
                next_states, actions, rews, demand_step = world.step_vhalf(
                    ac_nns, step, total_states, 0.5)

            update_target = False
            if (step + epoch * max_steps) % target_update_int == 0:
                update_target = True

            if critic_mode is nns.CriticMode.INDIVIDUAL:
                for p_ind in range(world.player_num):
                    memories[p_ind].push(states[p_ind], actions[p_ind], 
                                         rews[p_ind], next_states[p_ind])
                    if len(memories[p_ind]) > batch_size:
                        ddpg.update(ac_nns[p_ind], critic_criterion, 
                                    optimizers[p_ind], memories[p_ind], 
                                    batch_size) 
            elif critic_mode is nns.CriticMode.TOTAL_STATES:
                states_arr = np.concatenate(states)
                next_states_arr = np.concatenate(next_states)
                for p_ind in range(world.player_num):
                    memories[p_ind].push(states_arr, actions[p_ind], rews[p_ind], 
                                         next_states_arr)
                    if len(memories[0]) > batch_size:
                        ddpg.update(ac_nns[p_ind], critic_criterion,  
                                    optimizers[p_ind], memories[p_ind], batch_size) 
                        if update_target:
                            nns.deep_copy(ac_nns[p_ind], tau)

                
                      
            states = [s.copy() for s in next_states]
            episode_reward += sum(rews)
            rewards.append(rews)
            sample_policies.append(np.concatenate(actions))
            if step % max_steps == max_steps-1 and azure_run is not None:
                azure_log(azure_run, rewards, sample_policies, action_slices, 
                          max_steps, world.player_num)
               
            
    end_time = time.time()
    print(f'training took {np.round(end_time - begin_time,2)} seconds')
    return rewards, sample_policies
