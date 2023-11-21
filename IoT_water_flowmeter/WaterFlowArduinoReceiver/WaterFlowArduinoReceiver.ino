// Receiver Code

// Receiver pin 4
// Transmitter pin 5

#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

int pulseCount = 0;

BlynkTimer timer;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";
char pass[] = "";

void myTimer(){ 
  Blynk.virtualWrite(V0, pulseCount);
}

void setup() {
  // put your setup code here, to run once.
  // begin serial communication to use the serial monitor and communicate with the Uno.
  Serial.begin(9600);
  // begin communication with the blynk cloud via Wi-fi.
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, myTimer);
}

void loop() {
  // put your main code here, to run repeatedly.

  char buffer[20] = "";
  if (Serial.available() > 0) {
    Serial.readBytesUntil('\n', buffer, 20);
    pulseCount = atoi(buffer); // convert char to int type.
    Serial.print("Flow count value: ");
    Serial.println(pulseCount);
  }

  Blynk.run();
  timer.run();
}

float calculate_volume() {
  // Code to calculate the volume goes here...
  // Don't forget to return value.
  return 0;
}

float calculate_flow_rate() {
  // Code to calculate the flow rate goes here...
  // Don't foget to return value.
  return 0;
}

