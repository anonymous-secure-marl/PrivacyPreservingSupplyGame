# -*- coding: utf-8 -*-

import subprocess
import torch
import random
import time
import numpy as np
import actor_critic.actor_critic_config as conf
import actor_critic.actor_critic_networks as ac_nets

use_cuda = torch.cuda.is_available()
device   = torch.device("cuda" if use_cuda else "cpu")

##################################
torch.manual_seed(0)
np.random.seed(0)
random.seed(0)
##################################


def transform_action(actions, action_mode, a_inds):
    act = actions if action_mode is conf.Mode.TOTAL else actions[:,a_inds]
    if action_mode is conf.Mode.UNIT:
        act = actions
    return act

def transform_state(states, state_mode, obs_inds, slices):
    if states.ndim <= 1:
        states = np.expand_dims(states, 0)
    if state_mode is conf.Mode.TOTAL:
        # print(f'here total')
        state = states
    elif state_mode is conf.Mode.NEIGHBOR:
        # print(f'here neighbor')
        state = []
        for s_ind in slices: 
            # print(f'state shape {states.shape}')
            state.append(states[:, s_ind])
            # print(f'state shape {states[s_ind].shape}')
        if type(states) is torch.Tensor:
            state = torch.cat(state, 1)  
        else:
            state = np.concatenate(state, axis=1)
        # print(f'state shape is {state.shape}')
    elif state_mode is conf.Mode.UNIT:
        state = states
    else: #state_mode is conf.Mode.INDIVIDUAL or NONE
        # print(f'here individual, \n obs_inds is {obs_inds}')
        state = states if obs_inds is None else states[:, obs_inds]

    return state

def construct_next_actions(models, next_states, critic_action_mode, 
                           actor_state_mode, p=None, obs_inds=None, 
                           p_graph=None, slice_graph=None):
    next_actions = []
    if critic_action_mode is (conf.Mode.TOTAL or conf.Mode.NEIGHBOR ):
        Iterator = (range(len(models)) if critic_action_mode is conf.Mode.TOTAL 
                    else p_graph[p])
        for key in Iterator: 
            n_slices = (None if actor_state_mode is not conf.Mode.NEIGHBOR 
                        else slice_graph[key])
            s_inp = transform_state(next_states, actor_state_mode, 
                                    obs_inds[key], n_slices)
            next_actions.append(models[key].actor_targ(s_inp))
        next_actions = torch.cat(next_actions, dim=1).detach()
    elif actor_state_mode is conf.Mode.UNIT or critic_action_mode is conf.Mode.UNIT:
        s_inp = transform_state(next_states, actor_state_mode, None, None)
        next_actions = models.actor_targ(s_inp).detach()
    elif p is not None:# critic_action_mode is INDIVIDUAL
        # print(f' in construct action 2')
        # print(f'states shape {next_states.shape}')
        n_slices = (None if actor_state_mode is not conf.Mode.NEIGHBOR 
                    else slice_graph[p])
        s_inp = transform_state(next_states, actor_state_mode, obs_inds[p], n_slices)
        # print(f' models[p] {models[p]}')
        next_actions =  models[p].actor_targ(s_inp).detach() 
        # 
    else: # no p_ind single player scenario
        # print(f' in construct action 3')
        next_actions =  models.actor_targ(next_states).detach() 
    # print(f'action shape {next_actions.shape}')
    return next_actions

def update_target(SampleBatch, models, optimizers, replay_buffer, batch_size,
               obs_inds=None, a_inds=None, r_inds=None, p_ind = None, 
               actor_state_mode=None, critic_action_mode = None, 
               critic_state_mode=None, p_graph=None, slice_graph=None,
               gamma=0.99):
# def update_target(SampleBatch, SampleBatch_c, models, optimizers, replay_buffer, batch_size,
#                obs_inds=None, a_inds=None, r_inds=None, p_ind = None, 
#                actor_state_mode=None, critic_action_mode = None, 
#                critic_state_mode=None, p_graph=None, slice_graph=None,
#                gamma=0.99):
    if hasattr(models, 'actor'):
        model = models 
    else:
        model = models[p_ind]
    p_slices = None if slice_graph is None or p_ind is None else slice_graph[p_ind]
    p_obs_ind = None if p_ind is None else obs_inds[p_ind]
    
    states, actions, rewards, next_states = replay_buffer.sample(SampleBatch, batch_size)
#     states, actions, rewards, next_states = replay_buffer.sample(SampleBatch, SampleBatch_c, batch_size)
    
    states      = torch.FloatTensor(states).to(device)
    next_states = torch.FloatTensor(next_states).to(device)
    actions     = torch.FloatTensor(actions).to(device)
    rewards     = torch.FloatTensor(rewards).to(device)    
    if actor_state_mode is conf.Mode.UNIT:
        reward = torch.sum(rewards, dim=1)
    else:
        reward = rewards if p_ind is None else rewards[:, r_inds]
    
    actor_inp = transform_state(states, actor_state_mode, p_obs_ind, p_slices)
    
    opt_action, actor_weights = model.actor(actor_inp, get_model=True) 
    ############################
    actor_weights.retain_grad()
    ############################
    
    opt_actions = actions.clone()
    if p_ind is None or actor_state_mode is conf.Mode.UNIT: # single RL scenario
        opt_actions = opt_action 
        policy_loss = -(model.critic(states, opt_actions)).mean()
    else: 
        opt_actions[:, a_inds] = opt_action
#         #################################
#         opt_actions.retain_grad()
        opt_action.retain_grad()
#         #################################
        critic_state = transform_state(states, critic_state_mode, p_obs_ind, 
                                       p_slices) 
        critic_actions = transform_action(opt_actions, critic_action_mode, 
                                          a_inds)
        
        policy_loss = -(model.critic(critic_state, critic_actions)).mean()
#         ##############################################################
#         policy_loss, xx = ((model.critic).forward2(states, opt_actions))
#         policy_loss = (-policy_loss).mean()
#         ##############################################################
        
        
    loss_pse = torch.mean(torch.pow(actor_weights, 2))
   
    next_actions = construct_next_actions(models, next_states, 
                                          critic_action_mode, actor_state_mode, 
                                          p=p_ind, obs_inds=obs_inds, 
                                          p_graph=p_graph, 
                                          slice_graph=slice_graph)
    
    if p_ind is None:
        critic_next_state = next_states
    else:
        critic_next_state = transform_state(next_states, critic_state_mode, 
                                            p_obs_ind, p_slices) # 
    target_value = torch.squeeze(model.critic_targ(critic_next_state, next_actions))
    ###############################################################################
