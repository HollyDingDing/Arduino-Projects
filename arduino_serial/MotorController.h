

#ifndef MotorController_h
#define MotorController_h



#include "Arduino.h"


class MotorController {
    
  public:
    byte dcMotor[4][3];
//    int dcMotor2[3];
//    int dcMotor3[3];
//    int dcMotor4[3];
    String MOTOR_FORWARD[4];
    String MOTOR_BACK[4];
//    String MOTOR1_FORWARD;
//    String MOTOR1_BACK;
//    String MOTOR2_FORWARD;
//    String MOTOR2_BACK;
//    String MOTOR3_FORWARD;
//    String MOTOR3_BACK;
//    String MOTOR4_FORWARD;
//    String MOTOR4_BACK;
    MotorController(byte motor1[3], byte motor2[3], byte motor3[3], byte motor4[3]);
    
    void motorSetter(byte motor1[3], byte motor2[3], byte motor3[3], byte motor4[3]);
    void motor_direction_setter(byte motor_id, String forward, String back);
    void moveMotor(byte motor[3], String turn, byte motor_speed);
    void stopMotor(byte pwm);
    void stopAll();
    void printMotorDefinition();
    void moveForward(byte motor_speed);
    void moveBack(byte motor_speed);
    void rotate(String turnDirection, byte motor_speed);
    void moveParallel(String parallelDirection, byte motor_speed);
    void moveDiagonal(String turnDirection, byte motor_speed);
    void moveBias(byte motor_id, byte motor_speed, byte bias, byte move_weights);
//    ~MotorController();
};


#endif
