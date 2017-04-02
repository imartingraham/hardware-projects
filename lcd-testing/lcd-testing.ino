#include <LiquidCrystal.h>

#include "DHT.h"

#define DHTPIN 2     // what digital pin we're connected to

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal lcd(A1, A2, 10, 11, 12, 13);

// Degree symbol bitmap
byte degree[8] = {
  B01000,
  B10100,
  B01000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
};

void setup() {
  lcd.createChar(0, degree);
  lcd.begin(16,2);
  Serial.begin(115200);
  Serial.println("DHTxx test!");

}

void loop() {
//  int tempReading = analogRead(tempPin);
//  float tempVolts = (tempReading * volts) / 1024.0;
//  float tempC = (tempVolts - 0.5) * 100.0;
//  float tempF = (tempC * 9.0 / 5.0 )+ 32.0;

  float tempF = dht.readTemperature(true);
  float h = dht.readHumidity();


  if (isnan(h) || isnan(tempF)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  lcd.print("Temp:          F");
  lcd.setCursor(10, 0);
  lcd.print(tempF);
  lcd.setCursor(0, 1);

  lcd.print("Humidity:      %");
  lcd.setCursor(10, 2);
  lcd.print(h);
  lcd.setCursor(0, 0);

  delay(1500);


  
} 


