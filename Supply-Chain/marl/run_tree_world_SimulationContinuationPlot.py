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


##################################
torch.manual_seed(0)
np.random.seed(0)
random.seed(0)
##################################

# azure log settings
use_azure_log = False    
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
plot_output = False
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


chain = world.World(None, line_graph, theta=theta, min_sigma=min_sigma)



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
lr_a = 1e-4
#5 * 1e-10 #  1e-4 # 
lr_c = 1e-3 #  1e-3 # 
a_in, a_out, c_state, c_act = ac_fig.actor_critic_dims(
    ac_config, chain.players, chain.graph)
obs_shapes = [p.observation_space.shape[0] for p in chain.players]
action_shapes = [p.action_space.low.shape[0] for p in chain.players]
ac_nns, optimizers = ac_nets.get_trainers(
    chain, a_in, a_out, c_state, c_act, device, lr_a, lr_c, nn_size=128) 

if ac_config.actor_mode is ac_fig.Mode.UNIT:
    ac_nns = ac_nns[0]


if (int(sys.argv[2]))==3:
    (ac_nns[0].actor).load_state_dict(torch.load("SupplyGameResults/SupplyGameHistory/Player0ActorParams"))
    (ac_nns[0].actor).eval()

    (ac_nns[0].actor_targ).load_state_dict(torch.load("SupplyGameResults/SupplyGameHistory/Player0Actor_targParams"))
    (ac_nns[0].actor_targ).eval()

    (ac_nns[0].critic).load_state_dict(torch.load("SupplyGameResults/SupplyGameHistory/Player0CriticParams"))
    (ac_nns[0].critic).eval()

    (ac_nns[0].critic_targ).load_state_dict(torch.load("SupplyGameResults/SupplyGameHistory/Player0Critic_targParams"))
    (ac_nns[0].critic_targ).eval()


    (ac_nns[1].actor).load_state_dict(torch.load("SupplyGameResults/SupplyGameHistory/Player1ActorParams"))
    (ac_nns[1].actor).eval()

    (ac_nns[1].actor_targ).load_state_dict(torch.load("SupplyGameResults/SupplyGameHistory/Player1Actor_targParams"))
    (ac_nns[1].actor_targ).eval()

    (ac_nns[1].critic).load_state_dict(torch.load("SupplyGameResults/SupplyGameHistory/Player1CriticParams"))
    (ac_nns[1].critic).eval()

    (ac_nns[1].critic_targ).load_state_dict(torch.load("SupplyGameResults/SupplyGameHistory/Player1Critic_targParams"))
    (ac_nns[1].critic_targ).eval()


if (int(sys.argv[2]))==1:
    (ac_nns[0].actor).load_state_dict(torch.load("SupplyGameResults/SupplyGameHistory_Individual/Player0ActorParams"))
    (ac_nns[0].actor).eval()

    (ac_nns[0].actor_targ).load_state_dict(torch.load("SupplyGameResults/SupplyGameHistory_Individual/Player0Actor_targParams"))
    (ac_nns[0].actor_targ).eval()

    (ac_nns[0].critic).load_state_dict(torch.load("SupplyGameResults/SupplyGameHistory_Individual/Player0CriticParams"))
    (ac_nns[0].critic).eval()

    (ac_nns[0].critic_targ).load_state_dict(torch.load("SupplyGameResults/SupplyGameHistory_Individual/Player0Critic_targParams"))
    (ac_nns[0].critic_targ).eval()


    (ac_nns[1].actor).load_state_dict(torch.load("SupplyGameResults/SupplyGameHistory_Individual/Player1ActorParams"))
    (ac_nns[1].actor).eval()

    (ac_nns[1].actor_targ).load_state_dict(torch.load("SupplyGameResults/SupplyGameHistory_Individual/Player1Actor_targParams"))
    (ac_nns[1].actor_targ).eval()

    (ac_nns[1].critic).load_state_dict(torch.load("SupplyGameResults/SupplyGameHistory_Individual/Player1CriticParams"))
    (ac_nns[1].critic).eval()

    (ac_nns[1].critic_targ).load_state_dict(torch.load("SupplyGameResults/SupplyGameHistory_Individual/Player1Critic_targParams"))
    (ac_nns[1].critic_targ).eval()




# epoch = 0
# max_steps = 4
raw_mat_cost = 0.5
replay_buffer_size = 1000000
#1000000
memories = ut.ReplayBuffer(1000000)





l = 9900 * 40
s_arr_len = 10
a_arr_len = 4
r_len = 2
next_s_arr_len = 10

