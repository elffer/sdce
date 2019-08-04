
# Capstone Project
------

[image1]: ./imgs/cycling_record.png "cycling_images"
[image2]: ./imgs/diagram.png "cycling_images"


This is the Capstone project for the Udacity Self-Driving Car Nanodegree. I developed codes to guide a real self-driving car around a test track. Using the Robot Operating System (ROS), I created nodes for traffic light detection and classification, trajectory planning, and control.


### Individual Submission
---

Name 				| Udacity Account
---------------- | ---------------------
Weigang Liu | bmwlwg@163.com


### Towards the Requirements

* Stop and restart PID controllers depending on the state of '/vehicle/dbw_enabled'.
* Launch correctly using the launch files provided in the capstone repo.
* Smoothly follow waypoints in the simulator.
* Respect the target top speed set for the waypoints 'twist.twist.linear.x' in 'waypoint_loader.py'. The velocity of the vehicle is limited to under 40 kph.
* Stop at traffic lights when needed.
* Publish throttle, steering, and brake commands at 50hz.
* Applied SSD MobileNet architecture for traffic light detection.
* As shown in the following figure, in **simulation testing**, the vehicle can drive to the end of the road.
* For **real world testing**, 'traffic_light_bag_files/loop_with_traffic_light.bag' has been applied to boradcast the rostopic of '/image_color'. The 'tl_detector' node can classify the images and works on real life images.


