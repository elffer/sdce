#include <math.h>
#include <uWS/uWS.h>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include "Eigen-3.3/Eigen/Core"
#include "Eigen-3.3/Eigen/QR"
#include "helpers.h"
#include "json.hpp"
#include "MPC.h"

// for convenience
using nlohmann::json;
using std::string;
using std::vector;
using Eigen::VectorXd;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

int main() {
  uWS::Hub h;

  // MPC is initialized here!
  MPC mpc;

  h.onMessage([&mpc](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length,
                     uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    string sdata = string(data).substr(0, length);
    //std::cout << sdata << std::endl;
    if (sdata.size() > 2 && sdata[0] == '4' && sdata[1] == '2') {
      string s = hasData(sdata);
      if (s != "") {
        auto j = json::parse(s);
        string event = j[0].get<string>();

        // ************* Part I: Fetch State Info from Simulator ***********//
        if (event == "telemetry") {
          // j[1] is the data JSON object
          VectorXd ptsx(6);
          VectorXd ptsy(6);
          double px = j[1]["x"];
          double py = j[1]["y"];
          double psi = j[1]["psi"];
          double v = j[1]["speed"];
          v = v * 0.447;
          double steer_angle = j[1]["steering_angle"];  // steering angle
          double acceleration = j[1]["throttle"];
     
          // transfer reference points to the coordinate of the vehicle (sdc)
          for(int i=0; i < 6; i++){
             double ptsx_i = j[1]["ptsx"][i];
             double ptsy_i = j[1]["ptsy"][i];
             double dx = ptsx_i - px;
             double dy = ptsy_i - py;

             ptsx[i] = cos(-psi) * dx - sin(-psi) * dy;
             ptsy[i] = sin(-psi) * dx + cos(-psi) * dy;
          }

          // derive the coeffs of the line to predict
          auto coeffs = polyfit(ptsx, ptsy, 3);

          // calculate the cross track error and the orientation error
          auto cte = coeffs[0];

          // calculate the orientation error
          double epsi = - atan(coeffs[1]);

          // consider latency in the predictive model
          const double latency = LATENCY;
          const double Lf = LF;
          double px_real = v * cos(0) * latency;
          double py_real = v * sin(0) * latency;
          double psi_real = - v * steer_angle * latency / Lf;
          double v_real = v + acceleration * latency;
          double cte_real = cte + v * sin(epsi) *latency;
          double epsi_real = epsi + psi_real;

          VectorXd state(6);
          state << px_real, py_real, psi_real, v_real, cte_real, epsi_real;

          // *************** Part II: Sove the Optimization Problem *************//
          /**
           * Calculate steering angle and throttle using MPC.
           * Both are in between [-1, 1].
           */
          auto vars = mpc.Solve(state, coeffs);

          // ********** Part III: Fetch Optimized Steering and Throttle  *********//
          double steer_value;
          double throttle_value;

          steer_value = vars[0] / deg2rad(25);
          throttle_value = vars[1];

          json msgJson;
          // NOTE: Remember to divide by deg2rad(25) before you send the 
          //   steering value back. Otherwise the values will be in between 
          //   [-deg2rad(25), deg2rad(25] instead of [-1, 1].
          msgJson["steering_angle"] = -steer_value;
          msgJson["throttle"] = throttle_value;
          std::cout << "steering_angle = " << -steer_value << std::endl;
          std::cout << "throttle = " << throttle_value << std::endl;

          // ****************** Part IV: Set Predicted Line ******************//
          // Display the MPC predicted trajectory 
          vector<double> mpc_x_vals;
          vector<double> mpc_y_vals;

          mpc_x_vals = mpc.pred_x;
          mpc_y_vals = mpc.pred_y;

          /**
           * Add (x,y) points to list here, points are in reference to 
           *   the vehicle's coordinate system the points in the simulator are 
           *   connected by a Green line
           */

          msgJson["mpc_x"] = mpc_x_vals;
          msgJson["mpc_y"] = mpc_y_vals;


          // ****************** Part V: Set Reference Line ******************//
          // Display the waypoints/reference line
          vector<double> next_x_vals;
          vector<double> next_y_vals;

          /**
           * Add (x,y) points to list here, points are in reference to 
           *   the vehicle's coordinate system the points in the simulator are 
           *   connected by a Yellow line
           */
          for(int i=0; i < ptsx.size(); i++){
             next_x_vals.push_back(ptsx[i]);
             next_y_vals.push_back(ptsy[i]);
          }

          msgJson["next_x"] = next_x_vals;
          msgJson["next_y"] = next_y_vals;


          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          //std::cout << msg << std::endl;
          // Latency
          // The purpose is to mimic real driving conditions where
          //   the car does actuate the commands instantly.
          //
          // Feel free to play around with this value but should be to drive
          //   around the track with 100ms latency.
          //
          // NOTE: REMEMBER TO SET THIS TO 100 MILLISECONDS BEFORE SUBMITTING.
          std::this_thread::sleep_for(std::chrono::milliseconds(100));
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }  // end "telemetry" if
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }  // end websocket if
  }); // end h.onMessage

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code,
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  
  h.run();
}
