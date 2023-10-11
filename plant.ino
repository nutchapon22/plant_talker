//include dht libery
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTTYPE DHT11
#define DHTPIN 2
DHT dht(DHTPIN, DHTTYPE);
//light sensor
#define lightPin A0

//soi sensor
#define soiPin D1

//include LIne Notify and set token and wifi
#include <TridentTD_LineNotify.h>

#define SSID ""
#define PASSWORD ""
#define LINE_TOKEN ""


void setup() {
  // put your setup code here, to run once:
  dht.begin();
  pinMode(D1, INPUT);
  //Line notify setup
  Serial.begin(115200);
  Serial.println(LINE.getVersion());

  //Wi-Fi setup
  WiFi.begin(SSID, PASSWORD);
  Serial.printf("Wi-Fi connecting to: \n", SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("hmmm..");
    delay(1000);
  }
  Serial.printf("\nWi-Fi connected ^^\n");
  //end of setup

  //set line
  LINE.setToken(LINE_TOKEN);
}

void loop() {
  //delay so that the sensor is not overloaded or not too stressed
  delay(1000 * 60 * 60);

  //measures light
  int light = getLight();
  Serial.printf("Light: %d\n", light);

  //measures temperature(C)
  float temperature = getTemperature();
  Serial.printf("Temperature: %f\n", temperature);

  //measures moisture in dirt
  int moisture = getMoise();
  Serial.printf("Moisture: %d\n", moisture);

  displayNotify(light, temperature, moisture);
}
int getLight() {
  int values = analogRead(lightPin);
  return values;
}

float getTemperature() {
  float values = dht.readTemperature();
  return values;
}

int getMoise() {
  int values = digitalRead(soiPin);
  return values;
}

void displayNotify(int light, int temperature, float moisture) {
  int wet = 0;
  int dry = 1;

  //give status conditioF for Notify

  //light status
  if (light < 500) {
    LINE.notify("dude, i think it's really dark here...");
  } else {
    LINE.notify("hmmm, it's time to sunbathing! ^^");
  }

  //temperature status
  if (temperature < 20) {
    LINE.notify("damn man, i'm too cool");
  } else if (temperature >= 30) {
    LINE.notify("damn i'm too hot, too hot!");
  } else {
    LINE.notify("hmmm.., not bad not bad ^^");
  }

  //mois status
  if (moisture < wet) {
    LINE.notify("damn dude that's too wet, too wet");
  } else if (moisture >= dry) {
    LINE.notify("damn dud that's too dry, give me some water");
  } else {
    LINE.notify("That's perfect mois ^^");
  }
}
