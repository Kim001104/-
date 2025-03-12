#include <Arduino.h>
#include <TaskScheduler.h>

const int redLedPin = 13;
const int yellowLedPin = 12;
const int greenLedPin = 11;

Scheduler runner;

void toggleRedLed();
void toggleYellowLed();
void toggleGreenLed();

Task tRedLed(1000, TASK_FOREVER, &toggleRedLed, &runner, true);
Task tYellowLed(1000, TASK_FOREVER, &toggleYellowLed, &runner, false);
Task tGreenLed(1000, TASK_FOREVER, &toggleGreenLed, &runner, false);

void setup() {
  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
}

void loop() {
  runner.execute();
}

void toggleRedLed() {
  static bool state = false;
  digitalWrite(redLedPin, state ? HIGH : LOW);
  state = !state;
  if (!state) {
    tRedLed.disable();
    tYellowLed.enable();
  }
}

void toggleYellowLed() {
  static bool state = false;
  digitalWrite(yellowLedPin, state ? HIGH : LOW);
  state = !state;
  if (!state) {
    tYellowLed.disable();
    tGreenLed.enable();
  }
}

void toggleGreenLed() {
  static bool state = false;
  digitalWrite(greenLedPin, state ? HIGH : LOW);
  state = !state;
  if (!state) {
    tGreenLed.disable();
    tRedLed.enable();
  }
}