#include <Pixy2.h>
#include <SoftwareSerial.h> // 引用「軟體序列埠」程式庫

#define CENTER_X 39
#define R 11
#define L_WEIGHT -3.0
#define R_WEIGHT 3.0


SoftwareSerial BT(3, 2); // 設定軟體序列埠(接收腳, 傳送腳)
Pixy2 pixy;

char command; // 接收序列埠值的變數
const byte EA = 9;
const byte M1 = 4;
const byte M2 = 5;
const byte M3 = 6;
const byte M4 = 7;
const byte EB = 10;

// 設定 PWM 輸出值
const int speedHigh = 60;
const int speedTurn = 80;
const int speedLow = 40;

unsigned long mTime = 0, mVec = 0;

void moveRobot(int Hx, int Hy, int Tx, int Ty) {
  int myTurn = Hx - Tx;
  int leftS, rightS;
  int maxSpeed;
  if (myTurn > 22) {
    maxSpeed = speedTurn;
    myTurn = map(myTurn, -40, 40, -80, 120);
    if (myTurn > 0) {
      leftS = maxSpeed + myTurn + (R * R_WEIGHT);
      rightS = maxSpeed - myTurn;
    } else {
      leftS = maxSpeed + myTurn - (R * L_WEIGHT);
      rightS = maxSpeed - myTurn;
    }
  } else {
    maxSpeed = speedHigh;
    if (abs((Hx + Tx) / 2 - 39) > 10 && myTurn <= 10) {
      myTurn = map(((Hx + Tx) / 2 - 39), -10, 10, -30, 30);
    } else if (abs((Hx + Tx) / 2 - 39) <= 10 && myTurn > 10) {
      myTurn = map(myTurn, -15, 15, -40, 40);
    } else if (abs((Hx + Tx) / 2 - 39) > 10 && Hx - Tx > 10) {
      myTurn = map(myTurn, -15, 15, -40, 40);
    }
    leftS = maxSpeed + myTurn;
    rightS = maxSpeed - myTurn;

  }
  
  if (leftS > 150) {
    leftS = 150;
  } else if (leftS < -150) {
    leftS = -150;
  }
  if (rightS > 150) {
    rightS = 150;
  } else if (rightS < -150) {
    rightS = -150;
  }
  if (millis() - mTime > 500) {
    Serial.println("LeftS: " + String(leftS) + ", " + "RightS: " + String(rightS));
    Serial.println("MyTurn: " + String(myTurn));
    mTime = millis();
  }
  
  if (leftS > 0) {
    digitalWrite(M1, HIGH);
    digitalWrite(M2, LOW);
  } else {
    digitalWrite(M1, LOW);
    digitalWrite(M2, HIGH);
  }

  if (rightS > 0) {
    digitalWrite(M3, HIGH);
    digitalWrite(M4, LOW);
  } else {
    digitalWrite(M3, LOW);
    digitalWrite(M4, HIGH);
  }
  analogWrite(EA, abs(leftS));
  analogWrite(EB, abs(rightS));
  delay(100);
}

