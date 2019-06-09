#ifndef HELPERS_H
#define HELPERS_H

#include <math.h>
#include <string>
#include <vector>


#include "Eigen-3.3/Eigen/Dense"

// for convenience
using std::string;
using std::vector;

using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::Vector3d;

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
//   else the empty string "" will be returned.
string hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_first_of("}");
  if (found_null != string::npos) {
    return "";
  } else if (b1 != string::npos && b2 != string::npos) {
    return s.substr(b1, b2 - b1 + 2);
  }
  return "";
}

//
// Helper functions related to waypoints and converting from XY to Frenet
//   or vice versa
//

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Calculate distance between two points
double distance(double x1, double y1, double x2, double y2) {
  return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

// Calculate closest waypoint to current x, y position
int ClosestWaypoint(double x, double y, const vector<double> &maps_x, 
                    const vector<double> &maps_y) {
  double closestLen = 100000; //large number
  int closestWaypoint = 0;

  for (int i = 0; i < maps_x.size(); ++i) {
    double map_x = maps_x[i];
    double map_y = maps_y[i];
    double dist = distance(x,y,map_x,map_y);
    if (dist < closestLen) {
      closestLen = dist;
      closestWaypoint = i;
    }
  }

  return closestWaypoint;
}

// Returns next waypoint of the closest waypoint
int NextWaypoint(double x, double y, double theta, const vector<double> &maps_x, 
                 const vector<double> &maps_y) {
  int closestWaypoint = ClosestWaypoint(x,y,maps_x,maps_y);

  double map_x = maps_x[closestWaypoint];
  double map_y = maps_y[closestWaypoint];

  double heading = atan2((map_y-y),(map_x-x));

  double angle = fabs(theta-heading);
  angle = std::min(2*pi() - angle, angle);

  if (angle > pi()/2) {
    ++closestWaypoint;
    if (closestWaypoint == maps_x.size()) {
      closestWaypoint = 0;
    }
  }

  return closestWaypoint;
}

// Transform from Cartesian x,y coordinates to Frenet s,d coordinates
vector<double> getFrenet(double x, double y, double theta, 
                         const vector<double> &maps_x, 
                         const vector<double> &maps_y) {
  int next_wp = NextWaypoint(x,y, theta, maps_x,maps_y);

  int prev_wp;
  prev_wp = next_wp-1;
  if (next_wp == 0) {
    prev_wp  = maps_x.size()-1;
  }

  double n_x = maps_x[next_wp]-maps_x[prev_wp];
  double n_y = maps_y[next_wp]-maps_y[prev_wp];
  double x_x = x - maps_x[prev_wp];
  double x_y = y - maps_y[prev_wp];

  // find the projection of x onto n
  double proj_norm = (x_x*n_x+x_y*n_y)/(n_x*n_x+n_y*n_y);
  double proj_x = proj_norm*n_x;
  double proj_y = proj_norm*n_y;

  double frenet_d = distance(x_x,x_y,proj_x,proj_y);

  //see if d value is positive or negative by comparing it to a center point
  double center_x = 1000-maps_x[prev_wp];
  double center_y = 2000-maps_y[prev_wp];
  double centerToPos = distance(center_x,center_y,x_x,x_y);
  double centerToRef = distance(center_x,center_y,proj_x,proj_y);

  if (centerToPos <= centerToRef) {
    frenet_d *= -1;
  }

  // calculate s value
  double frenet_s = 0;
  for (int i = 0; i < prev_wp; ++i) {
    frenet_s += distance(maps_x[i],maps_y[i],maps_x[i+1],maps_y[i+1]);
  }

  frenet_s += distance(0,0,proj_x,proj_y);

  return {frenet_s,frenet_d};
}

// Transform from Frenet s,d coordinates to Cartesian x,y
vector<double> getXY(double s, double d, const vector<double> &maps_s, 
                     const vector<double> &maps_x, 
                     const vector<double> &maps_y) {
  int prev_wp = -1;

  while (s > maps_s[prev_wp+1] && (prev_wp < (int)(maps_s.size()-1))) {
    ++prev_wp;
  }

  int wp2 = (prev_wp+1)%maps_x.size();

  double heading = atan2((maps_y[wp2]-maps_y[prev_wp]),
                         (maps_x[wp2]-maps_x[prev_wp]));
  // the x,y,s along the segment
  double seg_s = (s-maps_s[prev_wp]);

  double seg_x = maps_x[prev_wp]+seg_s*cos(heading);
  double seg_y = maps_y[prev_wp]+seg_s*sin(heading);

  double perp_heading = heading-pi()/2;

  double x = seg_x + d*cos(perp_heading);
  double y = seg_y + d*sin(perp_heading);

  return {x,y};
}

// Elffer defined:

/**
 * Function: Jerk minimizing trajectory
 */
vector<double> JMT(vector<double> &start, vector<double> &end, double T) {
  /**
   * Calculate the Jerk Minimizing Trajectory that connects the initial state
   * to the final state in time T.
   *
   * @param start - the vehicles start location given as a length three array
   *   corresponding to initial values of [s, s_dot, s_double_dot]
   * @param end - the desired end state for vehicle. Like "start" this is a
   *   length three array.
   * @param T - The duration, in seconds, over which this maneuver should occur.
   *
   * @output an array of length 6, each value corresponding to a coefficent in 
   *   the polynomial:
   *   s(t) = a_0 + a_1 * t + a_2 * t**2 + a_3 * t**3 + a_4 * t**4 + a_5 * t**5
   *
   * EXAMPLE
   *   > JMT([0, 10, 0], [10, 10, 0], 1)
   *     [0.0, 10.0, 0.0, 0.0, 0.0, 0.0]
   */
   Vector3d alpha2;
   Vector3d B(end[0] - (start[0]+start[1]*T+0.5*start[2]*T*T), end[1]-(start[1]+start[2]*T), end[2]-start[2]);
   
   MatrixXd A(3, 3); // coefficient matrix
   A << pow(T, 3), pow(T, 4), pow(T, 5), 
        3*pow(T, 2), 4*pow(T, 3), 5*pow(T, 4),
        6*T, 12*pow(T, 2), 20*pow(T, 3);
      
   // 线性方程求解 Ax =B;
   // Matrix4d A;
   // Vector4d x = A.colPivHouseholderQr().solve(B);
   alpha2 = A.colPivHouseholderQr().solve(B);
   
  return {start[0],start[1],start[2]/2,alpha2[0],alpha2[1],alpha2[2]};
}

/**
 * Function: Choose the best lane to change
 */
int choose_next_state(int cur_lane, double car_s, double car_d, double car_speed, double safty_range, int prev_size, const vector<vector<double>> &sensor_fusion){
    
  /**
   * Function: Find the best lane to change, which is safe and provide chance for ego vehicle to speed up.
   * Approach: 
   * 1. set up positive reward for each of the lane, i.e., the opposite view of cost function;
   * 2. check each of the lanes, if it has car nearby the ego vehicle at current time, or if it has car neaby the ego vehicle in predicted future time,
   *    its reward will be reduced, i.e. to zero in this function;
   * 3. Finally, the lane with the best reward will be selected.
   *
   * @param cur_lane - current lane of the ego vehicle;
   * @param safty_range - the gap being kept between ego vehicle and its nearby vehicle to keep safety;
   * @param prev_size - the lenth of array for saving previous states of the vehicles;
   * @param sensor_fusion - the location and speed information of orther vehicles in the road, provided by sensors;
   *
   * @output the best lane:
   */  

  int next_lane = cur_lane;
  vector<double> rewards;
  double best_reward = 0;
  vector<int> next_lanes;
  vector<int> options = {-1, 0, 1};
  
  // find optional lanes
  for(int i=0; i<options.size(); i++)
  {
   if((cur_lane+options[i]>=0) && (cur_lane+options[i]<=2))
   {
    next_lanes.push_back(options[i]);
    rewards.push_back(1);   
    }
   }
   
  // define the time gap to check the safty range in the future
  double dt = safty_range/car_speed;
  
  
  // update rewards according to safty
  for(int i=0; i<next_lanes.size(); i++)
  {
      double next_car_s = car_s + car_speed*dt;
      double next_car_d = car_d + 4*next_lanes[i];
      
      for(int j=0; j<sensor_fusion.size(); j++)
      {
          // filter out cars in the lane of ego car
          float d = sensor_fusion[j][6];
          if(d<(next_car_d+2) && d>(next_car_d-2))
          {
              double vx = sensor_fusion[j][3];
              double vy = sensor_fusion[j][4];
              double check_speed = sqrt(vx*vx+vy*vy);
              double check_car_s = sensor_fusion[j][5];
              
              // make a prediction of the sensored car
              check_car_s += ((double)prev_size*.02*check_speed);
              
              // update rewards according to safty in the near range
              // if any car in the lane is in front of the ego car 
              // and in the safety range, the reward would be reduced
              if((check_car_s>car_s) && (check_car_s-car_s)<safty_range)
              {
                rewards[i] *= 0;
               }
              // if any car in the lane is at the back of the ego car 
              // and in the safety range, the reward would be reduced
              if((check_car_s<car_s) && (car_s - check_car_s)<(safty_range/4))
              {
                rewards[i] *= 0;
               }               
              
              // compared the predicted positions of ego car and other cars in the lane
              // if their distance is smaller than safty range, the reward of the lane will be reduced
              check_car_s += ((double)dt*check_speed);              
              if((check_car_s>next_car_s) && (check_car_s-next_car_s)<safty_range)
              {
                rewards[i] *= 0;
               }

            }
        }
   }
   
   // find out the lane with the best reward
   for(int i=0; i<rewards.size(); i++)
   {
     if(rewards[i]>best_reward)
     {
      best_reward = rewards[i];
      next_lane = cur_lane +  next_lanes[i];
      }
    }
    
  return next_lane;
 }

#endif  // HELPERS_H