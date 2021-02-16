#include <math.h>
#include <uWS/uWS.h>
#include <iostream>
#include <string>
#include "json.hpp"
#include "PID.h"

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
 */


// for convenience
using nlohmann::json;
using std::string;

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
  double throttle = 0.3;
  /**
   * TODO: Initialize the pid variable.
   */
  // pid.Init(0.08, 0.0, 0.0); //quite good !
  //pid.Init(0.075, 0.0, 0.0); 31 s
  //pid.Init(0.05, 0.0, 0.0); //33s
  //pid.Init(0.04, 0.0, 0.0); //34s
  
  
  pid.Init(0.03, 0.0, 0.0); //34mph go to 1st big turn BEST FOR TIME BEING !
  // but need retest because it was before the correction with prev_cte ...
  // but will be the same because Kd was 0 anyway.
 
  // pid.Init(0.08, 3.0, 0.0); --> still bad even with prev_cte correction
 
  // Next : adjust throttle, try to reduce according to error
  // then test, if ok, can try to test Kd parameter.
  
  // + need to print time to see until how long I go ....
  
  
  
  //pid.Init(0.02, 0.0, 0.0); //34s

  //pid.Init(0.08, 0.1, 0.0); // out !
  //pid.Init(0.08, 0.05, 0.0); // out !
  // pid.Init(0.08, 0.05, 0.0); // out !
  
  //pid.Init(0.08, 0.00025, 0.0);
  
  //pid.Init(0.08, 3.0, 0.0); //completely out ...
  //pid.Init(0.225, 4.0, 0.0); // completely out ...
  //pid.Init(0.225, 4.0, 0.0004); // completely out....
  //pid.Init(-0.12, -1.2, 0.0); // completely out ...
  //pid.Init(0.12, -2.7, 0.0); // completely out ...
  //pid.Init(0.2, 3.0, 0.0003); // completely out ...

  
  h.onMessage([&pid,&throttle](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, 
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
          // simulator will provide you the cross track error (CTE) and 
          // the velocity (mph) in order to compute the appropriate steering angle.
          
          // Implement first a P controler
          pid.UpdateControllers(cte);
          // PROBLEMATIC : TO CHECK ...
          steer_value = pid.GetPIDController();
          
          // check steer_value between [-1; +1]
          if((steer_value <-1)||(steer_value >1)){
            std::cout << "ERROR steer_value outside bounds : " << steer_value << std::endl;
            if(steer_value <-1) steer_value = -1;
            if(steer_value >-1) steer_value = +1;
          }
          
          // DEBUG
          std::cout << "CTE: " << cte << " Steering Value: " << steer_value 
                    << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          
          // if cte increasing ---> throttle *= 0.9 but bound by 0.1
          // if cte decreasing --> throttel *= 1.1 but bound by 0.3
          // msgJson["throttle"] = 0.3;
          
          msgJson["throttle"] = throttle;
          
          
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
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