#     print(str(p_ind) + " CriticTarget $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$")
#     print(target_value)
    ###############################################################################
    expected_value = reward + gamma * target_value
    
    
    ###############################################################################
#     print(str(p_ind) + " CriticReward $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$")
#     print(reward)
    ###############################################################################
    
    
    ####################################
#     print(str(p_ind) + " Critic_next_states_next_actions $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$")
# #     torch.set_printoptions(profile="full")
#     print(torch.cat([critic_next_state, next_actions],1))
# #     torch.set_printoptions(profile="default")
      
#     print(str(p_ind) + " CriticTargetExpectedValue $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$")
#     print(expected_value)
    ####################################
    
    if p_ind is None: # single RL scenario
        value = torch.squeeze(model.critic(states, actions))
    else:
        critic_state = transform_state(states, critic_state_mode, p_obs_ind, 
                                       p_slices)
        critic_actions = transform_action(actions, critic_action_mode, a_inds)

        value = torch.squeeze(model.critic(critic_state, critic_actions))
        ##########################
#         value.retain_grad()
#         print(str(p_ind) + " CriticForwardValue  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$")
#         print(value)
        ##########################

    value_loss =  torch.nn.MSELoss()(value, expected_value.detach())
    
    
    optimizers.actor.zero_grad()
    (1e-3 * loss_pse + policy_loss).backward()
#     policy_loss.backward()
#     (1e-3 * loss_pse).backward()
    ###########################
#     print(str(p_ind) + " Actor Grads")
#     print(str(p_ind) + "OutDerWrtInp: ")
#     print(opt_action.grad)
    
#     print(str(p_ind) + "layer3Der1: ")
#     print(actor_weights.grad)
    
#     print(str(p_ind) + "layer1WDer: ")
#     print(model.actor.linear1.weight.grad)
#     print(str(p_ind) + "layer1bDer: ")
#     print(model.actor.linear1.bias.grad)
    
#     print(str(p_ind) + "layer2WDer: ")
#     print(model.actor.linear2.weight.grad)
#     print(str(p_ind) + "layer2bDer: ")
#     print(model.actor.linear2.bias.grad)
    
#     print(str(p_ind) + "layer3WDer: ")
#     print(model.actor.linear3.weight.grad)
#     print(str(p_ind) + "layer3bDer: ")
#     print(model.actor.linear3.bias.grad)
    ########################### 
    optimizers.actor.step()

    
    
    optimizers.critic.zero_grad()
    value_loss.backward()
    ###########################
#     print(str(p_ind) + " Critic Grads")
    
#     print(str(p_ind) + "layer3Der: ")
#     print(value.grad)
    
#     print(str(p_ind) + "layer1WDer: ")
#     print(model.critic.linear1.weight.grad)
#     print(str(p_ind) + "layer1bDer: ")
#     print(model.critic.linear1.bias.grad)
    
#     print(str(p_ind) + "layer2WDer: ")
#     print(model.critic.linear2.weight.grad)
#     print(str(p_ind) + "layer2bDer: ")
#     print(model.critic.linear2.bias.grad)
    
#     print(str(p_ind) + "layer3WDer: ")
#     print(model.critic.linear3.weight.grad)
#     print(str(p_ind) + "layer3bDer: ")
#     print(model.critic.linear3.bias.grad)
    ###########################
    optimizers.critic.step()
    

def update_target0in0(SampleBatch, SampleBatch_c, models, optimizers, replay_buffer, batch_size,
               obs_inds=None, a_inds=None, r_inds=None, p_ind = None, 
               actor_state_mode=None, critic_action_mode = None, 
               critic_state_mode=None, p_graph=None, slice_graph=None,
               gamma=0.99):
    
    ############################################################################################
    l=len(replay_buffer)
    prev_len = 396000




    sample_arr = []
    for i in range(batch_size):
        x = int(SampleBatch.readline())
        SampleBatch_c.write(str(x))
        SampleBatch_c.write("\n")
        sample_arr.append(x)
    
    
#     sample_arr = []
#     f = open("SampleArrayByPlayer0.txt","r")
#     for i in range(batch_size):
#         x = int(f.readline())
#         sample_arr.append(x)
#     f.close()
    
    sampleStatesByPlayer0 = []
    sampleActionsByPlayer0 = []
    sampleNextStatesByPlayer0 = []
    sampleRewardsPlayer0 = []
    for i in sample_arr:
        states = list((replay_buffer.buffer[i])[0])
        actions = list((replay_buffer.buffer[i])[1])
        next_states = list((replay_buffer.buffer[i])[3])
        reward = ((replay_buffer.buffer[i])[2])[0]

        sampleStatesByPlayer0.append(states)
        sampleActionsByPlayer0.append(actions)
        sampleNextStatesByPlayer0.append(next_states)
        sampleRewardsPlayer0.append(reward)

    q = np.concatenate(((np.array(sampleStatesByPlayer0))[:,0:5],np.zeros((batch_size,5))),axis=1)
    f = open("SampleStatesByPlayer0.txt","w")
    for i in range(batch_size):
        for j in range(10):
            f.write(str(q[i][j]))
            f.write("\n")
    f.close()
    
    
    z = np.concatenate(((np.array(sampleStatesByPlayer0))[:,0:5],np.zeros((batch_size,5)),(np.array(sampleActionsByPlayer0))[:,0:2],np.zeros((batch_size,2))),axis=1)
    f = open("SampleStatesActionsByPlayer0ToBeUsedInCriticForward.txt","w")
    for i in range(batch_size):
        for j in range(14):
            f.write(str(z[i][j]))
            f.write("\n")
    f.close()
    
        
    w = np.concatenate(((np.array(sampleNextStatesByPlayer0))[:,0:5],np.zeros((batch_size,5))),axis=1)
    f = open("SampleNextStatesByPlayer0.txt","w")
    for i in range(batch_size):
        for j in range(10):
            f.write(str(w[i][j]))
            f.write("\n")
    f.close()
    
        
    #############################################################################################    
    
    
    
    
    ##################################################################
    
    
    print("1st command starts in 0in0")
    
    command = "cat SampleStatesByPlayer0.txt wNb_Player0Actor.txt action_lim_high.txt | ../../../EzPC/SCI/vital/build/ActorForward0 r=1 add=10.0.0.5 > ActorForawrdResultPlayer0.txt; sed -i '1,12d ; s/^.*: //g' ActorForawrdResultPlayer0.txt"
    subprocess.call(command, shell=True)
    
    print("1st command ends in 0in0")
    
    
    Player0Action = []
    f = open("ActorForawrdResultPlayer0.txt","r")
    for i in range(batch_size):
        arr = []
        for j in range(2):
            x = float(f.readline())
            arr.append(x)
        Player0Action.append(arr)
    f.close()
    
    y = np.concatenate((q,np.array(Player0Action),np.zeros((batch_size,2))),axis=1)
    
    f = open("SampleStatesActionsByPlayer0ToBeUsedInActorForward.txt","w")
    for i in range(batch_size):
        for j in range(14):
            f.write(str(y[i][j]))
            f.write("\n")
    f.close()
    
