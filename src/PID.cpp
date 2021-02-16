#include "PID.h"

/**
 * Change history
 * v0.0 : Initial files
 * v1.0 : Fill Init()
 *        Rename p_error, to p_controller, same for d and i
 *        Rename UpdateError() to UpdateControllers()
 *        Rename TotalError() to GetPIDController()
 *        Add prev_cte, int_cte attribute and use it
 *        Correction : was not updating prev_cte to cte at the end
 *        Add PID::IsCTEIncreasing() for throttle control
 */


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
  prev_cte = 0.0;
  int_cte = 0.0;
}

//void PID::UpdateError(double cte) {
void PID::UpdateControllers(double cte) {
    /**
   * TODO: Update PID errors based on cte.
   */
  
  // cover case of first cte reported (prev_cte still 0)
  if(prev_cte == 0) prev_cte = cte;
  
  int_cte += cte;
  
  // The PID::UpdateError method calculates proportional,
  // integral and derivative errors
  p_controller = -Kp*cte;
  d_controller = -Kd * (cte - prev_cte);
  i_controller = -Ki * int_cte;
  
  prev_cte = cte ;
}

// double PID::TotalError() {
double PID::GetPIDController() {
  
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
  
  return(p_controller + d_controller + i_controller);  // TODO: Add your total error calc here!
}

bool PID::IsCTEIncreasing() {
  // z = (x > y) ? z : y;
  return((d_controller >=0 ) ? false :true);
}