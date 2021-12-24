#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"
#include <CountTime.h>
CountTime times(0); CountTime nowtimes(0); CountTime staytime(0); CountTime PressTime(0); CountTime ConsistPress(0); CountTime Consist(0);

#define DHTPIN 2
#define DHT11_PIN 6
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH 16
#define imageWidth 96
#define imageHeight 64
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int turn = 6;
int add = 7;
int decrease = 8;
int temp;
int ConsistTime = 500;
float pressing = 50;
boolean button = false;
boolean button2 = false;
boolean change = false;
boolean transform = false;
boolean starting = false;
boolean state = false;
boolean TimeState = false;
float settemp = 6.0;



//unsigned long times, nowtimes, staytime, PressTime, ConsistPress = 0, Consist;
void setup() {
  dht.begin();
  pinMode(turn, INPUT);
  pinMode(add, INPUT);
  pinMode(decrease, INPUT);
  pinMode(9, OUTPUT); //繼電器
  digitalWrite(9, LOW);
  digitalWrite(turn, HIGH);
  digitalWrite(add, HIGH);
  digitalWrite(decrease, HIGH);
  Serial.begin(9600);


  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  display.display();
  delay(1000);
  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(12, 10);
  display.println("Refriger");
  display.display();
  delay(700);
  display.clearDisplay();
  //started();
  display.clearDisplay();
  times.Now(); nowtimes.Now(); staytime.Now(); PressTime.Now(); ConsistPress.Now() ; Consist.Now();

}

void loop() {
  //display.drawBitmap(0,0,test_5,128, 64,1);
  //!digitalRead(7)&&!digitalRead(8) 表同時按下7跟8
  while (change == false) {
    DhtSenser();
    ADD();
    Dec();
    Delaytime();
    Turn();
    delay(1);
  }

  while (change == true) {
    float t = dht.readTemperature();//讀取攝氏
    if (t - settemp > 1.5 && settemp - t < 0) {
      digitalWrite(9, LOW); //接常開
    }
    else {
      digitalWrite(9, HIGH);
    }
    ADD();
    Dec();
    Delaytime();
    serial();
    Turn();
    delay(1);
  }
}


void started() {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 5);
  for (int i = 0; i < 5; i++) {
    display.print("Please set the temperature");
    display.print(".");
    display.display();
    delay(400);
  }
  serial();
}
void ADD() {

  if (millis() - times.OldTime > 20) {
    if (settemp == -16 || settemp == 30) {
      settemp = 6;
    }
    if (digitalRead(add) == LOW && button == false) {
      settemp++;
      serial();
      staytime.OldTime = staytime.Now();
      button = true;
    }
    else if (digitalRead(add) != LOW && button == true) {
      button = false;
    }
  }
  times.OldTime = times.Now();
}
void Dec() {
  if (millis() - nowtimes.OldTime > 20) {
    if (settemp == -16 || settemp == 30) {
      settemp = 6;
    }
    if (digitalRead(decrease) == LOW && button2 == false) {
      settemp--;
      serial();
      staytime.OldTime = staytime.Now();
      button2 = true;
    }
    else if (digitalRead(decrease) != LOW && button2 == true) {
      button2 = false;
    }
  }
  nowtimes.OldTime = nowtimes.Now();
}
void serial() {
  //  float t = dht.readTemperature();//讀取攝氏
  //  Serial.print("SET:");
  //  Serial.println(settemp);
  //  Serial.print("實際與設定差:");
  //  Serial.println(t - settemp);
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 5);
  display.print("setT:");
  display.print(settemp);
  display.display();
}

void Turn() {
  if (digitalRead(turn) == LOW && change == state && transform == false) {
    state = !state;
    change = state;
    transform = true;
    staytime.OldTime = millis() - 3050;
    serial();

  }
  else if (digitalRead(turn) == HIGH && transform == true) {
    transform = false;
  }
}

void Delaytime() {
  while (millis() - staytime.OldTime < 3000) {
    if (digitalRead(add) == HIGH && digitalRead(decrease) == HIGH) {
      PressTime.OldTime = PressTime.Now();
      Consist.OldTime = Consist.Now();
    }
    TimeState = true;
    pressing = 50;
    while ( digitalRead(add) == LOW || digitalRead(decrease) == LOW && millis() - PressTime.OldTime > 2000 ) {
      if (digitalRead(add) == LOW)settemp++;
      else if (digitalRead(decrease) == LOW)settemp--;
      if (TimeState == true) {
        ConsistPress.OldTime = millis() - (Consist.OldTime + 2000);
        if (ConsistPress.OldTime >= 2 * ConsistTime ) {
          TimeState = false;
        }
      }
      else if (TimeState == false) {
        ConsistPress.OldTime = millis() - Consist.OldTime;
      }
      Serial.println(ConsistPress.OldTime);
      serial();
      if (ConsistPress.OldTime >= 2 * ConsistTime ) {
        Serial.println(pressing);
        pressing = 0.01 * pressing;
        Consist.OldTime = Consist.Now();
        ConsistTime += 100;
      }
      if (settemp == -16 || settemp == 30) {
        settemp = 6;
      }
      delay(pressing);
    }
    ADD();
    Dec();
    Turn();
    delay(60);
    Delaytime();
  }
}

void DhtSenser() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();//讀取攝氏
  float f = dht.readTemperature(true);//讀取華氏
  if (isnan(h) || isnan(t) || isnan(f)) {
    //Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  float hif = dht.computeHeatIndex(f, h);//讀取體感華氏
  float hic = dht.computeHeatIndex(t, h, false);//讀取體感攝氏
  display.setTextSize(1);
  display.setTextColor(WHITE);
  //Serial.println((String("Humidity:") + String(h) + String("%  Temperature: ") + String(t)+String("°C ")+String(f)+String("°F  Heat index: ")+String(hic)+String("°C ")+String(hif)+String("°F")));
  display.clearDisplay();
  display.setCursor(11, 1);
  display.print("H:");
  display.print(h);
  display.print("%");
  display.setCursor(11, 10);
  display.print("T:");
  display.print(t);
  display.print("C");
  display.setCursor(80, 10);
  display.print(f);
  display.print("F");
  display.setCursor(5, 20);
  display.print("HT:");
  display.print(hic);
  display.print("C");
  display.setCursor(80, 20);
  display.print(hif);
  display.print("F");
  display.display();
}
