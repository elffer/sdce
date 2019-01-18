#include "ukf.h"
#include <iostream>
#include "Eigen/Dense"

using Eigen::MatrixXd;
using Eigen::VectorXd;

/**
 * Initializes Unscented Kalman filter
 */
UKF::UKF() {
  is_initialized_ = false;
  time_us_ = 0;
    
  // if this is false, laser measurements will be ignored (except during init)
  use_laser_ = true;

  // if this is false, radar measurements will be ignored (except during init)
  use_radar_ = true;

  // initial state vector
  x_ = VectorXd(5);

  // initial covariance matrix
  P_ = MatrixXd(5, 5);

  // Process noise standard deviation longitudinal acceleration in m/s^2
  std_a_ = 30;

  // Process noise standard deviation yaw acceleration in rad/s^2
  std_yawdd_ = 30;
  
  /**
   * DO NOT MODIFY measurement noise values below.
   * These are provided by the sensor manufacturer.
   */

  // Laser measurement noise standard deviation position1 in m
  std_laspx_ = 0.15;

  // Laser measurement noise standard deviation position2 in m
  std_laspy_ = 0.15;

  // Radar measurement noise standard deviation radius in m
  std_radr_ = 0.3;

  // Radar measurement noise standard deviation angle in rad
  std_radphi_ = 0.03;

  // Radar measurement noise standard deviation radius change in m/s
  std_radrd_ = 0.3;
  
  /**
   * End DO NOT MODIFY section for measurement noise values 
   */
  
  /**
   * TODO: Complete the initialization. See ukf.h for other member properties.
   * Hint: one or more values initialized above might be wildly off...
   */

   x_ << 1, 1, 1, 1, 1;
   P_ << 0.0043,   -0.0013,    0.0030,   -0.0022,   -0.0020,
        -0.0013,    0.0077,    0.0011,    0.0071,    0.0060,
        0.0030,    0.0011,    0.0054,    0.0007,    0.0008,
        -0.0022,    0.0071,    0.0007,    0.0098,    0.0100,
        -0.0020,    0.0060,    0.0008,    0.0100,    0.0123;

  n_x_ = 5;
  n_aug_ = 7;
  lambda_ = 3 - n_aug_;
  
  // set vector for weights
  VectorXd weights_ = VectorXd(2*n_aug_+1);
  double weight_0 = lambda_ / (lambda_+n_aug_);
  double weight = 0.5/(lambda_+n_aug_);
  weights_(0) = weight_0;

  for (int i=1; i<2*n_aug_+1; ++i) {  
    weights_(i) = weight;
  }
}

UKF::~UKF() {}

void UKF::ProcessMeasurement(MeasurementPackage meas_package) {
  /**
   * TODO: Complete this function! Make sure you switch between lidar and radar
   * measurements.
   */
  /*
   * Initialization
   */
  if(!is_initialized_){
//      std::cout << "Not initialized yet." << std::endl;
      if(meas_package.sensor_type_ == MeasurementPackage::RADAR){
          std::cout << "It's a radar signal." << std::endl;
          float rho;
          float phy;
          float drho;
          
          rho = meas_package.raw_measurements_[0];
          phy = meas_package.raw_measurements_[1];
          drho = meas_package.raw_measurements_[2];
          
          x_ << rho*cosf(phy),
                rho*sinf(phy),
                0,
                0,
                0;
          }
          else if(meas_package.sensor_type_ == MeasurementPackage::LASER){
          x_ << meas_package.raw_measurements_[0],
                meas_package.raw_measurements_[1],
                0,
                0,
                0; 
          }
    
  // Finised initializing.
  time_us_ = meas_package.timestamp_;
  is_initialized_ = true;
  
  std::cout << "Finished initialization!" << std::endl;
  return;
      }
      
  /* Prediction */
  float dt = (meas_package.timestamp_ - time_us_)/1000000.0;
  time_us_ = meas_package.timestamp_;
  
  Prediction(dt);
  
}

void UKF::Prediction(double delta_t) {
  /**
   * TODO: Complete this function! Estimate the object's location. 
   * Modify the state vector, x_. Predict sigma points, the state, 
   * and the state covariance matrix.
   */
   // Create augmented mean vector
   VectorXd x_aug_ = VectorXd(n_aug_);
   
   // Create augmented state covariance
   MatrixXd P_aug_ = MatrixXd(n_aug_, n_aug_);
   
   // Create sigma point matrix
   MatrixXd Xsig_pred_ = MatrixXd(n_aug_, 2*n_aug_+1);
   
   // Create augmented mean state
   x_aug_.head(n_x_) = x_;
   x_aug_(n_aug_-1) = 0;
   x_aug_(n_aug_-2) = 0;
   
}

void UKF::UpdateLidar(MeasurementPackage meas_package) {
  /**
   * TODO: Complete this function! Use lidar data to update the belief 
   * about the object's position. Modify the state vector, x_, and 
   * covariance, P_.
   * You can also calculate the lidar NIS, if desired.
   */
}

void UKF::UpdateRadar(MeasurementPackage meas_package) {
  /**
   * TODO: Complete this function! Use radar data to update the belief 
   * about the object's position. Modify the state vector, x_, and 
   * covariance, P_.
   * You can also calculate the radar NIS, if desired.
   */
}