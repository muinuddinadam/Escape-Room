#include <SPI.h>
#include <Ethernet.h>
#include <FastLED.h>

byte mac[] = {0xDE, 0xED, 0xBE, 0xEF, 0xFD, 0xEA}; // Setting MAC address (must be unique for each device)
IPAddress ip(192, 168, 0, 158); // Setting IP address (must be unique for each device)
EthernetClient client; // Initialize the Ethernet client
EthernetServer server(49155); // Setting the ethernet server

const int buttonPin = 2; // The pin that the button is connected to
bool buttonState = false;
bool lastButtonState = false;
unsigned long debounceDelay = 50;
unsigned long lastDebounceTime = 0;
unsigned long buttonHoldTime = 10000;
unsigned long buttonPressStartTime = 0;
bool counted = false;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
}

void loop() {
  bool reading = !digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState) {
        buttonPressStartTime = millis();
      } else {
        if ((millis() - buttonPressStartTime) >= buttonHoldTime && !counted) {
          counted = true;
          Serial.println("Button held for 10 seconds, event counted");
          client.write("Generator online");
        }
        else{
          client.write("Generator still offline");
          Serial.println("Generator still offline");
        }
      }
    }
  }
  
  lastButtonState = reading;

  EthernetClient client = server.available();
  if (client) {
    Serial.println("Client connected");
    while (client.connected()) {
      if (client.available()) {
        String message = client.readStringUntil('\n');
        message.trim();
        Serial.println("Received message: " + message);

        if (message == "generatorrequest") {
          if (counted) {
            Serial.println("Generator Online");
            client.write("Generator Online");
            counted = false; // Reset the counted flag so it can detect the next button hold event
          } else {
            Serial.println("Generator still offline");
            client.write("Generator still offline");
          }
        }
      }
    }
    client.stop();
  }
}