f = open("SupplyGameResults/SupplyGameHistory/ReplayBuffer.txt","r")
for i in range(l):
    s_arr=[]
    a_arr=[]
    r=[]
    next_s_arr=[]
    for i in range(s_arr_len):
        x = float(f.readline())
        s_arr.append(x)
    for i in range(a_arr_len):
        x = float(f.readline())
        a_arr.append(x)
    for i in range(r_len):
        x = float(f.readline())
        r.append(x)
    for i in range(next_s_arr_len):
        x = float(f.readline())
        next_s_arr.append(x)
        
    memories.push(np.array(s_arr), np.array(a_arr), np.array(r), np.array(next_s_arr))
        
f.close()





max_epochs  = 1 # 20
#10000 #if len(sys.argv)<=3 else int(sys.argv[3])
max_steps   = 1 # 40
#40
batch_size  = 128
target_interval  = 5 # if len(sys.argv)<=4 else int(sys.argv[4])
# rewards2, sample_policies2 = alg.train_marl(
#     chain, ac_nns, optimizers, memories, max_epochs, max_steps, batch_size, 
#     device, azure_run=run_azure, target_update_int=target_interval, retail_market=retail_market, 
#     raw_mat_cost=raw_mat_cost, ac_fig=ac_config)
rewards2, sample_policies2, selling_record2 = alg.train_marl(
    chain, ac_nns, optimizers, memories, max_epochs, max_steps, batch_size, 
    device, azure_run=run_azure, target_update_int=target_interval, retail_market=retail_market, 
    raw_mat_cost=raw_mat_cost, ac_fig=ac_config)


prev_len = 396000

l = len(memories)
print(l)

if (int(sys.argv[2]))==3:
    
    f = open("SupplyGameResults/SupplyGameSimulationContinuationResults/ReplayBufferNew.txt","w")
    for i in range(prev_len, l):
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
    torch.save(net.state_dict(), "SupplyGameResults/SupplyGameSimulationContinuationResults/Player0ActorParams")
    f = open("SupplyGameResults/SupplyGameSimulationContinuationResults/wNb_Player0Actor.txt","w")
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
    torch.save(net.state_dict(), "SupplyGameResults/SupplyGameSimulationContinuationResults/Player0CriticParams")
    f = open("SupplyGameResults/SupplyGameSimulationContinuationResults/wNb_Player0Critic.txt","w")
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
    torch.save(net.state_dict(), "SupplyGameResults/SupplyGameSimulationContinuationResults/Player0Actor_targParams")
    f = open("SupplyGameResults/SupplyGameSimulationContinuationResults/wNb_Player0Actor_targ.txt","w")
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
    torch.save(net.state_dict(), "SupplyGameResults/SupplyGameSimulationContinuationResults/Player0Critic_targParams")
    f = open("SupplyGameResults/SupplyGameSimulationContinuationResults/wNb_Player0Critic_targ.txt","w")
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
    torch.save(net.state_dict(), "SupplyGameResults/SupplyGameSimulationContinuationResults/Player1ActorParams")
    f = open("SupplyGameResults/SupplyGameSimulationContinuationResults/wNb_Player1Actor.txt","w")
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
    torch.save(net.state_dict(), "SupplyGameResults/SupplyGameSimulationContinuationResults/Player1CriticParams")
    f = open("SupplyGameResults/SupplyGameSimulationContinuationResults/wNb_Player1Critic.txt","w")
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
    torch.save(net.state_dict(), "SupplyGameResults/SupplyGameSimulationContinuationResults/Player1Actor_targParams")
    f = open("SupplyGameResults/SupplyGameSimulationContinuationResults/wNb_Player1Actor_targ.txt","w")
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
    torch.save(net.state_dict(), "SupplyGameResults/SupplyGameSimulationContinuationResults/Player1Critic_targParams")
    f = open("SupplyGameResults/SupplyGameSimulationContinuationResults/wNb_Player1Critic_targ.txt","w")
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



    opt = (optimizers[0].actor)
    f = open("SupplyGameResults/SupplyGameSimulationContinuationResults/mvt_Player0Actor.txt","w")
    for ind in range(6):
        m = opt.state_dict()['state'][ind]['exp_avg']
        sz = m.size()  
        if ind%2 == 0:
            for i in range(sz[0]):
                for j in range(sz[1]):
                    f.write(str((m[i][j]).item()))
                    f.write("\n")
        else:
            for j in range(sz[0]):
                    f.write(str((m[j]).item()))
                    f.write("\n")

    for ind in range(6):
        v = opt.state_dict()['state'][ind]['exp_avg_sq']
        sz = v.size()        
        if ind%2 == 0:
            for i in range(sz[0]):
                for j in range(sz[1]):
                    f.write(str((v[i][j]).item()))
                    f.write("\n")
        else:
            for j in range(sz[0]):
                    f.write(str((v[j]).item()))
                    f.write("\n")
    f.close()


    opt = (optimizers[0].critic)
    f = open("SupplyGameResults/SupplyGameSimulationContinuationResults/mvt_Player0Critic.txt","w")
    for ind in range(6):
        m = opt.state_dict()['state'][ind]['exp_avg']
        sz = m.size()  
        if ind%2 == 0:
            for i in range(sz[0]):
                for j in range(sz[1]):
                    f.write(str((m[i][j]).item()))
                    f.write("\n")
        else:
            for j in range(sz[0]):
                    f.write(str((m[j]).item()))
                    f.write("\n")

    for ind in range(6):
        v = opt.state_dict()['state'][ind]['exp_avg_sq']
        sz = v.size()        
        if ind%2 == 0:
            for i in range(sz[0]):
                for j in range(sz[1]):
                    f.write(str((v[i][j]).item()))
                    f.write("\n")
        else:
            for j in range(sz[0]):
                    f.write(str((v[j]).item()))
                    f.write("\n")
    f.close()


    opt = (optimizers[1].actor)
    f = open("SupplyGameResults/SupplyGameSimulationContinuationResults/mvt_Player1Actor.txt","w")
    for ind in range(6):
        m = opt.state_dict()['state'][ind]['exp_avg']
        sz = m.size()  
        if ind%2 == 0:
            for i in range(sz[0]):
                for j in range(sz[1]):
                    f.write(str((m[i][j]).item()))
                    f.write("\n")
        else:
            for j in range(sz[0]):
                    f.write(str((m[j]).item()))
                    f.write("\n")

    for ind in range(6):
        v = opt.state_dict()['state'][ind]['exp_avg_sq']
        sz = v.size()        
        if ind%2 == 0:
            for i in range(sz[0]):
                for j in range(sz[1]):
                    f.write(str((v[i][j]).item()))
                    f.write("\n")
        else:
            for j in range(sz[0]):
                    f.write(str((v[j]).item()))
                    f.write("\n")
    f.close()


    opt = (optimizers[1].critic)
    f = open("SupplyGameResults/SupplyGameSimulationContinuationResults/mvt_Player1Critic.txt","w")
    for ind in range(6):
        m = opt.state_dict()['state'][ind]['exp_avg']
        sz = m.size()  
        if ind%2 == 0:
            for i in range(sz[0]):
                for j in range(sz[1]):
                    f.write(str((m[i][j]).item()))
                    f.write("\n")
        else:
            for j in range(sz[0]):
                    f.write(str((m[j]).item()))
                    f.write("\n")

    for ind in range(6):
        v = opt.state_dict()['state'][ind]['exp_avg_sq']
        sz = v.size()        
        if ind%2 == 0:
            for i in range(sz[0]):
                for j in range(sz[1]):
                    f.write(str((v[i][j]).item()))
                    f.write("\n")
        else:
            for j in range(sz[0]):
                    f.write(str((v[j]).item()))
                    f.write("\n")
    f.close()


    # opt = (optimizers[0].critic)
    # print(opt.state_dict()['state'][0]['step'])

    # opt = (optimizers[1].actor)
    # print(opt.state_dict()['state'][0]['step'])

    # opt = (optimizers[1].critic)
    # print(opt.state_dict()['state'][0]['step'])



