#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include <util/delay.h>

#define LCD_RS  PB4
#define LCD_E   PB3
#define LCD_D4  PB2
#define LCD_D5  PB6 
#define LCD_D6  PB0
#define LCD_D7  PD7

void lcdInit();
void lcdCommand(uint8_t cmd);
void lcdData(uint8_t data);
void lcdPrint(const char *str);
void lcdSetCursor(uint8_t col, uint8_t row);
void lcdClear();
void lcdCreateChar(uint8_t location, uint8_t charmap[]);

#endif
