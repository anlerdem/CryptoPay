#include <ESP8266Firebase.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

WiFiClient client;

/* Tanımlar */
const char *ssid = "anlErdem";    // WiFi ayarları
const char *password = "123456789";
#define LED D5   // Çıkış pini

double balance=0.00170320;
const char *host = "api.bscscan.com";
const int httpsPort = 443;  // HTTPS = 443 ve HTTP = 80
double extraction=0;
double deger;
int sonuc;
int i=15;
int x=60;
String zaman;
#define PROJECT_ID "projeodevi"   // Firebase Proje ID.
#define FINGERPRINT "5A 80 FB 80 F8 CD DF B4 C0 22 15 C6 42 DF 88 9A F3 C9 39 75"
Firebase firebase(PROJECT_ID, FINGERPRINT); 

// Power on setup

void setup() {
 delay(1000);
 Serial.begin(115200);
 pinMode(LED, OUTPUT);
 WiFi.mode(WIFI_OFF);        // Yeniden bağlanma sorunu önlemi (Uzun bekleme)
 delay(1000);
 WiFi.mode(WIFI_STA);        // ESP WiFi hotspot

 WiFi.begin(ssid, password);     // Wifi bağlantısı
 Serial.println("");

 Serial.print("Connecting");
 // Bağlantı bekleniyor
 while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
 }

 // Bağlantı başarılıysa ip adresi gösterilir
 Serial.println("");
 Serial.print("Connected to ");
 Serial.println(ssid);
 Serial.print("IP address: ");
 Serial.println(WiFi.localIP()); 
}

// Led

void OPENmyBlinker() {
 digitalWrite(LED, HIGH);   // Led yakma

}
void counter(){
  firebase.setInt("Durum/Calisiyor mu", 1);
   for(i=0;i>=0;i--){
      for(x=59;x>=0;x=x-10){
if(i<10&&x<10){
  zaman=String("0")+i+String(":0")+x;
  firebase.setString("KalanSure/Zaman", zaman);
  OPENmyBlinker();
      Serial.println(zaman);
      delay(3000);
      
      
  }
  if(i<10&&x>=10){
   
  zaman=String("0")+i+String(":")+x;
  firebase.setString("KalanSure/Zaman", zaman);
  OPENmyBlinker();
      Serial.println(zaman);
      delay(3000);
    }
if(i>=10&&x<10){
  
  zaman=i+String(":0")+x;
  firebase.setString("KalanSure/Zaman", zaman);
  OPENmyBlinker();
      Serial.println(zaman);
      delay(3000);
  
  }
  if(i>=10&&x>=10){
  zaman=i+String(":")+x;
  firebase.setString("KalanSure/Zaman", zaman);
  OPENmyBlinker();
      Serial.println(zaman);
      delay(3000);
  }
  
  }}

  }

// Program

void loop() {

 Serial.println(host);




 String ADCData, getData, Link;


 // Veri çağırma
 Link = "/api?module=account&action=balance&address=0xd97f85b77679bfF3661f8988a19c9F8907712815&tag=latest&apikey=YP5YHC9CI86BWX6P92E9IWZMPD3VHXGC13";

 Serial.print("requesting URL: ");
 Serial.println(host+Link);
if (client.connect(host,80))     
  { 
 client.print(String("GET ") + Link + " HTTP/1.1\r\n" +
              "Host: " + host + "\r\n" +
              "Connection: close\r\n\r\n");

 Serial.println("request sent");}

 while (client.connected()) {
   String line = client.readStringUntil('\n');
   if (line == "\r") {
     Serial.println("headers received");
     break;
   }
 }

 Serial.println("reply was:");
 Serial.println("==========");
 String line;
 while(client.available()){
   line = client.readStringUntil('\n');  // Veriyi oku

   // Stream & input;
   Serial.println(line); // Çıktıyı yazdır

   StaticJsonDocument<1024> doc;

   DeserializationError error = deserializeJson(doc, line);

   if (error) {
     Serial.print(F("deserializeJson() failed: "));
     Serial.println(error.f_str());
     return;
   }

   const char* status = doc["status"]; // "1"
   const char* message = doc["message"]; // "OK"
    const char* result = doc["result"]; // "102273320000000000"
   String stringveri = String(result); 
   stringveri.remove(6,10);
   sonuc=stringveri.toInt();
   
   deger=sonuc/100000000.0;
   Serial.print("result:");
     Serial.println(deger,8);

   if(deger <= 0) {
    Serial.println("led kapalı");  
   
    }
    
else {
      if(deger==balance){
        balance=deger;
        extraction=deger-balance;
        Serial.println(extraction,7); }
      if(deger>balance){
        extraction=deger-balance;
        Serial.println(extraction,7);   
        if(extraction>0.00009&&extraction<0.00011)
        {
          Serial.println("led açık");
        balance=deger;
        counter();
        firebase.setInt("Durum/Calisiyor mu", 0);
        digitalWrite(LED, LOW);
        zaman=String("00:59");
  firebase.setString("KalanSure/Zaman", zaman);
          }
      
        }
     
     
       }
  



 }
 Serial.println("==========");
 Serial.println("islem tamamlandı");

 delay(5000);  // 10 saniyede bir kodu çalıştır
}
