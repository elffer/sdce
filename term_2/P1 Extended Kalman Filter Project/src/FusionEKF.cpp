#include "FusionEKF.h"
#include <iostream>
#include "Eigen/Dense"
#include "tools.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;
using std::cout;
using std::endl;
using std::vector;

/**
 * Constructor.
 */
FusionEKF::FusionEKF() {
  
  is_initialized_ = false;

  previous_timestamp_ = 0;
  

  // initializing matrices
  R_laser_ = MatrixXd(2, 2);
  R_radar_ = MatrixXd(3, 3);
  H_laser_ = MatrixXd(2, 4);
  Hj_ = MatrixXd(3, 4);

  
  //measurement covariance matrix - laser
  R_laser_ << 0.0225, 0,
              0, 0.0225;
  
  //measurement covariance matrix - radar
  R_radar_ << 0.09, 0, 0,
              0, 0.0009, 0,
              0, 0, 0.09;
  

  /**
   * Initializing the FusionEKF.
   * Set the process and measurement noises
   */

  // create a 4D state vector
  ekf_.x_ = VectorXd(4);
  
  // state covariance matrix P
  ekf_.P_ = MatrixXd(4, 4);
  ekf_.P_ << 1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 330, 0,
        0, 0, 0, 330;

  // measurement matrix
  H_laser_ << 1, 0, 0, 0,
              0, 1, 0, 0;

  // state transition matrix
  ekf_.F_ = MatrixXd(4, 4);
  ekf_.F_ << 1, 0, 1, 0,
        0, 1, 0, 1,
        0, 0, 1, 0,
        0, 0, 0, 1;

  ekf_.Q_ = MatrixXd(4, 4);
  ekf_.Q_ <<  0, 0, 0, 0,
         0, 0, 0, 0,
         0, 0, 0, 0,
         0, 0, 0, 0;

  // set the acceleration noise components
  noise_ax = 22;
  noise_ay = 22;

}

/**
 * Destructor.
 */
FusionEKF::~FusionEKF() {}

void FusionEKF::ProcessMeasurement(const MeasurementPackage &measurement_pack) {
  /**
   * Initialization
   */
  if (!is_initialized_) {
    /**
     * Initialize the state ekf_.x_ with the first measurement.
     * Create the covariance matrix.
     * You'll need to convert radar from polar to cartesian coordinates.
     */

    // first measurementat the Kalman filter equations and extended Kalman filter equations are very similar. The main differences are:
    cout << "EKF: " << endl;

    if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR) {
      // Convert radar from polar to cartesian coordinates 
      //         and initialize state.
      std::cout << "RADAR" << std::endl;
      float rho;
      float phy;
      
      rho = measurement_pack.raw_measurements_[0];
      phy = measurement_pack.raw_measurements_[1];
      
      ekf_.x_ << rho*cosf(phy),
                rho*sinf(phy),
                0,
                0;
    }
    else if (measurement_pack.sensor_type_ == MeasurementPackage::LASER) {
      // Initialize state.
        ekf_.x_ << measurement_pack.raw_measurements_[0], 
              measurement_pack.raw_measurements_[1], 
              0, 
              0;
    }

    // done initializing, no need to predict or update
    previous_timestamp_ = measurement_pack.timestamp_;
    is_initialized_ = true;
    return;
  }

  /**
   * Prediction
   */

  /**
   * Update the state transition matrix F according to the new elapsed time.
   * Time is measured in seconds.
   * Update the process noise covariance matrix.at the Kalman filter equations and extended Kalman filter equations are very similar. The main differences are:
   * Use noise_ax = 9 and noise_ay = 9 for your Q matrix.
   */
  float dt = (measurement_pack.timestamp_ - previous_timestamp_) / 1000000.0;
  previous_timestamp_ = measurement_pack.timestamp_;
  
  // 1. Modify the F matrix so that the time is integrated
  ekf_.F_(0, 2) = dt;
  ekf_.F_(1, 3) = dt;
  // 2. Set the process covariance matrix Q
  ekf_.Q_ = MatrixXd(4, 4);
  ekf_.Q_ << 0.25*pow(dt, 4)*noise_ax, 0, 0.5*pow(dt, 3)*noise_ax, 0,
           0, 0.25*pow(dt, 4)*noise_ay, 0, 0.5*pow(dt, 3)*noise_ay,
           0.5*pow(dt, 3)*noise_ax, 0, pow(dt, 2)*noise_ax, 0,
           0, 0.5*pow(dt, 3)*noise_ay, 0, pow(dt, 2)*noise_ay;  
           
  ekf_.Predict();

  /**
   * Update
   */

  /**
   * - Use the sensor type to perform the update step.
   * - Update the state and covariance matrices.
   */

  if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR) {
    // Radar updates
  Tools tools;
  Hj_ = tools.CalculateJacobian(ekf_.x_);
  ekf_.H_ = Hj_;
  ekf_.R_ = R_radar_;
  ekf_.UpdateEKF(measurement_pack.raw_measurements_);

  } else {

    // Laser updates
  ekf_.H_ = H_laser_;
  ekf_.R_ = R_laser_;
  ekf_.Update(measurement_pack.raw_measurements_);
  }

  // print the output  
  // cout << "x_ = " << ekf_.x_ << endl;
  // cout << "P_ = " << ekf_.P_ << endl;
}
