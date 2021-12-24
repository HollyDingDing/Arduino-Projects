
unsigned long time_now=0;
void setup() {
Serial.begin(9600);
time_now=millis();
}
void loop() {
 
//millis()會一直計算時間，所以用if判斷式可以讓另一件事同時進行了，2秒後重刷
if(millis()-time_now>2000){
  Serial.println("success");
  time_now=millis();
}


}
