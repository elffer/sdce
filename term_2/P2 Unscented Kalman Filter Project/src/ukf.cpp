#include "ukf.h"
#include "tools.h"
#include "Eigen/Dense"
#include <iostream>

//using namespace std;
using Eigen::MatrixXd;
using Eigen::VectorXd;
//using std::vector;

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
  std_a_ = 1;

  // Process noise standard deviation yaw acceleration in rad/s^2
  std_yawdd_ = 0.6;
  
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

   x_ << 1, 1, 1, 1, 0.1;
   P_ << 0.15,   0,    0,   0,   0,
         0,   0.15,    0,   0,   0,
         0,   0,    1,   0,   0,
         0,   0,    0,   1,   0,
         0,   0,    0,   0,   1;

  n_x_ = 5;
  n_aug_ = 7;
  lambda_ = 3 - n_aug_;
  
  // init predicted sigma points of x
  Xsig_pred_ = MatrixXd(n_x_, 2*n_aug_+1);
  
  // set vector for weights
  weights_ = VectorXd(2*n_aug_+1);
  double weight_0 = lambda_ / (lambda_+n_aug_);
  weights_(0) = weight_0;
  for(int i=1; i<2*n_aug_+1; i++){
      weights_(i) = 0.5 / (lambda_ + n_aug_);
      }

}

UKF::~UKF() {}


  /*****************************************************************************
  *                                Initialization
  ****************************************************************************/
void UKF::ProcessMeasurement(MeasurementPackage measurement_pack) {
  /********************
   *  Initialization
   ********************/
  if (!is_initialized_) {
    if (measurement_pack.sensor_type_ == MeasurementPackage::RADAR) 
    {
      /**
      Convert radar from polar to cartesian coordinates and initialize state.
      */
      x_(0) = cos(measurement_pack.raw_measurements_[1]) * measurement_pack.raw_measurements_[0], //px
      x_(1) = sin(measurement_pack.raw_measurements_[1]) * measurement_pack.raw_measurements_[0], //py
      x_(2) = abs(measurement_pack.raw_measurements_[2]);

    }
    else if (measurement_pack.sensor_type_ == MeasurementPackage::LASER) 
    {
      //set the state with the initial location and zero velocity
      x_(0) = measurement_pack.raw_measurements_[0], //px
      x_(1) = measurement_pack.raw_measurements_[1];
    }
    
    time_us_ = measurement_pack.timestamp_;
    is_initialized_ = true;    // done initializing, no need to predict or update
    return;
  }

  /***********************
  *  Prediction
  ************************/

  //compute the time elapsed between the current and previous measurements
  double delta_t = (measurement_pack.timestamp_ - time_us_) / 1000000.0;  //dt - expressed in seconds
  time_us_ = measurement_pack.timestamp_;

  Prediction(delta_t);

  /**********************
  *  Update
  ***********************/

  if (use_radar_ && measurement_pack.sensor_type_ == MeasurementPackage::RADAR) {
    UpdateRadar(measurement_pack);     // Radar updates
  } 
  else if (use_laser_ && measurement_pack.sensor_type_ == MeasurementPackage::LASER) {
    UpdateLidar(measurement_pack);        // Laser updates
  }

}


  /*****************************************************************************
  *                                Prediction
  ****************************************************************************/
