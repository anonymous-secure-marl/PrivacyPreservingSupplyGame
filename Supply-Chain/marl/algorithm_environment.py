# -*- coding: utf-8 -*-
import numpy as np
import matplotlib
import matplotlib.pyplot as plt


from multi_chain_model import SupplyChain
import util as ut
from comp_maddpg import World, Player
import time 

SMALL_SIZE = 20
MEDIUM_SIZE = 22
BIGGER_SIZE = 28

plt.rc('font', size=MEDIUM_SIZE)          # controls default text sizes
plt.rc('axes', titlesize=MEDIUM_SIZE)     # fontsize of the axes title
plt.rc('axes', labelsize=MEDIUM_SIZE)    # fontsize of the x and y labels
plt.rc('xtick', labelsize=MEDIUM_SIZE)    # fontsize of the tick labels
plt.rc('ytick', labelsize=MEDIUM_SIZE)    # fontsize of the tick labels
plt.rc('legend', fontsize=MEDIUM_SIZE)    # legend fontsize
plt.rc('figure', titlesize=BIGGER_SIZE)  # fontsize of the figure title


    
# def plot_policies(world, rewards, sample_policies, smooth_interval=200, 
#                   savefig=False, folder=None):
# def plot_policies(world, rewards, sample_policies, selling_record, stock_level, smooth_interval=200, 
#                   savefig=False, folder=None):
def plot_policies(world, rewards, sample_policies, selling_record, smooth_interval=200, 
                  savefig=False, folder=None):
    #smooth_interval = 200
    rewards_arr = np.array(rewards)
    rewards_avg = []
#     horizontal_line = 0
    #1000 - smooth_interval
#     print("Cumulative Differnce in Rewards: " + str(sum([abs(rewards_arr[i][0]-rewards_arr[i][1]) for i in range(len(rewards))])))
    
    for j in range(world.player_num):
        rewards_avg.append([np.mean(rewards_arr[i:i+smooth_interval, j]) 
                                    for i in range(len(rewards)-smooth_interval)])
    if world.player_num > 1:
        rewards_sum = []
        for ind in range(len(rewards_avg[0])):
            rewards_sum.append(sum([
                rewards_avg[j][ind] for j in range(world.player_num)]))
            
            
    plt.figure(figsize=(10,5))
    if world.player_num > 1:
        plt.plot(rewards_sum, label='total rewards')
    for j in range(world.player_num):
        plt.plot(rewards_avg[j], label=f'rewards {j}')
    plt.legend() 
    plt.grid()
#     plt.title('Moving Average of Rewards: Individual Players\' + Total')
    plt.tight_layout()
    ###############################
    plt.ylim([-8,12])
    plt.xlabel("t")
    plt.ylabel("reward")
    ###############################
    if savefig:
#         plt.savefig(f'{folder}/rewards_{time.time()}.png')
        plt.savefig(f'{folder}/rewards.pdf',format="pdf", bbox_inches="tight")
       
        
    
        
    if hasattr(world, 'entities'):
        action_shapes = [p.chain.action_space.low.shape[0] 
                         for p in world.entities]
    else:
        action_shapes = [p.action_space.low.shape[0] for p in world.players]
        
    action_s = ut.concatenate_spaces(action_shapes) 
    policies_arr = np.squeeze(np.array(sample_policies))
    policies = [[] for _ in range(world.player_num)]
    for p_ind in range(world.player_num):
        for a_ind in range(action_s[p_ind][0], action_s[p_ind][1]):
            policies[p_ind].append([np.mean(policies_arr[i:i+smooth_interval, a_ind]) 
                            for i in range(len(sample_policies)-smooth_interval)])
    
    ###############################################################################
    sale = []
    selling_record_arr = np.array(selling_record)
    for p_ind in range(world.player_num):
        sale.append([np.mean(selling_record_arr[i:i+smooth_interval, p_ind]) 
                            for i in range(len(sample_policies)-smooth_interval)])
