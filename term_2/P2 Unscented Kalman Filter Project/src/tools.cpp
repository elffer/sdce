#include "tools.h"

using Eigen::VectorXd;
using std::vector;

Tools::Tools() {}

Tools::~Tools() {}

VectorXd Tools::CalculateRMSE(const vector<VectorXd> &estimations,
                              const vector<VectorXd> &ground_truth) {
  /**
   * TODO: Calculate the RMSE here.
   */
     /**
   * TODO: Calculate the RMSE here.
   */
  VectorXd rmse(4);
  VectorXd sum_dif(4);
  rmse << 0,0,0,0;
  
  if(estimations.size()!=ground_truth.size()||estimations.size()==0){
//      std::cout << "The estimation does not work !" << std::endl;
      return rmse;
      }

  // TODO: accumulate squared residuals
  for (std::size_t i=0; i < estimations.size(); ++i) {
    // ... your code here
    VectorXd residual = estimations[i] - ground_truth[i];
    
    // coefficient-wise multiplication
    residual = residual.array()*residual.array();
    
    rmse += residual;
    
  }

  // TODO: calculate the mean
  rmse = rmse/estimations.size();

  // TODO: calculate the squared root
  rmse = rmse.array().sqrt();
      

  // return the result
  return rmse;
}