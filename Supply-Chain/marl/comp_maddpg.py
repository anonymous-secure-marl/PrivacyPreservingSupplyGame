# -*- coding: utf-8 -*-

from multi_chain_model import SupplyChain
import util as ut 
import torch
import numpy as np
import matplotlib.pyplot as plt


device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

class Distribution:
    # equal: each farmer gets an equal share of the distributor's demand regardless of price
    equal = 1
    # fcfs: the farmer with the cheapest price offer gets the most distributor's demand
    # and if the distributor's demand is unmet after the cheapest farmer saturated 
    # his production capacity then it goes towards the next farmer
    first_come_first_serve = 2
    # probabilistic: each farmer gets a portion of the demand proportional to
    # (1/p_i)/ sum_j (1 / p_j) where p_i is the price charged by player i. 
    probabilistic = 3
    # individual: distributor interacts with each farmer separately 
    individual = 4

    
class Player(object):
    def __init__(self, name=''):
        self.name = name
        self.chain = None
        self.action = None
        self.action_noise = None
        
class World(object):
    def __init__(self, mode=Distribution.individual):
        
        self.mode = mode
        print(f'distribution mode is {self.mode}')
        self.farmers = []
        self.distributors = []
        self.retailers = []
        
    def reset(self):
        player_states = []
        
        for p_ind in range(self.player_num):
            p = self.entities[p_ind]
            p.chain.reset()
            p.action_noise.reset()
            player_states.append(p.chain.state.copy())
        return player_states
    
    # return all entities in the world
    @property 
    def entities(self):
        return self.farmers + self.distributors + self.retailers 
    
    @property
    def player_num(self):
        #TODO: Extend to multi-level MADDPG
        return len(self.entities)
    
    @property
    def farmer_num(self):
        return len(self.farmers)
    
    @property
    def distributor_num(self):
        return len(self.distributors)
    
    @property
    def retailer_num(self):
        return len(self.retailers)
        
    def rewards(self, player, quantity_sold, player_key):
        """ player_key[0]: horizontal position, 
            player_key[1]: vertical position.
        """
        profit =  quantity_sold * player.action[0]
        #TODO: extend to more than 2 levels.
        cost = 0
        if player_key[1] == 0 or (self.farmer_num == 1 and self.distributor_num == 1):
            if player_key[1] == 0:  # farmers
                incoming_price = player.chain.linear_supply(player.action[1])
            else:
                incoming_price = self.entities[player_key[1]-1].action[0]
            cost = player.action[1] * incoming_price
        elif player_key[1] == 2:
            # is distributor
            #TODO: currently only works for 2 tier SC. 
            cost =  sum([self.distributors[i-1].action[0] * player.action[i]
                        for i in range(1, self.distributor_num+1)])
        elif player_key[1] == 1:
            cost = sum([self.farmers[i-1].action[0] * player.action[i] 
                        for i in range(1, self.farmer_num+1)])
            # print(f'-------------- player profit is {profit}')
            # print(f'--------- sold quantity{quantity_sold}')
        reward = player.chain.reward_coeff * (profit - cost)

        
        return reward

    def purchase(self, distributor, demand, supplier):
        print('WARNING should not be here')
        return 0
    
    def divvy_up(self):
        """ Actions are set and previous states are set. Now figure out how 
           much each person sold. """
        # quantity sold set by upstream player
        quantity_sold = [0 for i in range(self.player_num)]
        orig_demand = []
        distributor = self.entities[-1]
        # distributor gets market demand function
        market_demand = distributor.chain.linear_demand(distributor.action[0])
        if market_demand < 0:
            print(f'negative market demand in {market_demand}')
        print('WARNING should not be here')
        
        if self.mode is Distribution.individual:
            # action_sum = sum([distributor.action[i+1] for i in range(farmer_num)])
            for p_ind in range(self.farmer_num):# is a farmer
                demand = distributor.action[p_ind+1] # / action_sum * market_demand
                orig_demand.append(demand)
                # print(f'demand for farmer {p_ind} is {demand}')
                unfulfilled = self.purchase(
                    distributor, demand, self.farmers[p_ind])
                quantity_sold[p_ind] = demand-unfulfilled
                # print(f'farmer {p_ind} sold quantity {quantity_sold[p_ind]}')
                if unfulfilled > 0: 
                    distributor.action[p_ind+1] += -unfulfilled
                    # print(f'actually sold {distributor.action[p_ind + 1]}')
            orig_demand.append(market_demand)
            # for the distributor, penalize if demand isn't met. 
            # print(f'distributor {distributor.chain.state[0]}')
            # print(f'quantity {quantity_sold[-1]}')
            # print(f'market demand {market_demand}')
            market_penalty = 0
            print('WARNING should not be here')
            # if distributor.chain.state[0] < market_demand:
            #     market_penalty = (
            #         distributor.chain.demand_penalty * 
            #         (market_demand - distributor.chain.state[0]))
        elif self.mode is Distribution.equal: 
            unfulfilled = 0
            for p_ind in range(self.farmer_num):# is a farmer
                demand = distributor.action[1] / self.farmer_num + unfulfilled
                unfulfilled = self.purchase(
                    distributor, demand, self.farmers[p_ind])
                quantity_sold[p_ind] = demand - unfulfilled
                # print(f'farmer {p_ind} sold quantity {quantity_sold[p_ind]}')
            if unfulfilled > 0: 
                # print('quantity unmet: {unfulfilled}')
                distributor.action[1] += -unfulfilled
                
        elif self.mode is Distribution.first_come_first_serve:
            #TODO: untested code.
            # buy from cheapest player first
            player_offers = [p.action[0] for p in self.farmers]
            unfulfilled =  distributor.action[1]
            for tries in range(self.farmer_num): # all farmers try to satsify demand
                unfulfilled_prev = unfulfilled
                val, ind = min((val, ind) for (ind, val) in enumerate(
                    player_offers))
                unfulfilled = self.purchase(distributor, unfulfilled, 
                                            self.players[ind])
                quantity_sold[ind] = unfulfilled_prev - unfulfilled
                player_offers[ind] += 99999999
                
                if unfulfilled  == 0 : 
                    break
            if unfulfilled > 0: # after trying all farmers, still unfulfilled orders
                distributor.action[1] += -unfulfilled    

        # if step_ind  == 39:
        #     print(f'market demand {round(quantity_sold[-1],2)} '
        #           f'distributor state {round(distributor.chain.state[0], 2)} '
        #           f'f1 demand {round(quantity_sold[0],2)} '
        #           f'f2 demand {round(quantity_sold[1],2)} '
        #           f'f3 demand {round(quantity_sold[2],2)} '
        #           f'penalty {round(distributor_penalty, 2)}')
        
        return quantity_sold, market_penalty, orig_demand
    
    def threep_divvy(self):
        if self.mode is not Distribution.individual:
            raise AssertionError("Shouldn't be here.")
        quantity_sold = [0 for i in range(self.player_num)]
        orig_demand = []
        #TODO: extend to multiplayers per level
        # retailer gets market demand function
        retailer = self.entities[-1]
        market_demand = retailer.chain.linear_demand(retailer.action[0])
        # print(f'market demand {round(market_demand,2)}')
        # quantity_sold.append(min(market_demand, retailer.chain.state[0]))
        
        for p_ind in range(self.player_num - 1):
            supplier = self.entities[p_ind]
            demander = self.entities[p_ind+1]

            demand = demander.action[1]
            orig_demand.append(demand)
            unfulfilled = self.purchase(demander, demand, supplier)
            quantity_sold[p_ind] = demand - unfulfilled
            if unfulfilled > 0:
                demander.action[1] += -unfulfilled
        orig_demand.append(market_demand)
        # for the retailer, penalize if demand isn't met. 
        # print(f'retailer {retailer.chain.state[0]}')
        # print(f'quantity {quantity_sold[-1]}')
        # print(f'market demand {market_demand}')
        market_penalty = 0
        # if retailer.chain.state[0] < market_demand:
        #     market_penalty = (retailer.chain.demand_penalty * 
        #                       (market_demand - retailer.chain.state[0]))
                
        return quantity_sold, market_penalty, orig_demand
    
    def step(self, get_action, step_ind):
        player = self.entities[0]
        action = get_action[0].actor.get_action(player.chain.state.copy())
        action =  player.action_noise.get_action(action, step_ind)
        player.chain.normalize_actions(action)
        
        incoming_demand = player.chain.linear_demand(action[0])
        incoming_price = [player.chain.linear_supply(action[1])]
        next_state, reward, _ = player.chain.step_no_inventory(action, incoming_price, 
                                      incoming_demand)
        return [next_state.copy()], [action], [reward], 0
    
    def get_and_normalize_actions(self, get_action, step_ind, is_total_states):
        if is_total_states:
            states_list = [p.chain.state.copy() for p in self.entities]
            states_arr = np.concatenate(states_list)
        for p_ind in range(self.player_num):
            player = self.entities[p_ind]  
            if not is_total_states:
                action = get_action[p_ind].actor.get_action(
                    player.chain.state.copy())
            else:
                action = get_action[p_ind].actor.get_action(states_arr)
            player.action = player.action_noise.get_action(action, step_ind)
            player.chain.normalize_actions(player.action) 
            
    def get_player_demands(self):
        demands = [0 for _ in range(self.player_num)]
        for distributor in self.distributors:
            for f_ind in range(self.farmer_num):
                demands[f_ind] += distributor.action[f_ind+1]
            
        for d_ind in range(self.distributor_num):
            distributor = self.distributors[d_ind]
            p_ind = d_ind + self.farmer_num
            d_action = distributor.action
            demands[p_ind] += distributor.chain.linear_demand(d_action[0])
            
        return demands 
    
    def get_player_prices(self):
        prices = [[] for _ in range(self.player_num)]
        p_ind = 0
        for f in self.farmers:
            prices[p_ind].append(f.chain.linear_supply(f.action[1]))
            p_ind +=1
            
        for d in self.distributors:
            for f_ind in range(self.farmer_num):
                prices[p_ind].append(self.farmers[f_ind].action[0])
            p_ind += 1
            
        return prices
        
        
    def step_vhalf(self, get_action,step_ind, total_states=False):

        # step 1: get player actions and normalize them
        self.get_and_normalize_actions(get_action, step_ind, total_states)
        # step 2: get player demands pre-enforcing state constraints
        demands = self.get_player_demands()
        # step 3: get player prices pre-enforcing state constraints
        prices = self.get_player_prices()
        
        # step 4: update forecast states
        observations = []
        rewards = []
        for p_ind in range(self.player_num):
            player = self.entities[p_ind]
            
            bottom_sc = False if p_ind >= self.farmer_num else True
            top_sc = True if p_ind >= self.farmer_num else False
            next_state, rew, unfulfilled = player.chain.step_no_inventory(
                player.action, prices[p_ind], demands[p_ind], 
                dock_price=bottom_sc, dock_demand=(top_sc or bottom_sc))
            observations.append(next_state.copy())
            rewards.append(rew)
            if unfulfilled > 0 and not top_sc:
                for dist in self.distributors:
                    dist.action[1+p_ind] += -unfulfilled / self.distributor_num


        return observations, [p.action for p in self.entities], rewards, 0
    
    def step_v2(self, get_action, step_ind):
        # step 1: get player actions and normalize them
        for p_ind in range(self.player_num):
            player = self.entities[p_ind]
            action = get_action[p_ind].actor.get_action(player.chain.state)
            player.action = player.action_noise.get_action(action, step_ind)
            player.chain.normalize_actions(player.action) 
            
        # step 2: get player demands pre-enforcing state constraints
        demands = [0 for _ in range(self.player_num)]
        for distributor in self.distributors:
            for f_ind in range(self.farmer_num):
                demands[f_ind] += distributor.action[f_ind+1]
            
        for retailer in self.retailers:
            for d_ind in range(self.distributor_num):
                demands[d_ind + self.farmer_num] += retailer.action[d_ind+1]
                
        for p_ind in range(self.player_num - self.retailer_num, self.player_num):
            retailer = self.entities[p_ind]
            r_action = retailer.action
            demands[p_ind] += retailer.chain.linear.demand(r_action[0])
    
        # step 3: get player prices pre-enforcing state constraints
        prices = [[] for _ in range(self.player_num)]
        p_ind = 0
        for f in self.farmers:
            prices[p_ind].append(f.chain.linear_supply(f.action[1]))
            p_ind +=1
            
        for d in self.distributors:
            for f_ind in range(self.farmer_num):
                prices[p_ind].append(self.farmers[f_ind].action[0])
            p_ind += 1
        
        for r in self.retailers:
            for d_ind in range(self.distributor_num):
                prices[p_ind].append(self.distributors[d_ind].action[0])
            p_ind += 1
            
        # step 4: update forecast states
        for p_ind in range(self.player_num):
            player = self.entities[p_ind]
            player.chain.update_forecasts(prices[p_ind], demands[p_ind])
                    
        # step 5: update inventory  + enforce inventory constraints
        observations = []
        rewards = []
        for f_ind in range(self.farmer_num):
            farm = self.farmers[f_ind]
            next_state, rew, unfulfilled = farm.chain.step(
                farm.action, prices[f_ind], demands[f_ind], False)
            
            # farmers discourage price
            observations.append(next_state)
            rewards.append(rew)
            for dist in self.distributors:
                dist.action[1 + f_ind] += -unfulfilled / self.distributor_num
        
        for d_ind in range(self.distributor_num):
            dist = self.distributors[d_ind]
            p_ind = self.farmer_num + d_ind
            next_state, rew, unfulfilled = dist.chain.step(
                dist.action, prices[p_ind],demands[p_ind], False)
            
            # distributors discourage quantity
            observations.append(next_state)
            rewards.append(rew)
            for retailer in self.retailers:
                retailer.action[1 + d_ind] += unfulfilled / self.retailer_num
        
        for r_ind in range(self.retailer_num):
            retailer = self.retailers[r_ind]
            p_ind = self.farmer_num + self.distributor_num + r_ind
            next_state, rew, unfulfilled = retailer.chain.step(
                retailer.action, prices[p_ind], demands[p_ind], False)
            observations.append(next_state)
            rewards.append(rew)
            # if using penalty functions
            market_penalty=0
            if unfulfilled < demands[p_ind] and False:
                market_penalty = retailer.chain.demand_penalty * unfulfilled
                rewards[-1] += market_penalty
                
        return observations, [p.action for p in self.entities], rewards, market_penalty    
                
        # use multi_chain_model.step to get next state& rewards
    # def step(self, get_action, step_ind):
    #     """ This should be called before player update. """
    #     # actions = []
    #     # Step 1:get player actions and normalize them
    #     original_demand = []
    #     for p_ind in range(self.player_num):
    #         player = self.entities[p_ind]
    #         player.action = get_action(player.chain.state, p_ind, step_ind)
    #         player.chain.normalize_actions(player.action)
    #         original_demand.append(player.action.copy())                
    #     # Step 2: players link downstream and up stream actions
    #     # quantity sold set by upstream player
    #     if self.retailer_num == 0:
    #         quantity_sold, market_penalty, orig_demand = self.divvy_up()
    #     else: 
    #         quantity_sold, market_penalty, orig_demand = self.threep_divvy()

        
    #     # step 3: receive rewards
    #     rewards = self.report(quantity_sold, market_penalty)
    #     # print(f'distributor demand penalty = {self.distributors[0].chain.demand_penalty}')
    #     # step 4: update agent inventory
    #     for i, player in enumerate(self.entities):
    #         quantity_bought = [player.action[j+1] for j in 
    #                            range(player.chain.input_prices)]
    #         if i != self.player_num - 1:
    #             price_bought = [player.chain.linear_supply(player.action[1])]
    #         else:
    #             price_bought = [self.entities[j].action[0] for j in
    #                             range(self.farmer_num)]
        
    #         # print(f'updating inventory for player {i}: price {price_bought}')
    #         player.chain.update_inventory(quantity_bought, quantity_sold[i],  
    #                                       price_bought, orig_demand[i])
    #         # if step_ind == 39 and i == len(self.entities) - 1:
    #         #     print(f'distributor state {round(distributor.chain.state[0], 2)} ')
    #         # print(f'player {i} inventory {player.chain.state}')
    #     observations = [p.chain.state for p in self.entities]
    #     return observations, [p.action for p in self.entities], rewards, market_penalty
    
    def report(self, quantity_sold, market_penalty):
        rewards = []
        g_ind = 0
        for farmer in self.farmers:
            reward = self.rewards(farmer, quantity_sold[g_ind], (g_ind, 0))
            rewards.append(reward)
            g_ind +=1
        # print(f'penalty is {market_penalty}')
        for distributor in self.distributors:
            reward = self.rewards(distributor, quantity_sold[g_ind], 
                                  (g_ind - self.farmer_num, 1))
            rewards.append(reward)
            g_ind +=1
        
        for retailer in self.retailers:
            reward = self.rewards(retailer, quantity_sold[g_ind], 
                                  (g_ind - self.farmer_num - self.distributor_num, 2))
            rewards.append(reward)
        
        last_player = self.entities[-1]
        rewards[-1] += -market_penalty * last_player.chain.reward_coeff
        return rewards
    



    