void setup() {
  Serial.begin(115200);
  Serial.print("Starting...\n");
  pixy.init();
  pixy.setLamp(1, 1);
  Serial.println(pixy.changeProg("line"));
  pinMode(EA, OUTPUT);
  pinMode(EB, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(M3, OUTPUT);
  pinMode(M4, OUTPUT);
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  digitalWrite(M3, HIGH);
  digitalWrite(M4, LOW);
  stop();

  mTime = millis();
  mVec = millis();
}



int turn = 0;

void loop() {
  int8_t i;
  uint8_t Hx, Hy, Tx, Ty;
  char buf[128];
  pixy.line.getAllFeatures();
  if (millis() - mVec > 500) {
    pixy.line.vectors[0].print();
    mVec = millis();
  }
  
  Hx = pixy.line.vectors[0].m_x1;
  Hy = pixy.line.vectors[0].m_y1;
  Tx = pixy.line.vectors[0].m_x0;
  Ty = pixy.line.vectors[0].m_y0;

  if (Hy - Ty > 0) {
    moveRobot(Tx, Ty, Hx, Hy);
  } else if (Hy - Ty < 0) {
    moveRobot(Hx, Hy, Tx, Ty);
  } else {
    stop();
  }
//  moveCar(Hx, Hy, Tx, Ty);
}










void moveCar(int Hx,int Hy, int Tx, int Ty) {
  if ((Hx - Tx) > 15) {
    Serial.println("Turn Right\n");
    turnRight();
  } else if ((Hx - Tx) < -15) {
    Serial.println("Turn Left\n");
    turnLeft();
  } else if ((Hx - Tx) > 2) {
    Serial.println("little Right\n");
    littleRight();
  } else if ((Hx - Tx) < -2) {
    Serial.println("little Left\n");
    littleLeft();
  } else {
    Serial.println("Forward\n");
    forward();
  }
}



void stop() { // 馬達停止
  analogWrite(EA, 0); // 馬達 A 的 PWM 輸出
  analogWrite(EB, 0); // 馬達 B 的 PWM 輸出
}
void forward() { // 馬達轉向：前進
  analogWrite(EA, speedHigh); // 馬達 A 的 PWM 輸出
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  analogWrite(EB, speedHigh); // 馬達 A 的 PWM 輸出
  digitalWrite(M3, HIGH);
  digitalWrite(M4, LOW);
}
void backward() { // 馬達轉向：後退
  analogWrite(EA, speedHigh); // 馬達 A 的 PWM 輸出
  digitalWrite(M2, HIGH);
  digitalWrite(M1, LOW);
  analogWrite(EB, speedHigh); // 馬達 A 的 PWM 輸出
  digitalWrite(M4, HIGH);
  digitalWrite(M3, LOW);
}
void turnRight() { // 馬達轉向：右轉
  analogWrite(EA, speedTurn); // 馬達 A 的 PWM 輸出
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  analogWrite(EB, 0); // 馬達 A 的 PWM 輸出
  digitalWrite(M3, HIGH);
  digitalWrite(M4, LOW);
}
void turnLeft() { // 馬達轉向：左轉
  analogWrite(EA, 0); // 馬達 A 的 PWM 輸出
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  analogWrite(EB, speedTurn); // 馬達 A 的 PWM 輸出
  digitalWrite(M3, HIGH);
  digitalWrite(M4, LOW);
}
void littleRight() { // 馬達微調轉向：右轉
  analogWrite(EA, speedHigh); // 馬達 A 的 PWM 輸出
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  analogWrite(EB, speedLow); // 馬達 A 的 PWM 輸出
  digitalWrite(M3, HIGH);
  digitalWrite(M4, LOW);
}
void littleLeft() { // 馬達微調轉向：左轉
  analogWrite(EA, speedLow); // 馬達 A 的 PWM 輸出
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  analogWrite(EB, speedHigh); // 馬達 A 的 PWM 輸出
  digitalWrite(M3, HIGH);
  digitalWrite(M4, LOW);
}



void moveBias(int bias) {
  Serial.print("Ori: "+ String(bias) + ", ");
  bias = constrain(bias, -40, 40);
  Serial.print("Constrain: " + String(bias) + ", ");
  bias = map(bias, -40, 40, -60, 60);
  Serial.println("Map: " + String(bias) + ", ");
  int speedL = speedHigh + bias;
  int speedR = speedHigh - bias;
  speedL = constrain(speedL, -40, speedHigh + 80);
  speedR = constrain(speedR, -40, speedHigh + 80);
  if (speedL < 0) {
    digitalWrite(M1, LOW);
    digitalWrite(M2, HIGH);
  } else {
    digitalWrite(M1, HIGH);
    digitalWrite(M2, LOW);
  }
  if (speedR < 0) {
    digitalWrite(M3, LOW);
    digitalWrite(M4, HIGH);
  } else {
    digitalWrite(M3, HIGH);
    digitalWrite(M4, LOW);
  }
  Serial.println("LS: " + String(speedL) + ", " + "RS: " + String(speedR));
  analogWrite(EA, speedL);
  analogWrite(EB, speedR);
  if (speedL < 5 ||speedR < 5) {
    delay(500);
  }
}
