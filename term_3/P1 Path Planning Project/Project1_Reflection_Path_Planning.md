
# Project Reflection: Path Planning
------

[image1]: ./images/change_lanes_1.png "change_lanes_1"
[image2]: ./images/change_lanes_2.png "change_lanes_2"
[image3]: ./images/change_lanes_3.png "change_lanes_3"

[image4]: ./images/high_speed_1.png "high_speed_1"
[image5]: ./images/high_speed_2.png "high_speed_2"
[image6]: ./images/high_speed_3.png "high_speed_3"

[image7]: ./images/no_incident_range.png "no_incident_range.png"

**This reflection will be presented with the following key points regarding to the project.**

* The **objects** to achieve;
* The main **frame** applied to meet the objects;
* How the coding meet the requirements of the **rubics**;
* **Reflections** about how to improve;


## 1. The objectives achieved
------

To design a good path planning algorithm, the ego car would be designed to persuit the objectives of safety, legacy, feasibility, comfort and efficency.
The detailed objectives of the project include:
* [Legacy] The ego car will go along the lane center;
* [Legacy] The ego car should drive with a speed under the limit of 50mph;
* [Comfort] The ego car should avoid jerk;
* [Safety][Legacy] The ego car should avoid collision;
* [Efficiency] The ego car can change lanes when other cars in front of it drive slowly;
* [Efficiency] Besides of meeting the other four objectives, the ego car should try to drive fast for efficency;
* [Feasibility] Control the acceleration in the range of throttle.

## 2. The frame of the project coding
------

### 2.1 Input

The path planning is designed based on many sorts of information about different elements in the transportation system.

The information applied in the project include,
* Map of way points for the high way;
* Speed limit and lane numbers of the high way;
* The coordinates, driving direction, speed, current lane and previous path of the ego car;
* Sensor fusion information that provides coordinates and speeds of other cars in the high way.


### 2.2 Main steps

To acheive the objectives of the project, the ego car would dectect other cars in the high way to keep safety and efficiency. If it detects that some car in front of it drives slow and might course collision, it will slow down and make a descision about whether to change lanes and which lane to change to.
After a descision been made, the ego car will generate a spline for lane change. Finally, sparse points on the spline would be selected as a list of locations in front of the ego car to approach.

#### 2.2.1 Check the SAFETY and EFFICIENCY of the car
The ego car will detect if any car in front of it is at a distance less than the safety requirement. If there is one, the ego car would set a flag to tell there is a car that is too close. The codes for detection can be found from line 119 through line 144 in the file 'main.cpp'.

#### 2.2.2 Take actions and make descision to keep SAFE and increase EFFICIENCY

Once there is a warn of 'too close', the ego car will reduce its speed and make a descision about lane change, as shown in line 151 through line 159 in the file 'main.cpp'. 

A new function was designed in the file 'helpers.h' named 'choose_next_state()' contributed to find the best next state, i.e., to change to another lane or simply keep lane. Detailed explaination has been provided with the code in 'helpers.h'. For convenience, the main approach is given as follows,
   * a). set up positive reward for each of the lane, i.e., the opposite view of cost function;
   * b). check each of the lanes, if it has car nearby the ego car at current time, or if it has car neaby the ego car in predicted future time, its reward will be reduced, i.e. set to zero in this function;
   * c). finally, the lane with the best reward will be selected.

#### 2.2.3 Execute the speed / lane change descision with spline

Based on the descision given by the earlier step, the ego car will generate a trajectory to execute. The trajectory generation is developed based on three steps.

* Step a): the ego car would fetch and create widely spaced points (5 points applied, given by 'ptsx' and 'ptsy' in the file of 'main.cpp') to generate the spline, which is shown in line 168 through line 208;
* Step b): with the tool of spline and points selected above, a spline object would be created as shown in line 236 and line 239 in the file 'main.cpp';
* Step c): based on the target distance and the reference speed, future (x, y) points on the spline would be selected as tajectory, i.e., 'next_x_vals' and 'next_y_vals' in lines through line 252 to line 279 in 'main.cpp'.

### 2.3 Output

The outputs are the trajectory points the ego car going to approach soon.



## 3. How does the code meets the requiments of the rubic
------

### 3.1 Compilation

**The code compiles correctly.**

Ans: The code can be compiled without errors with cmake and make.

### 3.2 Valid Trajectories

**The car is able to drive at least 4.32 miles without incident..**

Ans: The car can drive without any incident far more than 8 miles. With the submitted version of code, the ego car can go without any incident as long as the simulator is not turned off.

![No_incident][image7]


**The car drives according to the speed limit.**

Ans: The ego car drives within the speed limit in all the conditions. If it is obstructed by traffic, it will reduce its speed to avoid collision and seek if it is better to change lanes. Otherwise, if it is not obstructed by traffic, it will gradually increase its speed to a value near the speed limit, i.e., 49.5mph in the code.

![Speed limit][image4]
![Speed limit][image5]
![Speed limit][image6]

**Max Acceleration and Jerk are not Exceeded.**

Ans: With the application of spline, the ego car does not exceed the acceleration and jerk requirements. 

**Car does not have collisions.**

Ans: The car does not contact with any of the other cars on the road.

**The car stays in its lane, except for the time between changing lanes.**

Ans: The ego car always stays in its lane unless it decides to change lanes.

**The car is able to change lanes**

Ans: The ego car is able to change lanes when it is obstructed by other cars in its lane and it is safe to change lanes.

![Change lane][image1]
![Change lane][image2]
![Change lane][image3]


### 3.3 Reflection

**There is a reflection on how to generate paths.**

Ans: A detailed reflection on the code model has been provided in the part 2 above, i.e., '**2. The frame of the project coding**'. 

As required, I have made a copy of part 2 as 'Model Documentation'.


## 4. Further improvements
------

The function of 'choose_next_state()' in the file 'helpers.h' can be improved by considering the speed of cars in other lanes, and more flexible setting of rewards can be applied to include various factors in transportation. Meanwhile, the prediction of other cars in the function is designed based on a liner model. It might be challenged by situations that cars driving behind might accelerate to course a collision. In that case, more complicated models should be applied to avoid collision.
