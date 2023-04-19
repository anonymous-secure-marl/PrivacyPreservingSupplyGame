# -*- coding: utf-8 -*-

import numpy as np
import supply_game.player as player
import networkx as nx
import random
import torch
import util as ut
import actor_critic.actor_critic_updates as ac_update
import actor_critic.actor_critic_config as ac_fig
# import matplotlib.pyplot as plt

##################################
torch.manual_seed(0)
np.random.seed(0)
random.seed(0)
##################################

class World():
    def __init__(self, players, default_graph=None, theta=0.3, min_sigma=0.15):
        """ Should generate a random directed tree graph, but for now, 
            stick with a handcrafted tree graph. 
        """
        self.graph = default_graph    
        if default_graph is None:
            self.construct_random_graph(players)
        self.players = []
        self.noises = []
        self.player_num = self.graph.number_of_nodes()
        for i in range(self.player_num):
            # need to expand player definition to have multiiple costs and demands
            # print(f'player {i}, in {self.graph.in_degree(i)}, out {self.graph.out_degree(i)}')
            self.players.append(player.Player(
                self.graph.in_degree(i), self.graph.out_degree(i)))
            self.noises.append(ut.OUNoise(
                self.players[-1].action_space, theta=theta, min_sigma=min_sigma))

        
    def construct_random_graph(self, players):
        self.graph = nx.DiGraph()
        self.graph.add_nodes_from([i for i in range(players)])
        threshold = 1
        parents = [0]
        while players > threshold:
            next_thres = 2**(int(threshold)) + threshold
            next_players = [i for i in 
                            range(threshold, min(players, next_thres))]
            # print(next_players, )
            for p in next_players:
                parent = random.choice(parents)
                self.graph.add_edge(parent, p)
            # randomly select some players to have more than one parent 
            # print(f'sampled {int(len(next_players)/2)}')
            for p in random.sample(next_players, int(len(next_players)/2)):
                parent = random.choice(parents)
                self.graph.add_edge(parent, p)
            threshold = next_thres
            parents = next_players
            
    def print_graph(self):
        nx.draw(self.graph,with_labels=True)
        
    # def reset(self,f):
    #     states = []
    #     [states.append(p.reset(f)) for p in self.players]
    #     return states 
    def reset(self,f,ff):
        states = []
        [states.append(p.reset(f,ff)) for p in self.players]
        return states 
    
    def order_fulfillment(self, demand, prod_sold):
        fulfillment = []
        for p in range(self.player_num):
            total_demand = sum([d for d in demand[:, p] if d >= 0])
            total_sell = sum(prod_sold[p])
            fulfillment.append(total_demand - total_sell)
        return fulfillment
                
    # def update(self, g, policies, retail_market, raw_mat_cost, step, 
    #            ac_config=None):
    def update(self, g, gg, policies, retail_market, raw_mat_cost, step, 
               ac_config=None):
        ###########################################
        print("#######################################################################")
        print("update starts")
        ###########################################
        current_states = []
        next_states = []
        rewards = []
        actions = []
        price = -1* np.ones((self.player_num, self.player_num))
        demand = -1* np.ones((self.player_num, self.player_num))
        available_supply = - 1 * np.ones((self.player_num, self.player_num))

        # p_{ij} = price charged by player i to player j
        # q_{ij} = demand demanded by player i to player j
        # s_{ij} = supply passed from player i to player j
        # price[0, 0] = raw_mat_cost # constant raw material price seen by player 0
        for p_ind in range(self.player_num):
            # print(f'on player {p_ind}')
            p = self.players[p_ind]
            
            # players with no supplier are chain suppliers
            if self.graph.in_degree(p_ind) == 0:
                price[p_ind, p_ind] = raw_mat_cost
            # update player costs based on previous p's suppliers
            price_i = price[:, p_ind]
            cost_i = price_i[price_i >= 0]
            # print(f' player supply cost {p.obs}')
            
            p.step_1(cost_i) # update player price state
            
            # print(f' updated supply cost {p.obs}')
            current_states.append(p.obs.copy())
           
        
        
            # get and normalize actions
            # print(f'player {p_ind}')
            # print(f' has actor attribute {hasattr(policies, "actor")}')
            if hasattr(policies, 'actor'):
                states = np.concatenate([p.obs.copy() for p in self.players])
                p_state = ac_update.transform_state(
                    states, ac_config.actor_mode, ac_config.obs_inds,
                    ac_config.slice_graph[p_ind])
                p_action = policies.actor.get_action(p_state).detach().cpu()
                p_action = torch.squeeze(p_action)
                # print(f' player action right after actor {p_action}')
                p_action = p_action[ac_config.a_inds[p_ind]]
                # print(f' player action right after actor {p_action}')
                
            elif hasattr(policies[p_ind], 'actor'):
                print("state " + str(p_ind) + "\n")
                s = self.players[p_ind].obs.copy()
                print(s)
                
                # print(f'state: {p.obs}')
                if ac_config is not None and ac_config.actor_mode is not ac_fig.Mode.INDIVIDUAL:
                    states = np.concatenate([p.obs.copy() for p in self.players])
                    p_state = ac_update.transform_state(
                        states, ac_config.actor_mode, ac_config.obs_inds, 
                        ac_config.slice_graph[p_ind])
                else:
                    p_state = p.obs.copy()
                # print(f'player {p_ind} p_state {p_state.shape}')
                p_action = policies[p_ind].actor.get_action(
                    p_state).detach().cpu().numpy()
                ####################################
