/*TWSR статус регистр Содержит статус последней операции (биты 7–3) и предделитель TWPS (биты 1–0)., TWBR регистр частоты scl, TWDR дата регистр Байт данных для передачи/приёма , TWCR ругистр управления
TWINT (бит 7) — флаг прерывания: сбрасывается в 0 при старте операции, =1 при завершении
TWEN (бит 2) — включить TWI  
TWSTA (бит 5) — сгенерировать START  
TWSTO (бит 4) — сгенерировать STOP  
TWEA (бит 6) — включить отправку ACK при чтении |
BCD 
DS1307 хранит числа в BCD:  
DS1307 имеет 64 байта RAM, первые 8 — часы/дата (в BCD):
Бит CH (Clock Halt) в 0x00, бит 7:

    CH = 1 → генератор остановлен (заводское состояние)  
    CH = 0 → часы идут


0x00 Секунды `CH
0x01 Минуты `0
0x02 Часы `12/24
0x03 День недели 1=Пн, ..., 7=Вс
0x04 Дата `0
0x05 Месяц `0
0x06 Год `10-года
0x07 Управление SQW/OUT, RS0–RS1, ~OUT

БИТ 6 - 12/24 h mode
Бит 7 — CH (Clock Halt), он не часть значения секунд.  



    setup()  
        Инициализирует UART (Serial.begin)  
        Инициализирует TWI (TWBR=72, TWSR=0)  
        Вызывает DS1307_set_time()
    DS1307_set_time()  
        Берёт __TIME__ = "HH:MM:SS"  
        Парсит → hour, minute, second (dec)  
        Конвертирует в BCD  
        Пишет 7 байт в DS1307: сек, мин, час, день_нед, день, месяц, год  
        Важно: секунды пишутся с CH=0 — часы запускаются!
    loop()  
        Каждую секунду читает регистры 0x00, 0x01, 0x02  
        Маскирует служебные биты  
        Конвертирует BCD → DEC  
        Выводит HH:MM:SS в Serial



 TWCR – TWI Control Register

Главный управляющий регистр. Важные биты:
Бит
	
Имя
	
Назначение
7 TWINT Флаг завершения операции. Сбрасывается в 0 при запуске операции, устанавливается в 1 по завершении. Нужно писать 1 для сброса флага и продолжения.
6 TWEA Разрешить отправку ACK (при чтении). У тебя не используется — т.к. читаешь по 1 байту и отправляешь NACK.
5 TWSTA Сгенерировать START условие (начало I²C-транзакции).
4 TWSTO Сгенерировать STOP условие (завершение транзакции).
2 TWEN Включить модуль TWI. Обязателен!


Адрес устройства: 0x68 (7-битный).

    При записи: 0xD0 = 0x68 << 1 | 0
    При чтении: 0xD1 = 0x68 << 1 | 1

*/





/* [START]
0xD0 (0x68<<1 | 0) → WRITE → ACK от DS1307
0x00 (номер регистра) → ACK
[REPEATED START]
0xD1 (0x68<<1 | 1) → READ → ACK
[DS1307 выдаёт 1 байт]
→ мастер посылает NACK (т.к. 1 байт)
[STOP]*/

/* 
TWI_start();      // [START]
TWI_write(0xD0);  // SLA+W
TWI_write(reg);   // адрес регистра

TWI_start();      // [REPEATED START]
TWI_write(0xD1);  // SLA+R
data = TWI_readNACK(); // читаем + NACK
TWI_stop();       // [STOP]*/

#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>
void TWI_init(void) {
    TWSR = 0x00; //делитель = 1  
    TWBR = 72; // ~100 kHz @ 16 MHz //частота для scl
}

void TWI_start(void) {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

void TWI_stop(void) {
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
    while (TWCR & (1 << TWSTO));
}

void TWI_write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

uint8_t TWI_readNACK(void) {
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

uint8_t DS1307_read(uint8_t reg) {
    TWI_start();
    TWI_write(0xD0); //адрес на запись Чтобы сказать DS1307: «я хочу выбрать регистр»
    TWI_write(reg); //Номер регистра Например, 0x00 — секунды

    TWI_start(); //Меняем направление 
    TWI_write(0xD1); // 0x68 << 1 | 1
    uint8_t data = TWI_readNACK();
    TWI_stop();
    return data;
}

void DS1307_set_time(void) {


    uint8_t year = 25; 
    uint8_t month = 12; 
    uint8_t day = 18;

    // Парсим __TIME__ — "HH:MM:SS"
    const char* t = __TIME__;
    uint8_t hour   = (t[0] - '0') * 10 + (t[1] - '0');
    uint8_t minute = (t[3] - '0') * 10 + (t[4] - '0');
    uint8_t second = (t[6] - '0') * 10 + (t[7] - '0');

    // Конвертируем в BCD
    #define DEC_TO_BCD(val) ((val / 10 * 16) + (val % 10))

    TWI_start();
    TWI_write(0xD0);
    TWI_write(0x00); // reg 0 — seconds

    TWI_write(DEC_TO_BCD(second));
    TWI_write(DEC_TO_BCD(minute));
    TWI_write(DEC_TO_BCD(hour));        
    TWI_write(0x05);                    
    TWI_write(DEC_TO_BCD(day));
    TWI_write(DEC_TO_BCD(month));
    TWI_write(DEC_TO_BCD(year));        

    TWI_stop();
}

uint8_t bcd2dec(uint8_t bcd) {
    return (bcd >> 4) * 10 + (bcd & 0x0F);
}

void setup() {
    Serial.begin(9600);
    while (!Serial); 

    TWI_init();

    DS1307_set_time();

    delay(100);
    Serial.println("RTC initialized. Time is running.");
}

void loop() {
    uint8_t sec = bcd2dec(DS1307_read(0x00) & 0x7F); // маскируем CH-бит
    uint8_t min = bcd2dec(DS1307_read(0x01));
    uint8_t hr  = bcd2dec(DS1307_read(0x02) & 0x3F); // маскируем 12/24-биты

    Serial.print(hr < 10 ? "0" : ""); Serial.print(hr);
    Serial.print(":");
    Serial.print(min < 10 ? "0" : ""); Serial.print(min);
    Serial.print(":");
    Serial.print(sec < 10 ? "0" : ""); Serial.println(sec);

    delay(1000);
}

// int main(){
//       Serial.begin(9600);
//     while (!Serial); 

//     TWI_init();

//     DS1307_set_time();

//     delay(100);
//     Serial.println("RTC initialized. Time is running.");

//     while(1){
//           uint8_t sec = bcd2dec(DS1307_read(0x00) & 0x7F); // маскируем CH-бит
//     uint8_t min = bcd2dec(DS1307_read(0x01));
//     uint8_t hr  = bcd2dec(DS1307_read(0x02) & 0x3F); // маскируем 12/24-биты

//     Serial.print(hr < 10 ? "0" : ""); Serial.print(hr);
//     Serial.print(":");
//     Serial.print(min < 10 ? "0" : ""); Serial.print(min);
//     Serial.print(":");
//     Serial.print(sec < 10 ? "0" : ""); Serial.println(sec);

//     delay(1000);
//     }

//     return 0;
// }
