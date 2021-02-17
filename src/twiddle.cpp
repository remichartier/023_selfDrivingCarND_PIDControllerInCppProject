#include "twiddle.h"

/**
 * Change history
 * v1.2 : Create file
 *
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
}
