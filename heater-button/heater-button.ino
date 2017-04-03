
/*
 *  Simple HTTP get webclient test
 */
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

#include "config.h"

const int LED_PIN = 15;
const int BUTTON_PIN = 12;

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  payload[length] = '\0';
  String receivedChars = String((char*)payload);
  
  Serial.print(receivedChars);
  
  if(receivedChars == "on"){
    digitalWrite(LED_PIN, HIGH);
  }
  if(receivedChars == "off"){
    digitalWrite(LED_PIN, LOW);
  }
  Serial.println();
}

void reconnect() {
  while(!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if(client.connect("ESP8266 Client")) {
      Serial.println("connected");
      client.subscribe(HEATER_STATUS_TOPIC);
    }else{
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  client.setServer(MQTT_HOST, 1883);
  client.setCallback(callback);

  pinMode(0, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  delay(100);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  
  WiFi.begin(WIFI_SSID, WIFI_PASS);
    
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;
int buttonState = 0;
int ledState = 0;
int servoAngle = 0;
void loop() {
  digitalWrite(0, HIGH);
  buttonState = digitalRead(BUTTON_PIN);
  
  if (buttonState == LOW){
    Serial.println("BUTTON PRESSED");
    Serial.println("Toggle Heater");
    client.publish(HEATER_TOPIC, "0");

    delay(1000);
  }
  if(!client.connected()) {
    reconnect();
  }
  client.loop();

}
