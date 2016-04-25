#include <IRremote.h>
#include "IRremoteSetting.h"
long rsl = 0;

byte tanggalJam[14];//yp=0,ys=0,yr=2,ysr=0,mp=0,ms=0,dp=0,ds=0,sp=0,ss=0,mp=0,ms=0,hp=0,hs=0
#include "4094Setting.h"

//RTC  DS3231 //A4 - SDA //A5 - SCL
#include <Wire.h>
#define DS3231_I2C_ADDRESS 0x68
#include "RTCSetting.h"
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
bool dateUpdate = false; byte lastDay = 0; byte lastMonth = 0;

#include <TimerOne.h>
volatile bool timeUpdate = false;
enum mode {RUN, EDIT, ALARM, STDBY};
byte mode = EDIT;

#include <avr/pgmspace.h>
#include "jadwal_2.h"

#define IQOMAT 10//300 //detik                                                            
#define BUZZERON 3 //detik
#define BUZZERSHOLAT 5 //kali
#define pinBuzzer 7
boolean buzzerStatus = false;

#define led 6
boolean ledStat = false;  //also as flag every 1 second

static int tempReg = 0; //temporary iqomat timer in second

static  uint8_t dataJam[7];
static  uint8_t dataMenit[7];

static unsigned char lastMinute = 0;

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

  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  dateUpdate = true;

  //mode =  ALARM; tempReg = IQOMAT;
  mode =  STDBY; tempReg = 0;
  //mode = EDIT;
  pinMode(pinBuzzer, OUTPUT);
}
byte tanggalJamSetting[14];
bool startSetting = false; byte jamdigitPos = 0;
byte i = 0;