#     print("2nd command starts in 0in0")
    
#     command = "cat SampleStatesActionsByPlayer0ToBeUsedInActorForward.txt wNb_Player0Critic.txt | ../../../EzPC/SCI/vital/build/CriticForward0 r=1 add=10.0.0.5 > CriticForwardComponentsToBeUsedInActorBackwardPlayer0.txt; sed -i -n '140,16523p; 49292,65675p' CriticForwardComponentsToBeUsedInActorBackwardPlayer0.txt; sed -i 's/^.*: //g' CriticForwardComponentsToBeUsedInActorBackwardPlayer0.txt"
#     subprocess.call(command, shell=True)
    
#     print("2nd command ends in 0in0")
    
    ##################################################################
    
    
    
    
    ###############################################################################
    print("3rd command starts in 0in0")
    
    command = "cat SampleNextStatesByPlayer0.txt wNb_Player0Actor_targ.txt action_lim_high.txt | ../../../EzPC/SCI/vital/build/ActorForward0 r=1 add=10.0.0.5 > EstimatedNextActionsPlayer0.txt; sed -i -n '13,268p' EstimatedNextActionsPlayer0.txt; sed -i 's/^.*: //g' EstimatedNextActionsPlayer0.txt"
    subprocess.call(command, shell=True)
    
    print("3rd command ends in 0in0")
    
    
    print("4th command starts in 0in0")
    
    command = "cat SampleNextStatesByPlayer0.txt | ../../../EzPC/SCI/vital/build/ActorForward0 r=2 add=10.0.0.5 > /dev/null"
    subprocess.call(command, shell=True)
    
    print("4th command ends in 0in0")
    
    
    Player0EstimatedNextAction = []
    f = open("EstimatedNextActionsPlayer0.txt","r")
    for i in range(batch_size):
        arr = []
        for j in range(2):
            x = float(f.readline())
            arr.append(x)
        Player0EstimatedNextAction.append(arr)
    f.close()
    
    v = np.concatenate((w,np.array(Player0EstimatedNextAction),np.zeros((batch_size,2))),axis=1)
    
    f = open("SampleNextStatesNextActionsByPlayer0.txt","w")
    for i in range(batch_size):
        for j in range(14):
            f.write(str(v[i][j]))
            f.write("\n")
    f.close()
    
    
    print("5th command starts in 0in0")
    
    command = "cat SampleNextStatesNextActionsByPlayer0.txt wNb_Player0Critic_targ.txt | ../../../EzPC/SCI/vital/build/CriticForward0 r=1 add=10.0.0.5 > CriticTargetPlayer0.txt; sed -i -n '12,139p' CriticTargetPlayer0.txt; sed -i 's/^.*: //g' CriticTargetPlayer0.txt"
    subprocess.call(command, shell=True)
    
    print("5th command ends in 0in0")
    
    Player0CriticTarget_temp = []
    f = open("CriticTargetPlayer0.txt","r")
    for i in range(batch_size):
        x = float(f.readline())
        Player0CriticTarget_temp.append(x)
    f.close()
    
    Player0CriticTarget = (np.array(sampleRewardsPlayer0)) + gamma * (np.array(Player0CriticTarget_temp))
    f = open("CriticTargetPlayer0.txt","w")
    for i in range(batch_size):
        f.write(str(Player0CriticTarget[i]))
        f.write("\n")
    f.close()
    
#     print("6th command starts in 0in0")
    
#     command = "cat SampleStatesActionsByPlayer0ToBeUsedInCriticForward.txt wNb_Player0Critic.txt | ../../../EzPC/SCI/vital/build/CriticForward0 r=1 add=10.0.0.5 > CriticForwardResultPlayer0.txt; sed -i '1,11d; s/^.*: //g' CriticForwardResultPlayer0.txt"
#     subprocess.call(command, shell=True)
    
#     print("6th command ends in 0in0")
    
#     print("7th command starts in 0in0")
    
#     command = "cp ActorForawrdResultPlayer0.txt ActorForawrdResultPlayer0_cp.txt; sed -i '1,256d' ActorForawrdResultPlayer0_cp.txt"
#     subprocess.call(command, shell=True)
    