/**
* Predicts sigma points, the state, and the state covariance matrix.
* @param {double} delta_t the change in time (in seconds) between the last
* measurement and this one.
*/
void UKF::Prediction(double delta_t) {

  /*******************************
  *  Augment Sigma Points
  ********************************/
   // Create augmented mean vector
   VectorXd x_aug_ = VectorXd(n_aug_);
   x_aug_.head(5) = x_;
   x_aug_(5) = 0;
   x_aug_(6) = 0;
   
   // Create augmented state covariance
   MatrixXd P_aug_ = MatrixXd(n_aug_, n_aug_);
   P_aug_.fill(0.0);
   P_aug_.topLeftCorner(5, 5) = P_;
   P_aug_(5, 5) = std_a_*std_a_;
   P_aug_(6, 6) = std_yawdd_*std_yawdd_;
   
   // create square root of P_aug_
   MatrixXd L_ = P_aug_.llt().matrixL();
   
   // create augmented sigma point matrix
   MatrixXd Xsig_aug_ = MatrixXd(n_aug_, 2*n_aug_+1);
   Xsig_aug_.col(0)  = x_aug_;
   
   for (int i = 0; i< n_aug_; i++) {
       Xsig_aug_.col(i+1)       = x_aug_ + sqrt(lambda_+n_aug_) * L_.col(i);
       Xsig_aug_.col(i+1+n_aug_) = x_aug_ - sqrt(lambda_+n_aug_) * L_.col(i);
   }

  /*********************************
  *  Predict Sigma Points
  **********************************/
  //predict sigma points
  for (int i = 0; i < 2 * n_aug_ + 1; i++){
    // extract values for better readability
    double p_x_ = Xsig_aug_(0, i);
    double p_y_ = Xsig_aug_(1, i);
    double v_ = Xsig_aug_(2, i);
    double yaw_ = Xsig_aug_(3, i);
    double yawd_ = Xsig_aug_(4, i);
    double nu_a_ = Xsig_aug_(5, i);
    double nu_yawdd_ = Xsig_aug_(6, i);
       
    // predict state values
    double px_p_, py_p_;
       
    // avoid division by zero
    if(fabs(yawd_)>0.001){
        px_p_ = p_x_ + v_/yawd_*(sin(yaw_+yawd_*delta_t)-sin(yaw_));
        py_p_ = p_y_ + v_/yawd_*(cos(yaw_) - cos(yaw_+yawd_*delta_t));
        }else{
        px_p_ = p_x_ + v_*delta_t*cos(yaw_);
        py_p_ = p_y_ + v_*delta_t*sin(yaw_);
            }
       
    double v_p_ = v_;
    double yaw_p_ = yaw_+yawd_*delta_t;
    double yawd_p_ = yawd_;

    // add noise
    px_p_ = px_p_ + 0.5*nu_a_*delta_t*delta_t*cos(yaw_);
    py_p_ = py_p_ + 0.5*nu_a_*delta_t*delta_t*sin(yaw_);
    v_p_ = v_p_ + nu_a_*delta_t;
    yaw_p_ = yaw_p_ + 0.5*nu_yawdd_*delta_t*delta_t;
    yawd_p_ = yawd_p_ + nu_yawdd_*delta_t;

    // write predicted sigma point into right column
    Xsig_pred_(0, i) = px_p_;
    Xsig_pred_(1, i) = py_p_;
    Xsig_pred_(2, i) = v_p_;
    Xsig_pred_(3, i) = yaw_p_;
    Xsig_pred_(4, i) = yawd_p_;
  }

  /***************************************************
  *  Convert Predicted Sigma Points to Mean/Covariance
  ****************************************************/
   // predict state mean
   x_.fill(0);
   for(int i=0; i < 2*n_aug_+1; i++){
        x_ += weights_(i)*Xsig_pred_.col(i);
        }
   
   // predict state covaraince matrix
   // set bias vector for each sigma point
   VectorXd bias_;
   P_.fill(0);
   for(int i=0; i<2*n_aug_+1; i++){
       bias_ = Xsig_pred_.col(i)-x_;
       while(bias_(3)>M_PI) bias_(3) -= 2.*M_PI;
       while(bias_(3)<-M_PI) bias_(3) += 2.*M_PI;
       P_ += weights_(i)*bias_*bias_.transpose();
       }

}


  /*****************************************************************************
  *                                Update - LIDAR
  ****************************************************************************/
/**
 * Updates the state and the state covariance matrix using a laser measurement.
 * @param {MeasurementPackage} meas_package
 */
void UKF::UpdateLidar(MeasurementPackage meas_package) {
   // predict RADAR measurement
   // create matrix for sigma points in measurement space
   int n_z_ = 2;
   MatrixXd Zsig_ = MatrixXd(n_z_, 2*n_aug_+1);
   
   // mean predicted measurement
   VectorXd z_pred_ = VectorXd(n_z_);
   
   // transform sigma points into measurement space
   for(int i=0; i<2*n_aug_+1; i++){
       double p_x_ = Xsig_pred_(0, i);
       double p_y_ = Xsig_pred_(1, i);

       // measurement model
       Zsig_(0, i) = p_x_;
       Zsig_(1, i) = p_y_;
       }
    
    // mean predicted measurement
    z_pred_.fill(0.0);
    for(int i=0; i<2*n_aug_+1; i++){
        z_pred_ = z_pred_ + weights_(i)*Zsig_.col(i);
        }
    
    // measurement covariance matrix S_
    MatrixXd S_ = MatrixXd(n_z_, n_z_);
    S_.fill(0.0);
    // create matrix for cross correlation
    MatrixXd Tc_ = MatrixXd(n_x_, n_z_);
    Tc_.fill(0.0);
    for(int i=0; i<2*n_aug_+1; i++){
        //residual
        VectorXd z_diff_ = Zsig_.col(i) - z_pred_;   
        S_ = S_ + weights_(i)*z_diff_*z_diff_.transpose();
        VectorXd x_diff_ = Xsig_pred_.col(i) - x_;
        while (x_diff_(3)> M_PI) x_diff_(3)-=2.*M_PI;
        while (x_diff_(3)<-M_PI) x_diff_(3)+=2.*M_PI;
        // calculate cross correlation matrix
        Tc_ = Tc_ + weights_(i)*x_diff_*z_diff_.transpose();
        }
        
    // add measurement noise covariance matrix
    MatrixXd R_ = MatrixXd(n_z_, n_z_);
    R_ << std_laspx_*std_laspx_, 0,
          0, std_laspy_*std_laspy_;
    S_ = S_ + R_;
    
   
   // Kalman gain K
   MatrixXd K_ = Tc_*S_.inverse();

   
   // residual
   VectorXd z_diff = meas_package.raw_measurements_ - z_pred_;

   // update state mean and covariance matrix
   x_ = x_ + K_*z_diff;
   P_ = P_ - K_*S_*K_.transpose();
   
}


  /*****************************************************************************
  *                                Update - RADAR
  ****************************************************************************/