*Note: Find the latest version of this project on
[Github](https://github.com/elffer/sdce/CarND-Capstone).*



### Contents

* [Project Components](#Project-Components)
  * [Project Diagram](#Project-Diagram)
  * [Visualization](#Visualization)
  * [Traffic Light Detection](#Traffic-Light-Detection)
  * [Waypoint Updater](#Waypoint-Updater)
  * [Controller](#Controller)


* [Usage](#usage)
  * [Native installation](#Native-Installation)
  * [Docker Installation](#Docker-Installation)
  * [Simulation testing](#Simulation-Testing)
  * [Real world testing](#Real-World-Testing)

  
* [Trubleshooting](#Troubleshooting)

---

### Project Components

#### Project Diagram
* diagram of nodes and messages:

![diagram of nodes][image2]

#### Visualization

Here are images showing the vehicle driving at different sections of the road, such as stop at the red light,  passing the green light and stop at the final waypoint.

![Cycling][image1]

#### Traffic Light Detection

* The TensorFlow Object Detection API has been applied for training and inference.
* Separate models for simulator and Carla 
* Four output classes from light classification: GREEN, YELLOW, RED, NONE.
* See inference code in [light_classification/tl_classifier.py](https://github.com/elffer/sdce/blob/master/term_3/P3%20CarND-Capstone%20Project/ros/src/tl_detector/light_classification/tl_classifier.py)

#### Waypoint Updater

* The main purpose of this node is to update the base waypoints published by the node of 'waypoint loader.py'
* This node updates the velocity of the waypoints according to their distance to the stop line.
* To improve the searching efficiency, K-D tree has been applied to store the waypoints
* See code in [waypoint_updater.py](https://github.com/elffer/sdce/blob/master/term_3/P3%20CarND-Capstone%20Project/ros/src/waypoint_updater/waypoint_updater.py).

#### Controller

* The controlling commands include the commands regarding the braking, steering and throttle for acceleration.
* PID controller was applied for controlling the throttle value.
* The controlling commands are published at the rate of 50Hz.
* See code in [dbw_node.py](https://github.com/elffer/sdce/blob/master/term_3/P3%20CarND-Capstone%20Project/ros/src/twist_controller/dbw_node.py).


---

## Installation and usage of project code


Please use **one** of the two installation options, either native **or** docker installation.


### Native Installation

* Be sure that your workstation is running Ubuntu 16.04 Xenial Xerus or Ubuntu 14.04 Trusty Tahir. [Ubuntu downloads can be found here](https://www.ubuntu.com/download/desktop).
* If using a Virtual Machine to install Ubuntu, use the following configuration as minimum:
  * 2 CPU
  * 2 GB system memory
  * 25 GB of free hard drive space

  The Udacity provided virtual machine has ROS and Dataspeed DBW already installed, so you can skip the next two steps if you are using this.

* Follow these instructions to install ROS
  * [ROS Kinetic](http://wiki.ros.org/kinetic/Installation/Ubuntu) if you have Ubuntu 16.04.
  * [ROS Indigo](http://wiki.ros.org/indigo/Installation/Ubuntu) if you have Ubuntu 14.04.
* [Dataspeed DBW](https://bitbucket.org/DataspeedInc/dbw_mkz_ros)
  * Use this option to install the SDK on a workstation that already has ROS installed: [One Line SDK Install (binary)]
* Download the [Udacity Simulator](https://github.com/udacity/CarND-Capstone/releases).

### Docker Installation
[Install Docker](https://docs.docker.com/engine/installation/)

Build the docker container
```bash
docker build . -t capstone
```

Run the docker file
```bash
docker run -p 4567:4567 -v $PWD:/capstone -v /tmp/log:/root/.ros/ --rm -it capstone
```


### Port Forwarding
To set up port forwarding, please refer to the "uWebSocketIO Starter Guide" found in the classroom (see Extended Kalman Filter Project lesson).


### Simulation Testing

1. Clone the project repository
```bash
git clone https://github.com/udacity/CarND-Capstone.git
```

2. Install python dependencies
```bash
cd CarND-Capstone
pip install -r requirements.txt
```
3. Make and run styx
```bash
cd ros
catkin_make
source devel/setup.sh
roslaunch launch/styx.launch
```
4. Run the simulator

### Real World Testing
1. Download [training bag](https://s3-us-west-1.amazonaws.com/udacity-selfdrivingcar/traffic_light_bag_file.zip) that was recorded on the Udacity self-driving car.
2. Unzip the file
```bash
unzip traffic_light_bag_file.zip
```
3. Play the bag file
```bash
rosbag play -l traffic_light_bag_file/traffic_light_training.bag
```
4. Launch your project in site mode
```bash
cd CarND-Capstone/ros
roslaunch launch/site.launch
```
5. Confirm that traffic light detection works on real life images


## Troubleshooting

Troubleshooting takes me much time to get my codes work with the simulator. The section of **knowledge** from the course provides lots of helpful advices. 

#### 1. 'time out' of socketio
There are many different reasons that could lead to the problem of 'time out' from socketio. Here are the situations I encountered during testing.
* Installing the python packages referring to the 'requirements.txt is not enough for my case. The program can still cause the error of 'time out', even if the camera camera is not turned on. As suggested by the forum of **knowledge**, updating the package of 'python-socketio' to the newest version would solve the problem.
* The computing capability of laptop is not powerful enough. In the early start, I used a Thinkpad T420, purchased in 2012, to run the project. The program ran successively with the camera turned off. And the vehicle can drive to the final point. However, when the camera was turned on, the program turned out to output the error of time out. It showed that the CPU has been fully occupied. Then I made it out with an advanced laptop with better CPU and GPU.
* Other packages listed in the file of 'requirement.txt' should be installed with the suggested versions.
* Check if the package used is under the right environment. 'pip' might install a python package to an environment which is not used to run the program. Moreover, the command of 'import socketio' might use socketio from the package of 'socketio', rather than 'python-socketio'.

#### 2. swinging of vehicle

Even if the socketio has been applied correctly,  the vehicle might still swing and run of the road. For my case, it was caused by the computing power of the laptop. A useful solution is to reduce the rate in [waypoint_updater.py](https://github.com/elffer/sdce/blob/master/term_3/P3%20CarND-Capstone%20Project/ros/src/waypoint_updater/waypoint_updater.py).

#### 3. can't locate node [server.py] in package [styx]

This happened when I ran my codes in the workspace of the course. The reason is that some files had been limited by permissions. As suggested by the course material, 'if you are uploading your own previous work, be sure to make all Python files executable.' The command of ''chmod -R +x /src'' is quite useful.

### References

- [https://github.com/marcomarasca/SDCND-Traffic-Light-Detection](https://github.com/marcomarasca/SDCND-Traffic-Light-Detection)
- [https://becominghuman.ai/traffic-light-detection-tensorflow-api-c75fdbadac62](https://becominghuman.ai/traffic-light-detection-tensorflow-api-c75fdbadac62)
