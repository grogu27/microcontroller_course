int main() {
  TCCR1A |= 0x81;
  TCCR1B |= 0x0D;
  OCR1A = 0;
  DDRB |= 0x02;
  Serial.begin(9600);
  while (1) {
    for (uint16_t i = 0; i < 250; i += 20) {
      //Serial.println("up");
      //TCNT1 = OCR1A;
      OCR1A = i;
      //_delay_ms(150);
      //Serial.println(OCR1A);
    }
    // for (uint16_t i = 0; i <= 100; i += 10) {
    //   OCR1A = i;
    // }
    // for (uint16_t i = 0; i <= 100; i += 20) {
    //   OCR1A = i;
    // }
    // for (uint16_t i = 100; i > 0; i -= 20) {
    //   OCR1A = i;
    // }
    // for (uint16_t i = 100; i > 0; i -= 10) {
    //   OCR1A = i;
    // }
    for (uint16_t i = 241; i > 1; i -= 20) {
      //TCNT1 = OCR1A;
      //Serial.println("down");
      OCR1A = i;
      //_delay_ms(150);
    }
    //OCR1A = 100;
    //_delay_ms(500);
  }
}