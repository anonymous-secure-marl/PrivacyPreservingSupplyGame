# -*- coding: utf-8 -*-

import numpy as np
import time
import torch.nn as nn
import torch
import subprocess

import actor_critic.actor_critic_updates as ddpg
import actor_critic.actor_critic_config as ac_config
import actor_critic.actor_critic_networks as ac_nets
import util as ut
import armax.rolling_forecast as armax

##################################
torch.manual_seed(0)
np.random.seed(0)
##################################

def log_info(epoch, episode_reward, step, rews, actions, states, player_num):
    if player_num > 1:
        rewards = ''
        for i in range(player_num):
            rewards += f' r{i} = {np.round(rews[i])},'
        print(f'\r episode: {epoch} ' + rewards, end=' ')
              # f'{np.round(episode_reward/(step+1), 1)} '
              # f' r1 = {np.round(rews[0])}, r2 = {np.round(rews[1])} '
              # f'state = {np.round(states, 2)}' 
              # f'p0:{np.round(actions[0][0], 2)} '
              # f'{np.round(actions[0][1], 2)} '
              # f'p1:{np.round(actions[1][0], 2)} '
              # f'{np.round(actions[1][1], 2)} ', end=' ')
    else:
        print(f'\r episode: {epoch} '
              f'{np.round(episode_reward/(step+1), 1)} '
              f'state = {np.round(states, 2)} ' 
              f'actions = {np.round(actions, 2)} ', end=' ') 
        
def azure_log(azure_run, rewards, sample_policies, action_slices, player_num, 
              ful=None, stock=None, is_test=False):
    
    log_targs = ['reward', 'quantity', 'price']
    if is_test:
        log_targs = ['test_reward', 'test_price', 'test_quantity']
    azure_run.log('total reward', np.mean([sum(r) for r in rewards]))
    for i in range(player_num):
        azure_run.log(f'{log_targs[0]} {i}', np.mean([r[i] for r in rewards]))
        acts = [s[action_slices[i]] for s in sample_policies]
        
        azure_run.log(f'{log_targs[1]} {i}', np.mean([a[0] for a in acts]))
        azure_run.log(f'{log_targs[2]} {i}', np.mean([a[1] for a in acts]))
        for ind in range(2, len(acts[0])):
            azure_run.log(f'{log_targs[2]} {i}:{ind}', 
                          np.mean([a[ind] for a in acts]))
        if ful is not None:
            azure_run.log(f'fulfillment {i}', ful[i])
        if stock is not None:
            azure_run.log(f'stock {i}', stock[i])

      
def train_marl(world, ac_nns, optimizers, memories, max_epochs, max_steps, 
               batch_size, device, tau=1e-2, azure_run=None, 
               critic_mode=ac_config.Mode.INDIVIDUAL, target_update_int=10,
               retail_market=None, raw_mat_cost=None, ac_fig=None):

    begin_time = time.time()
    rewards = []
    sample_policies = []
    critic_criterion = nn.MSELoss()

    # get action slices for azure logging
    if ac_fig is None:
        if hasattr(world, 'entities'):
            action_shapes = [p.chain.action_space.low.shape[0] 
                             for p in world.entities]
        else:
            action_shapes = [p.action_space.low.shape[0] for p in world.players] 
        action_s = ut.concatenate_spaces(action_shapes) # concatenated action space of each player
        action_slices = [slice(action_s[p][0], action_s[p][1]) 
                         for p in range(world.player_num)]
    else:
        action_slices = ac_fig.a_inds
    
    #########################################################################
    f_s = open("StateInializationAtEpochStart.txt","r")
    ff_s = open("StateInializationAtEpochStart_check.txt","w")
    g = open("NoiseRandValues.txt","r")
    gg = open("NoiseRandValues_check.txt","w")
    SampleBatchByPlayer0 = open("SampleArrayByPlayer0.txt","r")
    SampleBatchByPlayer0_c = open("SampleArrayByPlayer0_check.txt","w")
    SampleBatchByPlayer1 = open("SampleArrayByPlayer1.txt","r")
    SampleBatchByPlayer1_c = open("SampleArrayByPlayer1_check.txt","w")
    #########################################################################
    
    for epoch in range(max_epochs):
        states = world.reset(f_s,ff_s)
        if hasattr(world, 'noises'):
            [noise.reset() for noise in world.noises]
        episode_reward = 0
        actions = [[0,0] for _ in range(world.player_num)]
        next_states = [np.zeros(states[i].shape) for i in range(world.player_num)]
        rews = [0,0]
        avg_ful = [0 for _ in range(world.player_num)]
        avg_stock = [0 for _ in range(world.player_num)]
        # print('------------------eps begin -------------')
        for step in range(max_steps):
            print("epoch: " + str(epoch) + " step: " + str(step))
            #################################
            if step==0:
                p1 = world.players[1]
                state1 = p1.obs.copy()
    #             print("state 1 print start for it " + str(step) + "\n")
                f = open("state1.txt","w")
                for i in range(5):
                    f.write("0.0")
                    f.write("\n")
                for s in state1:
                    f.write(str(s))
                    f.write("\n")
                f.close()
