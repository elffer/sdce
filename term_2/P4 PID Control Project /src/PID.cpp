#include "PID.h"

/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {
  /**
   * TODO: Initialize PID coefficients (and errors, if needed)
   */
  Kp = Kp_;
  Ki = Ki_;
  Kd = Kd_;
  cte_pre = 0.0;
  int_cte = 0.0;

}

double PID::CalcValue(double cte){
  /* Calculate the control of mismatch */
  double value;
  double diff_cte;
  diff_cte = cte - cte_pre;
  cte_pre = cte;
  int_cte += cte;
  value = - Kp*cte - Kd*diff_cte - Ki*int_cte;
  
  return value;
}

double PID::CalcValue_throttle(double cte){
  /* Calculate the control of mismatch */
  double value;
  double diff_cte;
  diff_cte = cte - cte_pre;
  cte_pre = cte;
  int_cte = 0.9*int_cte + cte;
  value = - Kp*cte - Kd*diff_cte - Ki*int_cte;
  
  return value;
}

void PID::UpdateError(double cte) {
  /**
   * TODO: Update PID errors based on cte.
   */

}

double PID::TotalError() {
  /**
   * TODO: Calculate and return the total error
   */
  return 0.0;  // TODO: Add your total error calc here!
}