#     stock = []
#     stock_level_arr = np.array(stock_level)
#     for p_ind in range(world.player_num):
#         stock.append([np.mean(stock_level_arr[i:i+smooth_interval, p_ind]) 
#                             for i in range(len(sample_policies)-smooth_interval)])
    consumer_demand_arr = [10 - 2*policies_arr[i][3] for i in range(len(sample_policies))]
    consumer_demand = [np.mean(consumer_demand_arr[i:i+smooth_interval]) 
                            for i in range(len(sample_policies)-smooth_interval)]
    wastage_arr = [(policies_arr[i][0] - selling_record[i][1]) for i in range(len(sample_policies))]
    wastage = [np.mean(wastage_arr[i:i+smooth_interval]) 
                            for i in range(len(sample_policies)-smooth_interval)]
    ################################################################################
        
    plt.figure(figsize=(10,5))
#     plt.title('Policy during training')
    for p_ind in range(world.player_num):
        for ind in range(world.players[p_ind].in_num):
#             plt.plot(policies[p_ind][ind], label =f'demand by {p_ind}: {ind}')
            if p_ind==0:
                plt.plot(policies[p_ind][ind], label =f'demand by 0')
            if p_ind==1:
                plt.plot(policies[p_ind][ind], label =f'demand by 1')
    #########################################################
#     plt.plot(sale[0], label =f'sold by 0 to 1')
#     plt.plot(sale[1], label =f'sold by 1 to consumer market')
#     plt.plot(stock[0], label =f'stock of 0')
#     plt.plot(stock[1], label =f'stock of 1')
#     plt.plot(consumer_demand, label =f'consumer_demand')
    plt.plot(wastage, label =f'wastage')
    #########################################################
#     plt.title('Moving Average of Players\' Quantity Policies')
    plt.tight_layout()
    plt.legend() 
    plt.grid()
    ###############################
    plt.ylim([0,7])
    plt.xlabel("t")
    plt.ylabel("quantity")
    ###############################
    if savefig:
#         plt.savefig(f'{folder}/quantity_policy_{time.time()}.png') 
        plt.savefig(f'{folder}/quantity.pdf',format="pdf", bbox_inches="tight") 
        
        
        
        
        
        
    plt.figure(figsize=(10,5))
#     plt.title('Policy during training')
    for p_ind in range(world.player_num):        
        for ind in range(world.players[p_ind].in_num, len(policies[p_ind])):
#             plt.plot(policies[p_ind][ind], label =f'price {p_ind}: {ind}')
            if p_ind==0:
                plt.plot(policies[p_ind][ind], label =f'price charged by 0')
            if p_ind==1:
                plt.plot(policies[p_ind][ind], label =f'price charged by 1')
#     plt.title('Moving Average of Players\' Price Policies')
    plt.tight_layout()
    plt.legend() 
    plt.grid()
    ###############################
    plt.ylim([0,5])
    plt.xlabel("t")
    plt.ylabel("price")
    ###############################
    if savefig:
