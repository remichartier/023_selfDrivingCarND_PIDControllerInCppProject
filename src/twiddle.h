#ifndef TWIDDLE_H
#define TWIDDLE_H

/**
 * Change history
 * v1.2   Create file
 */

enum Param {P, D, I};

class Twiddle {
 public:
  /**
   * Constructor
   */
  Twiddle();

  /**
   * Destructor.
   */
  virtual ~Twiddle();

  /**
   * Initialize Twiddle.
   * @param (Kp_, Ki_, Kd_) The initial PID coefficients
   */
  void Init(double Kp_, double Ki_, double Kd_);

 //private:
  /**
   * p PID Coefficient
   */
  double p[3];
  /**
   * dp PID Coefficients
   */ 
  double dp[3];    
};

#endif  // PID_H