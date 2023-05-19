// Defining variables
const int buttonPin = 3; // The pin that the button is connected to
bool buttonState = false; // Variable for storing the button state (true if pressed, false if not pressed)

// Debounce-related variables
bool lastButtonState = HIGH;
unsigned long debounceDelay = 50;
unsigned long lastDebounceTime = 0;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // Configure buttonPin as INPUT with internal pull-up resistor
  Serial.begin(9600); // Start serial communication at 9600 baud rate
}

void loop() {
  bool reading = digitalRead(buttonPin); // Read the current state of the button

  // Check for button state change
  if (reading != lastButtonState) {
    lastDebounceTime = millis(); // Save the current time as the last debounce time
  }

  // If the button state has been stable for more than debounceDelay milliseconds, update buttonState
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) { // Button is active-low, so LOW means it's pressed
        Serial.println("Button state: true");
      } else {
        Serial.println("Button state: false");
      }
    }
  }

  lastButtonState = reading; // Save the last button state for the next loop iteration
}
