

#define DelayTime 20


void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  for (int a = 2; a < 9; a++) {
    pinMode(a, OUTPUT);
  }
  pinMode(9, INPUT);
  digitalWrite(9, HIGH);
  pinMode(10, INPUT);
  digitalWrite(10, HIGH);

  delay(2000);
}

bool button = false;
bool buttonUp = false;
bool change = false;
int count = 0;
int plusNum = 1;

void loop() {
//  if (digitalRead(9) == LOW && buttonUp == false) {
//    plusNum = change == true ? 1 : -1;
//    buttonUp = true;
//    change = !change;
//  } else if (digitalRead(9) != LOW && buttonUp != false) {
//    buttonUp = false;
//  }
  if (digitalRead(9) == LOW && buttonUp == false) {
    count += -int(plusNum);
    buttonUp = true;
  } else if (digitalRead(9) != LOW && buttonUp != false) {
    buttonUp = false;
  }

  if (digitalRead(10) == LOW && button == false) {
    count += int(plusNum);
    button = true;
  } else if (digitalRead(10) != LOW && button != false) {
    button = false;
  }
  repeat();

}

void zero() {
  shut();
  for (int i = 1; i < 10; i++) {
    digitalWrite(i, HIGH);
  }
  digitalWrite(8, LOW);
  delay(DelayTime);
}
void one() {
  shut();
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  delay(DelayTime);
}
void two() {
  shut();
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
  delay(DelayTime);
}
void three() {
  shut();
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  delay(DelayTime);
}
void four() {
  shut();
  digitalWrite(3, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(7, HIGH);
  delay(DelayTime);
}
void five() {
  shut();
  digitalWrite(2, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  delay(DelayTime);
}
void six() {
  shut();
  //  shut();
  for (int C = 1; C < 9; C++) {
    digitalWrite(C, HIGH);
  }
  digitalWrite(3, LOW);
  delay(DelayTime);
}
void seven() {
  shut();
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(7, HIGH);
  delay(DelayTime);
}
void eight() {
  shut();
  for (int a = 1; a < 9; a++) {
    digitalWrite(a, HIGH);
  }
  delay(DelayTime);
}
void nine() {
  shut();
  for (int a = 1; a < 9; a++) {
    digitalWrite(a, HIGH);
  }
  //digitalWrite(5,LOW);
  digitalWrite(6, LOW);
  delay(DelayTime);
}
void shut() {
  for (int b = 2; b < 9; b++) {
    digitalWrite(b, LOW);
  }
}
void repeat() {
  switch (count) {
    case -1:
      count = 9;
      break;
    case 1:
      one();
      break;
    case 2:
      two();
      break;
    case 3:
      three();
      break;
    case 4:
      four();
      break;
    case 5:
      five();
      break;
    case 6:
      six();
      break;
    case 7:
      seven();
      break;
    case 8:
      eight();
      break;
    case 9:
      nine();
      break;
    default:
      count = 0;
      zero();
      break;
  }
}
