Context and Forewords:

    1. The EzPC folder contains the cryptographic backend support for all the secure 2 PC components used in this work.
    2. The Supply-Chain folder contains the supply chain set up.
    3. In EzPC/EzPC, run "./setup_env_and_build.sh" and then do "source mpc_venv/bin/activate" to use the 2 PC toolchain.
    4. In EzPC/EzPC/EzPC there are .ezpc files for secure Actor and Critic forward and backward passes. 
    5. A xyz.ezpc file is written in a C like high level language EzPC. It is compiled to the corresponding cpp by running "./ezpc xyz.ezpc --codegen SECFLOAT" from EzPC/EzPC/EzPC folder. The resulting cpp file "xyz0.cpp" contains all the required cryptographic backend for the secure 2 PC inbuilt and linked with it. So, high level programming can be done in EzPC language instaed of delving deep in the cryptography.
    6. This "xyz0.cpp" is then moved to "EzPC/SCI/vital". In "EzPC/SCI/vital/CMakeLists.txt" a corresponding entry in added for "xyz0.cpp" and then "cmake .." followed by "make -j" helps to get the corresponding executable "xyz0". This is the desired secure 2 PC executable. 
    7. Secure 2 PC execuatbles of all the Actor and Critic forward and backward passes can be found in EzPC/SCI/vital/build in a ready made compiled way.

Run Instructions:

    1. To run the initial 9900 epoch simulation (per epoch 40 iterations of MADDPG) with simulated data run "python3 run_tree_world_firstSimulation.py 2 3 3 3" from the Supply-Chain/marl folder. "2 3 3 3" are some game parameters.
    2. Foolwed by the initial simulation we shall run 20 additional epochs with real secret data in 3 modes: No Data Sharing, Explicit Data Exchange and Secure 2PC.

        a. For No Data Sharing, run "python3 run_tree_world_SimulationContinuationPlot.py 2 1 1 1" from Supply-Chain/marl folder.

        b. For Explicit Data Exchange, run "python3 run_tree_world_SimulationContinuationPlot.py 2 3 3 3" from Supply-Chain/marl folder.

        c. For Secure 2 PC, we need 2 VMs to mimic the 2 securely parties 0 and 1. Say VM0 is VM0_Name@10.0.0.4 and VM1 is VM1_Name@10.0.0.5 . 10.0.0.4 and 10.0.0.5 are their private ips. We assume that the "UAI_rebuttal" folder is sitting in the "/home/VM0_Name" directory in VM0 and same for VM1. Thus, respective changes are required to made in the "Supply-Chain/marl/supply_game/tree_world_Player0.py", "Supply-Chain/marl/supply_game/tree_world_Player1.py" and "Supply-Chain/marl/actor_critic/actor_critic_updates.py" files i.e., VM names and ips need to updated.

        Then, run "python3 Player0.py 2 3 3 3" from VM0 "Supply-Chain/marl" folder and "python3 Player1.py 2 3 3 3" from VM1 "Supply-Chain/marl" folder. This will trigger the secure 2 PC 20 epoch run.
        

