// Pinout setup
const int buttonPin = 2;      // Push button input
const int reedPin = 3;        // Relay switch input
const int relayPin = 7;       // Relay for PC powerswitch
const int buttonLEDPin = 8;   // Push button LED
const int redPin = 11;         // ^
const int greenPin = 10;      // Baseplate RGB LED pins
const int bluePin = 9;       // v

// Baseplate RGB settings
int targetRed = 30;
int targetGreen = 255;
int targetBlue = 100;

int brightness = 0;           // Current LED brightness
int fadeStep = 5;             // Brightness step per iteration

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(reedPin, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);
  pinMode(buttonLEDPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  digitalWrite(relayPin, LOW);
  digitalWrite(buttonLEDPin, LOW);
  setLEDColor(0);
}

void loop() {
  bool zoharPresent = (digitalRead(reedPin) == LOW);
  bool buttonPressed = (digitalRead(buttonPin) == LOW);

  // Button LED indicator follows pendant presence
  digitalWrite(buttonLEDPin, zoharPresent ? HIGH : LOW);

  // Handle LED fade in/out
  if (zoharPresent && brightness < 255) {
    brightness += fadeStep;
    if (brightness > 255) brightness = 255;
    setLEDColor(brightness);
    delay(45);
  } 
  else if (!zoharPresent && brightness > 0) {
    brightness -= fadeStep;
    if (brightness < 0) brightness = 0;
    setLEDColor(brightness);
    delay(45);
  }

  // Relay activation logic
  if (buttonPressed && zoharPresent) {
    digitalWrite(relayPin, HIGH);       // ^ Relay ON
    delay(500);                         // | Simulate physical time to 'press' power button
    digitalWrite(relayPin, LOW);        // v Relay OFF
    delay(1000);                        // Debounce delay
  }
}

void setLEDColor(int brightnessLevel) {
  analogWrite(redPin, (targetRed * brightnessLevel) / 255);
  analogWrite(greenPin, (targetGreen * brightnessLevel) / 255);
  analogWrite(bluePin, (targetBlue * brightnessLevel) / 255);
}
