#include <Arduino.h>

// Defining variables
  // Toggle swithces
int firstFeaturePin = 8;
int secondFeaturePin = 9;
int firstStatePin = 10;
int secondStatePin = 11;

int feature;
int state;

bool featureOneComplete = false;
bool featureTwoComplete = false;
bool featureThreeComplete = false;
bool toggleComplete = false;
bool togglePrinted = false;

  // Fuel
int fuelButtonPin = 2;
bool fueled = false;
unsigned long fuelTimer = 0;
unsigned long fuelDuration = 1000;
int fuelButtonState = 0;
int lastFuelButtonState = 0;

  // Choke
int chokeButtonPin = 3;
bool chokeEngaged = false;
unsigned long chokeDebounceTimer = 0;

  // Ignition
int ignitionPin = 4;
bool lastIgnitionState = false;
bool generatorRunning = false;
unsigned long ignitionDebounceTimer = 0;

  // General
unsigned long debounceDelay = 50;
unsigned long lastDebounceTime = 0;
int dt = 1000;









void setup() {
  // put your setup code here, to run once:
  pinMode(firstFeaturePin, INPUT_PULLUP);
  pinMode(secondFeaturePin, INPUT_PULLUP);
  pinMode(firstStatePin, INPUT_PULLUP);
  pinMode(secondStatePin, INPUT_PULLUP);
  pinMode(fuelButtonPin, INPUT_PULLUP);
  pinMode(chokeButtonPin, INPUT_PULLUP);
  pinMode(ignitionPin, INPUT_PULLUP);
  Serial.begin(9600);
}









// Code for the toggle section of the generator puzzle
void featureSelection(){
  // Reading current state of the first toggle switch
  int firstFeatureRead = digitalRead(firstFeaturePin);
  int secondFeatureRead = digitalRead(secondFeaturePin);

  // Assigning a value to the feature variable depending on what position the first toggle switch is in
  if((firstFeatureRead == 0) && (secondFeatureRead == 1)){
    Serial.println("Social Media");
    feature = 2;
  }
  if((firstFeatureRead == 1) && (secondFeatureRead == 1)){
    Serial.println("Smart Choke");
    feature = 1;
  }
  if((firstFeatureRead == 1) && (secondFeatureRead == 0)){
    Serial.println("Air Conditioning");
    feature = 0;
  }
}


void featureState(){
  // Reading the current state of the second toggle switch
  int firstStateRead = digitalRead(firstStatePin);
  int secondStateRead = digitalRead(secondStatePin);

  // Assigning a value to the state variable depending on what position the second toggle is in
  if((firstStateRead == 0) && (secondStateRead == 1)){
    Serial.println("On");
    state = 2;
  }
  if((firstStateRead == 1) && (secondStateRead == 1)){
    Serial.println("Bypass");
    state = 1;
  }
  if((firstStateRead == 1) && (secondStateRead == 0)){
    Serial.println("Off");
    state = 0;
  }
}

void toggleCompletion() {
  // This if statement  ensures that the selected feature must be initially off, bypass, on, bypass, and back to off in order for the feature to be bypassed
if (!toggleComplete){
  featureSelection();
  if ((feature == 0) || (feature == 1) || (feature == 2)) {
    delay(dt);
    featureState();
    delay(dt);
    if (state == 0){
      featureState();
      delay(dt);
      if(state == 1){
        featureState();
        delay(dt);
        if(state == 2){
          featureState();
          delay(dt);
          if (state == 1){
            featureState();
            delay(dt);
            if(state == 0){
              if (feature == 0){
                featureOneComplete = true;
                Serial.println("Feature One Complete");
              }
              if (feature == 1){
                featureTwoComplete = true;
                Serial.println("Feature Two Complete");
              }
              if (feature == 2){
                featureThreeComplete = true;
                Serial.println("Feature Three Complete");
              }

            }
          }
        }
      }
    }
  }
}

// This if statement that makes the toggle part of the generator puzzle complete if all three features are in bypass mode
if (featureOneComplete && featureTwoComplete && featureThreeComplete){
  toggleComplete = true;
  if (!togglePrinted){
    Serial.println("Smart features bypassed");
    togglePrinted = true;
  }
}
}









// Code for the fuel section of the generator puzzle
void fuelCompletion(){
  // Reading the current state of the fuel button
  int fuelReading = digitalRead(fuelButtonPin);
  
  // This if statement checks if the button has changed state
  if (fuelReading != lastFuelButtonState) {
    fuelTimer = millis(); // Reset debounce timer
  }
  
  // This if statement checks if the button click was a valid click or if the state change was caused by electronic noise
  if ((millis() - fuelTimer) > debounceDelay) {
    if (fuelReading != fuelButtonState) {
      fuelButtonState = fuelReading;

      if (fuelButtonState == LOW) { // Button is pressed (active LOW)
        if (fuelTimer == 0) {
          fuelTimer = millis();
        }
      } else { // Button is released
        fuelTimer = 0;
      }
    }
  }

  // This if statement checks if the generator has been fueled and if the button has been held down for the length of the fuelTime or longer
  if (!fueled && fuelTimer > 0 && (millis() - fuelTimer) >= fuelDuration) {
    fueled = true;
    Serial.println("Generator fueled");
  }

  // Update the lastFuelButtonState to the current reading
  lastFuelButtonState = fuelReading;

}









// Code for the choke section of the generator puzzle
void chokeCompletion(){
  // Reads the state of the choke button
bool chokeButtonState = !digitalRead(chokeButtonPin);
unsigned long currentMillis = millis();


// Checks if the chokeButtonState is true, if the click is valid, and if the choke has been engaged, and assigns chokeEngaged = true if it is not true
if (chokeButtonState && (currentMillis - chokeDebounceTimer > debounceDelay) && !chokeEngaged){
  chokeDebounceTimer = currentMillis;
  chokeEngaged = true;
  Serial.println("Choke engaged");
}
}









// Code for the ignition section of the generator puzzle
void ignitionCompletion() {
  // Reads the ignition state and starts a timer
  bool ignitionState = !digitalRead(ignitionPin);
  unsigned long currentMillis = millis();

  // This if statement determines whether or not the ignition will turn on the generator based on the ignition state and whether the choke is engaged or not
  if (!generatorRunning) {
    if (chokeEngaged) {
      if (ignitionState != lastIgnitionState && (currentMillis - ignitionDebounceTimer > debounceDelay)) {
        ignitionDebounceTimer = currentMillis;
        if (!ignitionState) {
          lastIgnitionState = ignitionState;
        } else {
          generatorRunning = true;
          Serial.println("Generator online");
          lastIgnitionState = ignitionState;
        }
      }
    } else if (ignitionState) {
      lastIgnitionState = ignitionState;
    }
  } else {
    if (ignitionState != lastIgnitionState && (currentMillis - ignitionDebounceTimer > debounceDelay)) {
      ignitionDebounceTimer = currentMillis;
      if (!ignitionState) {
        generatorRunning = false;
        Serial.println("Generator offline");
        lastIgnitionState = ignitionState;
      }
    }
    // Assign chokeEngaged = false as after the generator has been turned on or turned off, the choke must be reset
    chokeEngaged = false;
  }
}









// Code for running the generator
void loop() {
  // put your main code here, to run repeatedly:
  fuelCompletion();
  if (fueled){
    toggleCompletion();
      if(toggleCompleted){
      chokeCompletion();
    }
  }
  ignitionCompletion();
}
