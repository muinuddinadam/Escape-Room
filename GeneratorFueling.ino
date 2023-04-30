// Define the pin number that is used for the button on the Arduino
int buttonPin = 2; // Defining the pin number that is being used for the button as 3

// Defining variables that we will need to use in this program
bool buttonState = false; // Variable that will store the button state and change depending on the state of the button (true if pressed, false if not pressed)
bool lastButtonState = false; // Variable that will store the last button state and changes depending on the last state of the button (the opposite of the buttonState)
unsigned long debounceDelay = 50; // Variable that will store the value of 50 milliseconds and be used to avoid detecting a false button presses due to electrical noise
unsigned long lastDebounceTime = 0; // Variable that will store the time the button last changed its state and work with the debounceDelay to ensure there is no false detection
unsigned long buttonHoldTime = 10000; // Variable that represents 10 seconds in millisecons, and is storing a value that represents the minimum required hold time for the button to be held down for
unsigned long buttonPressStartTime = 0; // Variable that will store the time that button was first pressed and be used to compare to the duration the button was pressed using the millis() built in timer function of Arduinos
bool counted = false; // Variable that will store whether the button has been held for 10 seconds or not


void setup() {
// Define the pin that we are going to be using in this program
pinMode(buttonPin,INPUT_PULLUP); // buttonPin is the pin we are using the for button, and we are expecting an input from it
Serial.begin(9600); // Start a serial communication with a baud rate of 9600 (not sure what this means)
}

void loop() {
  // Reads the current state of the button (the ! is a negate symbol in C++), reads true when the button is pressed and false when the button is not pressed
  bool reading = !digitalRead(buttonPin); 

  // Checks if the current button state is different than the lastButtonState, if it is then the lastDebounceTime counts the time between two button state changes (ex. from pressed to not pressed)
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  // Checks if the button state change was a false trigger or not
    // If true, there was a valid state change so we check if the current state (reading) does not equal the buttonState
      // If true, there was a valid state change so we make the buttonState = reading
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      // Checks if the buttonState is HIGH (pressed) because when the buttonState is true the button state is HIGH
        // If true, variable buttonPressStartTime counts the time from when the buttom press started using millis()
      if (buttonState) {
        buttonPressStartTime = millis();
      }
        // If false, execute the following code
          // Checks if the button is pressed for longer than 10 seconds and the event of being held down for 10 seconds 
            // If true, change counted = false to counted = true and print "Button held for 10 seconds, event conuted"
      else {
        if (millis() - buttonPressStartTime >= buttonHoldTime && !counted) {
          counted = true;
          Serial.println("Button held for 10 seconds, event counted");
        }
      }
    }
  }
  // Update the last button state to the current button state
  lastButtonState = reading;
    
}