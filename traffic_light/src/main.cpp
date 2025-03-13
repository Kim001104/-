#include <Arduino.h>
#include <TaskScheduler.h>
#include <PinChangeInterrupt.h>

// LED 핀 정의
const int RED_LED = 13;
const int YELLOW_LED = 12;
const int GREEN_LED = 11;
const int SWITCH_PIN1 = 2;  // Emergency (하드웨어 인터럽트 지원)
const int SWITCH_PIN2 = 3;  // Caution (하드웨어 인터럽트 지원)
const int SWITCH_PIN3 = 4;  // LED Blink (PinChangeInterrupt 사용)

// TaskScheduler 객체 생성
Scheduler runner;

// 스위치 상태 변수
volatile bool emergencyMode = false;
volatile bool cautionMode = false;
volatile bool blinkMode = false;
volatile unsigned long lastInterruptTime = 0;  // 디바운싱 타이머

// 인터럽트 서비스 루틴(ISR) 함수
void emergencyISR();
void cautionISR();
void blinkISR();

// 개별 작업 선언
void task1();
void task2();
void task3();
void task4();
void task5();
void task6();

// Task 객체 생성 (6초 주기)
Task t1(6000, TASK_FOREVER, &task1, &runner, false);
Task t2(6000, TASK_FOREVER, &task2, &runner, false);
Task t3(6000, TASK_FOREVER, &task3, &runner, false);
Task t4(6000, TASK_FOREVER, &task4, &runner, false);
Task t5(6000, TASK_FOREVER, &task5, &runner, false);
Task t6(500, TASK_FOREVER, &task6, &runner, false);  // LED 깜빡임 (0.5초 간격)

void setup() {
    Serial.begin(9600);  // 디버깅을 위한 시리얼 통신 시작

    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    
    pinMode(SWITCH_PIN1, INPUT_PULLUP);
    pinMode(SWITCH_PIN2, INPUT_PULLUP);
    pinMode(SWITCH_PIN3, INPUT_PULLUP);

    // 하드웨어 인터럽트 설정 (스위치 1, 2번)
    attachInterrupt(digitalPinToInterrupt(SWITCH_PIN1), emergencyISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(SWITCH_PIN2), cautionISR, CHANGE);

    // PinChangeInterrupt 설정 (스위치 3번)
    attachPinChangeInterrupt(digitalPinToPCINT(SWITCH_PIN3), blinkISR, CHANGE);

    // 기본 Task 시작
    t1.enableDelayed(1);
    t2.enableDelayed(2000);
    t3.enableDelayed(2500);
    t4.enableDelayed(4500);
    t5.enableDelayed(5500);
}

void loop() {
    if (emergencyMode) {  // Emergency 버튼 눌림
        digitalWrite(RED_LED, HIGH);
        digitalWrite(YELLOW_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        runner.disableAll();  // 모든 작업 정지
        t6.disable();  // 깜빡임 중지
    } else if (cautionMode) {  // Caution 버튼 눌림
        digitalWrite(RED_LED, LOW);
        digitalWrite(YELLOW_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        runner.disableAll();  // 모든 작업 정지
        t6.disable();  // 깜빡임 중지
    } else if (blinkMode) {  // LED 깜빡임 모드
        runner.disableAll();  // 기존 Task 중단
        if (!t6.isEnabled()) {
            Serial.println("🔵 깜빡임 시작! (task6 활성화)");
            t6.enable();  // LED 깜빡임 시작
        }
    } else {  // 기본 신호등 동작
        if (!t1.isEnabled() && !t6.isEnabled()) {
            runner.enableAll();
            Serial.println("🟢 기본 신호등 모드 실행");
        }
        t6.disable();  // 깜빡임 중지
    }

    runner.execute();
}

// Emergency 스위치 인터럽트 핸들러
void emergencyISR() {
    emergencyMode = !digitalRead(SWITCH_PIN1);
}

// Caution 스위치 인터럽트 핸들러
void cautionISR() {
    cautionMode = !digitalRead(SWITCH_PIN2);
}

// LED 깜빡임 스위치 인터럽트 핸들러 (PinChangeInterrupt 사용)
void blinkISR() {
    unsigned long currentTime = millis();
    if (currentTime - lastInterruptTime < 200) return;  // 디바운싱 (200ms)
    lastInterruptTime = currentTime;

    blinkMode = !blinkMode;  // 스위치를 누를 때마다 모드 변경
    Serial.print("🟡 blinkMode 변경: ");
    Serial.println(blinkMode ? "ON" : "OFF");
}

// 🛑 신호등 패턴
void task1() {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    delay(2000);
    digitalWrite(RED_LED, LOW);
}

void task2() {
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    delay(500);
    digitalWrite(YELLOW_LED, LOW);
}

void task3() {
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    delay(2000);
    digitalWrite(GREEN_LED, LOW);
}

void task4() {
    for (int i = 0; i < 3; i++) {
        digitalWrite(GREEN_LED, LOW);
        delay(250);
        digitalWrite(GREEN_LED, HIGH);
        delay(250);
    }
    digitalWrite(GREEN_LED, LOW);
}

void task5() {
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    delay(500);
    digitalWrite(YELLOW_LED, LOW);
}

// 🟢 **LED 깜빡임 모드 (0.5초마다 ON/OFF)**
void task6() {
    static bool toggle = false;
    toggle = !toggle;

    if (blinkMode) {
        digitalWrite(RED_LED, toggle);
        digitalWrite(YELLOW_LED, toggle);
        digitalWrite(GREEN_LED, toggle);
    } else {
        digitalWrite(RED_LED, LOW);
        digitalWrite(YELLOW_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        t6.disable();  // 깜빡임 중지
    }
}