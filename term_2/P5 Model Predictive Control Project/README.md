# CarND-Controls-MPC
Self-Driving Car Engineer Nanodegree Program

[image1]: Running.png "Visualization"

---
## Sumarize of the Codes

### Compilation

* The code can be compiled and run with the simulator;


### The Model

The main proccedures of the model are given as follows.

* 1. Fetch state infomations from the simulator, which include point positions of the middle line of the road, the xy postions, the orientation, speed, steering and throttle value of the vehicle.
* 2. Fit the waypoints with a three-order curve.
* 3. Transform the state information to the vehicle's coordinate system.
* 4. Solve and derive the optimized steering and throttle value to control the vehicle, with the transformed information and the fitting curve as input.
* 5. Output the steering and throttle value as actuators to the simulator for controlling the vehicle.

The optimization problem is solved with the IPOPT solver. Similar to general optimization problems, the frame includes the objective (cost) expresstion, the constraints and variables.

* **Cost Expression**: To achieve a good driving performance, the cost expression includs the cost of cross track error, the devation of the orientation, the dramatic changes of steering and acceleration. To avoide the problem of stopping, the reference speed is introduced the cost expression. The codes can be found from line 63 through line 89 in 'MPC.cpp'.
* **Constraints**: The constraints take two sorts of limitations into consider, i.e., the initial state of the vehicle and the updates of the intermediate states, as shown from line 95 through 134 in 'MPC.cpp'. Especially, the new state (state at time t+1) is derived from its pervious state (state at time t) by adding the variations. For example, 'v1 = v0 + a0 * (dt + latency) ' shows 'v1' can be derived from 'v0' by adding the change caused by 'a0' and the duration of time, 'dt+latency'.  Note that, in IPOPT, the constraints of equations are implemented by setting the lower bounds and the upper bounds with the same value, i.e., 0.
* **Variables**: The variables in IPOPT should be set as a vector. In this model, the varialbes are the states and the acutator values, all of which are included in the vector. Its length is 'N * state.size() + (N - 1) * 2', which can be found from line 150 through line 213 in 'MPC.cpp'. The lower and upper bounds are set in this part. 


### Timestep Length and Elapsed Duration

The change of timestep and duration values will greatly affect the performance of the MPC model.
* The various combinations have different performance on fitting the curve of the reference line. If the combination tries to fit a long section of the lap, it would be likely to deviate from the nearest part of the curve. On the other hand, if the combination tries a short section of the lap, the points on the limited length will mislead the orientation of the road as well.
* The elapsed duration is limited to providing a reasonable time for controlling the steering and acceleration. A large value of 'dt' might increase the cross track error and deviation of orientation, while a small value of 'dt' might limit the vehicle's ability for adjustment.
* During developing, I have tried the set of (N, dt) with (20, 0.05), (20, 0.1), (20, 0.15), (20, 0.2), (10, 0.1), (10, 0.15), (10, 0.2) and some other random combinations. For this model, I found the one with (10, 0.15) presented the best performance.


### Polynomial Fitting and MPC Preprocessing

In the code, the polynomial is fitted to waypoints in 'main.cpp' as shown from line 47 to line 66. Some preprocessing are carried out for the MPC model.

* The vehicle state is transformed to the vehicle's coordinate.
* The values of actuators are included considering the impact of latency.

### Model Predictive Control with Latency

The codes have take the 100 millisecond latency into consideration. Techniques designed to overcome latency are implemented in two sections, i.e., MPC preprocessing and constraints of the MPC model.

* In MPC preprocessing, the state will change due to latency. Before transfered to the MPC model, the state is updated according to the latency. The codes can be found from line 78 through line 85 in 'main.cpp'.
* In the constraints of MPC model, the relation between a state and its previous state will be affected by the elapsed duration and the latency. It can be considered as extended elapsed duration. The codes can be found from line 127 through line 134 in 'MPC.cpp'.

### Simulation

The car can drive along the lab well. 

For the current version of codes, the vehicle can circulate with a speed of 30mph. To achieve higher speed with stable performance, some approachies can be implemented, such as tuning of (N&dt) and introducing weights for different objective expressions.

 ![Circulating][image1]


---
## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `install-mac.sh` or `install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.

* **Ipopt and CppAD:** Please refer to [this document](https://github.com/udacity/CarND-MPC-Project/blob/master/install_Ipopt_CppAD.md) for installation instructions.
* [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page). This is already part of the repo so you shouldn't have to worry about it.
* Simulator. You can download these from the [releases tab](https://github.com/udacity/self-driving-car-sim/releases).
* Not a dependency but read the [DATA.md](./DATA.md) for a description of the data sent back from the simulator.


## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./mpc`.

## Build with Docker-Compose
The docker-compose can run the project into a container
and exposes the port required by the simulator to run.

1. Clone this repo.
2. Build image: `docker-compose build`
3. Run Container: `docker-compose up`
4. On code changes repeat steps 2 and 3.

## Tips

1. The MPC is recommended to be tested on examples to see if implementation behaves as desired. One possible example
is the vehicle offset of a straight line (reference). If the MPC implementation is correct, it tracks the reference line after some timesteps(not too many).
2. The `lake_track_waypoints.csv` file has waypoints of the lake track. This could fit polynomials and points and see of how well your model tracks curve. NOTE: This file might be not completely in sync with the simulator so your solution should NOT depend on it.
3. For visualization this C++ [matplotlib wrapper](https://github.com/lava/matplotlib-cpp) could be helpful.)
4.  Tips for setting up your environment are available [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)
5. **VM Latency:** Some students have reported differences in behavior using VM's ostensibly a result of latency.  Please let us know if issues arise as a result of a VM environment.

## Editor Settings

We have kept editor configuration files out of this repo to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!

More information is only accessible by people who are already enrolled in Term 2
of CarND. If you are enrolled, see [the project page](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/f1820894-8322-4bb3-81aa-b26b3c6dcbaf/lessons/b1ff3be0-c904-438e-aad3-2b5379f0e0c3/concepts/1a2255a0-e23c-44cf-8d41-39b8a3c8264a)
for instructions and the project rubric.

## Hints!

* You don't have to follow this directory structure, but if you do, your work
  will span all of the .cpp files here. Keep an eye out for TODOs.

## Call for IDE Profiles Pull Requests

Help your fellow students!

We decided to create Makefiles with cmake to keep this project as platform
agnostic as possible. We omitted IDE profiles to ensure
students don't feel pressured to use one IDE or another.

However! I'd love to help people get up and running with their IDEs of choice.
If you've created a profile for an IDE you think other students would
appreciate, we'd love to have you add the requisite profile files and
instructions to ide_profiles/. For example if you wanted to add a VS Code
profile, you'd add:

* /ide_profiles/vscode/.vscode
* /ide_profiles/vscode/README.md

The README should explain what the profile does, how to take advantage of it,
and how to install it.

Frankly, I've never been involved in a project with multiple IDE profiles
before. I believe the best way to handle this would be to keep them out of the
repo root to avoid clutter. Most profiles will include
instructions to copy files to a new location to get picked up by the IDE, but
that's just a guess.

One last note here: regardless of the IDE used, every submitted project must
still be compilable with cmake and make./

## How to write a README
A well written README file can enhance your project and portfolio and develop your abilities to create professional README files by completing [this free course](https://www.udacity.com/course/writing-readmes--ud777).
