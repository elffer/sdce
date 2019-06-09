
# Model Documentation
------

**Note: The contends in this document is part of the "Project1_Reflection_Path_Planning.md"**

## 1. Inputs of the code model

The path planning is designed based on many sorts of information about different elements in the transportation system.

The information applied in the project include,
* Map of way points for the high way;
* Speed limit and lane numbers of the high way;
* The coordinates, driving direction, speed, current lane and previous path of the ego car;
* Sensor fusion information that provides coordinates and speeds of other cars in the high way.


## 2 Main steps / frame of the code model

To acheive the objectives of the project, the ego car would dectect other cars in the high way to keep safety and efficiency. If it detects that some car in front of it drives slow and might course collision, it will slow down and make a descision about whether to change lanes and which lane to change to.
After a descision been made, the ego car will generate a spline for lane change. Finally, sparse points on the spline would be selected as a list of locations in front of the ego car to approach.

### 2.1 Check the SAFETY and EFFICIENCY of the vehicle
The ego car will detect if any car in front of it is at a distance less than the safety requirement. If there is one, the ego car would set a flag to tell there is a car that is too close. The codes for detection can be found from line 119 through line 144 in the file 'main.cpp'.

### 2.2 Take actions / descision to keep SAFE and increase EFFICIENCY

Once there is a warn of 'too close', the ego car will reduce its speed and make a descision about lane change, as shown in line 151 through line 159 in the file 'main.cpp'. 

A new function was designed in the file 'helpers.h' named 'choose_next_state()' contributed to find the best next state, i.e., to change to another lane or simply keep lane. Detailed explaination has been provided with the code in 'helpers.h'. For convenience, the main approach is given as follows,
   * a). set up positive reward for each of the lane, i.e., the opposite view of cost function;
   * b). check each of the lanes, if it has car nearby the ego car at current time, or if it has car neaby the ego car in predicted future time, its reward will be reduced, i.e. set to zero in this function;
   * c). finally, the lane with the best reward will be selected.

### 2.3 Execute the speed / lane change descision with spline

Based on the descision given by the earlier step, the ego car will generate a trajectory to execute. The trajectory generation is developed based on three steps.

* Step a): the ego car would fetch and create widely spaced points (5 points applied, given by 'ptsx' and 'ptsy' in the file of 'main.cpp') to generate the spline, which is shown in line 168 through line 208;
* Step b): with the tool of spline and points selected above, a spline object would be created as shown in line 236 and line 239 in the file 'main.cpp';
* Step c): based on the target distance and the reference speed, future (x, y) points on the spline would be selected as tajectory, i.e., 'next_x_vals' and 'next_y_vals' in lines through line 252 to line 279 in 'main.cpp'.

### 3 Output of the code model

The outputs are the trajectory points the ego car going to approach soon.

