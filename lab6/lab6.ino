#define c 1915
#define d 1700
#define e 1519
#define f 1432
#define g 1275
#define a 1136
#define b 1014
#define C 956

int main(){
  int periods[] = {3830, 3400, 3038, 2864, 2550, 2272, 2028, 1912}; //cdefgabC tone = half of a period
  //int toneFreq[] = {1915, 1700, 1519, 1432, 1275, 1136, 1014, 956};
  pinMode(2, OUTPUT);
  Serial.begin(9600);
  int pinRows[] = {14, 15, 16, 17};
  int pinCol[] = {13, 12, 8, 7};
  char Keys[4][4] = {{'1', '2', '3', 'A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};
  for (int i = 0; i < 4; i++){
    pinMode(pinRows[i],INPUT_PULLUP);
    pinMode(pinCol[i], OUTPUT);
    digitalWrite(pinCol[i], HIGH);
  }

  while (1){
    for (int i = 0; i < 4; i++){
      digitalWrite(pinCol[i], LOW);
      for (int j = 0; j < 4; j++){
        if (!(digitalRead(pinRows[j]))){
          Serial.println(Keys[j][i]);
          //for (int temp = 0;temp<10;temp++){
          digitalWrite(2, HIGH);
          switch (j) {
            case 0:
          _delay_us(c);
          digitalWrite(2, LOW);
          _delay_us(c);
          break;
            case 1:
          _delay_us(b);
          digitalWrite(2, LOW);
          _delay_us(b);
          break;
          case 2:
          _delay_us(d);
          digitalWrite(2, LOW);
          _delay_us(d);
          break;
          case 3:
          _delay_us(e);
          digitalWrite(2, LOW);
          _delay_us(e);
          break;
          }

          //}
        }
      }
      digitalWrite(pinCol[i], HIGH);
    }
  }
  return 0;
}