#     print("7th command ends in 0in0")
    
    
    print("8th command starts in 0in0")
    command = "cp wNb_Player0Critic.txt SpecialComponentOfCriticWnBForActorBackwardPlayer0.txt"
    subprocess.call(command, shell=True)
    print("8th command ends in 0in0")
    
    
    print("9th command starts in 0in0")
    command = "sed -i -n '11,12p ; 25,26p ; 39,40p ; 53,54p ; 67,68p ; 81,82p ; 95,96p ; 109,110p ; 123,124p ; 137,138p ; 151,152p ; 165,166p ; 179,180p ; 193,194p ; 207,208p ; 221,222p ; 235,236p ; 249,250p ; 263,264p ; 277,278p ; 291,292p ; 305,306p ; 319,320p ; 333,334p ; 347,348p ; 361,362p ; 375,376p ; 389,390p ; 403,404p ; 417,418p ; 431,432p ; 445,446p ; 459,460p ; 473,474p ; 487,488p ; 501,502p ; 515,516p ; 529,530p ; 543,544p ; 557,558p ; 571,572p ; 585,586p ; 599,600p ; 613,614p ; 627,628p ; 641,642p ; 655,656p ; 669,670p ; 683,684p ; 697,698p ; 711,712p ; 725,726p ; 739,740p ; 753,754p ; 767,768p ; 781,782p ; 795,796p ; 809,810p ; 823,824p ; 837,838p ; 851,852p ; 865,866p ; 879,880p ; 893,894p ; 907,908p ; 921,922p ; 935,936p ; 949,950p ; 963,964p ; 977,978p ; 991,992p ; 1005,1006p ; 1019,1020p ; 1033,1034p ; 1047,1048p ; 1061,1062p ; 1075,1076p ; 1089,1090p ; 1103,1104p ; 1117,1118p ; 1131,1132p ; 1145,1146p ; 1159,1160p ; 1173,1174p ; 1187,1188p ; 1201,1202p ; 1215,1216p ; 1229,1230p ; 1243,1244p ; 1257,1258p ; 1271,1272p ; 1285,1286p ; 1299,1300p ; 1313,1314p ; 1327,1328p ; 1341,1342p ; 1355,1356p ; 1369,1370p ; 1383,1384p ; 1397,1398p ; 1411,1412p ; 1425,1426p ; 1439,1440p ; 1453,1454p ; 1467,1468p ; 1481,1482p ; 1495,1496p ; 1509,1510p ; 1523,1524p ; 1537,1538p ; 1551,1552p ; 1565,1566p ; 1579,1580p ; 1593,1594p ; 1607,1608p ; 1621,1622p ; 1635,1636p ; 1649,1650p ; 1663,1664p ; 1677,1678p ; 1691,1692p ; 1705,1706p ; 1719,1720p ; 1733,1734p ; 1747,1748p ; 1761,1762p ; 1775,1776p ; 1789,1790p' SpecialComponentOfCriticWnBForActorBackwardPlayer0.txt"
    subprocess.call(command, shell=True)
    
    print("9th command ends in 0in0")
    
    print("10th command starts in 0in0")
#     command = "cat SampleStatesByPlayer0.txt AllZero.txt wNb_Player0Actor.txt mvt_Player0Actor.txt action_lim_high.txt ActorForawrdResultPlayer0_cp.txt SpecialComponentOfCriticWnBForActorBackwardPlayer0.txt CriticForwardComponentsToBeUsedInActorBackwardPlayer0.txt | ../../../EzPC/SCI/vital/build/ActorBackward0 r=1 add=10.0.0.5 > wNb_Player0Actor_temp.txt; sed -i '1,43d; s/^.*: //g' wNb_Player0Actor_temp.txt"
    
    command = "cat SampleStatesByPlayer0.txt AllZero.txt wNb_Player0Actor.txt mvt_Player0Actor.txt action_lim_high.txt SpecialComponentOfCriticWnBForActorBackwardPlayer0.txt SampleStatesActionsByPlayer0ToBeUsedInActorForward.txt wNb_Player0Critic.txt | ../../../EzPC/SCI/vital/build/ActorBackward0 r=1 add=10.0.0.5 > wNb_Player0Actor_temp.txt; sed -i '1,39d; s/^.*: //g' wNb_Player0Actor_temp.txt"
    subprocess.call(command, shell=True)
    print("10th command ends in 0in0")
    
    print("11th command starts in 0in0")
#     command = "cp wNb_Player0Actor_temp.txt wNb_Player0Actor.txt"
    command = "sed -n \"1,18178p\" wNb_Player0Actor_temp.txt > wNb_Player0Actor.txt; sed \"1,18178d\" wNb_Player0Actor_temp.txt > mvt_Player0Actor.txt"
    subprocess.call(command, shell=True)
    print("11th command ends in 0in0")
    
    print("12th command starts in 0in0")
#     command = "cat SampleStatesActionsByPlayer0ToBeUsedInCriticForward.txt CriticTargetPlayer0.txt wNb_Player0Critic.txt mvt_Player0Critic.txt CriticForwardResultPlayer0.txt | ../../../EzPC/SCI/vital/build/CriticBackward0 r=1 add=10.0.0.5 > wNb_Player0Critic_temp.txt; sed -i '1,37d; s/^.*: //g' wNb_Player0Critic_temp.txt"
    
    command = "cat SampleStatesActionsByPlayer0ToBeUsedInCriticForward.txt CriticTargetPlayer0.txt wNb_Player0Critic.txt mvt_Player0Critic.txt | ../../../EzPC/SCI/vital/build/CriticBackward0 r=1 add=10.0.0.5 > wNb_Player0Critic_temp.txt; sed -i '1,30d; s/^.*: //g' wNb_Player0Critic_temp.txt"
    subprocess.call(command, shell=True)
    print("12th command ends in 0in0")
    
    print("13th command starts in 0in0")
#     command = "cp wNb_Player0Critic_temp.txt wNb_Player0Critic.txt"
    command = "sed -n \"1,18561p\" wNb_Player0Critic_temp.txt > wNb_Player0Critic.txt; sed \"1,18561d\" wNb_Player0Critic_temp.txt > mvt_Player0Critic.txt"
    subprocess.call(command, shell=True)
    print("13th command ends in 0in0")
    
    
    ###############################################################################
    
    
    
def update_target1in0(SampleBatch, SampleBatch_c, models, optimizers, replay_buffer, batch_size,
               obs_inds=None, a_inds=None, r_inds=None, p_ind = None, 
               actor_state_mode=None, critic_action_mode = None, 
               critic_state_mode=None, p_graph=None, slice_graph=None,
               gamma=0.99):
    ############################################################################################
#     print("0th command starts in 1in0")
#     command = "rm SampleStatesByPlayer0.txt SampleStatesActionsByPlayer0.txt SampleNextStatesByPlayer0.txt EstimatedNextActionsPlayer0.txt SampleNextStatesNextActionsByPlayer0.txt"
#     subprocess.call(command, shell=True)
#     print("0th command ends in 1in0")
    
    l=len(replay_buffer)
