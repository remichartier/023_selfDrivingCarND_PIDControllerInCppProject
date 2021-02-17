#ifndef PID_H
#define PID_H

/**
 * Change history
 * v0.0 : Initial files
 * v1.0 : Rename p_error, to p_controller, same for d and i
 *        Rename UpdateError() to UpdateControllers()
 *        Rename TotalError() to GetPIDController() 
 *        add int_cte
 *        Change prototype UpdateControllers(), add prev_cte
 * v1.2   remove enum Param {P, D, I}; unused now
 */
#if 0
enum Param {P, D, I};
#endif // 0


class PID {
 public:
  /**
   * Constructor
   */
  PID();

  /**
   * Destructor.
   */
  virtual ~PID();

  /**
   * Initialize PID.
   * @param (Kp_, Ki_, Kd_) The initial PID coefficients
   */
  void Init(double Kp_, double Ki_, double Kd_);

  /**
   * Update the PID error variables given cross track error.
   * @param cte The current cross track error
   */
  // void UpdateError(double cte);
  void UpdateControllers(double prev_cte, double cte);

  /**
   * Calculate the total PID error.
   * @output The total PID error
   */
  // double PID::TotalError() {
  double GetPIDController();

 private:
  /**
   * PID Errors
   */
  double p_controller;
  double i_controller;
  double d_controller;

  /**
   * PID Coefficients
   */ 
  double Kp;
  double Ki;
  double Kd;
  
  /**
   * PID previous_cte, int_cte
   */ 
  double int_cte;
    
};

#endif  // PID_H