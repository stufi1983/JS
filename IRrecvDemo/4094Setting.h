//IC4094
//Pin 8 connected  Strobe (pin 1)
//Pin 9 connected  Data (pin 2)
//Pin 10 connected  Clock (pin 3)
#define strobePin 8
#define dataPin  9
#define clockPin  10

byte segChar[] = {0x04, 0x2f, 0x18, 0x09, 0x23, 0x41, 0x40, 0x0f, 0x00, 0x01};
#define STARTDIGIT digitalWrite(strobePin, LOW);
#define ENDDIGIT digitalWrite(strobePin, HIGH);

void enable4094(){
  pinMode(strobePin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void displayDigit(byte digit) {
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[digit]);
}

void displayJamMenit(byte jam, byte menit) {
  int angka = jam * 100 + menit;
  displayDigit(menit % 10);
  displayDigit(menit / 10);
  displayDigit(jam % 10);
  displayDigit(jam / 10);
}

void displayMenitDetik(byte menit, byte detik) {
  displayJamMenit(menit, detik);
}


