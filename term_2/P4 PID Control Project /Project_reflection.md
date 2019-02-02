
# Project Reflection

This note is to summarize the essential works in the project, as required by the rubic.

## Compilation

* The code can be compiled and the PID controller can control the vehicle to drive well along the track.

## Implementation

* The algorithm of the PID procedure follows the function taught by the course, i.e., $$steering = -\tau_{p}*cte - \tau_{d}*diff\_cte -\tau_{i}*int\_cte.$$
* I had also applied the PID procedure on the throttle according to the relation between cross track error and the throttle. However, I set $int\_cte = 0.9 * int\_cte + cte$ to update $int\_cte$, that is reducing the impact of earlier cross track errors. This was applied to reduce the speed when the error is positive or negative with large absolute value. The related codes are included in the PID::CalcValue() and PID::CalcValue_throttle() in PID.cpp.

## Reflection

### Effect of P, I, D components
* The proportional part output a value that is propotional to the cross track error. It provides the primary contribution in reducing the error, while it might cause ocillation if the proportional gain is too high, or slow response if th propotional gain is too small.
* The integration part is applied to eliminate the stady-state error. It is proportional to both the magnitude and the duration of the cross track error.
* The derivation part is applied to reduce the inertial in the oscillation. It improves the stability of the system.

### Tuning of the parameters

* The hyperparameters of P, I, D were chosen through manual tuning, similar to the twiddle while with larger steps.
* Moreover the hyperparameters of max throttle and its PID controller are also tuned, to increase the circling speed while keeping the driving safe.

## Simulation

* The vehicle can successfully drive around the track.

## Future works

* I have grasped the knowledge about tuning the hyperparameters of PID procedure with twiddle, while not finish it yet due to my time limit.
