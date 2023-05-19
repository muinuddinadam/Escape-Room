#include <Arduino.h>

int firstFeaturePin = 8;
int secondFeaturePin = 9;
int firstStatePin = 10;
int secondStatePin = 11;
int fuelButtonPin = 2;
int chokeButtonPin = 3;
int ignitionPin = 4;

int feature;
int state;

bool featureOneComplete = false;
bool featureTwoComplete = false;
bool featureThreeComplete = false;
bool toggleComplete = false;
bool togglePrinted = false;
bool fueled = false;
bool chokeEngaged = false;
bool generatorRunning = false;
bool lastIgnitionState = false;

unsigned long fuelDebounceTimer = 0;
unsigned long chokeDebounceTimer = 0;
unsigned long ignitionDebounceTimer = 0;
const unsigned long debounceDelay = 50;

int dt = 1000;

void setup() {
  pinMode(firstFeaturePin, INPUT_PULLUP);
  pinMode(secondFeaturePin, INPUT_PULLUP);
  pinMode(firstStatePin, INPUT_PULLUP);
  pinMode(secondStatePin, INPUT_PULLUP);
  pinMode(fuelButtonPin, INPUT_PULLUP);
  pinMode(chokeButtonPin, INPUT_PULLUP);
  pinMode(ignitionPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void featureSelection(){
  int firstFeatureRead = digitalRead(firstFeaturePin);
  int secondFeatureRead = digitalRead(secondFeaturePin);

  if((firstFeatureRead == 0) && (secondFeatureRead == 1)){
    //Serial.println("Social Media");
    feature = 2;
  }
  if((firstFeatureRead == 1) && (secondFeatureRead == 1)){
    //Serial.println("Smart Choke");
    feature = 1;
  }
  if((firstFeatureRead == 1) && (secondFeatureRead == 0)){
    //Serial.println("Air Conditioning");
    feature = 0;
  }
}


void featureState(){
  int firstStateRead = digitalRead(firstStatePin);
  int secondStateRead = digitalRead(secondStatePin);

  if((firstStateRead == 0) && (secondStateRead == 1)){
    //Serial.println("On");
    state = 2;
  }
  if((firstStateRead == 1) && (secondStateRead == 1)){
    //Serial.println("Bypass");
    state = 1;
  }
  if((firstStateRead == 1) && (secondStateRead == 0)){
    //Serial.println("Off");
    state = 0;
  }
}

void toggleCompletion() {
  // put your main code here, to run repeatedly:

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

if (featureOneComplete && featureTwoComplete && featureThreeComplete){
  toggleComplete = true;
  if (!togglePrinted){
    Serial.println("Toggle Switch Puzzle Complete");
    togglePrinted = true;
  }
}
}

void loop() {
  bool fuelButtonState = !digitalRead(fuelButtonPin);
  bool chokeButtonState = !digitalRead(chokeButtonPin);
  bool ignitionState = !digitalRead(ignitionPin);
  unsigned long currentMillis = millis();

  // Fuel button debounce
  if (fuelButtonState && ((currentMillis - fuelDebounceTimer) > debounceDelay)) {
    fuelDebounceTimer = currentMillis;
    Serial.println("Timer started");
    if (!digitalRead(fuelButtonPin)) { // Check if the button is still pressed
      fueled = true;
    }
  }

  toggleCompletion();
  fuelCompletion();
  chokeCompletion();
  ignitionCompletion();

  // Choke button debounce
  if (chokeButtonState && currentMillis - chokeDebounceTimer > debounceDelay) {
    chokeDebounceTimer = currentMillis;
    chokeEngaged = true;
  }

  // Ignition debounce and state change detection
  if (currentMillis - ignitionDebounceTimer > debounceDelay) {
    if (!lastIgnitionState && ignitionState) {
      if (fueled && toggleComplete && chokeEngaged) {
        generatorRunning = true;
        Serial.println("Generator started");
      }
    } else if (generatorRunning && lastIgnitionState && !ignitionState) {
      generatorRunning = false;
      chokeEngaged = false; // Reset the choke state when the generator is stopped
      Serial.println("Generator stopped");
    }
    lastIgnitionState = ignitionState; // Update the last ignition state for the next iteration
    ignitionDebounceTimer = currentMillis;
  }
}
