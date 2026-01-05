#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

void lcdPulse() {
    PORTB |= (1<<LCD_E);
    _delay_us(1);
    PORTB &= ~(1<<LCD_E);
    _delay_us(50);
}

void lcdWrite4(uint8_t nibble) {
    // D7 (PD7)
    if(nibble & 0x08) PORTD |= (1<<LCD_D7); else PORTD &= ~(1<<LCD_D7);
    // D6 (PB0) - теперь D8
    if(nibble & 0x04) PORTB |= (1<<LCD_D6); else PORTB &= ~(1<<LCD_D6);
    // D5 (PD6) - теперь D6
    if(nibble & 0x02) PORTD |= (1<<LCD_D5); else PORTD &= ~(1<<LCD_D5);
    // D4 (PB2) - D10
    if(nibble & 0x01) PORTB |= (1<<LCD_D4); else PORTB &= ~(1<<LCD_D4);
    
    lcdPulse();
}

void lcdCommand(uint8_t cmd) {
    PORTB &= ~(1<<LCD_RS); // RS = 0
    lcdWrite4(cmd >> 4);
    lcdWrite4(cmd & 0x0F);
    if(cmd == 0x01 || cmd == 0x02) _delay_ms(2);
}

void lcdData(uint8_t data) {
    PORTB |= (1<<LCD_RS); // RS = 1
    lcdWrite4(data >> 4);
    lcdWrite4(data & 0x0F);
    _delay_us(50);
}

void lcdInit() {
    // Настройка портов на вывод
    // D7 (PD7) и D5 (PD6) на PORTD
    DDRD |= (1<<LCD_D7) | (1<<LCD_D5);
    // RS (PB4), E (PB3), D4 (PB2), D6 (PB0) на PORTB
    DDRB |= (1<<LCD_RS) | (1<<LCD_E) | (1<<LCD_D4) | (1<<LCD_D6);
    
    _delay_ms(50);
    
    // Инициализация 4-битного режима
    PORTB &= ~(1<<LCD_RS);
    
    lcdWrite4(0x03);
    _delay_ms(5);
    lcdWrite4(0x03);
    _delay_us(150);
    lcdWrite4(0x03);
    _delay_us(150);
    lcdWrite4(0x02);
    _delay_us(150);
    
    // Настройка дисплея
    lcdCommand(0x28); // 4-bit, 2 lines, 5x8
    lcdCommand(0x0C); // Display on, cursor off
    lcdCommand(0x06); // Entry mode
    lcdCommand(0x01); // Clear
    _delay_ms(2);
}

void lcdClear() {
    lcdCommand(0x01);
    _delay_ms(2);
}

void lcdSetCursor(uint8_t col, uint8_t row) {
    uint8_t addr = (row == 0) ? 0x00 : 0x40;
    lcdCommand(0x80 | (addr + col));
}

void lcdPrint(const char *str) {
    while(*str) {
        lcdData(*str++);
    }
}