#             print("state 1 print end for it " + str(step) + "\n")
            #################################
            if step % 20 == 5:
                log_info(epoch, episode_reward, step, rews, actions, states, 
                         world.player_num)
            # world update
            if world.player_num == 1:
                if raw_mat_cost is None:
                    next_states, actions, rews, demand_step = world.step(
                        ac_nns, step+epoch*max_steps)
                    ful = None
                    stock = None
                else:
                    _, next_states, rews, actions, ful, stock = world.update(
                    ac_nns, retail_market, raw_mat_cost, step, ac_fig)
            else:
                total_states = critic_mode in [ac_config.Mode.TOTAL,
                                               ac_config.Mode.NEIGHBOR]
                if raw_mat_cost is not None:
                    # print(f' raw mat cost is not none')
                    # new update for tree_world
                    _, next_states, rews, actions, ful, stock = world.update(g,gg,
                    ac_nns, retail_market, raw_mat_cost, step, ac_fig)
                else:
                    # print(f' raw mat cost is NONE')
                    #original update function
                    next_states, actions, rews, demand_step = world.step_vhalf(
                        ac_nns, step, total_states, 0.5)
                    ful = None
                    stock = None
                if ful is not None and stock is not None:
                    for p in range(world.player_num):
                        avg_ful[p] += ful [p] / max_steps
                        avg_stock[p] += stock[p] / max_steps
                
            
            # this is where the algorithm stuff starts
            # print(f' in individual, reward = {rews}')
            if ac_fig is not None:
                next_s_arr = np.concatenate(next_states)
                s_arr = np.concatenate(states)
                a_arr = np.concatenate(actions)
                r = np.array(rews)
                memories.push(s_arr, a_arr, r, next_s_arr)
                prev_len = 396000
                if len(memories) > batch_size:
#                     print("entring ddpg")
                    # print(f' here models {ac_nns} ')
#                     [ddpg.update_target(
#                         ac_nns, optimizers[p], memories, batch_size,
#                         ac_fig.obs_inds, ac_fig.a_inds[p], r_inds=p, 
#                         p_ind=p, actor_state_mode=ac_fig.actor_mode,
#                         critic_action_mode=ac_fig.critic_action_mode,
#                         critic_state_mode=ac_fig.critic_state_mode,
#                         p_graph=world.graph, slice_graph=ac_fig.slice_graph)
#                     for p in range(len(optimizers))]
                    
                    print("ddpg 0in1 starts")
                    p=0
                    ddpg.update_target0in1(SampleBatchByPlayer0, SampleBatchByPlayer0_c,
                        ac_nns, optimizers[p], memories, batch_size,
                        ac_fig.obs_inds, ac_fig.a_inds[p], r_inds=p, 
                        p_ind=p, actor_state_mode=ac_fig.actor_mode,
                        critic_action_mode=ac_fig.critic_action_mode,
                        critic_state_mode=ac_fig.critic_state_mode,
                        p_graph=world.graph, slice_graph=ac_fig.slice_graph)
                    print("ddpg 0in1 ends")
                    
                    print("ddpg 1in1 starts")
                    p=1
                    ddpg.update_target1in1(SampleBatchByPlayer1, SampleBatchByPlayer1_c,
                        ac_nns, optimizers[p], memories, batch_size,
                        ac_fig.obs_inds, ac_fig.a_inds[p], r_inds=p, 
                        p_ind=p, actor_state_mode=ac_fig.actor_mode,
                        critic_action_mode=ac_fig.critic_action_mode,
                        critic_state_mode=ac_fig.critic_state_mode,
                        p_graph=world.graph, slice_graph=ac_fig.slice_graph)
                    print("ddpg 1in1 ends")
                    
                    
            else:
                # only works for INDIVIDUAL scenario right now
                for p_ind in range(world.player_num):
                    memories[p_ind].push(states[p_ind], actions[p_ind], 
                                         rews[p_ind], next_states[p_ind])
                    if len(memories[p_ind]) > batch_size:
                        ddpg.update(ac_nns[p_ind], critic_criterion, 
                                    optimizers[p_ind], memories[p_ind], 
                                    batch_size) 
                            
            if (step + epoch * max_steps) % target_update_int == 0:
                if ac_fig.actor_mode is ac_config.Mode.UNIT:
                    ac_nets.deep_copy(ac_nns, tau)
                else:
