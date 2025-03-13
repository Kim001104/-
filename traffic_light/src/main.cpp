#include <TaskScheduler.h>

// LED 핀 정의
const int RED_LED = 13;
const int YELLOW_LED = 12;
const int GREEN_LED = 11;
const int SWITCH_PIN1 = 2;  // 스위치 1번 (Emergency)
const int SWITCH_PIN2 = 3;  // 스위치 2번

// TaskScheduler 객체 생성
Scheduler runner;

// 개별 작업 선언
void task1();
void task2();
void task3();
void task4();
void task5();

// Task 객체 생성 (6초 주기로 실행)
Task t1(6000, TASK_FOREVER, &task1, &runner, false);
Task t2(6000, TASK_FOREVER, &task2, &runner, false);
Task t3(6000, TASK_FOREVER, &task3, &runner, false);
Task t4(6000, TASK_FOREVER, &task4, &runner, false);
Task t5(6000, TASK_FOREVER, &task5, &runner, false);

void setup() {
    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    
    pinMode(SWITCH_PIN1, INPUT_PULLUP);  // 내부 풀업 사용
    pinMode(SWITCH_PIN2, INPUT_PULLUP);  // 내부 풀업 사용

    // Task 시작
    t1.enableDelayed(1);      // 즉시 실행
    t2.enableDelayed(2000);   // 2초 후 실행
    t3.enableDelayed(2500);   // 2.5초 후 실행
    t4.enableDelayed(4500);   // 4.5초 후 실행
    t5.enableDelayed(5500);   // 5.5초 후 실행
}

void loop() {
    bool switch1State = digitalRead(SWITCH_PIN1);  // HIGH(1): 안 눌림, LOW(0): 눌림
    bool switch2State = digitalRead(SWITCH_PIN2);  // HIGH(1): 안 눌림, LOW(0): 눌림

    if (!switch1State) {  // Emergency 버튼 눌림 (LOW)
        digitalWrite(RED_LED, HIGH);
        digitalWrite(YELLOW_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        runner.disableAll();  // 모든 작업 정지
    } else if (!switch2State) {  // 스위치 2번 눌림 (LOW)
        digitalWrite(RED_LED, LOW);
        digitalWrite(YELLOW_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        runner.disableAll();  // 모든 작업 정지
    } else {  // 기본 동작 수행
        if (!t1.isEnabled()) {
            runner.enableAll();
        }
    }

    runner.execute();  // Task 실행
}

// ⏳ delay() 대신 millis() 기반 비동기 대기 함수
void delayWithoutBlocking(unsigned long duration) {
    unsigned long startTime = millis();
    while (millis() - startTime < duration) {
        if (!digitalRead(SWITCH_PIN1) || !digitalRead(SWITCH_PIN2)) return;  // 스위치가 눌리면 즉시 중단
    }
}

// 1️⃣ 빨간불 (2초)
void task1() {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    delayWithoutBlocking(2000);
    digitalWrite(RED_LED, LOW);
}

// 2️⃣ 노란불 (0.5초)
void task2() {
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    delayWithoutBlocking(500);
    digitalWrite(YELLOW_LED, LOW);
}

// 3️⃣ 초록불 (2초)
void task3() {
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    delayWithoutBlocking(2000);
    digitalWrite(GREEN_LED, LOW);
}

// 4️⃣ 초록불 깜빡임 (1초 동안 3번)
void task4() {
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
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    delayWithoutBlocking(500);
    digitalWrite(YELLOW_LED, LOW);
}
