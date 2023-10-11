#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "DHT.h"
//lcd
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//
LiquidCrystal_I2C lcd(0x3F, 20, 4);

#define lightPin A0
#define DHTTYPE DHT11
#define soiPin D6
const int DHTPin = 2;

//SSID dan Password wifi mu gan.const
const char* ssid = "UwU";           //--> Nama Wifi / SSID.
const char* password = "00000000";  //-->  Password wifi .


const char* host = "script.google.com";
const int httpsPort = 443;

// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

WiFiClientSecure client;  //--> Create a WiFiClientSecure object.

// Timers auxiliar variables
long now = millis();
long lastMeasure = 0;

String GAS_ID = "AKfycbykrW30e6v49Mm4zePj3P5cYGC_qQx2Lvnbj3NjNWVAyQM0Oh27FSHKqrWHRJ839X0";  //--> spreadsheet script ID

//============================================ void setup
void setup() {
  //lcd
  lcd.init();  // initialize the lcd

  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(2, 1);
  // put your setup code here, to run once:
  pinMode(D6, INPUT);
  Serial.begin(115200);
  delay(500);
  dht.begin();

  WiFi.begin(ssid, password);  //--> Connect to your WiFi router
  Serial.println("");


  //----------------------------------------Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  //----------------------------------------

  client.setInsecure();
}
//==============================================================================
//============================================================================== void loop
void loop() {
  delay(1000*60);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  int l = getLight();
  int mo = getMoise();
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  float hic = dht.computeHeatIndex(t, h, false);
  static char temperatureTemp[7];
  dtostrf(hic, 6, 2, temperatureTemp);
  static char humidityTemp[7];
  dtostrf(h, 6, 2, humidityTemp);



  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t Heat index: ");
  Serial.print(hic);
  Serial.println(" *C ");
  //
  sendData(t, h, l, mo);
}
//*****
//==============================================================================

void sendData(float value, float value2, float value3, float value4) {
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);

  //----------------------------------------Connect to Google host
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  //----------------------------------------

  //----------------------------------------Proses dan kirim data

  float string_temp = value;
  float string_humi = value2;
  float string_light = value3;
  float string_soi = value4;
  String text_temp;
  String text_light;
  String text_hm;
  String text_soi;

  //light
  if (string_light < 200) {
    text_light = "%E0%B8%95%E0%B8%A3%E0%B8%87%E0%B8%99%E0%B8%B5%E0%B9%89%E0%B9%81%E0%B8%AA%E0%B8%87%E0%B8%81%E0%B8%B3%E0%B8%A5%E0%B8%B1%E0%B8%87%E0%B8%94%E0%B8%B5%E0%B9%80%E0%B8%9A%E0%B8%A2%E0%B8%A2";
    lcd.setCursor(2, 2);
    lcd.printf("(@ u @");
  } else {
    text_light = "%E0%B8%95%E0%B8%A3%E0%B8%87%E0%B8%99%E0%B8%B5%E0%B9%89%E0%B8%A1%E0%B8%B7%E0%B8%94%E0%B8%88%E0%B8%B1%E0%B8%87%E0%B9%80%E0%B8%A5%E0%B8%A2%E0%B8%99%E0%B9%89%E0%B8%B2%E0%B8%B2";
    warningSign();
    lcd.setCursor(2, 2);
    lcd.printf("(0 0)");
  }
  //soi
  if (string_soi == 1) {
    text_soi = "%E0%B8%84%E0%B8%AD%E0%B9%81%E0%B8%AB%E0%B9%89%E0%B8%87%E0%B9%81%E0%B8%A5%E0%B9%89%E0%B8%A7%E0%B8%99%E0%B9%89%E0%B8%B2%E0%B8%B2";
  } else {
    lcd.printf("(o w o)");
    text_soi = "%E0%B8%AD%E0%B8%B4%E0%B9%88%E0%B8%A1%E0%B8%AD%E0%B8%A2%E0%B8%B9%E0%B9%88%E0%B9%80%E0%B8%A5%E0%B8%A2";
  }
  //textT
  if (string_temp > 45) {
    text_temp = "%E0%B8%95%E0%B8%A3%E0%B8%87%E0%B8%99%E0%B8%B5%E0%B9%89%E0%B8%A3%E0%B9%89%E0%B8%AD%E0%B8%99%E0%B8%A1%E0%B8%B2%E0%B8%81%E0%B9%80%E0%B8%A5%E0%B8%A2%E0%B8%A2";
  } else if (string_temp < 10) {
    text_temp = "%E0%B8%AB%E0%B8%99%E0%B8%B2%E0%B8%A7%E0%B8%A1%E0%B8%B2%E0%B8%81%E0%B9%80%E0%B8%A5%E0%B8%A2%E0%B8%A2";
  } else {
    text_temp = "%E0%B8%AA%E0%B8%9A%E0%B8%B2%E0%B8%A2%E0%B8%88%E0%B8%B1%E0%B8%87%E0%B9%80%E0%B8%A5%E0%B8%A2%E0%B8%99%E0%B9%89%E0%B8%B2%E0%B8%B2";
  }
  String url = "/macros/s/" + GAS_ID + "/exec?temp=" + string_temp + "&humi=" + string_humi + "&light=" + text_light + "&soi=" + text_soi + "&textT=" + text_temp;  //  2 variables
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "User-Agent: BuildFailureDetectorESP8266\r\n" + "Connection: close\r\n\r\n");

  Serial.println("request sent");
  //----------------------------------------

  //---------------------------------------
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
  //----------------------------------------
}
int getLight() {
  int values = analogRead(lightPin);
  return values;
}
int getMoise() {
  int values = digitalRead(soiPin);
  return values;
}
void warningSign() {
  for (int i = 0; i < 9; i++) {
    lcd.setCursor(2, 1);
    lcd.printf("      ");
    lcd.printf("     !");
  }
}
