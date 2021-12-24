#include <SPI.h>  
#include <Wire.h>
#include <SoftwareSerial.h> 
#include "oled-Refrigerator.ino"
char BLUE;
SoftwareSerial BT(10,11);
//定義Arduino PIN10及PIN11分別為RX及TX腳 
#include <Adafruit_GFX.h>  
#include <Adafruit_SSD1306.h> 
#include "DHT.h"
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
 
int turn=6;
int add=7;
int decrease=8;
int temp;
boolean button=false;
boolean button2=false;
boolean change=false;
boolean transform=false;
boolean starting=false;
float settemp=6.0;

unsigned long nowtimes;
void setup() {
  BT.begin(9600); // 設定和 HC-06 通訊的速度 (預設 9600)
  dht.begin();
  pinMode(turn,INPUT);
  pinMode(add,INPUT);
  pinMode(decrease,INPUT);
  pinMode(9,OUTPUT);  //繼電器
  digitalWrite(9,LOW);
  digitalWrite(turn,HIGH);
  digitalWrite(add,HIGH);
  digitalWrite(decrease,HIGH);
  Serial.begin(9600);
   
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  display.display();
  delay(1000);
  display.clearDisplay(); 
 
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(12,10);
  display.println("Refriger"); 
  display.display();
  delay(700);
  display.clearDisplay();
  started();
  display.clearDisplay();
  
  nowtimes=millis();
  }
   
void loop() {
  
  //讀取藍芽模組訊息，並傳送到電腦
 if(millis()-nowtimes>100){
   if (BT.available()){
      BLUE=BT.read();
      Serial.println(BLUE);
      bluetooth();
   } 
   nowtimes=millis();
 }
  
  //display.drawBitmap(0,0,test_5,128, 64,1);
  
  change=false;
  //!digitalRead(7)&&!digitalRead(8) 表同時按下7跟8    
  while(change==false){
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
  display.setCursor(11,1);
  display.print("H:");
  display.print(h);
  display.print("%");
  display.setCursor(11,10);
  display.print("T:");
  display.print(t);
  display.print("C");
  display.setCursor(80,10);
  display.print(f);
  display.print("F");
  display.setCursor(5,20);
  display.print("HT:");
  display.print(hic);
  display.print("C");
  display.setCursor(80,20);
  display.print(hif);
  display.print("F"); 
  display.display();
  if(digitalRead(turn)==LOW&&change==false&&transform==false){
    change=true;
    transform=true;
    serial();
    display.setTextSize(2);
    display.clearDisplay();
    display.setCursor(0,5);
    display.print("setT:");
    display.print(settemp);
    display.display();
    }
  else if(digitalRead(turn)!=LOW&&transform==true){  
    transform=false;
    }
    }
  while(change==true){
  float t = dht.readTemperature();//讀取攝氏
  display.setTextSize(2);
  display.setTextColor(WHITE);
  while(settemp+15>=0||settemp-29<=0){
    if(t-settemp>1.5&&settemp-t<0){
      digitalWrite(9,LOW);//接常開
      }
    else{
      digitalWrite(9,HIGH);
    }
  ADD();
  Dec();  
    
  
  } 
  while(settemp==-16||settemp==30){
    settemp=6;
    serial();
  }
  if(digitalRead(turn)==LOW&&change==true&&transform==false){
    change=false;
    transform=true;
    }
  else if(digitalRead(turn)!=LOW&&transform==true){
    transform=false;
    }  
  }
    
  }
  

void started(){
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,5);
  display.print("Please set the temperature");
  display.display();
  delay(500);
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0,5);
  display.print("setT:");
  display.print(settemp);
  display.display();
  
  while(starting==false){
    if(digitalRead(turn)==LOW){
    starting=true;
    }
    ADD();
    Dec();
  }
}
void ADD(){
  if(digitalRead(add)==LOW&&button==false){
  settemp++;
  serial();
  display.clearDisplay();
  display.setCursor(0,5);
  display.print("setT:");
  display.print(settemp);
  display.display(); 
  button=true;   
  
    }
    else if(digitalRead(add)!=LOW&&button==true){
  button=false;
  }
}
void Dec(){
  if(digitalRead(decrease)==LOW&&button2==false){
  settemp--;
  serial();
  display.clearDisplay();
  display.setCursor(0,5);
  display.print("setT:");
  display.print(settemp);
  display.display(); 
  button2=true;   
  }
    else if(digitalRead(decrease)!=LOW&&button2==true){
  button2=false;
  }  
}
void serial(){
  float t = dht.readTemperature();//讀取攝氏
  
  
  Serial.print("SET:");
  Serial.println(settemp);
  Serial.print("實際與設定差:");
  Serial.println(t-settemp);
  
}
void bluetooth(){
  //判斷LED開關條件
 switch (BLUE){ 
  case 'a':
  ADD();
  break;
  case 'd':
  Dec();
  break;
  case 'O':
  Serial.println("OPEN");
  break;
  case 'F':
  Serial.println("OFF");
  break;
  }
}
