#define BLYNK_TEMPLATE_ID "TMPL6IVCuBlfd"
#define BLYNK_TEMPLATE_NAME "Mushroom Monitoring"
#define BLYNK_AUTH_TOKEN "vSpvpobSgEw4KjD_jGUS95rScifn-odg"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "POCO M3";  // type your wifi name
char pass[] = "satyanandagautama";  // type your wifi password
BlynkTimer timer;
#define DHTPIN 27 //Connect Out pin to D2 in NODE MCU
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define KIPAS 25
#define POMPA 26
#define VPIN_HUMIDITY    V1
#define VPIN_TEMPERATURE V0
#define VPIN_KIPAS       V2
#define VPIN_POMPA       V3

float TemperatureMax = 29;
int HumidityMin = 70;
float t;
int h;

BLYNK_CONNECTED() {
  Blynk.syncVirtual(VPIN_HUMIDITY);
  Blynk.syncVirtual(VPIN_TEMPERATURE);
  Blynk.syncVirtual(VPIN_KIPAS);
  Blynk.syncVirtual(VPIN_POMPA);
}

void sendSensor() {
  t = dht.readTemperature();
  h = dht.readHumidity();
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t);   // Printing terature on LCD
  lcd.print(" *C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(h);   // Printing terature on LCD
  lcd.print(" %");
}

void AutoControl() {
  if (t >= TemperatureMax) {
    digitalWrite(25, LOW);
    Blynk.virtualWrite(VPIN_KIPAS, HIGH);
    delay(500);
  }
  else
  {
    digitalWrite(25, HIGH);
    Blynk.virtualWrite(VPIN_KIPAS, LOW);
    delay(500);
  }
  if (h <= HumidityMin) {
    digitalWrite(26, LOW);
    Blynk.virtualWrite(VPIN_POMPA, HIGH);
    delay(500);
  }
  else {
    digitalWrite(26, HIGH);
    Blynk.virtualWrite(VPIN_POMPA, LOW);
    delay(500);
  }
}

void setup() {
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  Serial.begin(115200);
  lcd.begin();
  lcd.backlight();
  lcd.print("Mushroom");
  lcd.setCursor(0, 1);
  lcd.print("Monitoring");
  delay(3000);
  lcd.clear();
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(500L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
  AutoControl();
}
