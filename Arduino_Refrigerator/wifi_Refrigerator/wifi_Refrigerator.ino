#include <WiFi.h>
#include <SimpleDHT.h>
#include <WiFiClientSecure.h>


//請修改以下參數--------------------------------------------
char SSID[] = "Roger";
char PASSWORD[] = "artw5619";
String Linetoken = "xafn0N69HWsteJaeY5efpGmCZzRN5kiqyT7UKji276M";
int pinDHT11 = 14;//假設DHT11接在腳位GPIO14，麵包板左側序號8
//---------------------------------------------------------
SimpleDHT11 dht11(pinDHT11);//DHT11物件
WiFiClientSecure client;//網路連線物件
char host[] = "notify-api.line.me";//LINE Notify API網址

void setup() {
  Serial.begin(115200);
  //連線到指定的WiFi SSID
  Serial.print("Connecting Wifi: ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  //連線成功，顯示取得的IP
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
}

void loop() {

  //嘗試讀取溫濕度內容
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.println(err); delay(1000);
    return;
  }
  //讀取成功，將溫濕度顯示在序列視窗
  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, ");
  Serial.print((int)humidity); Serial.println(" H");

  //設定觸發LINE訊息條件為溫度超過30或濕度超過80
  if ((int)temperature >= 30 || (int)humidity >= 80) {
    //組成Line訊息內容
    String message = "檢測環境發生異常，請協助儘速派人查看處理，目前環境狀態：";
    message += "\n溫度=" + String(((int)temperature)) + " *C";
    message += "\n濕度=" + String(((int)humidity)) + " H";
    Serial.println(message);
    if (client.connect(host, 443)) {
      int LEN = message.length();
      //傳遞POST表頭
      String url = "/api/notify";
      client.println("POST " + url + " HTTP/1.1");
      client.print("Host: "); client.println(host);
      //權杖
      client.print("Authorization: Bearer "); client.println(Linetoken);
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: "); client.println( String((LEN + 8)) );
      client.println();      
      client.print("message="); client.println(message);
      client.println();
      //等候回應
      delay(2000);
      String response = client.readString();
      //顯示傳遞結果
      Serial.println(response);
      client.stop(); //斷線，否則只能傳5次
    }
    else {
      //傳送失敗
      Serial.println("connected fail");
    }
  }
  //每5秒讀取一次溫濕度
  delay(5000);
}
