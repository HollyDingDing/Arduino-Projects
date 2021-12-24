
#include "SCoop.h"
#include "MotorController.h"

// Define Motor Move Direction
#define L298N1_FORWARD "back"
#define L298N1_BACK "forward"
#define L298N2_FORWARD "forward"
#define L298N2_BACK "back"
#define L298N3_FORWARD "forward"
#define L298N3_BACK "back"
#define L298N4_FORWARD "back"
#define L298N4_BACK "forward"

// Define The Time And Speed Of Motor When Turning 90 Degrees
#define TURN_90_SPEED150_TIME 1000
#define FORWARD_LEFT_ROUTE 5000
#define FORWARD_DOWN_ROUTE 4000


#define CHANGE_ID 1
#define MOVING_WEIGHTS 5
#define MOVING_BIAS 1


String turn = "";
bool postponing = false;
bool moving = false;
bool returning = false;



byte motor1[3] = {2, 4, 3};
byte motor2[3] = {6, 7, 5};
byte motor3[3] = {8, 9, 10};
byte motor4[3] = {12, 13, 11};

MotorController controller = MotorController(motor1, motor2, motor3, motor4);


defineTimerRun(Timer2, 10) {
  sleep(100);
  while (moving == true) {
    controller.moveForward(150);
    sleep(FORWARD_LEFT_ROUTE);
    controller.rotate("left", 150);
    sleep(TURN_90_SPEED150_TIME);
    controller.moveForward(150);
    sleep(FORWARD_DOWN_ROUTE);
    controller.stopAll();
    moving = false;
  }
  while (returning == true) {
    controller.rotate("left", 150);
    sleep(TURN_90_SPEED150_TIME * 2);
    controller.moveForward(150);
    sleep(FORWARD_DOWN_ROUTE);
    controller.rotate("right", 150);
    sleep(TURN_90_SPEED150_TIME);
    controller.moveForward(150);
    sleep(FORWARD_LEFT_ROUTE);
    controller.stopAll();
    returning = false;
  }
}

defineTimerRun(Timer1, 10) {
  if (Serial.available()) {
    char c = Serial.read();
    Serial.println(turn);
    Serial.println(c);
    if (c == '@') {
      moving = true;
    }else if (c == '$') {
      returning = true;
    }
  }
}


void setup() {
  Serial.begin(9600);
  controller.motor_direction_setter(1, L298N1_FORWARD, L298N1_BACK);
  controller.motor_direction_setter(2, L298N2_FORWARD, L298N2_BACK);
  controller.motor_direction_setter(3, L298N3_FORWARD, L298N3_BACK);
  controller.motor_direction_setter(4, L298N4_FORWARD, L298N4_BACK);
  while (!Serial);
  mySCoop.start();
  controller.rotate("right", 150);
  delay(TURN_90_SPEED150_TIME);
  controller.stopAll();
}

void loop()  {
//  controller.moveForward(150);
//  delay(2000);
//  controller.rotate("right", 150);
//  delay(2000);
//  controller.rotate("left", 150);
//  delay(2000);
//  controller.moveBack(150);
//  delay(2000);
  mySCoop.sleep(10);
}
