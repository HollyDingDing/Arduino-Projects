#include <DS3231.h>
#include <Wire.h>
#include <Adafruit_GFX.h>          // Adafruit GFX graphics core library
#include <Adafruit_SSD1306.h>// Driver library for 'monochrome' 128x64 and 128x32 OLEDs
#include <IRremote.h>

IRsend irsend;
int khz = 38;

DS3231 Clock;
bool Century=false;
bool h12;
bool PM;
bool ADy, A12h, Apm;
byte ADay, AHour, AMinute, ASecond, ABits;

byte year, month, date, DoW, hour, minute, second;

int lastsecond =0;

#define OLED_RESET 4

Adafruit_SSD1306 display(OLED_RESET);
    
#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");  //沒修改Adafruit_SSD1306.h會被這行卡住
#endif


void setup() {
// Start the I2C interface
Wire.begin();
//Clock.setClockMode(false);  //  24h制
  Clock.setClockMode(true); // 12h制

 
/*
Clock.setSecond(30);//Set the second                  設定時間,設定完註解後再燒錄一次
Clock.setMinute(10);//Set the minute
Clock.setHour(0); //Set the hour
Clock.setDoW(7); //Set the day of the week
Clock.setDate(20); //Set the date of the month
Clock.setMonth(9); //Set the month of the year
Clock.setYear(20); //Set the year (Last two digits of the year)
*/

display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  display.clearDisplay();
  
  display.display();
  


// Start the serial interface
Serial.begin(9600);
pinMode(4,OUTPUT);
pinMode(5,INPUT);
//digitalWrite(4,LOW);
digitalWrite(5,HIGH);
}
void loop()
{
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



display.setTextSize(2);
display.setTextColor(WHITE);
display.setCursor(2, 8);  //座標
display.print(hour);
display.setCursor(31, 8);
display.print(":");  
display.setCursor(47, 8);
display.print(minute);  
display.setCursor(76, 8);
display.print(":");  
display.setCursor(92, 8);
display.println(second); 
unsigned int irSignal[] = {600,3450, 600,500, 600,500, 550,550, 600,450, 600,500, 600,500, 600,1500, 600,1500, 550,550, 550,550, 550,550, 550,550, 550,550, 550,500, 550,550, 550,550, 550,550, 550,550, 550,550, 550,550, 550,500, 600,500, 600,500, 600,500, 600,500, 550,550, 600,450, 600,500, 600,500, 600,500, 600,500, 600,500, 600,500, 600,500, 600,500, 550,500, 550,1550, 550,550, 600,500, 600,500, 600,500, 550,550, 550,550, 550,550, 550,1500, 600,500, 600,1500, 600,1500, 550,1550, 550,500, 600,500, 550,1550, 550,3500, 550}; //AnalysIR Batch Export (IRremote) - RAW
  //開冷氣數值
  //irSignal這串只是名字,可以更改
  //要讀取冷氣訊號可以用範例IRrecvDumpV2


if(PM&hour==10&minute==30&second==30){
  irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz);
  }
else if(hour==22&minute==30&second==30){
  irsend.sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz);
  }
 

if (h12) {
  
    if (PM) {
    display.setTextSize(1);
    display.setCursor(106,25);
    display.println("PM");  
    }
    else {
    display.setTextSize(1);
    display.setCursor(106, 25);
    display.println("AM");  
   }
 }
 else {
    display.setTextSize(1);
    display.setCursor(106, 25);
    display.println("24H");  
   }
 //display.setTextSize(1);  //顯示溫度
 //display.setCursor(2, 2);
// display.print(temperature);
// display.println(" C");  
 display.display();
 display.clearDisplay();
      






}






delay(30);
}
