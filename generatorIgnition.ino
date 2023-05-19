const int ignitionPin = 4; // The pin that the key ignition switch is connected to
bool ignitionInitiated = false; // Variable to store the ignition state

bool lastIgnitionState = false; // Variable to store the last ignition state
unsigned long debounceDelay = 50; // Debounce delay in milliseconds
unsigned long lastDebounceTime = 0; // Variable to store the time when the ignition state last changed

void setup() {
  pinMode(ignitionPin, INPUT_PULLUP); // Set the ignition pin as input with pullup resistor
  Serial.begin(9600); // Start serial communication at 9600 baud rate
}

void loop() {
  bool reading = digitalRead(ignitionPin) == LOW; // Read the state of the key ignition switch

  if (reading != lastIgnitionState) {
    lastDebounceTime = millis(); // Update the time when the ignition state last changed
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // If the ignition state is stable for more than debounce delay
    if (reading != ignitionInitiated) {
      ignitionInitiated = reading; // Update the ignitionInitiated variable

      // Print the ignition state only when it changes
      Serial.print("Ignition state: ");
      if (ignitionInitiated) {
        Serial.println("ON");
      } else {
        Serial.println("OFF");
      }
    }
  }

  lastIgnitionState = reading; // Update the last ignition state
  delay(100); // Wait for 100 milliseconds before repeating the loop
}
