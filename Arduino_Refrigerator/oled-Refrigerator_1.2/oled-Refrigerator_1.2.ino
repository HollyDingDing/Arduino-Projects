#include <SPI.h>  
#include <DS3231.h>
#include <Wire.h>  
#include <Adafruit_GFX.h>  
#include <Adafruit_SSD1306.h> 
#include "DHT.h"
#include "motoWiFiEsp.h"
#include <SoftwareSerial.h>
#include <motoLiquidCrystal_I2C.h>

SoftwareSerial esp8266_Serial(13,12);
WiFiEspClient esp_client;
int connect_status = WL_IDLE_STATUS;

LiquidCrystal_I2C mylcd(0x27,16,2);
  String ipAddressToString(const IPAddress& ipAddress ) {
  return String(ipAddress[0]) + String(".")+
  String(ipAddress[1]) + String(".")+
  String(ipAddress[2]) + String(".")+
  String(ipAddress[3]);
}

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

DS3231 Clock;
bool Century=false;
bool h12;
bool PM;
bool ADy, A12h, Apm;
byte ADay, AHour, AMinute, ASecond, ABits;
byte year, month, date, DoW, hour, minute, second;

int lastsecond =0; 
int turn=6;
int add=7;
int decrease=8;

void setup() {
  Serial.begin(9600);
  dht.begin();
  Wire.begin();
  esp8266_Serial.begin(9600);
  //Clock.setClockMode(false);  //  24h制
  Clock.setClockMode(true); // 12h制
  pinMode(turn,INPUT);
  pinMode(add,INPUT);
  pinMode(decrease,INPUT);
  pinMode(9,INPUT);
  
  digitalWrite(turn,HIGH);
  digitalWrite(add,HIGH);
  digitalWrite(decrease,HIGH);
  Serial.println("DHT TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(); 
  esp8266_Serial.listen();
  WiFi.init(&esp8266_Serial); 
  if(WiFi.status()==WL_NO_SHIELD) {
   Serial.println(F("Esp8266 module no present"));
   while(true);
   }
  while(connect_status != WL_CONNECTED) {
   Serial.println(F("Connect to router..."));
   connect_status = WiFi.begin("Roger","artw5619");
  }
  

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
  delay(1000);
  display.clearDisplay();
  //display.print(String("IP:") + String(ipAddressToString(WiFi.localIP())));
  Serial.print(String("IP:") + String(ipAddressToString(WiFi.localIP())));  

Clock.setSecond(55);//Set the second                  設定時間,設定完註解後再燒錄一次
Clock.setMinute(29);//Set the minute
Clock.setHour(22); //Set the hour
Clock.setDoW(6); //Set the day of the week
Clock.setDate(19); //Set the date of the month
Clock.setMonth(9); //Set the month of the year
Clock.setYear(20); //Set the year (Last two digits of the year)

  }
boolean button=false;
boolean button2=false;
boolean change=false;
boolean transform=false;
int temp;
int settemp=6;
    
void loop() {
  //display.drawBitmap(0,0,test_5,128, 64,1);
  Serial.println((String("IP:") + String(ipAddressToString(WiFi.localIP()))));
  //display.print(ipAddressToString(WiFi.localIP()));
  

  int second,minute,hour,date,month,year,temperature;
  second=Clock.getSecond();
  minute=Clock.getMinute();
  hour=Clock.getHour(h12, PM);
  date=Clock.getDate();
  month=Clock.getMonth(Century);
  year=Clock.getYear();
  //temperature=Clock.getTemperature();

  if(second-lastsecond>=1  || second-lastsecond==-59 ){  //每隔一秒顯示時間

    lastsecond=second;
    Serial.print("20");  //21century
    Serial.print(year);
    Serial.print('-');
    Serial.print(month);
    Serial.print('-');
    Serial.print(date);
    Serial.print(' ');
    Serial.print(hour);
    Serial.print(':');
    Serial.print(minute);
    Serial.print(':');
    Serial.println(second);
    Serial.print("Temperature=");
    //Serial.println(temperature);
    if(!PM&&hour==7&&minute==10){
      esp8266_Serial.listen();
      float t = dht.readTemperature();//讀取攝氏
      if (esp_client.connectSSL("notify-api.line.me",443)) {
        String data = "message="+String("TEMP:")+String(t)+String("℃");
        esp_client.println("POST /api/notify HTTP/1.1");
        esp_client.println("Host: notify-api.line.me");
        esp_client.println("Authorization: Bearer KFEHa2duS6tHFIqnwKjgBXv7w02EZd8FpvuQncpygfX");
        esp_client.println("Content-Type: application/x-www-form-urlencoded");
        esp_client.print("Content-Length: ");
        esp_client.println(data.length());
        esp_client.println();
        esp_client.println(data);
        esp_client.stop();
      }
    }
    /*
    else if(hour==22&minute==30&second==30){
      irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz);
    }
    */
  }
  change=false;
  //!digitalRead(7)&&!digitalRead(8) 表同時按下7跟8    
  while(change==false){
  float h = dht.readHumidity();
  float t = dht.readTemperature();//讀取攝氏
  float f = dht.readTemperature(true);//讀取華氏
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
  float hif = dht.computeHeatIndex(f, h);//讀取體感華氏
  float hic = dht.computeHeatIndex(t, h, false);//讀取體感攝氏
  display.setTextSize(1);
  display.setTextColor(WHITE);
  Serial.println((String("Humidity:") + String(h) + String("%  Temperature: ") + String(t)+String("°C ")+String(f)+String("°F  Heat index: ")+String(hic)+String("°C ")+String(hif)+String("°F")));
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
  while(settemp+15>=0||settemp-27<=0){
    if(t-settemp>1){
      digitalWrite(9,HIGH);
      }
    if(digitalRead(add)==LOW&&button==false){
  settemp++;
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
    else if(digitalRead(decrease)==LOW&&button2==false){
  settemp--;
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
  break;
  } 
  while(settemp==-16||settemp==28){
    settemp=6;
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
