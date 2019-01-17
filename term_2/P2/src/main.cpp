#include <math.h>
//#include <uWS/uWS.h>
#include <iostream>
#include <fstream>
//#include "json.hpp"
//#include "FusionEKF.h"
#include "ukf.h"
#include "tools.h"


#include <sstream>
#include <vector>
#include "Eigen/Dense"
#include "measurement_package.h"
//#include "tracking.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;

using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::string;
using std::vector;


// New main

int main() {

  /**
   * Set Measurements
   */
  vector<MeasurementPackage> measurement_pack_list;
  

  // hardcoded input file with laser and radar measurements
  string in_file_name_ = "obj_pose-laser-radar-synthetic-input.txt";
  ifstream in_file(in_file_name_.c_str(), ifstream::in);

  if (!in_file.is_open()) {
    cout << "Cannot open input file: " << in_file_name_ << endl;
  }

  string line;
  // set i to get only first 3 measurments
  int i = 0;
  while (getline(in_file, line) && (i<10)) {

    MeasurementPackage meas_package;

    istringstream iss(line);
    string sensor_type;
    iss >> sensor_type; // reads first element from the current line
    int64_t timestamp;
    if (sensor_type.compare("L") == 0) {  // laser measurement
      // read Laser measurements
      meas_package.sensor_type_ = MeasurementPackage::LASER;
      meas_package.raw_measurements_ = VectorXd(2);
      float px;
      float py;
      iss >> px;
      iss >> py;
      meas_package.raw_measurements_ << px, py;
      iss >> timestamp;
      meas_package.timestamp_ = timestamp;
      measurement_pack_list.push_back(meas_package);
      std::cout << "L" << std::endl;

    } else if (sensor_type.compare("R") == 0) {
      // read Radar measurements
      
      meas_package.sensor_type_ = MeasurementPackage::RADAR;
      meas_package.raw_measurements_ = VectorXd(3);
      float rho;
      float phy;
      float drho;
      iss >> rho;
      iss >> phy;
      iss >> drho;
      meas_package.raw_measurements_ << rho, phy, drho;
      
      iss >> timestamp;
      meas_package.timestamp_ = timestamp;
      measurement_pack_list.push_back(meas_package);
      std::cout << "R" << std::endl;
      
    }
    ++i;
  }
  
  if (in_file.is_open()) {
    in_file.close();
  }
  
  
  // Create a Kalman Filter instance
//  std::cout << "Debuged Here !" << std::endl;
  UKF ukf;

  // call the ProcessingMeasurement() function for each measurement
  size_t N = measurement_pack_list.size();
 
  // start filtering from the second frame 
  // (the speed is unknown in the first frame)
//  std::cout << "Start" << std::endl;
  for (size_t k = 0; k < N; ++k) {
//    std::cout << "a" << std::endl;
    ukf.ProcessMeasurement(measurement_pack_list[k]);
    
  }

//  std::cout << "The END !" << std::endl;
  return 0;

}