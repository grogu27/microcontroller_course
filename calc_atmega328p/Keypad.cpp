#include <avr/io.h>
#include <util/delay.h>
#include "keypad.h"

// (D5=R1, D4=R2, D3=R3, D2=R4)
const char keymap[4][4] = {
    // D5 (R1) -> {A3=C1, A2=C2, A1=C3, A0=C4}
    {'1', '2', '3', 'A'},
    // D4 (R2)
    {'4', '5', '6', 'B'},
    // D3 (R3)
    {'7', '8', '9', 'C'},
    // D2 (R4)
    {'*', '0', '#', 'D'}
};

void keypadInit() {
    // D2-D5  выходы
    DDRD |= 0x3C; // 0b00111100
    // Все строки HIGH
    PORTD |= 0x3C;
    
    // A0-A3 входы с pull-up
    DDRC &= 0xF0;
    PORTC |= 0x0F;
}

char keypadScan() {
    // Строки в порядке: D5, D4, D3, D2
    uint8_t row_pins[4] = {PD5, PD4, PD3, PD2};
    
    for(uint8_t row = 0; row < 4; row++) {
        // Опускаем строку
        PORTD &= ~(1 << row_pins[row]);
        _delay_us(50);
        
        // Читаем столбцы
        uint8_t cols = PINC & 0x0F;
        
        // Столбцы: A3=C1, A2=C2, A1=C3, A0=C4
        // В PINC: бит3=A3, бит2=A2, бит1=A1, бит0=A0
        uint8_t col_bits[4] = {3, 2, 1, 0}; // C1, C2, C3, C4
        
        for(uint8_t col = 0; col < 4; col++) {
            if(!(cols & (1 << col_bits[col]))) {
                _delay_ms(20);
                while(!(PINC & (1 << col_bits[col])));
                _delay_ms(20);
                
                PORTD |= (1 << row_pins[row]);
                return keymap[row][col];
            }
        }
        
        PORTD |= (1 << row_pins[row]);
    }
    return 0;
}
