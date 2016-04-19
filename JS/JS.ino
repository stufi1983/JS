//IC4094
//Pin connected  Strobe (pin 1)
int strobePin = A3;
//Pin connected  Data (pin 2)
int dataPin = A2;
//Pin connected  Clock (pin 3)
int clockPin = A1;
//Pin OE (pin 15)
int oePin = A2;

byte segChar[] = {0x04, 0x2f, 0x18, 0x09, 0x23, 0x41, 0x40, 0x0f, 0x00, 0x01};
#define STARTDIGIT digitalWrite(strobePin, LOW);
#define ENDDIGIT digitalWrite(strobePin, HIGH);

//RTC  DS3231
//A4 - SDA
//A5 - SCL

#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(strobePin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(oePin, OUTPUT);
  digitalWrite(oePin, HIGH);

  Wire.begin();

  // setDS3231time( 00,  38,  15,  02,  19,  04,  16);
  //delay(65535);
}

void loop() {

  //Bac RTC
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

  STARTDIGIT

  // displayMenitDetik(minute,second);
  displayJamMenit(hour, minute);
  ENDDIGIT

  //delay(1000);
}

void displayMenitDetik(byte menit, byte detik) {
  displayJamMenit(menit, menit);
}
void displayJamMenit(byte jam, byte menit) {
  int angka = jam * 100 + menit;
  displayDigit(menit % 10);
  displayDigit(menit / 10);
  displayDigit(jam % 10);
  displayDigit(jam / 10);
}

void displayDigit(byte digit) {
  shiftOut(dataPin, clockPin, MSBFIRST, segChar[digit]);
}

///RTC
void readDS3231time(byte *second,
                    byte *minute,
                    byte *hour,
                    byte *dayOfWeek,
                    byte *dayOfMonth,
                    byte *month,
                    byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
                   dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}
byte decToBcd(byte val) {
  return ( (val / 10 * 16) + (val % 10) );
}
byte bcdToDec(byte val) {
  return ( (val / 16 * 10) + (val % 16) );
}

