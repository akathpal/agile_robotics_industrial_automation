# Agile Robotics for Industrial Automation Competition (ARIAC)
This project is part of ENPM809B course offered by University of Maryland, College Park during Spring 2018.

Follow the instructions provided in the link below to setup the competition environment.
[ARIAC Competition Setup](https://bitbucket.org/osrf/ariac/wiki/2017/Home.md)

## Instructions to run the code:

* Clone the repository into `src` of your workspace 
* Go to the root of the workspace and execute `catkin_make`
* To run the example run the following command:
```
rosrun osrf_gear gear.py -f `catkin_find --share osrf_gear`/config/comp_conf1.yaml ~/ariac_ws/src/agile_robotics_industrial_automation/config/team_config.yaml
```
## Youtube Videos for Qualifiers
[Qual1a](https://www.youtube.com/watch?v=tEAGDLHdlyY&t=16s)
[Qual1b](https://www.youtube.com/watch?v=2djQ1RuXj2s)
[Qual2a](https://www.youtube.com/watch?v=pPSBvc6hQX4)
[Qual2b](https://www.youtube.com/watch?v=04zn6psg7uY)
[Qual3a](https://www.youtube.com/watch?v=5HyFe3qSEkk)
[Qual3b](https://www.youtube.com/watch?v=iujyKDMRZsw)

## Approach Overview
The basic goal of the ARIAC competition is to build the kits and fulfil the order. The first task
is to break down the entire task into sub-tasks. The qualifiers provided were the perfect
stepping stones for the same. Instead on focussing on solving the complete qualifier, first we
tried to solve the small tasks and how to deal with them.

Some of the unique elements of our approach:
* Flipping the parts
* Overall hybrid architecture and for conveyor pickup reactive architecture
* Pose Correction
* Computing the velocity of conveyor and updating part positions on conveyor

The next step after breaking down the tasks was to decide what sensors to use in order to
best achieve the results. Our main goal was to focus on the agility, being flexible to handle
various scenarios, of the process rather than lowering the overall cost of the environment.
Therefore, we have used five logical cameras, four on the bins and one on the conveyor. Using
logical cameras gave us the ability of handling scenarios like multiple parts on the same bin
as well as different configurations of the parts. We decided not to put cameras on the other
four bins since the robot cannot reach in configuration to pick parts from those bins. The
camera on the conveyor is not only being used to get the part type and position, but we are
also using to estimate the velocity of the conveyor since it is not constant. This computed
velocity is used to update the part positions on the conveyor with time.

## General tips 

* Create your own branch and work independently on it.
* Once you think your work is done, inform the team. 
* **Don't merge or commit to the master branch!**
* Please follow Google CPP guidelines while writing the code. 
* Try incorporating OOP concepts in the code right from the start because we will be building next projects on the top of this project. 
* This is not trivial so please start working on it as soon as possible.
