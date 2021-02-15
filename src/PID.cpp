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
  Kd = Kd_;
  Ki = Ki_;
}

void PID::UpdateError(double cte) {
  /**
   * TODO: Update PID errors based on cte.
   */
  
  // The PID::UpdateError method calculates proportional,
  // integral and derivative errors

}

double PID::TotalError() {
  /**
   * TODO: Calculate and return the total error
   */
  
  // the PID::TotalError calculates the total error using 
  // the appropriate coefficients.
  /*
  "Total Error" means that you need to sum up the `cte`, `difference of cte`, and `integral of cte`.
  `cte` helps to remove the error quickly, `difference of cte` prevents the car from overshoting the central line, 
  and `integral of cte` can eliminate system error. When you sum all these terms up, you can get a stable PID controller.
  
  The total error function is the one that you have to use to get the new steering
  */
  
  return 0.0;  // TODO: Add your total error calc here!
}

double PID::Get(Param i) {
  if(i == P) return Kp;
  if(i == D) return Kd;
  if(i == I) return Ki;
  return 0.0;
}
