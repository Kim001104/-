#include <Arduino.h>
#include <TaskScheduler.h>

#define LED_PIN 13

// TaskScheduler 인스턴스 생성
Scheduler runner;

// LED를 켜고 끄는 작업 정의
void LedOn();
void LedOff();
void CheckSerialInput();

Task t1(2000, TASK_FOREVER, &LedOn, &runner, true);
Task t2(0, TASK_ONCE, &LedOff, &runner, true);
Task t3(100, TASK_FOREVER, &CheckSerialInput, &runner, true);

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  runner.execute();
}

void LedOn() {
  Serial.println("[" + String(millis()) + "]LED ON");
  digitalWrite(LED_PIN, HIGH);
  t2.restartDelayed(200);  // t2 작업을 2초 뒤에 다시 시작
}

void LedOff() {
  Serial.println("[" + String(millis()) + "]LED OFF");
  digitalWrite(LED_PIN, LOW);
}

void CheckSerialInput() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    int newPeriod = input.toInt();
    if (newPeriod > 0) {
      t1.setInterval(newPeriod);
      Serial.println("New period: " + String(newPeriod));
    }
  }
}