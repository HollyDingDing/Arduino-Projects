


#define numOfValsRec 2
#define digitsPerValRec 4

int valsRec[numOfValsRec];
int stringLength = numOfValsRec * digitsPerValRec;
int counter = 0;
bool counterStart = false;
String recString;

void recData() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '$') {
      counterStart = true;
    }
    if (counterStart) {
      if (counter < stringLength) {
        recString = String(recString + c);
        counter++;
      }
      if (counter >= stringLength) {
        for (int i = 0; i < numOfValsRec; i++) {
          int num = (i * digitsPerValRec) + 1;
          valsRec[i] = recString.substring(num, num + digitsPerValRec).toInt();
        }

        recString = "";
        counter = 0;
        counterStart = false;
      }
      
    }
  }
}




int myPins[6] = {3, 4, 5, 11, 10, 9};

void moveRobot(int mySpeed, int myTurn, int maxSpeed = 255) {
  mySpeed = map(mySpeed, -100, 100, -maxSpeed, maxSpeed);  
  myTurn = map(myTurn, -100, 100, -maxSpeed, maxSpeed);
  int leftS = mySpeed - myTurn;
  int rightS = mySpeed + myTurn;

  leftS = constrain(leftS, -maxSpeed, maxSpeed);
  rightS = constrain(rightS, -maxSpeed, maxSpeed);
  
  if (leftS > 0) {
    digitalWrite(myPins[3], 0);
    digitalWrite(myPins[4], 1);
  } else {
    digitalWrite(myPins[3], 1);
    digitalWrite(myPins[4], 0);
  }

  if (rightS > 0) {
    digitalWrite(myPins[1], 0);
    digitalWrite(myPins[2], 1);
  } else {
    digitalWrite(myPins[1], 1);
    digitalWrite(myPins[2], 0);
  }
  analogWrite(myPins[5], abs(leftS));
  analogWrite(myPins[0], abs(rightS));
}

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 6; i++) {
    pinMode(i, OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  recData();
  moveRobot(valsRec[0], valsRec[1]);
}
