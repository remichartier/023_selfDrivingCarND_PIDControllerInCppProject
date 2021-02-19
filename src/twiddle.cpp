#include "twiddle.h"
#include <iostream>

using std::cout;
using std::endl;

/**
 * Change history
 * v1.2 : Create file
 *        Implementation twiddler algo (Run())
 *        Change order parameters Init(double Kp_, double Kd_, double Ki_)
 *        Modify tolerance for sum(dp) (0.00001 instead of 0.2)
 *        Correct twiddle method if() --> swithc()/Case:
 */


/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

Twiddle::Twiddle() {}

Twiddle::~Twiddle() {}

void Twiddle::Init(double Kp_, double Kd_, double Ki_) {
  /**
   * TODO: Initialize PID coefficients (and errors, if needed)
   */
  p[P] = Kp_;
  p[D] = Kd_;
  p[I] = Ki_;
  
  dp[P] = 0.1 * p[P];
  dp[D] = 0.1 * p[D];
  dp[I] = 0.1 * p[I];
  
  index = 0; // first rotational param to tune : P
  step = 0;
  tol = (dp[P] + dp[D] + dp[I]) * 0.2;
}

void Twiddle::Run(double cte) {
  /**
   * TODO: run Twiddle algorithm Cyclicly
   */
  std::cout << "Enter Twiddle::Run() : " ;
  std::cout << "p[P] = " << p[P] << "; ";
  std::cout << "p[D] = " << p[D] << "; ";
  std::cout << "p[I] = " << p[I] << "; ";
  std::cout << "dp[P] = " << dp[P] << "; ";
  std::cout << "dp[D] = " << dp[D] << "; ";
  std::cout << "dp[I] = " << dp[I] << std::endl;
  
  // Skip if sum(dp) < tolerance 0.2)
  if (dp[P] + dp[D] + dp[I] < tol){
    cout << "Sum of dp[] < 0.00001 ==> not running Twiddle algo" << endl;
    return;
  }
  
  switch(step) {
    case 0:
      // 1st step (0)
      best_error = cte;
      cout << "p[index] += dp[index]; index = " << index << "; p[index] = " << p[index] << "; dp[index] = " << dp[index] << "; "; 
      p[index] += dp[index];
      cout << "p[" << index << "] = " << p[index] << endl;
      step = 1;
      break;
    case 1:
      // 2nd step (1)
      if(cte < best_error){
        best_error = cte;
        dp[index] *= 1.1;
        index = (index + 1) % 3;
        step = 0;
      } else{
        cout << "p[index] -= 2*dp[index]; index = " << index << "; p[index] = " << p[index] << "; dp[index] = " << dp[index] << "; "; 
        p[index] -= 2*dp[index];
        cout << "p[" << index << "] = " << p[index] << endl;
        step = 2;
      }
      break;
      
    case 2:
      // 3nd step (2)
      if(cte < best_error){
        best_error = cte;
        dp[index] *= 1.1;
      } else{
        cout << "p[index] += dp[index]; index = " << index << "; p[index] = " << p[index] << "; dp[index] = " << dp[index] << "; "; 
        p[index] += dp[index];
        cout << "p[" << index << "] = " << p[index] << endl;
        dp[index] *= 0.9;
      }
      step = 0;
      index = (index + 1) % 3;
      break;
  } // end of switch()
  
  std::cout << "Exit Twiddle::Run() : " ;
  std::cout << "p[P] = " << p[P] << "; ";
  std::cout << "p[D] = " << p[D] << "; ";
  std::cout << "p[I] = " << p[I] << "; ";
  std::cout << "dp[P] = " << dp[P] << "; ";
  std::cout << "dp[D] = " << dp[D] << "; ";
  std::cout << "dp[I] = " << dp[I] << std::endl;
} // end method
