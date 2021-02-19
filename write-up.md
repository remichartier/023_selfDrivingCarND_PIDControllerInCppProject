# Udacity Self-Driving Car Nanodegree : PID Controller Project Write-up

Compilation Criteria | Criteria to meet specifications
-------------------- | -------------------------------
Your code should compile. | Code must compile without errors with cmake and make.

Code is compiling/building successfully : 

![Compilation](/images/005_compilation.png)

Implementation Criteria | Criteria to meet specifications
----------------------- | -------------------------------
The PID procedure follows what was taught in the lessons.| The base algorithm should follow what's presented in the lessons.

Here is how my algorithm follows what was presented in the lesson : 
- The `PID` Class defines the PID Coefficients (cf `pid.h` file) for Proportional, Differential and Integral coefficients
```
  /**
   * PID Coefficients
   */ 
  double Kp;
  double Ki;
  double Kd;
```

- The `PID` Class includes variables + methods to calculate each P/I/D controller components
```
// pid.h
// =====

  /**
   * PID Errors / Controller Coefficients
   */
  double p_controller;
  double i_controller;
  double d_controller;
  /**
   * PID int_cte to store sum of all cte accumulated
   */ 
  double int_cte;

// pid.cpp
// =======

// void PID::UpdateError(double cte) {
void PID::UpdateControllers(double prev_cte, double cte) {
    /**
   * TODO: Update PID errors based on cte.
   */
  
  int_cte += cte;
  
  // The PID::UpdateError method calculates proportional,
  // integral and derivative errors
  p_controller = -Kp * cte;
  d_controller = -Kd * (cte - prev_cte);
  i_controller = -Ki * int_cte;
}
```
- The main function and PID class contain instructions and methods to compute PID controller and assign the result to the steering variable in the main function which will send it to simulator to control the car steering.
```
// main.cpp
// ========

PID pid;
double prev_cte = 0;
...
twiddle.Init(0.08, 1, 0.001);   // --> best I think, I keep this one.
pid.Init(twiddle.p[P], twiddle.p[D], twiddle.p[I]);  
...
          pid.UpdateControllers(prev_cte,cte);
          steer_value = pid.GetPIDController();
          ...
          // Keep steer_value between [-1; +1]
          if((steer_value <-1)||(steer_value >1)){
            std::cout << "ERROR steer_value outside bounds : " << steer_value << std::endl;
            if(steer_value <-1) steer_value = -1;
            if(steer_value >-1) steer_value = +1;
          }

          // record prev_cte for next cycle
          prev_cte = cte;
          
// pid.cpp
// =======

// double PID::TotalError() {
double PID::GetPIDController() {
  ...
  return(p_controller + d_controller + i_controller);  // TODO: Add your total error calc here!
}
```



Reflection Criteria | Criteria to meet specifications
-------------------- | -------------------------------
Describe the effect each of the P, I, D components had in your implementation. | Student describes the effect of the P, I, D component of the PID algorithm in their implementation. Is it what you expected? Visual aids are encouraged, i.e. record of a small video of the car in the simulator and describe what each component is set to.


Reflection Criteria | Criteria to meet specifications
-------------------- | -------------------------------
Describe how the final hyperparameters were chosen. | Student discusses how they chose the final hyperparameters (P, I, D coefficients). This could be have been done through manual tuning, twiddle, SGD, or something else, or a combination!


Simulation Criteria | Criteria to meet specifications
-------------------- | -------------------------------
The vehicle must successfully drive a lap around the track. | No tire may leave the drivable portion of the track surface. The car may not pop up onto ledges or roll over any surfaces that would otherwise be considered unsafe (if humans were in the vehicle).