/**
 * Updates the state and the state covariance matrix using a radar measurement.
 * @param {MeasurementPackage} meas_package
 */
void UKF::UpdateRadar(MeasurementPackage meas_package) {
   // predict RADAR measurement
   // create matrix for sigma points in measurement space   
   int n_z_ = 3;
   MatrixXd Zsig_ = MatrixXd(n_z_, 2*n_aug_+1);
   
   // mean predicted measurement
   VectorXd z_pred_ = VectorXd(n_z_);
   
   // measurement covariance matrix S_
   MatrixXd S_ = MatrixXd(n_z_, n_z_);
   
   // transform sigma points into measurement space
   for(int i=0; i<2*n_aug_+1; i++){
       double p_x_ = Xsig_pred_(0, i);
       double p_y_ = Xsig_pred_(1, i);
       double v_ = Xsig_pred_(2, i);
       double yaw_ = Xsig_pred_(3, i);
       
       double v1_ = cos(yaw_)*v_;
       double v2_ = sin(yaw_)*v_;
       
       // measurement model
       Zsig_(0, i) = sqrt(p_x_*p_x_ + p_y_*p_y_);
       Zsig_(1, i) = atan2(p_y_, p_x_);
       Zsig_(2, i) = (p_x_*v1_ + p_y_*v2_) / sqrt(p_x_*p_x_ + p_y_*p_y_);
       }

    // mean predicted measurement
    z_pred_.fill(0.0);
    for(int i=0; i<2*n_aug_+1; i++){
        z_pred_ = z_pred_ + weights_(i)*Zsig_.col(i);
        }
    
    // innovation covaiance matrix S
    S_.fill(0.0);
    for(int i=0; i<2*n_aug_+1; i++){
        //residual
        VectorXd z_diff_ = Zsig_.col(i) - z_pred_;
        
        // angle normalization
        while(z_diff_(1)>M_PI) z_diff_(1) -= 2.*M_PI;
        while(z_diff_(1)<-M_PI) z_diff_(1) += 2.*M_PI;
        
        S_ = S_ + weights_(i)*z_diff_*z_diff_.transpose();
        }
    
    // add measurement noise covariance matrix
    MatrixXd R_ = MatrixXd(n_z_, n_z_);
    R_ << std_radr_*std_radr_, 0, 0,
          0, std_radphi_*std_radphi_, 0,
          0, 0, std_radrd_*std_radrd_;
    S_ = S_ + R_;
   
    // create matrix for cross correlation
    MatrixXd Tc_ = MatrixXd(n_x_, n_z_);
    
    // calculate cross correlation matrix
    Tc_.fill(0.0);
    for(int i=0; i<2*n_aug_+1; i++){
        // residual
        VectorXd z_diff1_ = Zsig_.col(i) - z_pred_;
        // angle normalization
        while(z_diff1_(1)>M_PI) z_diff1_(1) -= 2.*M_PI;
        while(z_diff1_(1)<-M_PI) z_diff1_(1) += 2.*M_PI;
        
        // state difference
        VectorXd x_diff_ = Xsig_pred_.col(i) - x_;

        // angle normalization
        while (x_diff_(3)> M_PI) x_diff_(3)-=2.*M_PI;
        while (x_diff_(3)<-M_PI) x_diff_(3)+=2.*M_PI;
        
        Tc_ = Tc_ + weights_(i)*x_diff_*z_diff1_.transpose();
        }
   
   // Kalman gain K
   MatrixXd K_ = Tc_*S_.inverse();
   
   // residual
   VectorXd z_diff = meas_package.raw_measurements_ - z_pred_;
   
   // angle normalization
   while(z_diff(1)>M_PI) z_diff(1)-=2.*M_PI;
   while(z_diff(1)<-M_PI) z_diff(1)+=2.*M_PI;
   
   // update state mean and covariance matrix
   x_ = x_ + K_*z_diff;
   P_ = P_ - K_*S_*K_.transpose();
   
}