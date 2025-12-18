#define SDA_PIN  A4
#define SCL_PIN  A5

void i2cDelay() {
  delayMicroseconds(5); // простая задержка, чтобы соблюсти тайминг
}

void i2cStart() {
  pinMode(SDA_PIN, OUTPUT);
  pinMode(SCL_PIN, OUTPUT);
  digitalWrite(SDA_PIN, HIGH);
  digitalWrite(SCL_PIN, HIGH);
  i2cDelay();
  digitalWrite(SDA_PIN, LOW); // START
  i2cDelay();
  digitalWrite(SCL_PIN, LOW);
}

void i2cStop() {
  digitalWrite(SDA_PIN, LOW);
  digitalWrite(SCL_PIN, HIGH);
  i2cDelay();
  digitalWrite(SDA_PIN, HIGH); // STOP
}

void i2cWriteBit(bool bit) {
  digitalWrite(SDA_PIN, bit ? HIGH : LOW);
  i2cDelay();
  digitalWrite(SCL_PIN, HIGH);
  i2cDelay();
  digitalWrite(SCL_PIN, LOW);
}

bool i2cReadBit() {
  pinMode(SDA_PIN, INPUT);
  digitalWrite(SCL_PIN, HIGH);
  i2cDelay();
  bool bit = digitalRead(SDA_PIN);
  digitalWrite(SCL_PIN, LOW);
  return bit;
}

void i2cWriteByte(uint8_t byte) {
  for (int i = 7; i >= 0; i--) {
    i2cWriteBit(byte & (1 << i));
  }
  // ACK
  pinMode(SDA_PIN, INPUT);
  digitalWrite(SCL_PIN, HIGH);
  i2cDelay();
  digitalWrite(SCL_PIN, LOW);
  pinMode(SDA_PIN, OUTPUT);
}

uint8_t i2cReadByte(bool ack) {
  uint8_t byte = 0;
  for (int i = 7; i >= 0; i--) {
    if (i2cReadBit()) byte |= (1 << i);
  }
  // ACK/NACK
  i2cWriteBit(!ack);
  return byte;
}

#define DS3231_ADDR 0x68

uint8_t bcdToDec(uint8_t val) {
  return (val >> 4) * 10 + (val & 0x0F);
}

void readDS3231() {
  i2cStart();
  i2cWriteByte(DS3231_ADDR << 1 | 0); // адрес + запись
  i2cWriteByte(0x00);                  // регистр секунд
  i2cStart();
  i2cWriteByte(DS3231_ADDR << 1 | 1); // адрес + чтение

  uint8_t sec  = bcdToDec(i2cReadByte(true));
  uint8_t min  = bcdToDec(i2cReadByte(true));
  uint8_t hour = bcdToDec(i2cReadByte(false));

  i2cStop();

  Serial.print("Time: ");
  Serial.print(hour); Serial.print(":");
  Serial.print(min);  Serial.print(":");
  Serial.println(sec);
}

int main() {
  init();
  Serial.begin(9600);
  pinMode(SDA_PIN, OUTPUT);
  pinMode(SCL_PIN, OUTPUT);

  while (1) {
    readDS3231();
    delay(1000);
  }
}
