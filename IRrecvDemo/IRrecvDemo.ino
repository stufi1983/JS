#include <IRremote.h>
#include "IRremoteSetting.h"
long rsl = 0;

#include "4094Setting.h"

//RTC  DS3231 //A4 - SDA //A5 - SCL
#include <Wire.h>
#define DS3231_I2C_ADDRESS 0x68
#include "RTCSetting.h"
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;


#include <TimerOne.h>
volatile bool timeUpdate = false;
enum mode {RUN, EDIT};
byte mode = EDIT;

void setup()
{
  irrecv.enableIRIn(); // Start the IR receiver
  enable4094(); //Start 4094
  Wire.begin(); //RTC
  Serial.begin(9600); //Serial

  //uncomment to set RTC time
  //setDS3231time( 00,  38,  15,  02,  19,  04,  16); delay(65535);
  Timer1.initialize(1000000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here

}


void loop() {
  if (REMOTEPRESS) {
    if (results.value != 0xFFFFFFFF)
      rsl = results.value;
    irrecv.resume(); // Receive the next value
    if (rsl == tombolRemote[EQ]) {
      mode = EDIT;
    }
    //Serial.println(rsl, HEX);
  }

  if (timeUpdate) {
    readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
    STARTDIGIT
    if (mode == EDIT)
      displayMenitDetik(minute, second);
    if (mode == RUN)
      displayJamMenit(hour, minute);
    ENDDIGIT
    timeUpdate = false;
  }
  Serial.println(second);

}

///Timer
void timerIsr()
{
  //Baca RTC
  timeUpdate = true;
}

///RTC Function
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


