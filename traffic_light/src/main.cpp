#include <Arduino.h>
#include <TaskScheduler.h>

// LED 핀 정의
const int RED_LED = 13;
const int YELLOW_LED = 12;
const int GREEN_LED = 11;
const int SWITCH_PIN = 2;  // 스위치 핀

// TaskScheduler 객체 생성
Scheduler runner;

// 개별 작업 선언
void task1();  // 빨간불 (2초)
void task2();  // 노란불 (0.5초)
void task3();  // 초록불 (2초)
void task4();  // 초록불 깜빡임 (1초 동안 3번)
void task5();  // 노란불 (0.5초)
void delayWithoutBlocking(unsigned long duration); // delayWithoutBlocking 함수 선언
void switchPressed();  // 스위치 인터럽트 핸들러

// Task 객체 생성 (모두 6초 주기로 실행)
Task t1(6000, TASK_FOREVER, &task1, &runner, false);
Task t2(6000, TASK_FOREVER, &task2, &runner, false);
Task t3(6000, TASK_FOREVER, &task3, &runner, false);
Task t4(6000, TASK_FOREVER, &task4, &runner, false);
Task t5(6000, TASK_FOREVER, &task5, &runner, false);

// 스위치 상태 플래그
volatile bool switchState = false;

void switchPressed() {
    switchState = !switchState;  // 상태 토글 (눌림 / 뗌);
}

void setup() {
    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(SWITCH_PIN, INPUT_PULLUP);  // 내부 풀업 사용

    // 인터럽트 설정 (CHANGE: 눌림과 뗌 모두 감지)
    attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), switchPressed, CHANGE);

    // Task 시작
    t1.enableDelayed(1);      // 즉시 실행
    t2.enableDelayed(2000);   // 2초 후 실행
    t3.enableDelayed(2500);   // 2.5초 후 실행
    t4.enableDelayed(4500);   // 4.5초 후 실행
    t5.enableDelayed(5500);   // 5.5초 후 실행
}

void loop() {
    if (switchState) {
        // 스위치 눌린 경우 -> 즉시 모든 LED 끄기, Task 멈춤
        digitalWrite(RED_LED, LOW);
        digitalWrite(YELLOW_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        runner.disableAll();
    } else {
        // 스위치 뗀 경우 -> Task 다시 실행
        if (!t1.isEnabled()) {
            runner.enableAll();
        }
    }

    runner.execute();
}

// ⏳ delay() 대신 millis() 기반 비동기 대기 함수
void delayWithoutBlocking(unsigned long duration) {
  unsigned long startTime = millis();
  while (millis() - startTime < duration) {
      if (switchState) return;  // 스위치가 눌리면 즉시 중단
  }
}

// 1️⃣ 빨간불 (2초)
void task1() {
    if (switchState) return;  // 스위치가 눌리면 즉시 종료
    digitalWrite(RED_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    delayWithoutBlocking(2000);
    digitalWrite(RED_LED, LOW);
}

// 2️⃣ 노란불 (0.5초)
void task2() {
    if (switchState) return;
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    delayWithoutBlocking(500);
    digitalWrite(YELLOW_LED, LOW);
}

// 3️⃣ 초록불 (2초)
void task3() {
    if (switchState) return;
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    delayWithoutBlocking(2000);
    digitalWrite(GREEN_LED, LOW);
}

// 4️⃣ 초록불 깜빡임 (1초 동안 3번)
void task4() {
    if (switchState) return;
    for (int i = 0; i < 3; i++) {
        digitalWrite(GREEN_LED, LOW);
        delayWithoutBlocking(250);
        digitalWrite(GREEN_LED, HIGH);
        delayWithoutBlocking(250);
    }
    digitalWrite(GREEN_LED, LOW);
}

// 5️⃣ 노란불 (0.5초)
void task5() {
    if (switchState) return;
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    delayWithoutBlocking(500);
    digitalWrite(YELLOW_LED, LOW);
}