#     sample_arr = []
#     time.sleep(3)
#     f = open("SampleArrayByPlayer1.txt","r")
#     for i in range(batch_size):
#         x = int(f.readline())
#         sample_arr.append(x)
#     f.close()

    sample_arr = []
    for i in range(batch_size):
        x = int(SampleBatch.readline())
        SampleBatch_c.write(str(x))
        SampleBatch_c.write("\n")
        sample_arr.append(x)
    
    sampleStatesByPlayer0 = []
    sampleActionsByPlayer0 = []
    sampleNextStatesByPlayer0 = []
    for i in sample_arr:
        states = list((replay_buffer.buffer[i])[0])
        actions = list((replay_buffer.buffer[i])[1])
        next_states = list((replay_buffer.buffer[i])[3])

        sampleStatesByPlayer0.append(states)
        sampleActionsByPlayer0.append(actions)
        sampleNextStatesByPlayer0.append(next_states)

    q = np.concatenate(((np.array(sampleStatesByPlayer0))[:,0:5],np.zeros((batch_size,5)),(np.array(sampleActionsByPlayer0))[:,0:2],np.zeros((batch_size,2))),axis=1)
    f = open("SampleStatesByPlayer0.txt","w")
    for i in range(batch_size):
        for j in range(10):
            f.write(str(q[i][j]))
            f.write("\n")
    f.close()
    
    f = open("SampleStatesActionsByPlayer0.txt","w")
    for i in range(batch_size):
        for j in range(14):
            f.write(str(q[i][j]))
            f.write("\n")
    f.close()
    
    
    w = np.concatenate(((np.array(sampleNextStatesByPlayer0))[:,0:5],np.zeros((batch_size,5))),axis=1)
    f = open("SampleNextStatesByPlayer0.txt","w")
    for i in range(batch_size):
        for j in range(10):
            f.write(str(w[i][j]))
            f.write("\n")
    f.close()
    #############################################################################################
    
    ##################################################################
    print("1st command starts in 1in0")
    command = "cat SampleStatesByPlayer0.txt | ../../../EzPC/SCI/vital/build/ActorForward0 r=2 add=10.0.0.5 > /dev/null"
    subprocess.call(command, shell=True)
    print("1st command ends in 1in0")
    
#     print("2nd command starts in 1in0")
#     command = "cat SampleStatesActionsByPlayer0.txt | ../../../EzPC/SCI/vital/build/CriticForward0 r=2 add=10.0.0.5 > /dev/null"
#     subprocess.call(command, shell=True)
#     print("2nd command ends in 1in0")
    ##################################################################
    
    ###############################################################################
    print("3rd command starts in 1in0")
    command = "cat SampleNextStatesByPlayer0.txt | ../../../EzPC/SCI/vital/build/ActorForward0 r=2 add=10.0.0.5 > /dev/null"
    subprocess.call(command, shell=True)
    print("3rd command ends in 1in0")
    
    print("4th command starts in 1in0")
    command = "cat SampleNextStatesByPlayer0.txt wNb_Player0Actor_targ.txt action_lim_high.txt | ../../../EzPC/SCI/vital/build/ActorForward0 r=1 add=10.0.0.5 > EstimatedNextActionsPlayer0.txt; sed -i -n '13,268p' EstimatedNextActionsPlayer0.txt; sed -i 's/^.*: //g' EstimatedNextActionsPlayer0.txt"
    subprocess.call(command, shell=True)
    print("4th command ends in 1in0")
    
    
    
    Player0EstimatedNextAction = []
    f = open("EstimatedNextActionsPlayer0.txt","r")
    for i in range(batch_size):
        arr = []
        for j in range(2):
            x = float(f.readline())
            arr.append(x)
        Player0EstimatedNextAction.append(arr)
    f.close()
    
    v = np.concatenate((w,np.array(Player0EstimatedNextAction),np.zeros((batch_size,2))),axis=1)
    
    f = open("SampleNextStatesNextActionsByPlayer0.txt","w")
    for i in range(batch_size):
        for j in range(14):
            f.write(str(v[i][j]))
            f.write("\n")
    f.close()
    
    print("5th command starts in 1in0")
    command = "cat SampleNextStatesNextActionsByPlayer0.txt | ../../../EzPC/SCI/vital/build/CriticForward0 r=2 add=10.0.0.5 > /dev/null"
    subprocess.call(command, shell=True)
    print("5th command ends in 1in0")
    
#     print("6th command starts in 1in0")
#     command = "cat SampleStatesActionsByPlayer0.txt | ../../../EzPC/SCI/vital/build/CriticForward0 r=2 add=10.0.0.5 > /dev/null"
#     subprocess.call(command, shell=True)
#     print("6th command ends in 1in0")
    
    
    print("7th command starts in 1in0")
    command = "cat SampleStatesByPlayer0.txt SampleStatesActionsByPlayer0.txt | ../../../EzPC/SCI/vital/build/ActorBackward0 r=2 add=10.0.0.5 > /dev/null"
    subprocess.call(command, shell=True)
    print("7th command ends in 1in0")
    
    print("8th command starts in 1in0")
    command = "cat SampleStatesActionsByPlayer0.txt | ../../../EzPC/SCI/vital/build/CriticBackward0 r=2 add=10.0.0.5 > /dev/null"
    subprocess.call(command, shell=True)
    print("8th command starts in 1in0")
    ###############################################################################

    
    
def update_target0in1(SampleBatch, SampleBatch_c, models, optimizers, replay_buffer, batch_size,
               obs_inds=None, a_inds=None, r_inds=None, p_ind = None, 
               actor_state_mode=None, critic_action_mode = None, 
               critic_state_mode=None, p_graph=None, slice_graph=None,
               gamma=0.99):
    ############################################################################################
    l=len(replay_buffer)
#     sample_arr = []
#     time.sleep(3)
#     f = open("SampleArrayByPlayer0.txt","r")
#     for i in range(batch_size):
#         x = int(f.readline())
#         sample_arr.append(x)
#     f.close()

    sample_arr = []
    for i in range(batch_size):
        x = int(SampleBatch.readline())
        SampleBatch_c.write(str(x))
        SampleBatch_c.write("\n")
        sample_arr.append(x)
    
    sampleStatesByPlayer1 = []
    sampleActionsByPlayer1 = []
    sampleNextStatesByPlayer1 = []
    for i in sample_arr:
        states = list((replay_buffer.buffer[i])[0])
        actions = list((replay_buffer.buffer[i])[1])
        next_states = list((replay_buffer.buffer[i])[3])

        sampleStatesByPlayer1.append(states)
        sampleActionsByPlayer1.append(actions)
        sampleNextStatesByPlayer1.append(next_states)

    q = np.concatenate((np.zeros((batch_size,5)),(np.array(sampleStatesByPlayer1))[:,5:10],np.zeros((batch_size,2)),(np.array(sampleActionsByPlayer1))[:,2:4]),axis=1)
    f = open("SampleStatesByPlayer1.txt","w")
    for i in range(batch_size):
        for j in range(10):
            f.write(str(q[i][j]))
            f.write("\n")
    f.close()
    
    f = open("SampleStatesActionsByPlayer1.txt","w")
    for i in range(batch_size):
        for j in range(14):
            f.write(str(q[i][j]))
            f.write("\n")
    f.close()
    
    
    w = np.concatenate((np.zeros((batch_size,5)),(np.array(sampleNextStatesByPlayer1))[:,5:10]),axis=1)
    f = open("SampleNextStatesByPlayer1.txt","w")
    for i in range(batch_size):
        for j in range(10):
            f.write(str(w[i][j]))
            f.write("\n")
    f.close()
    #############################################################################################
    
    ##################################################################
    
    print("1st command starts in 0in1")
    command = "cat SampleStatesByPlayer1.txt | ../../../EzPC/SCI/vital/build/ActorForward0 r=2 add=10.0.0.4 > /dev/null"
    subprocess.call(command, shell=True)
    print("1st command ends in 0in1")
    
