# -*- coding: utf-8 -*-

from collections import namedtuple
from enum import Enum

actor_critic_parameters = namedtuple('actor_critic_parameters', [
    'actor_mode', 'critic_state_mode', 'critic_action_mode', 'obs_inds',
    'a_inds', 'slice_graph'])

# set p_inds = None for single player RL
class Mode(Enum):
    INDIVIDUAL=1
    NEIGHBOR=2
    TOTAL=3
    UNIT = 4
def get_state_act_dims(players):
    state_dims = [p.observation_space.shape[0] for p in players]
    action_dims = [p.action_space.low.shape[0] for p in players]
    return state_dims, action_dims

def actor_critic_dims(ac_fig, players, graph=None):
    """Generate the actor/critic state dimensions for one player. """
    state_dims, action_dims = get_state_act_dims(players)
    maps = [state_dims, state_dims, action_dims]
    output = [] # actor_input_dims, critic state_dims, critic action_dims
    p_num = len(players)
    for c_ind in range(3): 
        # corresponds to the first three values of ac_fig
        # should be actor critic size configs
        print(f'ac_fig mode {ac_fig[c_ind]} for {c_ind}')
        dims = []
        if ac_fig[c_ind] is Mode.INDIVIDUAL:
            dims = [maps[c_ind][p] for p in range(p_num)]
            # dims = {p: maps[c_ind][p] for p in players}
        elif ac_fig[c_ind] is Mode.NEIGHBOR:
            # include both predecessor and successor neigbhors
            if graph is None:
                print('actor critic config needs a graph')
            for p in range(p_num):
                dims.append(maps[c_ind][p])
                for j in graph.successors(p): dims[-1] += maps[c_ind][j]
                for j in graph.predecessors(p):  dims[-1] += maps[c_ind][j]                    
        elif ac_fig[c_ind] is Mode.TOTAL:
            dims = [sum(maps[c_ind]) for _ in players]
        elif ac_fig[c_ind] is Mode.UNIT:
            dims = [sum(maps[c_ind])]
            
        output.append(dims)
    actor_input_dims = output[0]
    if ac_fig.actor_mode is Mode.UNIT:
        actor_output_dims = [sum([action_dims[p]for p in range(p_num)])]
    else:      
        actor_output_dims = [action_dims[p] for p in range(p_num)]
    critic_state_dims = output[1]
    critic_action_dims = output[2]
    
    return actor_input_dims, actor_output_dims, critic_state_dims, critic_action_dims


