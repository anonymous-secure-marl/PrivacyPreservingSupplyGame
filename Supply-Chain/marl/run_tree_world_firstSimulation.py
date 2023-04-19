# -*- coding: utf-8 -*-
import numpy as np
import supply_game.tree_world as world
import networkx as nx
import algorithm_environment as alg_env
import util as ut
import actor_critic.actor_critic_config as ac_fig
import actor_critic.actor_critic_networks as ac_nets
import torch 
import sys
import algorithms as alg
# from azureml.core import Run
import random, time


# azure log settings
use_azure_log = False    
run_azure = Run.get_context() if use_azure_log else None
if use_azure_log: 
    print('running with azure log, disable with use_azure_log')
    
# plotting config 
plot_output = True
#False
if plot_output: print('plotting with matplotlib, diable with plot_output')

device   = torch.device("cuda" if torch.cuda.is_available() else "cpu")

def retail_market(price):
    return max(10 - float(sys.argv[1]) * price + 0.05 * np.random.normal(), 0)

# set up graph and supply chain
player_num = 2
line_graph = nx.DiGraph()
line_graph.add_nodes_from([i for i in range(player_num)])
line_graph.add_edge(0, 1)

min_sigma = 0.3  if len(sys.argv)<=5 else float(sys.argv[5])
theta = 0.2  if len(sys.argv)<=6 else float(sys.argv[6])

# chain = world.World(None, ladder_graph, theta=theta, min_sigma=min_sigma)
chain = world.World(None, line_graph, theta=theta, min_sigma=min_sigma)
# chain.print_graph()


##################################################################################################################
# set up actor critic mode
print(f' system arg {sys.argv}')
actor_mode = ac_fig.Mode.INDIVIDUAL if len(sys.argv)<=2 else ac_fig.Mode(int(sys.argv[2]))
critic_state_mode = ac_fig.Mode.INDIVIDUAL if len(sys.argv)<=3 else ac_fig.Mode(int(sys.argv[3]))
critic_action_mode = ac_fig.Mode.INDIVIDUAL if len(sys.argv)<=4 else ac_fig.Mode(int(sys.argv[4]))
obs_inds, a_inds, r_inds = ut.player_slices(chain.players)
slice_graph = []
for p in range(player_num):
    undirected_neighbors = [obs_inds[p]] + \
        [obs_inds[n] for n in chain.graph.predecessors(p)] + \
        [obs_inds[n] for n in chain.graph.successors(p)]
    slice_graph.append(undirected_neighbors)
ac_config = ac_fig.actor_critic_parameters(
    actor_mode=actor_mode, critic_state_mode=critic_state_mode, 
    critic_action_mode=critic_action_mode, obs_inds=obs_inds, a_inds=a_inds,  
    slice_graph=slice_graph)

# generate actor critics
lr_a = 1e-4 #  1e-5 # 
lr_c = 1e-3 #  1e-4 # 
a_in, a_out, c_state, c_act = ac_fig.actor_critic_dims(
    ac_config, chain.players, chain.graph)
obs_shapes = [p.observation_space.shape[0] for p in chain.players]
action_shapes = [p.action_space.low.shape[0] for p in chain.players]
ac_nns, optimizers = ac_nets.get_trainers(
    chain, a_in, a_out, c_state, c_act, device, lr_a, lr_c, nn_size=128) 

if ac_config.actor_mode is ac_fig.Mode.UNIT:
    ac_nns = ac_nns[0]
    
    
epoch = 0
max_steps = 4
raw_mat_cost = 0.5
replay_buffer_size = 1000000
#1000000
memories = ut.ReplayBuffer(1000000)


max_epochs  = 9900
#10000 #if len(sys.argv)<=3 else int(sys.argv[3])
max_steps   = 40
batch_size  = 128
target_interval  = 5 # if len(sys.argv)<=4 else int(sys.argv[4])
rewards, sample_policies = alg.train_marl(
    chain, ac_nns, optimizers, memories, max_epochs, max_steps, batch_size, 
    device, azure_run=run_azure, target_update_int=target_interval, retail_market=retail_market, 
    raw_mat_cost=raw_mat_cost, ac_fig=ac_config)


l = len(memories)
s_arr_len = 10
a_arr_len = 4
r_len = 2
next_s_arr_len = 10



f = open("ReplayBuffer.txt","w")
for i in range(l):
    s_arr, a_arr, r, next_s_arr = memories.buffer[i]
    for x in s_arr:
        f.write(str(x))
        f.write("\n")
    for x in a_arr:
        f.write(str(x))
        f.write("\n")
    for x in r:
        f.write(str(x))
        f.write("\n")
    for x in next_s_arr:
        f.write(str(x))
        f.write("\n")
    
f.close()