#     print("2nd command starts in 0in1")
#     command = "cat SampleStatesActionsByPlayer1.txt | ../../../EzPC/SCI/vital/build/CriticForward0 r=2 add=10.0.0.4 > /dev/null"
#     subprocess.call(command, shell=True)
#     print("2nd command ends in 0in1")
    ##################################################################
    
    ###############################################################################
    print("3rd command starts in 0in1")
    command = "cat SampleNextStatesByPlayer1.txt | ../../../EzPC/SCI/vital/build/ActorForward0 r=2 add=10.0.0.4 > /dev/null"
    subprocess.call(command, shell=True)
    print("3rd command ends in 0in1")
    
    print("4th command starts in 0in1")
    command = "cat SampleNextStatesByPlayer1.txt wNb_Player1Actor_targ.txt action_lim_high.txt | ../../../EzPC/SCI/vital/build/ActorForward0 r=1 add=10.0.0.4 > EstimatedNextActionsPlayer1.txt; sed -i -n '13,268p' EstimatedNextActionsPlayer1.txt; sed -i ' s/^.*: //g' EstimatedNextActionsPlayer1.txt"
    subprocess.call(command, shell=True)
    print("4th command ends in 0in1")
    
    Player1EstimatedNextAction = []
    f = open("EstimatedNextActionsPlayer1.txt","r")
    for i in range(batch_size):
        arr = []
        for j in range(2):
            x = float(f.readline())
            arr.append(x)
        Player1EstimatedNextAction.append(arr)
    f.close()
    
    v = np.concatenate((w,np.zeros((batch_size,2)),np.array(Player1EstimatedNextAction)),axis=1)
    
    f = open("SampleNextStatesNextActionsByPlayer1.txt","w")
    for i in range(batch_size):
        for j in range(14):
            f.write(str(v[i][j]))
            f.write("\n")
    f.close()
    
    print("5th command starts in 0in1")
    command = "cat SampleNextStatesNextActionsByPlayer1.txt | ../../../EzPC/SCI/vital/build/CriticForward0 r=2 add=10.0.0.4 > /dev/null"
    subprocess.call(command, shell=True)
    print("5th command ends in 0in1")
    
#     print("6th command starts in 0in1")
#     command = "cat SampleStatesActionsByPlayer1.txt | ../../../EzPC/SCI/vital/build/CriticForward0 r=2 add=10.0.0.4 > /dev/null"
#     subprocess.call(command, shell=True)
#     print("6th command starts in 0in1")
    
    
    print("7th command starts in 0in1")
    command = "cat SampleStatesByPlayer1.txt SampleStatesActionsByPlayer1.txt | ../../../EzPC/SCI/vital/build/ActorBackward0 r=2 add=10.0.0.4 > /dev/null"
    subprocess.call(command, shell=True)
    print("7th command ends in 0in1")
    
    
    print("8th command starts in 0in1")
    command = "cat SampleStatesActionsByPlayer1.txt | ../../../EzPC/SCI/vital/build/CriticBackward0 r=2 add=10.0.0.4 > /dev/null"
    subprocess.call(command, shell=True)
    print("8th command ends in 0in1")
    ###############################################################################
    

    
    
def update_target1in1(SampleBatch, SampleBatch_c, models, optimizers, replay_buffer, batch_size,
               obs_inds=None, a_inds=None, r_inds=None, p_ind = None, 
               actor_state_mode=None, critic_action_mode = None, 
               critic_state_mode=None, p_graph=None, slice_graph=None,
               gamma=0.99):
    ############################################################################################
