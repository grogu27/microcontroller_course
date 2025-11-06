#include <Arduino.h>

// ТЕСТ ПИНОВ - проверяем, работают ли пины 10-13
const int testPins[] = {10, 11, 12, 13};

int main() {
  // Инициализация Arduino
  init();
  
  // Настройка пинов 10-13 как выходы (аналогично pinMode)
  DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB4) | (1 << DDB5);
  
  int count = 0;
  
  while(1) {
    count++;
    
    // Выключаем ВСЕ пины 10-13, сохраняя другие биты порта B
    PORTB &= ~((1 << PORTB2) | (1 << PORTB3) | (1 << PORTB4) | (1 << PORTB5));
    
    // Включаем нужную комбинацию
    switch (count) {
      case 1:
        PORTB |= (1 << 2);  // Пин 10
        break;
      case 2:
        PORTB |= (1 << 3);  // Пин 11
        break;
      case 3:
        PORTB |= (1 << 2) | (1 << 3);  // Пины 10+11
        break;
      case 4:
        PORTB |= (1 << 4);  // Пин 12
        break;
      case 5:
        PORTB |= (1 << 4) | (1 << 2);  // Пины 12+10
        break;
      case 6:
        PORTB |= (1 << 4) | (1 << 2) | (1 << 3);  // Пины 12+10+11
        break;
      case 7:
        PORTB |= (1 << 5);  // Пин 13
        break;
      case 8:
        PORTB |= (1 << 5) | (1 << 2);  // Пины 13+10
        break;
      case 9:
        PORTB |= (1 << 5) | (1 << 2) | (1 << 3);  // Пины 13+10+11
        break;
      case 10:
        PORTB |= (1 << 5) | (1 << 2) | (1 << 3) | (1 << 4);  // Все пины
        break;
      default:
        count = 0;  // Сброс счетчика
        break;
    }
    
    delay(1000);
    
    // Выключаем все пины 10-13 после задержки
    PORTB &= ~((1 << PORTB2) | (1 << PORTB3) | (1 << PORTB4) | (1 << PORTB5));

    delay(1000);
  }
  
  return 0;
}
//// ТЕСТ ПИНОВ - проверяем, работают ли пины 10-13
//const int testPins[] = {10, 11, 12, 13};
//
//void setup() {
//  Serial.begin(9600);
//  for (int i = 0; i < 4; i++) {
//    pinMode(testPins[i], OUTPUT);
//  }
//  Serial.println("Тест пинов запущен");
//}
//
//void loop() {
//  // Поочередно включаем каждый пин
//  for (int i = 0; i < 4; i++) {
//    digitalWrite(testPins[i], HIGH);
//    Serial.print("Пин ");
//    Serial.print(testPins[i]);
//    Serial.println(" включен (HIGH)");
//    delay(2000);
//    digitalWrite(testPins[i], LOW);
//    Serial.print("Пин ");
//    Serial.print(testPins[i]);
//    Serial.println(" выключен (LOW)");
//    delay(1000);
//  }
//}
