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

- The `PID` Class includes variables + methods to calculate each P/I/D controller components.
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
- The main function and PID class contain instructions and methods to compute PID controller and assign the result to the steering variable in the main function which will send it to simulator to control the car steering. Note : As I also tried to implement Twiddle algorithm, I first set Kp, Kd, Ki Coefficient through Twiddle object, which attributes are used to initialize the Kp, Kd, Ki attributes for the `pid` object.
```
// main.cpp
// ========

PID pid;
double prev_cte = 0;
...
twiddle.Init(0.08, 1.0, 0.001);   // --> best I think, I keep this one.
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
- So to summarize, i implemented the same base algorithm as presented in the PID lessons, having new steering value being assigned to `p_controller + d_controller + i_controller` and having the following to the 3 controllers : `p_controller = -Kp * cte; d_controller = -Kd * (cte - prev_cte); i_controller = -Ki * int_cte;` which matches with the PID equation. 

Reflection Criteria | Criteria to meet specifications
-------------------- | -------------------------------
Describe the effect each of the P, I, D components had in your implementation. | Student describes the effect of the P, I, D component of the PID algorithm in their implementation. Is it what you expected? Visual aids are encouraged, i.e. record of a small video of the car in the simulator and describe what each component is set to.

- My final choices for the P/D/I coefficients were respectively (0.08, 1.0, 0.001) as I found through different tests that they were giving me the best results to steer the car for at least one full lap of the simulator track.
- Having the P coefficient alone (with D and I coefficent set at 0.0) would never be enough due to big steering oscillations occuring when only having P proportional controller activated. The longuer car would drive, the higher the oscillations would be, and inevitable driving the car out of the track.
  - Video example : (P = 0.08) TBD.
- Adding the D Differential coefficient and controller, with a fine tuned value for the D Coefficient, would help driving the car through the entire lap. It would lower the oscillations of the P controller. The steering error corrections would be more 'calculated', rather than be oscillations as it was for the P controller alone. But those corrections would be rather strong if using only the PD controller and would not be comfortable and not be compatible with a smooth ride.
  - Video example :  (P = 0.08, D = 1.0), TBD.
- Adding the I coefficient may smoothen a bit the steering changes, but I see it also has an effect of steering or correcting the steering more toward the middle of the road, it the target position / trajectory we expect the car to follow. And therefore it is helping the car to behave better in track curves and turns as it is helping more into steering towards center of lane and not staying at border of the road especially in hard turns in the race track.
- - Video example :  (P = 0.08, D = 1.0, I = 0.001), TBD.
That's how I felt the contributions of the coefficients were while testing with different PID coefficient values, most of the impacts of coefficients were expected according to the PID lesson, except that I saw the I coefficient was really finishing the job of keeping the error close to minimum to follow the intented direction / trajectory / expected steering of the car.


Reflection Criteria | Criteria to meet specifications
-------------------- | -------------------------------
Describe how the final hyperparameters were chosen. | Student discusses how they chose the final hyperparameters (P, I, D coefficients). This could be have been done through manual tuning, twiddle, SGD, or something else, or a combination!


Simulation Criteria | Criteria to meet specifications
-------------------- | -------------------------------
The vehicle must successfully drive a lap around the track. | No tire may leave the drivable portion of the track surface. The car may not pop up onto ledges or roll over any surfaces that would otherwise be considered unsafe (if humans were in the vehicle).
