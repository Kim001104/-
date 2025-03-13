#include <TaskScheduler.h>

// LED 핀 정의
const int RED_LED = 13;
const int YELLOW_LED = 12;
const int GREEN_LED = 11;

// TaskScheduler 객체 생성
Scheduler runner;

// 개별 작업 선언
void task1();  // 빨간불 (2초)
void task2();  // 노란불 (0.5초)
void task3();  // 초록불 (2초)
void task4();  // 초록불 깜빡임 (1초 동안 3번)
void task5();  // 노란불 (0.5초)

// Task 객체 생성 (모두 6초 주기로 실행)
Task t1(6000, TASK_FOREVER, &task1, &runner, false);
Task t2(6000, TASK_FOREVER, &task2, &runner, false);
Task t3(6000, TASK_FOREVER, &task3, &runner, false);
Task t4(6000, TASK_FOREVER, &task4, &runner, false);
Task t5(6000, TASK_FOREVER, &task5, &runner, false);

void setup() {
    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);

    // 각 Task를 일정 시간 간격으로 시작 (서로 겹치지 않도록)
    t1.enableDelayed(1);      // 즉시 실행
    t2.enableDelayed(2000);   // 2초 후 실행
    t3.enableDelayed(2500);   // 2.5초 후 실행
    t4.enableDelayed(4500);   // 4.5초 후 실행
    t5.enableDelayed(5500);   // 5.5초 후 실행
}

void loop() {
    runner.execute();
}

// 1️⃣ 빨간불 (2초)
void task1() {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    delay(2000);
    digitalWrite(RED_LED, LOW);
}

// 2️⃣ 노란불 (0.5초)
void task2() {
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    delay(500);
    digitalWrite(YELLOW_LED, LOW);
}

// 3️⃣ 초록불 (2초)
void task3() {
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    delay(2000);
    digitalWrite(GREEN_LED, LOW);
}

// 4️⃣ 초록불 깜빡임 (1초 동안 3번)
void task4() {
    for (int i = 0; i < 3; i++) {
        digitalWrite(GREEN_LED, LOW);
        delay(250);
        digitalWrite(GREEN_LED, HIGH);
        delay(250);
    }
    digitalWrite(GREEN_LED, LOW);
}

// 5️⃣ 노란불 (0.5초)
void task5() {
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    delay(500);
    digitalWrite(YELLOW_LED, LOW);
}
