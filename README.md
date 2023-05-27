# Sensored-BLDC-Control
Deep Reinforcement Learning for mobile robot navigation in ROS Gazebo simulator. Using Twin Delayed Deep Deterministic Policy Gradient (TD3) neural network, a robot learns to navigate to a random goal point in a simulated environment while avoiding obstacles. Obstacles are detected by laser readings and a goal is given to the robot in polar coordinates. Trained in ROS Gazebo simulator with PyTorch. Tested with ROS Noetic on Ubuntu 20.04 with python 3.8.10 and pytorch 1.10.

Installation and code overview tutorial available here

Training example:



ICRA 2022 and IEEE RA-L paper:

Some more information about the implementation is available here

Please cite as:

@ARTICLE{9645287,
  author={Cimurs, Reinis and Suh, Il Hong and Lee, Jin Han},
  journal={IEEE Robotics and Automation Letters}, 
  title={Goal-Driven Autonomous Exploration Through Deep Reinforcement Learning}, 
  year={2022},
  volume={7},
  number={2},
  pages={730-737},
  doi={10.1109/LRA.2021.3133591}}
Installation
Main dependencies:

ROS Noetic
PyTorch
Tensorboard
Clone the repository:

$ cd ~
### Clone this repo
$ git clone https://github.com/reiniscimurs/DRL-robot-navigation
The network can be run with a standard 2D laser, but this implementation uses a simulated 3D Velodyne sensor

Compile the workspace:

$ cd ~/DRL-robot-navigation/catkin_ws
### Compile
$ catkin_make_isolated
Open a terminal and set up sources:

$ export ROS_HOSTNAME=localhost
$ export ROS_MASTER_URI=http://localhost:11311
$ export ROS_PORT_SIM=11311
$ export GAZEBO_RESOURCE_PATH=~/DRL-robot-navigation/catkin_ws/src/multi_robot_scenario/launch
$ source ~/.bashrc
$ cd ~/DRL-robot-navigation/catkin_ws
$ source devel_isolated/setup.bash
Run the training:

$ cd ~/DRL-robot-navigation/TD3
$ python3 train_velodyne_td3.py
To check the training process on tensorboard:

$ cd ~/DRL-robot-navigation/TD3
$ tensorboard --logdir runs
To kill the training process:

$ killall -9 rosout roslaunch rosmaster gzserver nodelet robot_state_publisher gzclient python python3
Once training is completed, test the model:

$ cd ~/DRL-robot-navigation/TD3
$ python3 test_velodyne_td3.py
Gazebo environment:



Rviz:


