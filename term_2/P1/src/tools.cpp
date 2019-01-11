#include "tools.h"
#include <iostream>

#include <math.h>

using Eigen::VectorXd;
using Eigen::MatrixXd;
using std::vector;

Tools::Tools() {}

Tools::~Tools() {}

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
                              const vector<VectorXd> &ground_truth) {
  /**
   * Calculate the RMSE here.
   */
  VectorXd rmse(4);
  VectorXd sum_dif(4);
  rmse << 0,0,0,0;
  
  if(estimations.size()!=ground_truth.size()||estimations.size()==0){
      std::cout << "The estimation does not work !" << std::endl;
      return rmse;
      }

  // Accumulate squared residuals
  for (std::size_t i=0; i < estimations.size(); ++i) {
    // ... your code here
    VectorXd residual = estimations[i] - ground_truth[i];
    
    // coefficient-wise multiplication
    residual = residual.array()*residual.array();
    
    rmse += residual;
    
  }

  // Calculate the mean
  rmse = rmse/estimations.size();

  // Calculate the squared root
  rmse = rmse.array().sqrt();
      

  // return the result
  return rmse;
}

MatrixXd Tools::CalculateJacobian(const VectorXd& x_state) {
  /**
   * Calculate a Jacobian here.
   */
  MatrixXd Hj(3,4);
  // recover state parameters
  float px = x_state(0);
  float py = x_state(1);
  float vx = x_state(2);
  float vy = x_state(3);
  float radius = sqrt(px*px+py*py);

  if(radius < 0.01){
  std::cout << "Something Could goes wrong heare!" << std::endl;
  radius = 0.01;
  }

    // compute the Jacobian matrix
  Hj << px/radius, py/radius, 0, 0,
        -py/(radius*radius), px / (radius*radius), 0, 0,
        (py*(vx*py-vy*px))/pow(radius, 3), (px*(vy*px-vx*py))/pow(radius, 3), px/radius, py/radius;

  return Hj;
}