void updateJamSetting() {
  STARTDIGITTHN
  for (byte x = 0; x < 14; x++) {
    displayDigitThn(tanggalJamSetting[x]);
  }
  ENDDIGITTHN
}
void updateRTC(){
  setDS3231time(  tanggalJamSetting[9]*10+tanggalJamSetting[8],  tanggalJamSetting[11]*10+tanggalJamSetting[10],   tanggalJamSetting[13]*10+tanggalJamSetting[12],dayOfWeek,  
  tanggalJamSetting[7]*10+tanggalJamSetting[6],  tanggalJamSetting[5]*10+tanggalJamSetting[4],  tanggalJamSetting[1]*10+tanggalJamSetting[0]);
}
void loop() {
  if (REMOTEPRESS) {
    //if (results.value != 0xFFFFFFFF)
      rsl = results.value;
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value

    if (rsl == tombolRemote[EQ]) {
      if (mode != EDIT) {
        mode = EDIT;
        startSetting = true;
      }
      else {
        //mode = STDBY;
      }
    }

    if (mode == EDIT) {
      if (rsl == tombolRemote[NOL]) {
        tanggalJamSetting[14 - jamdigitPos] = 0;
        jamdigitPos++; updateJamSetting();
      }
      if (rsl == tombolRemote[SATU]) {
        tanggalJamSetting[14 - jamdigitPos] = 1;
        jamdigitPos++; updateJamSetting();
      }
      if (rsl == tombolRemote[DUA]) {
        tanggalJamSetting[14 - jamdigitPos] = 2;
        jamdigitPos++; updateJamSetting();
      }
      if (rsl == tombolRemote[TIGA]) {
        tanggalJamSetting[14 - jamdigitPos] = 3;
        jamdigitPos++; updateJamSetting();
      }
      if (rsl == tombolRemote[EMPAT]) {
        tanggalJamSetting[14 - jamdigitPos] = 4;
        jamdigitPos++; updateJamSetting();
      }
      if (rsl == tombolRemote[LIMA]) {
        tanggalJamSetting[14 - jamdigitPos] = 5;
        jamdigitPos++; updateJamSetting();
      }
      if (rsl == tombolRemote[ENAM]) {
        tanggalJamSetting[14 - jamdigitPos] = 6;
        jamdigitPos++; updateJamSetting();
      }
      if (rsl == tombolRemote[TUJUH]) {
        tanggalJamSetting[14 - jamdigitPos] = 7;
        jamdigitPos++; updateJamSetting();
      }
      if (rsl == tombolRemote[DELAPAN]) {
        tanggalJamSetting[14 - jamdigitPos] = 8;
        jamdigitPos++; updateJamSetting();
      }
      if (rsl == tombolRemote[SEMBILAN]) {
        tanggalJamSetting[14 - jamdigitPos] = 9;
        jamdigitPos++; updateJamSetting();
      }

      if (jamdigitPos > 14) {
        mode = STDBY;
        updateRTC();
      }
    }
    //Serial.println(rsl, HEX);
  }

  if (mode == STDBY) {
    if (timeUpdate) {
      readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

      STARTDIGITTHN
      displayTglBulanTahun(dayOfMonth, month, year);
      displayJamMenitThn(hour, minute, second);
      ENDDIGITTHN

      updateJadwal(month, dayOfMonth, hour, minute);

      timeUpdate = false;
      if (tempReg > (0 - BUZZERSHOLAT)) {
        tempReg--;
      }

      ledStat = !ledStat;

      if ((lastDay != dayOfMonth) && (lastMonth != month)) {
        dateUpdate = true;
      }

    }
  }

  if (mode == EDIT) {
    if (startSetting) {
      STARTDIGITTHN
      for (byte x = 0; x < 14; x++) {
        tanggalJamSetting[x] = 10;//tanggalJam[x];
        displayDigitThn(tanggalJamSetting[x]);
      }
      ENDDIGITTHN
      startSetting = false;
      jamdigitPos = 0;
    }
  }

  if (dateUpdate) {
    dataJadwal(month, dayOfMonth);

    STARTDIGIT
    for (int i = 1; i <= 6; i++) { //array start from 1
      displayJamMenit(dataJam[i], dataMenit[i]);
    }
    ENDDIGIT

    lastDay = dayOfMonth; lastMonth = month;
    dateUpdate = false;
  }


  if (mode == ALARM) {
    //Buzzer masuk waktu sholat                                      //-------------------------------------------------

    if ((tempReg >= (IQOMAT - BUZZERON))) {
      if (!buzzerStatus) {
        buzzerStatus = HIGH;
        Serial.println("Buzzer On ");
      }
    } else {
      if (buzzerStatus) {
        buzzerStatus = LOW;
        Serial.println("Buzzer Off ");
      }
    }
    digitalWrite(pinBuzzer, buzzerStatus);

    int x = tempReg / 60;
    if (x > 0) {  //more than 1 minute
      digitalWrite(led, ledStat);
      if (ledStat) {
        Serial.print("Iqomat:");
        Serial.print(x, DEC);
        Serial.println(" menit");
        delay(200);
        ledStat = false;
      }
    } else if (tempReg >= 0) { //less than 1 minute
      if (ledStat) {
        Serial.print("Iqomat:");
        Serial.print(tempReg, DEC);
        Serial.println(" detik");
        delay(200);
        ledStat = false;

        //ToDo: Display counter
        STARTDIGIT
        displayMenitDetik(x, tempReg % 60);
        ENDDIGIT

        if (tempReg == 0) {
          Serial.println("Sholat time!");
          //ToDo: Reset display counter
          STARTDIGIT
          for (int i = 1; i <= 6; i++) { //array start from 1
            displayJamMenit(dataJam[i], dataMenit[i]);
          }
          ENDDIGIT

        }
      }
    }


    if (tempReg <= 0) { //if iqomat time up
      // Prayer start ..............
      if (ledStat) {
        digitalWrite(pinBuzzer, HIGH);
        Serial.print("Buzzer On ");
        delay(500);
        digitalWrite(pinBuzzer, LOW);
        Serial.println("Buzzer Off ");
        ledStat = false;
      }
    }
    if (tempReg <= (0 - BUZZERSHOLAT)) {
      mode = STDBY;
      Serial.println("Standby!");
    }
  }
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


void updateJadwal(uint8_t bulan, uint8_t tgl, uint8_t jam, uint8_t menit) {
  if (checkMinuteChange(menit)) {
    if (checkDayChange(tgl)) {
      dataJadwal(bulan, tgl);
      Serial.println("Update Jadwal");
    }
    compareRTC(jam, menit);
  }

}

void dataJadwal(uint8_t bulan, uint8_t tgl) {
  Serial.print("Update jadwal: ");
  for (unsigned char i = 1; i <= 6; i++) {
    int x = ( (i - 1) * 32)  + tgl;

    switch (bulan) {
      case 1: dataMenit[i] = pgm_read_byte_near(jadwalMnt1 + x); dataJam[i] = pgm_read_byte_near(jadwalJam1 + x); break;
      case 2: dataMenit[i] = pgm_read_byte_near(jadwalMnt2 + x); dataJam[i] = pgm_read_byte_near(jadwalJam2 + x); break;
      case 3: dataMenit[i] = pgm_read_byte_near(jadwalMnt3 + x); dataJam[i] = pgm_read_byte_near(jadwalJam3 + x); break;
      case 4: dataMenit[i] = pgm_read_byte_near(jadwalMnt4 + x); dataJam[i] = pgm_read_byte_near(jadwalJam4 + x); break;
      case 5: dataMenit[i] = pgm_read_byte_near(jadwalMnt5 + x); dataJam[i] = pgm_read_byte_near(jadwalJam5 + x); break;
      case 6: dataMenit[i] = pgm_read_byte_near(jadwalMnt6 + x); dataJam[i] = pgm_read_byte_near(jadwalJam6 + x); break;
      case 7: dataMenit[i] = pgm_read_byte_near(jadwalMnt7 + x); dataJam[i] = pgm_read_byte_near(jadwalJam7 + x); break;
      case 8: dataMenit[i] = pgm_read_byte_near(jadwalMnt8 + x); dataJam[i] = pgm_read_byte_near(jadwalJam8 + x); break;
      case 9: dataMenit[i] = pgm_read_byte_near(jadwalMnt9 + x); dataJam[i] = pgm_read_byte_near(jadwalJam9 + x); break;
      case 10: dataMenit[i] = pgm_read_byte_near(jadwalMnt10 + x); dataJam[i] = pgm_read_byte_near(jadwalJam10 + x); break;
      case 11: dataMenit[i] = pgm_read_byte_near(jadwalMnt11 + x); dataJam[i] = pgm_read_byte_near(jadwalJam11 + x); break;
      case 12: dataMenit[i] = pgm_read_byte_near(jadwalMnt12 + x); dataJam[i] = pgm_read_byte_near(jadwalJam12 + x); break;
      default: break;
    }

    Serial.print(dataJam[i], DEC);
    Serial.print(":");
    Serial.print(dataMenit[i], DEC);
    Serial.print(" ");
  }
  /*
  #if DEBUG
  for (unsigned char i = 0; i <= 3; i++) {
    Serial.print(pgm_read_byte_near(jadwalJI[0][i]),DEC);
    Serial.print(":");
    Serial.print(pgm_read_byte_near(jadwalJI[1][i]),DEC);
    Serial.print(" ");
  }
  Serial.println("");
  #endif*/
}

boolean checkMinuteChange(uint8_t menit) {
  if (menit != lastMinute) {
    lastMinute = menit;
    return true;
  } else return false;
}
boolean checkDayChange(uint8_t tgl) {
  if (tgl != lastDay) {
    lastDay = tgl;
    return true;
  } else return false;
}

void compareRTC(uint8_t jam, uint8_t menit) {
  if (jam == dataJam[1] && menit == dataMenit[1]) {
    mode = ALARM;
    //kata = FS(kataK);
    tempReg = IQOMAT;
    Serial.print("Imsyak"); debugJam();
  } else if (jam == dataJam[2] && menit == dataMenit[2]) {
    mode = ALARM;
    //kata = FS(kataS);
    tempReg = IQOMAT;
    Serial.print("Subuh"); debugJam();
  } else if (jam == dataJam[3] && menit == dataMenit[3]) {
    mode = ALARM;
    //kata = FS(kataD);
    tempReg = IQOMAT;
    Serial.print("Dhuhur"); debugJam();
  } else if (jam == dataJam[4] && menit == dataMenit[4]) {
    mode = ALARM;
    //kata = FS(kataA);
    tempReg = IQOMAT;
    Serial.print("Ashar"); debugJam();
  } else if (jam == dataJam[5] && menit == dataMenit[5]) {
    mode = ALARM;
    //kata = FS(kataM);
    tempReg = IQOMAT;
    Serial.print("Magrib"); debugJam();
  } else if (jam == dataJam[6] && menit == dataMenit[6]) {
    mode = ALARM;
    //kata = FS(kataI);
    tempReg = IQOMAT;
    Serial.print("Isya"); debugJam();
  } else return;

}

void debugJam() {
  Serial.print(year, DEC); Serial.print("/");
  Serial.print(month, DEC); Serial.print("/");
  Serial.print(dayOfMonth, DEC); Serial.print(" ");
  Serial.print(hour, DEC); Serial.print(":");
  Serial.print(minute, DEC); Serial.print(":");
  Serial.print(second, DEC); Serial.println(" ");
}
