#ifndef TWIDDLE_H
#define TWIDDLE_H

/**
 * Change history
 * v1.2   Create file
 *        Change order parameters Init(double Kp_, double Kd_, double Ki_)
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
   * @param (Kp_, Kd_, Ki_) The initial PID coefficients
   */
  void Init(double Kp_, double Kd_, double Ki_);
  /**
   * Run Twiddle Algo.
   * @param (cte) Cross Track Error reported from Simulator
   */
  void Run(double cte);

 //private:
  /**
   * p PID Coefficient
   */
  double p[3];
  /**
   * dp PID Coefficients
   */ 
  double dp[3];    
  
  unsigned int index;
  unsigned int step;
  double best_error;
  double tol;
};

#endif  // PID_H