#                     [ac_nets.deep_copy(ac, tau) for ac in ac_nns]   
                    ###########################################################
#                     print("YESSSSSSSSSSSSSSSSSSSSS")
                    
                    actor_params = []
                    f = open("wNb_Player1Actor.txt","r")
                    for i in range(18178):
                        x = float(f.readline())
                        actor_params.append(x)
                    f.close()
                    
                    actor_targ_params = []
                    f = open("wNb_Player1Actor_targ.txt","r")
                    for i in range(18178):
                        x = float(f.readline())
                        actor_targ_params.append(x)
                    f.close()
                    
                    critic_params = []
                    f = open("wNb_Player1Critic.txt","r")
                    for i in range(18561):
                        x = float(f.readline())
                        critic_params.append(x)
                    f.close()
                    
                    critic_targ_params = []
                    f = open("wNb_Player1Critic_targ.txt","r")
                    for i in range(18561):
                        x = float(f.readline())
                        critic_targ_params.append(x)
                    f.close()
                    
                    actor_targ_updated_params = (1 - tau) * (np.array(actor_targ_params)) + tau * (np.array(actor_params))
                    critic_targ_updated_params = (1 - tau) * (np.array(critic_targ_params)) + tau * (np.array(critic_params))
                    
                    f = open("wNb_Player1Actor_targ.txt","w")
                    for i in range(18178):
                        f.write(str(actor_targ_updated_params[i]))
                        f.write("\n")
                    f.close()
                    
                    f = open("wNb_Player1Critic_targ.txt","w")
                    for i in range(18561):
                        f.write(str(critic_targ_updated_params[i]))
                        f.write("\n")
                    f.close()
                    
                    ###########################################################

            states = [s.copy() for s in next_states]
            episode_reward += sum(rews)
            rewards.append(rews)
            sample_policies.append(np.concatenate(actions))
        if azure_run is not None: # run once per epoch
            azure_log(azure_run, rewards[-max_steps:], 
                      sample_policies[-max_steps:], action_slices, world.player_num, 
                      avg_ful, avg_stock)
               
            
    end_time = time.time()
    
    #############################
    f_s.close()
    ff_s.close()
    g.close()
    gg.close()
    SampleBatchByPlayer0.close()
    SampleBatchByPlayer0_c.close()
    SampleBatchByPlayer1.close()
    SampleBatchByPlayer1_c.close()
    #############################
    
    print(f'training took {np.round(end_time - begin_time,2)} seconds')
    return rewards, sample_policies

def run_marl(ac_nns, world, max_epochs, max_steps, 
             critic_mode=ac_config.Mode.INDIVIDUAL, azure_run=None):
    rewards = []
    sample_policies = []
    if azure_run is not None:
        action_shapes = [p.chain.action_space.low.shape[0] 
                         for p in world.entities]
        action_s = ut.concatenate_spaces(action_shapes) # concatenated action space of each player
        action_slices = [slice(action_s[p][0], action_s[p][1]) 
                          for p in range(world.player_num)]
    for epoch in range(max_epochs):
        states = world.reset()
        episode_reward = 0
        rews = [0,0]
        actions = [[0,0],[0,0]]
        next_states = [np.zeros(states[i].shape) for i in range(world.player_num)]
        # print('------------------eps begin -------------')
        for step in range(max_steps):
            log_info(epoch, episode_reward, step, rews, actions, states, 
                     world.player_num)
                
            if world.player_num == 1:
                next_states, actions, rews, demand_step = world.step(
                    ac_nns, step)
            else:
                total_states = critic_mode in [ac_config.Mode.TOTAL_STATES,
                                               ac_config.Mode.MADDPG]
                next_states, actions, rews, demand_step = world.step_vhalf(
                    ac_nns, step, total_states, 0.5)                    
                    
            episode_reward += sum(rews)
            rewards.append(rews)
            sample_policies.append(np.concatenate(actions))

            if azure_run is not None:
                azure_log(azure_run, rewards, sample_policies, action_slices, 
                          max_steps, world.player_num, is_test=True)
                    
    return rewards, sample_policies           



   
                    
                    