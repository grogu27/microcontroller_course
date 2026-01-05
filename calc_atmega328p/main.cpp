#include <avr/io.h>
#include <util/delay.h>
#include "SafeState.h"
#include "lcd.h"
#include "keypad.h"
#define SERVO_LOCK_POS   0
#define SERVO_UNLOCK_POS 90

void servoInit() {
    DDRB |= (1<<PB1); // D9 как выход
    
    // Fast PWM Mode 14, prescaler=8 для сервопривода 50Hz
    TCCR1A = (1<<COM1A1) | (1<<WGM11);
    TCCR1B = (1<<WGM13) | (1<<WGM12) | (1<<CS11);
    ICR1 = 39999; // 20ms период при 16MHz и prescaler=8
}

void setServoAngle(uint8_t angle) {
    // Используем полный диапазон 0-180°
    if(angle > 180) angle = 180;
    // 0° = 1ms = 2000 тиков
    // 90° = 1.5ms = 3000 тиков  
    // 180° = 2ms = 4000 тиков
    // Формула для полного диапазона:
    OCR1A = 2000 + (angle * 2000) / 180;
}

void lockServo()   { 
    setServoAngle(SERVO_LOCK_POS);
    _delay_ms(500); 
}

void unlockServo() { 
    setServoAngle(SERVO_UNLOCK_POS);
    _delay_ms(500); 
}

SafeState safeState;

void delayMsVariable(uint16_t ms) {
    while(ms--) _delay_ms(1);
}

void showStartupMessage() {
    lcdClear();
    lcdSetCursor(4,0);
    lcdPrint("Welcome!");
    _delay_ms(1000);

    lcdSetCursor(0,1);
    lcdPrint("ArduinoSafe v1.0");
    _delay_ms(1000);
}

void showWaitScreen(int delayMillis) {
    lcdSetCursor(2,1);
    lcdPrint("[..........]");
    lcdSetCursor(3,1);
    
    int stepDelay = delayMillis / 10;
    
    for(int i=0;i<10;i++){
        if(stepDelay > 0) {
            for(int j = 0; j < stepDelay; j += 10) {
                _delay_ms(10);
            }
        }
        lcdData('=');
    }
}

void showUnlockMessage() {
    lcdClear();
    lcdSetCursor(4,0);
    lcdPrint("Unlocked!");
    showWaitScreen(800);
}

char inputSecretCode(char *codeBuffer) {
    lcdSetCursor(5,1);
    lcdPrint("[____]");
    lcdSetCursor(6,1);
    uint8_t pos = 0;
    while(pos < 4) {
        char key = keypadScan();
        if(key >= '0' && key <= '9') {
            lcdData('*');
            codeBuffer[pos++] = key;
            _delay_ms(100); 
        }
        _delay_ms(10); 
    }
    codeBuffer[4] = 0;
    return 1;
}

uint8_t setNewCode() {
    char newCode[5], confirmCode[5];
    lcdClear();
    lcdSetCursor(0,0);
    lcdPrint("Enter new code:");
    inputSecretCode(newCode);

    lcdClear();
    lcdSetCursor(0,0);
    lcdPrint("Confirm code:");
    inputSecretCode(confirmCode);

    for(uint8_t i=0;i<4;i++){
        if(newCode[i]!=confirmCode[i]){
            lcdClear();
            lcdSetCursor(1,0);
            lcdPrint("Code mismatch");
            lcdSetCursor(0,1);
            lcdPrint("Safe not locked!");
            _delay_ms(2000);
            return 0;
        }
    }
    safeState.setCode(newCode);
    lcdClear();
    lcdPrint("New code set!");
    _delay_ms(1000);
    return 1;
}

void safeUnlockedLogic() {
    lcdClear();
    lcdSetCursor(0,0);
    lcdPrint("Safe Unlocked");
    lcdSetCursor(0,1);
    lcdPrint("A=NewCode #=Lock");

    char key = 0;
    
    while(1) {
        key = keypadScan();
        if(key == 'A' || key == '#') break;
        _delay_ms(10);
    }

    if(key == 'A') {
        // Устанавливка нового кода
        setNewCode();
        // После установки кода остаемся в разблокированном состоянии
    } else if(key == '#') {
        // Блокировка сейфа
        lcdClear();
        lcdPrint("Locking...");
        safeState.lock();
        lockServo();
        showWaitScreen(100);
    }
}

void safeLockedLogic() {
    lcdClear();
    lcdSetCursor(0,0);
    lcdPrint("Safe Locked!");
    lcdSetCursor(0,1);
    lcdPrint("Code: [____]");
    lcdSetCursor(6,1);

    char userCode[5];
    inputSecretCode(userCode);
    
    lcdClear();
    lcdPrint("Checking...");
    _delay_ms(500);
    
    uint8_t unlockedSuccessfully = safeState.unlock(userCode);
    showWaitScreen(200);

    if(unlockedSuccessfully) {
        showUnlockMessage();
        unlockServo();
    } else {
        lcdClear();
        lcdSetCursor(0,0);
        lcdPrint("Access Denied!");
        showWaitScreen(1000);
    }
}

int main(void) {
    lcdInit();
    servoInit();
    keypadInit();
    
    // Начальное состояние - сервопривод устанавливается в положение сейфа
    if(safeState.locked()) {
        lockServo();
    } else {
        unlockServo();
    }
    
    lcdClear();
    if(safeState.locked()) {
        lcdPrint("Safe: LOCKED");
        lcdSetCursor(0,1);
        lcdPrint("Default: 1234");
    } else {
        lcdPrint("Safe: UNLOCKED");
    }
    _delay_ms(2000);
    
    showStartupMessage();

    while(1) {
        if(safeState.locked()) {
            safeLockedLogic();
        } else {
            safeUnlockedLogic();
        }
        
        _delay_ms(10);
    }
}