#     print("0th command starts in 1in1")
#     command = "rm SampleStatesByPlayer1.txt SampleStatesActionsByPlayer1ToBeUsedInCriticForward.txt SampleNextStatesByPlayer1.txt ActorForawrdResultPlayer1.txt SampleStatesActionsByPlayer1ToBeUsedInActorForward.txt CriticForwardComponentsToBeUsedInActorBackwardPlayer1.txt EstimatedNextActionsPlayer1.txt SampleNextStatesNextActionsByPlayer1.txt CriticTargetPlayer1.txt CriticForwardResultPlayer1.txt ActorForawrdResultPlayer1_cp.txt SpecialComponentOfCriticWnBForActorBackwardPlayer1.txt wNb_Player1Actor_temp.txt wNb_Player1Critic_temp.txt"
#     subprocess.call(command, shell=True)
#     print("0th command ends in 1in1")
    
    l=len(replay_buffer)
    prev_len = 396000


    sample_arr = []
    for i in range(batch_size):
        x = int(SampleBatch.readline())
        SampleBatch_c.write(str(x))
        SampleBatch_c.write("\n")
        sample_arr.append(x)
    
    sampleStatesByPlayer1 = []
    sampleActionsByPlayer1 = []
    sampleNextStatesByPlayer1 = []
    sampleRewardsPlayer1 = []
    for i in sample_arr:
        states = list((replay_buffer.buffer[i])[0])
        actions = list((replay_buffer.buffer[i])[1])
        next_states = list((replay_buffer.buffer[i])[3])
        reward = ((replay_buffer.buffer[i])[2])[1]

        sampleStatesByPlayer1.append(states)
        sampleActionsByPlayer1.append(actions)
        sampleNextStatesByPlayer1.append(next_states)
        sampleRewardsPlayer1.append(reward)

    q = np.concatenate((np.zeros((batch_size,5)),(np.array(sampleStatesByPlayer1))[:,5:10]),axis=1)
    f = open("SampleStatesByPlayer1.txt","w")
    for i in range(batch_size):
        for j in range(10):
            f.write(str(q[i][j]))
            f.write("\n")
    f.close()
    
    
    z = np.concatenate((np.zeros((batch_size,5)),(np.array(sampleStatesByPlayer1))[:,5:10],np.zeros((batch_size,2)),(np.array(sampleActionsByPlayer1))[:,2:4]),axis=1)
    f = open("SampleStatesActionsByPlayer1ToBeUsedInCriticForward.txt","w")
    for i in range(batch_size):
        for j in range(14):
            f.write(str(z[i][j]))
            f.write("\n")
    f.close()
    
        
    w = np.concatenate((np.zeros((batch_size,5)),(np.array(sampleNextStatesByPlayer1))[:,5:10]),axis=1)
    f = open("SampleNextStatesByPlayer1.txt","w")
    for i in range(batch_size):
        for j in range(10):
            f.write(str(w[i][j]))
            f.write("\n")
    f.close()
    
        
    #############################################################################################    
    
    
    
    
    ##################################################################
    print("1st command starts in 1in1")
    
    command = "cat SampleStatesByPlayer1.txt wNb_Player1Actor.txt action_lim_high.txt | ../../../EzPC/SCI/vital/build/ActorForward0 r=1 add=10.0.0.4 > ActorForawrdResultPlayer1.txt; sed -i '1,12d ; s/^.*: //g' ActorForawrdResultPlayer1.txt"
    subprocess.call(command, shell=True)
    
    print("1st command ends in 1in1")
    
    Player1Action = []
    f = open("ActorForawrdResultPlayer1.txt","r")
    for i in range(batch_size):
        arr = []
        for j in range(2):
            x = float(f.readline())
            arr.append(x)
        Player1Action.append(arr)
    f.close()
    
    y = np.concatenate((q,np.zeros((batch_size,2)),np.array(Player1Action)),axis=1)
    
    f = open("SampleStatesActionsByPlayer1ToBeUsedInActorForward.txt","w")
    for i in range(batch_size):
        for j in range(14):
            f.write(str(y[i][j]))
            f.write("\n")
    f.close()
    
#     print("2nd command starts in 1in1")
    
#     command = "cat SampleStatesActionsByPlayer1ToBeUsedInActorForward.txt wNb_Player1Critic.txt | ../../../EzPC/SCI/vital/build/CriticForward0 r=1 add=10.0.0.4 > CriticForwardComponentsToBeUsedInActorBackwardPlayer1.txt; sed -i -n '140,16523p; 49292,65675p' CriticForwardComponentsToBeUsedInActorBackwardPlayer1.txt; sed -i 's/^.*: //g' CriticForwardComponentsToBeUsedInActorBackwardPlayer1.txt"
#     subprocess.call(command, shell=True)
    
#     print("2nd command ends in 1in1")
    
    ##################################################################
    
    
    
    
    ###############################################################################
    print("3rd command starts in 1in1")
    
    command = "cat SampleNextStatesByPlayer1.txt wNb_Player1Actor_targ.txt action_lim_high.txt | ../../../EzPC/SCI/vital/build/ActorForward0 r=1 add=10.0.0.4 > EstimatedNextActionsPlayer1.txt; sed -i -n '13,268p' EstimatedNextActionsPlayer1.txt; sed -i 's/^.*: //g' EstimatedNextActionsPlayer1.txt"
    subprocess.call(command, shell=True)
    
    print("3rd command ends in 1in1")
    
    
    print("4th command starts in 1in1")
    
    command = "cat SampleNextStatesByPlayer1.txt | ../../../EzPC/SCI/vital/build/ActorForward0 r=2 add=10.0.0.4 > /dev/null"
    subprocess.call(command, shell=True)
    
    print("4th command ends in 1in1")
    
    
    Player1EstimatedNextAction = []
    f = open("EstimatedNextActionsPlayer1.txt","r")
    for i in range(batch_size):
        arr = []
        for j in range(2):
            x = float(f.readline())
            arr.append(x)
        Player1EstimatedNextAction.append(arr)
    f.close()
    
    v = np.concatenate((w,np.zeros((batch_size,2)),np.array(Player1EstimatedNextAction)),axis=1)
    
    f = open("SampleNextStatesNextActionsByPlayer1.txt","w")
    for i in range(batch_size):
        for j in range(14):
            f.write(str(v[i][j]))
            f.write("\n")
    f.close()
    
    print("5th command starts in 1in1")
    
    command = "cat SampleNextStatesNextActionsByPlayer1.txt wNb_Player1Critic_targ.txt | ../../../EzPC/SCI/vital/build/CriticForward0 r=1 add=10.0.0.4 > CriticTargetPlayer1.txt; sed -i -n '12,139p' CriticTargetPlayer1.txt; sed -i 's/^.*: //g' CriticTargetPlayer1.txt"
    subprocess.call(command, shell=True)
    
    print("5th command ends in 1in1")
    
    
    
    Player1CriticTarget_temp = []
    f = open("CriticTargetPlayer1.txt","r")
    for i in range(batch_size):
        x = float(f.readline())
        Player1CriticTarget_temp.append(x)
    f.close()
    
    
    Player1CriticTarget = (np.array(sampleRewardsPlayer1)) + gamma * (np.array(Player1CriticTarget_temp))
    f = open("CriticTargetPlayer1.txt","w")
    for i in range(batch_size):
        f.write(str(Player1CriticTarget[i]))
        f.write("\n")
    f.close()
    
    
    print("6th command starts in 1in1")
    
#     command = "cat SampleStatesActionsByPlayer1ToBeUsedInCriticForward.txt wNb_Player1Critic.txt | ../../../EzPC/SCI/vital/build/CriticForward0 r=1 add=10.0.0.4 > CriticForwardResultPlayer1.txt; sed -i '1,11d; s/^.*: //g' CriticForwardResultPlayer1.txt"
#     subprocess.call(command, shell=True)
    
#     print("6th command ends in 1in1")
    
    
#     print("7th command starts in 1in1")
    
#     command = "cp ActorForawrdResultPlayer1.txt ActorForawrdResultPlayer1_cp.txt; sed -i '1,256d' ActorForawrdResultPlayer1_cp.txt"
#     subprocess.call(command, shell=True)
    
