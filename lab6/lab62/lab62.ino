#define a 1318

int main() {
  pinMode(2, OUTPUT);
  while(1) {
    _delay_ms(100);
    for (int i = 0; i < 20; i++) {
    digitalWrite(2, HIGH);
    _delay_us(880);
    digitalWrite(2, LOW);
    _delay_us(880);
    }
    _delay_ms(100);
   for (int i = 0; i < 20; i++) {
    digitalWrite(2, HIGH);
    _delay_us(1047);
    digitalWrite(2, LOW);
    _delay_us(1047);
   }
   _delay_ms(400);
   for (int i = 0; i < 20; i++) {
    digitalWrite(2, HIGH);
    _delay_us(1175);
    digitalWrite(2, LOW);
    _delay_us(1175);
   }
   _delay_ms(100);
    for (int i = 0; i < 5; i++) {
    digitalWrite(2, HIGH);
    _delay_us(1875);
    digitalWrite(2, LOW);
    _delay_us(1875);
    }
    for (int i = 0; i < 5; i++) {
    digitalWrite(2, HIGH);
    _delay_us(1319);
    digitalWrite(2, LOW);
    _delay_us(1319);
    }
    _delay_ms(200);
    _delay_ms(100);
    for (int i = 0; i < 5; i++) {
    digitalWrite(2, HIGH);
    _delay_us(1319);
    digitalWrite(2, LOW);
    _delay_us(1319);
    }
    for (int i = 0; i < 10; i++) {
    digitalWrite(2, HIGH);
    _delay_us(1568);
    digitalWrite(2, LOW);
    _delay_us(1568);
    }
    for (int i = 0; i < 15; i++) {
    digitalWrite(2, HIGH);
    _delay_us(1319);
    digitalWrite(2, LOW);
    _delay_us(1319);
    }
    _delay_ms(100);
    for (int i = 0; i < 15; i++) {
    digitalWrite(2, HIGH);
    _delay_us(1175);
    digitalWrite(2, LOW);
    _delay_us(1175);
    }
    for (int i = 0; i < 15; i++) {
    digitalWrite(2, HIGH);
    _delay_us(1047);
    digitalWrite(2, LOW);
    _delay_us(1047);
    }
    for (int i = 0; i < 15; i++) {
    digitalWrite(2, HIGH);
    _delay_us(1175);
    digitalWrite(2, LOW);
    _delay_us(1175);
    }
    _delay_ms(100);
  }
  // while (1){
  //   for(int i = 0; i<150; i++){
  //   digitalWrite(2,HIGH);
  //   _delay_us(a);
  //   digitalWrite(2,LOW);
  //   _delay_us(a);
  //   }
  // _delay_ms(150);
  // for (int i = 0; i<300;i++){
  //   digitalWrite(2,HIGH);
  //   _delay_us(a);
  //   digitalWrite(2,LOW);
  //   _delay_us(a);
  // }

  // for (int i = 0; i<300;i++){
  //   digitalWrite(2,HIGH);
  //   _delay_us(a);
  //   digitalWrite(2,LOW);
  //   _delay_us(a);
  // } 
  //
  //}
  return 0;
}