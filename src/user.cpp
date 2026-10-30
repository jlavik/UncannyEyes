/*
Try to rewrite eliminating the smoothing and take the joystick movement as servo movement
*/
#if 0 // Change to 0 to disable this code (must enable ONE user*.cpp only!)
#include "Arduino.h"
#include "Servo.h"

#define JOYSTICK_1_PIN A0

//+++Function prototypes
void move_head();

//+++Declare global variables+++
static float smoothingPercentage = .07;
static bool targetHasBeenReached = false;
static bool headIsMoving = false; //Global variable to indicate head is in motion
static int startingValue = 1500;
static int targetValue;
static float currentValue;
static float delta;
static float previousSmoothValue = 0.0;
static float smoothValue = 0.0;
static int endingValue = 1500;
static uint32_t transitionTime = 1500;  //1500 milliseconds = 1.5 seconds
static uint32_t neckMoveStartTime = 22;

//+++Instantiate servo
Servo servo1;


//+++Setup+++   Called once near the end of the setup() function.
void user_setup(void) {
  servo1.attach(5);
  servo1.writeMicroseconds(1500);
  Serial.begin(19200);
}
//+++End of Setup+++


//+++Main Loop+++
void user_loop(void) {
  if(!headIsMoving) { //    Not in motion, check sensor for trigger...
    int pot1 = analogRead(JOYSTICK_1_PIN);   //OK... what sensor reading!!!!     pot1 = analogRead(JOYSTICK_X_PIN); // Raw (unclipped) X/Y reading
    if (pot1 <= 500){
      targetValue = 1000;   //left
    }//end_if   left
    else if (pot1 > 500 && pot1 < 522){  
      targetValue = 1500;   //center
    }//end_else if    center
    else{
      targetValue = 2000;    //right
    }//end_else   right
    if (targetValue != (int)endingValue){  //then initiate move
      neckMoveStartTime = millis();  //      Motion is triggered! Record neckMoveStartTime, set transition to 1.5 seconds and set animating flag:
      headIsMoving = true;
      targetHasBeenReached = false;
      previousSmoothValue = 0.0;
      startingValue = endingValue;
    }//end_if endingValue and targetValue are the same
  }//end_if (!headIsMoving)
  else{ //headISMoving
    uint32_t elapsed = millis() - neckMoveStartTime;
    if(elapsed < transitionTime && !targetHasBeenReached) {
      move_head();
    }//end_if still moving 
    else {  //head has completed motion (either time has elapsed or target has been reached)... reset things
      headIsMoving = false;
    }//end_else head has completed motion
  }//end_else headIsMoving
}//end_main loop
#endif // 0

/* void move_head (){
  currentValue = startingValue + previousSmoothValue;
  delta = targetValue - currentValue;
  smoothValue = (smoothingPercentage*delta) + ((1-smoothingPercentage)*previousSmoothValue);
  endingValue = currentValue + smoothValue;
  servo1.writeMicroseconds(endingValue);
  previousSmoothValue = smoothValue;
  if(abs(targetValue - endingValue) < 3){    //check if close enough to target to declare it done
    targetHasBeenReached = true;
    servo1.writeMicroseconds(targetValue);
    previousSmoothValue = 0.0;
    startingValue = endingValue;
  }//end_if close enough to target to declare it done
}//end_function move_head */