def make_3p_world(farmer_lim, distributor_lim, retailer_lim):
    world = World()
    world.num_players = 3
    farmer = Player('farmer_0')
    farmer.chain = SupplyChain(farmer_lim[0],farmer_lim[1])
    farmer.action_noise = ut.OUNoise(farmer.chain.action_space)
    world.farmers.append(farmer)
    
    distributor = Player('distributor_0')
    distributor.chain = SupplyChain(distributor_lim[0], distributor_lim[1], 1)
    distributor.action_noise = ut.OUNoise(distributor.chain.action_space)
    world.distributors.append(distributor)
    
    retailer = Player('retailer_0')
    retailer.chain = SupplyChain(retailer_lim[0], retailer_lim[1], 1)
    retailer.action_noise = ut.OUNoise(retailer.chain.action_space)
    world.retailers.append(retailer)
    
    
    # restart all players
    world.reset()
    
    return world


def execute(ac_nns, world, device, Iterations=200, steps_per_eps=40):
    total_rewards = np.zeros((steps_per_eps, Iterations))
    agent_rewards = np.zeros((world.player_num, steps_per_eps, Iterations))
    agent_actions = []
    for p_ind in range(world.player_num):
        agent_actions.append(np.zeros((
            len(world.entities[p_ind].chain.action_space.low), 
            steps_per_eps, Iterations)))

    quantity_sold = np.zeros((steps_per_eps, Iterations))
    demand = np.zeros((steps_per_eps, Iterations))
    

    for it in range(Iterations):
        observations = world.reset()
        for ep_ind in range(steps_per_eps):
            get_actions = []
            for p_ind in range(world.player_num):
                def get_action(state, ind):
                    # print(f'player {ind} state = {state}')
                    obs_tensor = torch.from_numpy(state).to(device, torch.float)
                    # print(f'player {ind} state = {obs_tensor}')
                    p_act = ac_nns[ind].actor(obs_tensor).detach().cpu().numpy()
                    return world.entities[ind].action_noise.get_action(p_act, ep_ind)
                get_actions.append(get_action)
                # print(f'player {p_ind} action {world.entities[p_ind].action}')
        
            # interact with env
            observations_new, actions, rews, demand_step = world.step(
                get_actions, ep_ind)
            
            total_rewards[ep_ind, it] = np.sum(rews)
            for i, action in enumerate(actions): 
                agent_actions[i][:, ep_ind, it] = action
                
            for i, rew in enumerate(rews): 
                agent_rewards[i, ep_ind, it] = rew 
                
            quantity_sold_step = observations[-1][0] - observations_new[-1][0]
            for farmer_ind in range(1, len(actions[-1])): 
                # distributor's last action is quantity bought per farmer
                quantity_sold_step += actions[-1][farmer_ind]
            quantity_sold[ep_ind, it] = (quantity_sold_step)
            demand[ep_ind, it]= demand_step
    
    plt.figure()
    plt.title('agent average rewards')
    for i in range(len(agent_rewards)):
        plt.plot([np.mean(agent_rewards[i, ep_ind, :]) 
                  for ep_ind in range(steps_per_eps)], label=world.entities[i].chain.name)
    plt.grid()
    plt.legend()
    plt.show()
    
    i = 0
    for p_i in agent_actions:
        plt.figure()
        plt.title(f'agent {i} actions')
        labels = ['price charged', 'quantity bought']
        if i == len(agent_actions)-1:
            labels = ['price_charged', 'f0 quantity bought', 'f1 quantity bought', 
                      'f2 quantity bought']
        [plt.plot([np.mean(p_i[j, ep_ind, :]) for ep_ind in range(steps_per_eps)], 
                 label = labels[j], alpha = 0.5) for j in range(len(p_i))]
        plt.grid()
        plt.legend()
        plt.show()
        i +=1
