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

#define strobePinThn 11
#define dataPinThn  12
#define clockPinThn  13
byte segCharThn[] = {0x7b,0x12,0x67,0x37,0x1e,0x3d,0x7d,0x13,0x7f,0x3f};
#define STARTDIGITTHN digitalWrite(strobePinThn, LOW);
#define ENDDIGITTHN digitalWrite(strobePinThn, HIGH);

void enable4094(){
  pinMode(strobePin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(strobePinThn, OUTPUT);
  pinMode(clockPinThn, OUTPUT);
  pinMode(dataPinThn, OUTPUT);
}


void displayDigitThn(byte digit) {
  shiftOut(dataPinThn, clockPinThn, MSBFIRST, segCharThn[digit]);
}
void displayJamMenitThn(byte jam, byte menit, byte detik) {
  displayDigitThn(detik % 10);
  displayDigitThn(detik / 10);
  displayDigitThn(menit % 10);
  displayDigitThn(menit / 10);
  displayDigitThn(jam % 10);
  displayDigitThn(jam / 10);
}
void displayTglBulanTahun(byte tgl, byte bln, byte tahun) {
  displayDigitThn(tahun % 10);
  displayDigitThn(tahun / 10);
  displayDigitThn(0);
  displayDigitThn(2);
  displayDigitThn(bln % 10);
  displayDigitThn(bln / 10);
  displayDigitThn(tgl % 10);
  displayDigitThn(tgl / 10);
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