net = ac_nns[0].actor
torch.save(net.state_dict(), "Player0ActorParams")
f = open("wNb_Player0Actor.txt","w")
for layer in net.children():
    if isinstance(layer, torch.nn.Linear):
        wt = layer.state_dict()['weight']
        bs = layer.state_dict()['bias']
        s1 = wt.size()
        s2 = bs.size()
        
        
        for i in range(s1[0]):
            for j in range(s1[1]):
                f.write(str((wt[i][j]).item()))
                f.write("\n")
        for i in range(s2[0]):
            f.write(str((bs[i]).item()))
            f.write("\n")
f.close()

net = ac_nns[0].critic
torch.save(net.state_dict(), "Player0CriticParams")
f = open("wNb_Player0Critic.txt","w")
for layer in net.children():
    if isinstance(layer, torch.nn.Linear):
        wt = layer.state_dict()['weight']
        bs = layer.state_dict()['bias']
        s1 = wt.size()
        s2 = bs.size()
        
        
        for i in range(s1[0]):
            for j in range(s1[1]):
                f.write(str((wt[i][j]).item()))
                f.write("\n")
        for i in range(s2[0]):
            f.write(str((bs[i]).item()))
            f.write("\n")
f.close()

net = ac_nns[0].actor_targ
torch.save(net.state_dict(), "Player0Actor_targParams")
f = open("wNb_Player0Actor_targ.txt","w")
for layer in net.children():
    if isinstance(layer, torch.nn.Linear):
        wt = layer.state_dict()['weight']
        bs = layer.state_dict()['bias']
        s1 = wt.size()
        s2 = bs.size()
        
        
        for i in range(s1[0]):
            for j in range(s1[1]):
                f.write(str((wt[i][j]).item()))
                f.write("\n")
        for i in range(s2[0]):
            f.write(str((bs[i]).item()))
            f.write("\n")
f.close()

net = ac_nns[0].critic_targ
torch.save(net.state_dict(), "Player0Critic_targParams")
f = open("wNb_Player0Critic_targ.txt","w")
for layer in net.children():
    if isinstance(layer, torch.nn.Linear):
        wt = layer.state_dict()['weight']
        bs = layer.state_dict()['bias']
        s1 = wt.size()
        s2 = bs.size()
        
        
        for i in range(s1[0]):
            for j in range(s1[1]):
                f.write(str((wt[i][j]).item()))
                f.write("\n")
        for i in range(s2[0]):
            f.write(str((bs[i]).item()))
            f.write("\n")
f.close()

net = ac_nns[1].actor
torch.save(net.state_dict(), "Player1ActorParams")
f = open("wNb_Player1Actor.txt","w")
for layer in net.children():
    if isinstance(layer, torch.nn.Linear):
        wt = layer.state_dict()['weight']
        bs = layer.state_dict()['bias']
        s1 = wt.size()
        s2 = bs.size()
        
        
        for i in range(s1[0]):
            for j in range(s1[1]):
                f.write(str((wt[i][j]).item()))
                f.write("\n")
        for i in range(s2[0]):
            f.write(str((bs[i]).item()))
            f.write("\n")
f.close()

net = ac_nns[1].critic
torch.save(net.state_dict(), "Player1CriticParams")
f = open("wNb_Player1Critic.txt","w")
for layer in net.children():
    if isinstance(layer, torch.nn.Linear):
        wt = layer.state_dict()['weight']
        bs = layer.state_dict()['bias']
        s1 = wt.size()
        s2 = bs.size()
        
        
        for i in range(s1[0]):
            for j in range(s1[1]):
                f.write(str((wt[i][j]).item()))
                f.write("\n")
        for i in range(s2[0]):
            f.write(str((bs[i]).item()))
            f.write("\n")
f.close()

net = ac_nns[1].actor_targ
torch.save(net.state_dict(), "Player1Actor_targParams")
f = open("wNb_Player1Actor_targ.txt","w")
for layer in net.children():
    if isinstance(layer, torch.nn.Linear):
        wt = layer.state_dict()['weight']
        bs = layer.state_dict()['bias']
        s1 = wt.size()
        s2 = bs.size()
        
        
        for i in range(s1[0]):
            for j in range(s1[1]):
                f.write(str((wt[i][j]).item()))
                f.write("\n")
        for i in range(s2[0]):
            f.write(str((bs[i]).item()))
            f.write("\n")
f.close()

net = ac_nns[1].critic_targ
torch.save(net.state_dict(), "Player1Critic_targParams")
f = open("wNb_Player1Critic_targ.txt","w")
for layer in net.children():
    if isinstance(layer, torch.nn.Linear):
        wt = layer.state_dict()['weight']
        bs = layer.state_dict()['bias']
        s1 = wt.size()
        s2 = bs.size()
        
        
        for i in range(s1[0]):
            for j in range(s1[1]):
                f.write(str((wt[i][j]).item()))
                f.write("\n")
        for i in range(s2[0]):
            f.write(str((bs[i]).item()))
            f.write("\n")
f.close()


########################################################################################################################

if plot_output: 
    print('plotting outputs')
    alg_env.plot_policies(chain, rewards, sample_policies, savefig=True,
                          folder = 'supply_game/results')
















