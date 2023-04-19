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
from azureml.core import Run
import random, time


# azure log settings
use_azure_log = True    
run_azure = Run.get_context() if use_azure_log else None
if use_azure_log: 
    print('running with azure log, disable with use_azure_log')
    # set random seed
    # seed = int(time.time())
    # random.seed(seed)
    # np.random.seed(seed)
    # torch.manual_seed(seed)
    # run_azure.log('seed', seed)

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
# line_graph.add_edge(1, 2)
# player_num = 4
# ladder_graph = nx.DiGraph()
# ladder_graph.add_nodes_from([i for i in range(player_num)])
# ladder_graph.add_edge(0, 2)
# ladder_graph.add_edge(0, 3)
# ladder_graph.add_edge(1, 2)
# ladder_graph.add_edge(1, 3)
min_sigma = 0.3  if len(sys.argv)<=5 else float(sys.argv[5])
theta = 0.2  if len(sys.argv)<=6 else float(sys.argv[6])

# chain = world.World(None, ladder_graph, theta=theta, min_sigma=min_sigma)
chain = world.World(None, line_graph, theta=theta, min_sigma=min_sigma)
# chain.print_graph()

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
max_steps = 40
raw_mat_cost = 0.5
replay_buffer_size = 1000000
#1000000
memories = ut.ReplayBuffer(1000000) 

max_epochs  = 10000
#10000 #if len(sys.argv)<=3 else int(sys.argv[3])
max_steps   = 40
batch_size  = 128
target_interval  = 5 # if len(sys.argv)<=4 else int(sys.argv[4])
rewards, sample_policies = alg.train_marl(
    chain, ac_nns, optimizers, memories, max_epochs, max_steps, batch_size, 
    device, azure_run=run_azure, target_update_int=target_interval, retail_market=retail_market, 
    raw_mat_cost=raw_mat_cost, ac_fig=ac_config)



if plot_output: 
    print('plotting outputs')
    alg_env.plot_policies(chain, rewards, sample_policies, savefig=True,
                          folder = 'supply_game/results')
