#                 print("p_state")
#                 print(p_state)
                ####################################
            else:
                p_action = policies[p_ind](p.obs)
            # print(f' player action {p_action}')
            # print(f' action space {p.action_space}')
            
            
            
            p_action = self.noises[p_ind].get_action(g,p_action,step)
#             p_action = self.noises[p_ind].get_action(g,gg,p_action, step)
            
#             ###################################
#             print("Before Normalization")
#             print(p_action)
#             ###################################
            
            p_action = np.squeeze(p.normalize_actions(p_action))
            
            ####################################
#             print("After Normalization")
            print("action " + str(p_ind) + "\n")
            print(p_action)
            ####################################
            
            # print(f' player action {p_action}')
            # print(f' player in number {p.in_num}')
            actions.append(p_action) # these should be lists?
            # p_action = p_action.numpy()
            order = p_action[:p.in_num]
            cost = p_action[p.in_num:]
            if self.graph.in_degree(p_ind) == 0:
                # root gets all his orders fulfilled
                # print(f' available_supply.shape {available_supply.shape} ')
                # print(f' order {order.shape}')
                available_supply[p_ind, p_ind] = sum(order) 
            else:
                next_supplier = 0
                for s_ind in self.graph.predecessors(p_ind):
                    # supplier = self.players[s_ind]
                    demand[p_ind, s_ind] = order[next_supplier]
                    next_supplier +=1
            if self.graph.out_degree(p_ind) == 0: # market facing retailer
                demand[p_ind, p_ind] = retail_market(cost[0])
            else:
                next_retailer = 0
                for r_ind in self.graph.successors(p_ind):
                    
                    price[p_ind, r_ind] = cost[next_retailer]
                    next_retailer += 1
                    
#             print("price\n")
#             print(price)
            
#             print("demand\n")
#             print(demand)
            
#             print("available_supply\n")
#             print(available_supply)
                
                
        selling_record = []
        
        
        
        for p_ind in range(self.player_num):
            demands = [d for d in demand[:, p_ind] if d >= 0]
            supplies = [s for s in available_supply[:, p_ind] if s >= 0]
            # print(f'supplies {supplies}')
            prod_price = actions[p_ind][self.players[p_ind].in_num:]
            next_state, reward, prod_sold = self.players[p_ind].step_2(
                demands, supplies, prod_price)
            selling_record.append(prod_sold)
            if self.graph.out_degree(p_ind) != 0: # not retail market facing
                next_retailer = 0
                for r_ind in self.graph.successors(p_ind):
                    available_supply[p_ind, r_ind] = prod_sold[next_retailer]
                    next_retailer += 1
                    
            ###########################################
#             print("available_supply\n")
#             print(available_supply)
            ###########################################
            next_states.append(next_state)
            rewards.append(reward)
            
        fulfillment = self.order_fulfillment(demand, selling_record)
        stock_level = [p.stored_products() for p in self.players]
        # print(f' rewards after update  {rewards}')
        
        
        
        ###########################################
        print("update ends")
        print("#######################################################################")
        ###########################################
        
#         return current_states, next_states, rewards, actions, fulfillment, stock_level
        return current_states, next_states, rewards, actions, fulfillment, stock_level, selling_record
    
# supply_chain = World(7)
# supply_chain.print_graph()
# plt.show()
# def policy_gen(in_degree, out_degree):
#     if in_degree == 0:
#         in_degree = 1
#     if out_degree == 0:
#         out_degree = 1
#     return lambda x: np.concatenate((np.ones(in_degree), np.ones(out_degree)))
# def retail_market(price):
#     return max(20 - 0.2 * price, 0)

# policies = [policy_gen(supply_chain.graph.in_degree(p), 
#                        supply_chain.graph.out_degree(p)) 
#             for p in range(supply_chain.player_num)]



# # empty run of simulation to see results add up 
# epoch = 0
# replay_buffer_size = 1000000
# replay_buffer = ut.ReplayBuffer(replay_buffer_size)
# raw_mat_cost = 0.1
# while epoch < 10:
#     states  = supply_chain.reset()
#     episode_reward = [0 for p in supply_chain.players]
#     for step in range(40):
#         next_states, rewards, actions = supply_chain.update(
#             policies, retail_market, raw_mat_cost)
        
#         replay_buffer.push(states, actions, rewards, next_states)
#         # training happens here
        
#         states = next_states.copy()
#         for p_ind in range(supply_chain.player_num):
#             episode_reward[p_ind] += rewards[p_ind]
#     print(f' episode reward is {episode_reward}')
#     epoch += 1