#         plt.savefig(f'{folder}/price_policy_{time.time()}.png') 
        plt.savefig(f'{folder}/price.pdf',format="pdf", bbox_inches="tight") 
        
        
        
        
        
        
        
    ##########################################################################
    f = open("Reward_Quantity_results_" + str(int(len(rewards)/40)) + "ep.txt", "w")
    f.write("For total " + str(int(len(rewards)/40)) + " epochs: \n")
    
    R0 = sum([rewards_arr[i][0] for i in range(len(rewards))])/(len(rewards))
    R1 = sum([rewards_arr[i][1] for i in range(len(rewards))])/(len(rewards))
    f.write("Average Reward for Player 0: " + str(round(R0,4)) + "\n")
    f.write("Average Reward for Player 1: " + str(round(R1,4)) + "\n")
    f.write("Average Total Reward: " + str(round(R0+R1,4)) + "\n")
    f.write("Average Difference in Reward: " + str(round(abs(R0-R1),4)) + "\n")
    
    
    D0 = sum([policies_arr[i][0] for i in range(len(sample_policies))])/(len(rewards))
    D1 = sum([policies_arr[i][2] for i in range(len(sample_policies))])/(len(rewards))
    S0 = sum([selling_record[i][0] for i in range(len(selling_record))])/(len(rewards))
    S1 = sum([selling_record[i][1] for i in range(len(selling_record))])/(len(rewards))
    D_consumer = sum([10 - 2*policies_arr[i][3] for i in range(len(sample_policies))])/(len(rewards))
    f.write("Average Demand by Player 0: " + str(round(D0,4)) + "\n")
    f.write("Average Demand by Player 1: " + str(round(D1,4)) + "\n")
    f.write("Average Demand by Consumer: " + str(round(D_consumer,4)) + "\n")
    f.write("Average Sale by Player 0: " + str(round(S0,4)) + "\n")
    f.write("Average Sale by Player 1: " + str(round(S1,4)) + "\n")
    f.write("Average Wastage: " + str(round(abs(D0-S1),4)) + "\n")
    
    f.write("############\n")
    
    f.close()
    ##########################################################################
        
def plot_evaluation(p_ind, orig_policy, eval_policy, rewards, eval_rewards, 
                    smooth_interval=200):
    policies_arr = np.squeeze(np.array(orig_policy))
    policies = [] 
    for a_ind in range(len(orig_policy[0])):
        policies.append([np.mean(policies_arr[i:i+smooth_interval, a_ind]) 
                        for i in range(len(orig_policy)-smooth_interval)])
        
    checked_arr = np.squeeze(np.array(eval_policy))
    new_policies = []
    for a_ind in range(len(eval_policy[0])):
        new_policies.append([np.mean(checked_arr[i:i+smooth_interval, a_ind]) 
                        for i in range(len(eval_policy)-smooth_interval)])
    plt.figure(figsize=(10,5))
    plt.plot(policies[0], label='orig_price')
    for ind in range(1, len(new_policies)):
        plt.plot(policies[ind], label=f'orig_quantity {ind}')
    plt.plot(new_policies[0], label='new_price')
    for ind in range(1, len(new_policies)):
        plt.plot(new_policies[ind], label=f'new_price {ind}')
    plt.legend() 
    plt.grid()
    plt.show()  
    
    rewards_arr = np.array(rewards)
    checked_rew_arr = np.array(eval_rewards)
    rewards_avg = [np.mean(rewards_arr[i:i+smooth_interval, p_ind]) 
                   for i in range(len(rewards)-smooth_interval)]
    checked_avg =  [np.mean(checked_rew_arr[i:i+smooth_interval, p_ind]) 
                   for i in range(len(eval_rewards)-smooth_interval)]
    
    # compare reward
    plt.figure()
    plt.plot(rewards_avg, label='orig_reward')
    plt.plot(checked_avg, label='new_reward')
    plt.legend() 
    plt.grid()
    plt.show()
    
    
def make_world(num_farmers, farmer_lim, distributor_lim):
    world = World()
    world.num_players = num_farmers + 1
    # initialize farmers
    for p in range(num_farmers):
        farmer = Player(f'farmer_{p}')
        print(f'initializing farmer {p}')
        farmer.chain = SupplyChain(farmer_lim[0], farmer_lim[1])
        farmer.action_noise = ut.OUNoise(farmer.chain.action_space)
        world.farmers.append(farmer)
    # initialize distributor
    distributor = Player('distributor_0')
    print('initiating distributor')
    distributor.chain = SupplyChain(
            distributor_lim[0], distributor_lim[1], num_farmers)
    distributor.action_noise = ut.OUNoise(distributor.chain.action_space)
    world.distributors.append(distributor)
   
    return world


    
    
    