########################################################################################################################

rewards = []
sample_policies = []


s_arr_len = 10
a_arr_len = 4
r_len = 2
next_s_arr_len = 10


f = open("SupplyGameResults/SupplyGameHistory/ReplayBuffer.txt","r")
for i in range(396000):
    s_arr=[]
    a_arr=[]
    r=[]
    next_s_arr=[]
    for i in range(s_arr_len):
        x = float(f.readline())
        s_arr.append(x)
    for i in range(a_arr_len):
        x = float(f.readline())
        a_arr.append(x)
    for i in range(r_len):
        x = float(f.readline())
        r.append(x)
    for i in range(next_s_arr_len):
        x = float(f.readline())
        next_s_arr.append(x)
        
    rewards.append(r)
    sample_policies.append(np.array(a_arr))
    
f.close()


#############################################################
rewards = rewards + rewards2
sample_policies = sample_policies + sample_policies2
################################################################


# print(rewards[396000:])
# print(sample_policies[396000:])
if plot_output: 
    print('plotting outputs')
#     alg_env.plot_policies(chain, rewards[396000:], sample_policies[396000:], savefig=True,
#                           folder = 'supply_game/results')
    if (int(sys.argv[2]))==3:
        alg_env.plot_policies(chain, rewards2, sample_policies2,selling_record2, savefig=True,
                              folder = 'supply_game/results_paper/Randomness2/Shared_CT')
    if (int(sys.argv[2]))==1:
        alg_env.plot_policies(chain, rewards2, sample_policies2,selling_record2, savefig=True,
                              folder = 'supply_game/results_paper/Randomness2/NotShared_CT')
















