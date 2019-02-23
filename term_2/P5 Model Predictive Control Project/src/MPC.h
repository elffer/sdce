#ifndef MPC_H
#define MPC_H

#include <vector>
#include "Eigen-3.3/Eigen/Core"

using std::vector;

#define LATENCY 0.1
#define STEPS 10
#define LF 2.67
#define DT 0.13

#define REF_CTE 0
#define REF_EPSI 0
#define REF_V 30*0.447

#define W_CTE 20
#define W_DELTA 20
#define W_DELTA_DYN 20

#define DED25RAD 0.436332


class MPC {
 public:
  MPC();
  vector<double> pred_x;
  vector<double> pred_y;

  virtual ~MPC();

  // Solve the model given an initial state and polynomial coefficients.
  // Return the first actuations.
  std::vector<double> Solve(const Eigen::VectorXd &state, 
                            const Eigen::VectorXd &coeffs);
};

#endif  // MPC_H
