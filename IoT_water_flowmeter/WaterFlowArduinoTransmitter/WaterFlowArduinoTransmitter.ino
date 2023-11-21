// Transmitter Code

// Push button pin 4
// Flow Sensor pin 18, an external interrupt pin
// Receiver pin 5
// Transmitter pin 6

#include <SoftwareSerial.h>

SoftwareSerial link(5, 6); // Rx, Tx

const int flowsensorPin = 18;
const int pushButton = 4;
int buttonState;
bool stateVariable = true;
volatile long int pulseCount; // remove int if an error is encountered.

void setup() {
  // put your setup code here, to run once.
  pinMode(flowsensorPin, INPUT);
  pinMode(pushButton, INPUT);

  // to look out for external interrupts from the water flow sensor.
  attachInterrupt(digitalPinToInterrupt(flowsensorPin), incrementPulseCount, RISING);

  // begin serial communication between transmitter (Uno 3) and receiver (esp32).
  link.begin(9600);
  // begin serial communication to use the serial monitor.
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly.

  buttonState = digitalRead(pushButton);

  print_data();

  // if condition to prevent continuous writing of data on pressing the momentary switch.  
  if (buttonState == HIGH && stateVariable == true) {
    link.println(pulseCount);
    stateVariable = false;
  }
  else if (buttonState == LOW && stateVariable == false) {
    stateVariable = true;
  }

}

// write_data function to write the recorded data of the water flow sensor
// into the serial monitor and transmit it to the esp32. 
void print_data() {
  Serial.print("Water flow count: ");
  Serial.println(pulseCount);
  
  delay(1000);
}

void incrementPulseCount() {
  pulseCount++;
}

int calculateVolume() {
  // Code to calculate the volume goes here...
  // Don't forget to return value.
}

int calculateFlowRate() {
  // Code to calculate the flow rate goes here...
  // Don't foget to return value.
}