#     print("7th command ends in 1in1")
    
    
    print("8th command starts in 1in1")
    command = "cp wNb_Player1Critic.txt SpecialComponentOfCriticWnBForActorBackwardPlayer1.txt"
    subprocess.call(command, shell=True)
    print("8th command ends in 1in1")
    
    print("9th command starts in 1in1")
    
    command = "sed -i -n '13,14p ; 27,28p ; 41,42p ; 55,56p ; 69,70p ; 83,84p ; 97,98p ; 111,112p ; 125,126p ; 139,140p ; 153,154p ; 167,168p ; 181,182p ; 195,196p ; 209,210p ; 223,224p ; 237,238p ; 251,252p ; 265,266p ; 279,280p ; 293,294p ; 307,308p ; 321,322p ; 335,336p ; 349,350p ; 363,364p ; 377,378p ; 391,392p ; 405,406p ; 419,420p ; 433,434p ; 447,448p ; 461,462p ; 475,476p ; 489,490p ; 503,504p ; 517,518p ; 531,532p ; 545,546p ; 559,560p ; 573,574p ; 587,588p ; 601,602p ; 615,616p ; 629,630p ; 643,644p ; 657,658p ; 671,672p ; 685,686p ; 699,700p ; 713,714p ; 727,728p ; 741,742p ; 755,756p ; 769,770p ; 783,784p ; 797,798p ; 811,812p ; 825,826p ; 839,840p ; 853,854p ; 867,868p ; 881,882p ; 895,896p ; 909,910p ; 923,924p ; 937,938p ; 951,952p ; 965,966p ; 979,980p ; 993,994p ; 1007,1008p ; 1021,1022p ; 1035,1036p ; 1049,1050p ; 1063,1064p ; 1077,1078p ; 1091,1092p ; 1105,1106p ; 1119,1120p ; 1133,1134p ; 1147,1148p ; 1161,1162p ; 1175,1176p ; 1189,1190p ; 1203,1204p ; 1217,1218p ; 1231,1232p ; 1245,1246p ; 1259,1260p ; 1273,1274p ; 1287,1288p ; 1301,1302p ; 1315,1316p ; 1329,1330p ; 1343,1344p ; 1357,1358p ; 1371,1372p ; 1385,1386p ; 1399,1400p ; 1413,1414p ; 1427,1428p ; 1441,1442p ; 1455,1456p ; 1469,1470p ; 1483,1484p ; 1497,1498p ; 1511,1512p ; 1525,1526p ; 1539,1540p ; 1553,1554p ; 1567,1568p ; 1581,1582p ; 1595,1596p ; 1609,1610p ; 1623,1624p ; 1637,1638p ; 1651,1652p ; 1665,1666p ; 1679,1680p ; 1693,1694p ; 1707,1708p ; 1721,1722p ; 1735,1736p ; 1749,1750p ; 1763,1764p ; 1777,1778p ; 1791,1792p' SpecialComponentOfCriticWnBForActorBackwardPlayer1.txt"
    subprocess.call(command, shell=True)
    
    print("9th command ends in 0in0")
    
    
    print("10th command starts in 1in1")
#     command = "cat SampleStatesByPlayer1.txt AllZero.txt wNb_Player1Actor.txt mvt_Player1Actor.txt action_lim_high.txt ActorForawrdResultPlayer1_cp.txt SpecialComponentOfCriticWnBForActorBackwardPlayer1.txt  CriticForwardComponentsToBeUsedInActorBackwardPlayer1.txt | ../../../EzPC/SCI/vital/build/ActorBackward0 r=1 add=10.0.0.4 > wNb_Player1Actor_temp.txt; sed -i '1,43d; s/^.*: //g' wNb_Player1Actor_temp.txt"
    
    command = "cat SampleStatesByPlayer1.txt AllZero.txt wNb_Player1Actor.txt mvt_Player1Actor.txt action_lim_high.txt SpecialComponentOfCriticWnBForActorBackwardPlayer1.txt SampleStatesActionsByPlayer1ToBeUsedInActorForward.txt wNb_Player1Critic.txt | ../../../EzPC/SCI/vital/build/ActorBackward0 r=1 add=10.0.0.4 > wNb_Player1Actor_temp.txt; sed -i '1,39d; s/^.*: //g' wNb_Player1Actor_temp.txt"
    subprocess.call(command, shell=True)
    print("10th command ends in 1in1")
    
    print("11th command starts in 1in1")
#     command = "cp wNb_Player1Actor_temp.txt wNb_Player1Actor.txt"
    command = "sed -n \"1,18178p\" wNb_Player1Actor_temp.txt > wNb_Player1Actor.txt; sed \"1,18178d\" wNb_Player1Actor_temp.txt > mvt_Player1Actor.txt"
    subprocess.call(command, shell=True)
    print("11th command ends in 1in1")
    
    
    print("12th command starts in 1in1")
#     command = "cat SampleStatesActionsByPlayer1ToBeUsedInCriticForward.txt CriticTargetPlayer1.txt wNb_Player1Critic.txt mvt_Player1Critic.txt CriticForwardResultPlayer1.txt | ../../../EzPC/SCI/vital/build/CriticBackward0 r=1 add=10.0.0.4 > wNb_Player1Critic_temp.txt; sed -i '1,37d; s/^.*: //g' wNb_Player1Critic_temp.txt"
    
    command = "cat SampleStatesActionsByPlayer1ToBeUsedInCriticForward.txt CriticTargetPlayer1.txt wNb_Player1Critic.txt mvt_Player1Critic.txt | ../../../EzPC/SCI/vital/build/CriticBackward0 r=1 add=10.0.0.4 > wNb_Player1Critic_temp.txt; sed -i '1,30d; s/^.*: //g' wNb_Player1Critic_temp.txt"
    subprocess.call(command, shell=True)
    print("12th command ends in 1in1")
    
    print("13th command starts in 1in1")
#     command = "cp wNb_Player1Critic_temp.txt wNb_Player1Critic.txt"
    command = "sed -n \"1,18561p\" wNb_Player1Critic_temp.txt > wNb_Player1Critic.txt; sed \"1,18561d\" wNb_Player1Critic_temp.txt > mvt_Player1Critic.txt"
    subprocess.call(command, shell=True)
    print("13th command ends in 1in1")
    
    
    ###############################################################################
