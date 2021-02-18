#include "twiddle.h"
#include <iostream>

using std::cout;
using std::endl;

/**
 * Change history
 * v1.2 : Create file
 *        Implementation twiddler algo (Run())
 */


/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

Twiddle::Twiddle() {}

Twiddle::~Twiddle() {}

void Twiddle::Init(double Kp_, double Ki_, double Kd_) {
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
}

void Twiddle::Run(double cte) {
  /**
   * TODO: run Twiddle algorithm Cyclicly
   */
  
  // Skip if sum(dp) < tolerance 0.2)
  if (dp[P] + dp[D] + dp[I] < 0.2){
    cout << "Sum of dp[] < 0.2 ==> not running Twiddle algo" << endl;
    return;
  }
  
  // 1st step (0)
  if(step == 0){
    best_error = cte;
    p[index] += dp[index]
    step = 1;
  } // step == 0
  
  // 2nd step (1)
  if(step == 1){
    if(cte < best_error){
      best_error = cte;
      dp[index] *= 1.1;
      index = (index + 1) % 3;
      step = 0;
    } else{
      p[index] -= 2*dp[index];
      step = 2;
    }
  } // if step == 1
  
  // 3nd step (2)
  if(step == 2){
    if(cte < best_err){
      best_error = cte;
      dp[index] *= 1.1;
        } else{
      p[index] += dp[index];
      dp[index] *= 0.9;
    }
    step = 0;
    index = (index + 1) % 3;
  } // if step == 2
  
} // end method
