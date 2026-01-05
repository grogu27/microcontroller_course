#include <avr/io.h>
#include <util/delay.h>
#include "SafeState.h"

#define EEPROM_ADDR_LOCKED   0
#define EEPROM_ADDR_CODE_LEN 1
#define EEPROM_ADDR_CODE     2
#define EEPROM_EMPTY 0xFF

#define SAFE_STATE_OPEN 0
#define SAFE_STATE_LOCKED 1

uint8_t eepromRead(uint8_t addr){
    while(EECR & (1<<EEPE)); // ждем завершения предыдущей записи
    EEAR = addr;              // устанавливаем адрес
    EECR |= (1<<EERE);        // запускаем чтение
    return EEDR;              // возвращаем данные
}

void eepromWrite(uint8_t addr, uint8_t data){
    while(EECR & (1<<EEPE));
    EEAR = addr;
    EEDR = data;
    EECR |= (1<<EEMPE);
    EECR |= (1<<EEPE);
}

SafeState::SafeState() {
    _locked = eepromRead(EEPROM_ADDR_LOCKED)==SAFE_STATE_LOCKED;
    
    // Если это первый запуск (EEPROM пуста), создаем пароль по умолчанию
    if(eepromRead(EEPROM_ADDR_CODE_LEN) == EEPROM_EMPTY) {
        // пароль по умолчанию "1234"
        setCode("1234");
        // По умолчанию сейф заблокирован
         setLock(true);
    }
}
void SafeState::lock() { setLock(true); }

bool SafeState::locked() { return _locked; }

bool SafeState::hasCode() { return eepromRead(EEPROM_ADDR_CODE_LEN) != EEPROM_EMPTY; }

void SafeState::setCode(String newCode){
    eepromWrite(EEPROM_ADDR_CODE_LEN, newCode.length());
    for(byte i=0;i<newCode.length();i++)
        eepromWrite(EEPROM_ADDR_CODE+i, newCode[i]);
}

bool SafeState::unlock(String code){
    byte len = eepromRead(EEPROM_ADDR_CODE_LEN);
    if(len == EEPROM_EMPTY) { setLock(false); return true; }
    if(code.length() != len) return false;
    for(byte i=0;i<len;i++){
        if(eepromRead(EEPROM_ADDR_CODE+i)!=code[i]) return false;
    }
    setLock(false);
    return true;
}

void SafeState::setLock(bool locked){
    _locked = locked;
    eepromWrite(EEPROM_ADDR_LOCKED, locked?SAFE_STATE_LOCKED:SAFE_STATE_OPEN);
}
