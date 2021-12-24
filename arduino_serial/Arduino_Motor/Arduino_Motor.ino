

//#include "MotorController.h"
#include <MotorController.h>

// Define Motor Move Direction
#define L298N1_FORWARD "back"
#define L298N1_BACK "forward"
#define L298N2_FORWARD "forward"
#define L298N2_BACK "back"
#define L298N3_FORWARD "back"
#define L298N3_BACK "forward"
#define L298N4_FORWARD "forward"
#define L298N4_BACK "back"

// Define The Time And Speed Of Motor When Turning 90 Degrees
#define TURN_90_SPEED150_TIME 1600


int motor1[3] = {2, 4, 3};
int motor2[3] = {6, 7, 5};
int motor3[3] = {8, 9, 10};
int motor4[3] = {12, 13, 11};

MotorController controller = MotorController(motor1, motor2, motor3, motor4);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  controller.motor_direction_setter(1, L298N1_FORWARD, L298N1_BACK);
  controller.motor_direction_setter(2, L298N2_FORWARD, L298N2_BACK);
  controller.motor_direction_setter(3, L298N3_FORWARD, L298N3_BACK);
  controller.motor_direction_setter(4, L298N4_FORWARD, L298N4_BACK);
  Serial.println("Hello. This is test");
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 3; i++) {
      Serial.print(String(controller.dcMotor[j][i])+" ");
    }
    Serial.println("");
  }
  
  controller.printMotorDefinition();
//  Serial.println(controller.dcMotor[0][1]);
//  Serial.println(controller.dcMotor[0][2]);
}

void loop() {
//  // put your main code here, to run repeatedly:
  controller.moveForward(150);
  delay(1550);
  controller.rotate("left", 150);
  delay(TURN_90_SPEED150_TIME);
  controller.rotate("right", 150);
  delay(TURN_90_SPEED150_TIME);
  controller.moveBack(150);
  delay(1550);
  
  
//  stopMotor(motor1[2]);
//  stopMotor(motor2[2]);
//  stopMotor(motor3[2]);
//  stopMotor(motor4[2]);
//  delay(3000);
//  rotate(motor1, motor2, motor3, motor4, "right", 150);
//  delay(3000);
//  stopMotor(motor1[2]);
//  stopMotor(motor2[2]);
//  stopMotor(motor3[2]);
//  stopMotor(motor4[2]);
//  delay(3000);
}

//
//void moveMotor(int pin1, int pin2, int pin3, String turn, int motor_speed) {
//  if (turn == "forward") {
//    digitalWrite(pin1, HIGH);
//    digitalWrite(pin2, LOW);
//  }else if (turn == "back") {
//    digitalWrite(pin1, LOW);
//    digitalWrite(pin2, HIGH);
//  }
//  Serial.println(turn == "forward");
//  Serial.print(String(pin1)+" ");
//  Serial.println(String(pin2)+" ");
//  
//  analogWrite(pin3, motor_speed);
//  
//}

//void stopAll(){
//  stopMotor(motor1[2]);
//  stopMotor(motor2[2]);
//  stopMotor(motor3[2]);
//  stopMotor(motor4[2]);
//}
//
//void stopMotor(int pwm) {
//  analogWrite(pwm, 0);
//}
//
//void moveForward(int l298n1[3], int l298n2[3], int l298n3[3], int l298n4[3], int motor_speed) {
//  moveMotor(l298n1[0], l298n1[1], l298n1[2], L298N1_FORWARD, motor_speed);
//  moveMotor(l298n2[0], l298n2[1], l298n2[2], L298N2_FORWARD, motor_speed);
//  moveMotor(l298n3[0], l298n3[1], l298n3[2], L298N3_FORWARD, motor_speed);
//  moveMotor(l298n4[0], l298n4[1], l298n4[2], L298N4_FORWARD, motor_speed);
//  Serial.println("Move Forward!!");
//}
//
//void moveBack(int l298n1[3], int l298n2[3], int l298n3[3], int l298n4[3], int motor_speed) {
//  moveMotor(l298n1[0], l298n1[1], l298n1[2], L298N1_BACK, motor_speed);
//  moveMotor(l298n2[0], l298n2[1], l298n2[2], L298N2_BACK, motor_speed);
//  moveMotor(l298n3[0], l298n3[1], l298n3[2], L298N3_BACK, motor_speed);
//  moveMotor(l298n4[0], l298n4[1], l298n4[2], L298N4_BACK, motor_speed);
//  Serial.println("Move Back!!");
//}
//
//void rotate(int l298n1[3], int l298n2[3], int l298n3[3], int l298n4[3], String turnDirection, int motor_speed) {
//  String direction1 = "";
//  String direction2 = "";
//  if (turnDirection == "right") {
//    moveMotor(l298n1[0], l298n1[1], l298n1[2], L298N1_BACK, motor_speed);
//    moveMotor(l298n2[0], l298n2[1], l298n2[2], L298N2_BACK, motor_speed);
//    moveMotor(l298n3[0], l298n3[1], l298n3[2], L298N3_FORWARD, motor_speed);
//    moveMotor(l298n4[0], l298n4[1], l298n4[2], L298N4_FORWARD, motor_speed);
//  }else if (turnDirection == "left") {
//    moveMotor(l298n1[0], l298n1[1], l298n1[2], L298N1_FORWARD, motor_speed);
//    moveMotor(l298n2[0], l298n2[1], l298n2[2], L298N2_FORWARD, motor_speed);
//    moveMotor(l298n3[0], l298n3[1], l298n3[2], L298N3_BACK, motor_speed);
//    moveMotor(l298n4[0], l298n4[1], l298n4[2], L298N4_BACK, motor_speed);
//  }
//  
//  Serial.println(turnDirection);
//}
//
//// 平移左或右
//// Only Move Left Direction Or Right Direction Without Turn Left Or Right
//void moveParallel(int l298n1[3], int l298n2[3], int l298n3[3], int l298n4[3], String parallelDirection, int motor_speed) {
//  if (parallelDirection == "right") {
//    moveMotor(l298n1[0], l298n1[1], l298n1[2], L298N1_BACK, motor_speed);
//    moveMotor(l298n2[0], l298n2[1], l298n2[2], L298N2_FORWARD, motor_speed);
//    moveMotor(l298n3[0], l298n3[1], l298n3[2], L298N3_BACK, motor_speed);
//    moveMotor(l298n4[0], l298n4[1], l298n4[2], L298N4_FORWARD, motor_speed);
//  } else if (parallelDirection == "left") {
//    moveMotor(l298n1[0], l298n1[1], l298n1[2], L298N1_FORWARD, motor_speed);
//    moveMotor(l298n2[0], l298n2[1], l298n2[2], L298N2_BACK, motor_speed);
//    moveMotor(l298n3[0], l298n3[1], l298n3[2], L298N3_FORWARD, motor_speed);
//    moveMotor(l298n4[0], l298n4[1], l298n4[2], L298N4_BACK, motor_speed);
//  }
//}
