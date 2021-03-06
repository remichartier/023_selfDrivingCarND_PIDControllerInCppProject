#include <math.h>
#include <uWS/uWS.h>
#include <iostream>
#include <string>
#include "json.hpp"
#include "PID.h"
#include <chrono>  // to measure timing/duration
#include "twiddle.h"
using namespace std::chrono; 

/**
 * Change history
 * v0.0 : Initial files
 * v1.0 : Initialize pid object
 *        Calculate steering using PID::UpdateControllers() and
 *          PID::GetPIDController()
 *        Handle case steer_value not within [-1,+1]
 *        Correction : was not updating prev_cte to cte at the end 
 *          --> Need to retest
 *        Add throttle global variable, initialize at 0.3
 *        Print timer to find out how long we can hold the track
 *        add prev_cte, use for throttle control
 *        pid.Init(0.03, 0.0001, 0.0); hardly but passed 1st big turn
 *          after the lake
 *        pid.Init(0.03, 0.0001, 2.5); go further but stops 
 *          because CTE always > 2 I think ie stops accelerating
 * v1.1   Comment speed control, see how it behaves without it...
 *        Back to Throttle always at 0.3 --> passing 1 laps
 * v1.2   Try to develop Twigger Algorithm
 *        Twiddle too unstable
 *        Best I could find so far : twiddle.Init(0.08, 1, 0.001);
 * v1.3   Clean code + comments
 *        Remove throttle variable as not used anymore
 */


// for convenience
using nlohmann::json;
using std::string;

#define USE_TWIDDLE 	0

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != string::npos) {
    return "";
  }
  else if (b1 != string::npos && b2 != string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main() {
  uWS::Hub h;

  PID pid;
  double prev_cte = 0;
  Twiddle twiddle;

  /**
   * TODO: Initialize the pid variable.
   */
  // pid.Init(0.08, 0.0, 0.0); //quite good !
  // pid.Init(0.075, 0.0, 0.0); 31 s
  // pid.Init(0.05, 0.0, 0.0); //33s
  // pid.Init(0.04, 0.0, 0.0); //34s
  // pid.Init(0.03, 0.0, 0.0); //34mph go to 1st big turn BEST FOR TIME BEING 
  // pid.Init(0.08, 0.0, 3.0); --> still bad even with prev_cte correction 
  // pid.Init(0.03, 0.0, 0.03); // --> right away to the ditch
  // pid.Init(0.03, 0.0, 0.0001);  // --> improved, pass 1st turn almost crashed.
  // --> may be need to increase Kd ... try 0.001
  // pid.Init(0.03, 0.0, 0.001); // nope ...
  // pid.Init(0.03, 0.0, 0.00001);
  // TRY on integral / Ki param
  // pid.Init(0.03, 1.0, 0.0001);  //Good but don't go further
  // pid.Init(0.03, 2.0, 0.0001);  // BETTER, go further !
  // pid.Init(0.03, 3.0, 0.0001); // go futher but then stops
  // pid.Init(0.03, 2.5, 0.0001);
  // twiddle.Init(0.03, 2.5, 0.0001); // GOOD !
  
  // twiddle.Init(0.08, 3, 0.0003);
  // twiddle.Init(0.08, 2, 0.0000);
  // twiddle.Init(0.08, 1, 0.0000);
  // twiddle.Init(0.08, 1, 0.005);
  // twiddle.Init(0.08, 1, 0.01); BAD
  
  twiddle.Init(0.08, 1.0, 0.001);   // --> best I think, I keep this one.

  // twiddle.Init(0.08, 4, 0.0000);
  // twiddle.Init(0.225, 4, 0.0004);
  // twiddle.Init(0.2, 3.0, 0.0003);
  // twiddle.Init(-0.12, -1.2, 0.0);
  // twiddle.Init(0.12,-2.7, 0.0);
  pid.Init(twiddle.p[P], twiddle.p[D], twiddle.p[I]);  
  
  // pid.Init(0.02, 0.0, 0.0); //34s
  // pid.Init(0.08 0.0, 0.1,); // out !
  // pid.Init(0.08, 0.0, 0.05); // out !
  // pid.Init(0.08, 0.0, 0.05); // out !
  
  //pid.Init(0.08, 0.0, 0.00025);
  
  // pid.Init(0.08, 0.0, 3.0); //completely out ...
  // pid.Init(0.225, 0.0, 4.0); // completely out ...
  // pid.Init(0.225, 0.0004, 4.0); // completely out....
  // pid.Init(-0.12, 0.0, -1.2); // completely out ...
  // pid.Init(0.12, 0.0, -2.7); // completely out ...
  // pid.Init(0.2, 0.0003, 3.0); // completely out ...

  // Use auto keyword to avoid typing long 
  // type definitions to get the timepoint 
  // at this instant use function now() 
  auto start = high_resolution_clock::now(); 
  
  h.onMessage([&pid,&start,&prev_cte,&twiddle](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, 
                     uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2') {
      auto s = hasData(string(data).substr(0, length));

      if (s != "") {
        auto j = json::parse(s);

        string event = j[0].get<string>();

        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<string>());
          double speed = std::stod(j[1]["speed"].get<string>());
          double angle = std::stod(j[1]["steering_angle"].get<string>());
          double steer_value;
          /**
           * TODO: Calculate steering value here, remember the steering value is
           *   [-1, 1].
           * NOTE: Feel free to play around with the throttle and speed.
           *   Maybe use another PID controller to control the speed!
           */
          
          // cover case of first cte reported (prev_cte still 0)
          if(prev_cte == 0){
            // ie first time pass here
            prev_cte = cte;
          } else if(USE_TWIDDLE){            
            // ie not first time --> use twiddle to test/adjust
            // PID parameters
            twiddle.Run(cte);
            pid.Init(twiddle.p[P], twiddle.p[D], twiddle.p[I]);
            // std::cout << "pid.Init(" << twiddle.p[P] << "," << twiddle.p[D] << "," << twiddle.p[I] << ")" << std::endl;
          }
          
          pid.UpdateControllers(prev_cte,cte);
          steer_value = pid.GetPIDController();
          
          
          // After function call, to calculate timing
          auto stop = high_resolution_clock::now(); 
          // Subtract stop and start timepoints and 
          // cast it to required unit. Predefined units 
          // are nanoseconds, microseconds, milliseconds, 
          // seconds, minutes, hours. Use duration_cast() 
          // function. 
          auto duration = duration_cast<seconds>(stop - start); 
          // To get the value of duration use the count() 
          // member function on the duration object 
          //std::cout << "t=" << duration.count() << "; "<< std::endl; 
          
          // keep steer_value between [-1; +1]
          if((steer_value <-1)||(steer_value >1)){
            std::cout << "ERROR steer_value outside bounds : " << steer_value << std::endl;
            if(steer_value <-1) steer_value = -1;
            if(steer_value >-1) steer_value = +1;
          }

          // record prev_cte for next cycle
          prev_cte = cte;

          
          json msgJson;
          msgJson["steering_angle"] = steer_value;
          
          msgJson["throttle"] = 0.3;
          
          
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          // std::cout << msg << std::endl;
          std::cout << msg << "; t = " << duration.count() 
            << "seconds ; CTE =" << cte << std::endl;
          
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }  // end "telemetry" if
      } else {
        // Manual driving
        string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }  // end websocket message if
  }); // end h.onMessage

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, 
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  
  h.run();
}