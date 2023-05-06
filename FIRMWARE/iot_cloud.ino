#include <LiquidCrystal.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "RTCDS1307.h"
int a = 2;
RTCDS1307 rtc(0x68);
const int rs = 9,  en = 8, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);



const int chipSelect = 10;
File myFile;
uint8_t year, month, weekday, day, hour, minute, second;
bool period = 0;

String m[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Agu", "Sep", "Oct", "Nov", "Dec"};
String w[7] = {"sun", "mon" , "tue", "wed", "thu", "fri", "sat" };

void  one_rtc(void)
{
  rtc.getDate(year, month, day, weekday);
  rtc.getTime(hour, minute, second, period);
  if (!(second % 3)) rtc.setMode(1 - rtc.getMode());
  rtc.getTime(hour, minute, second, period);
  lcd.setCursor(0, 3);

  lcd.print(day, DEC);
  lcd.print("/");
  lcd.print(m[month - 1]);
  lcd.print("/");
  lcd.print(year + 2000, DEC);
  lcd.print("  ");
  lcd.print(w[weekday - 1]);

  lcd.setCursor(0, 0);
  lcd.print(hour, DEC);
  lcd.print(":");
  lcd.print(minute, DEC);
  lcd.print(":");
  lcd.print(second, DEC);
  lcd.print(rtc.getMode() ? (period ? " PM" : " AM") : "");
  lcd.println();
  delay(1000);
}
void push_button(void)
{
  int data = digitalRead(2);
  if (data == HIGH)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("button pressed");
    delay(200);
    lcd.clear();
    one_rtc();
    delay(1500);
  }
}

void  check_sd(void)
{
  if (!SD.begin(chipSelect)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SD card initialization failed!");
    return;
  }
  delay(300);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SD card initialization done.");
  delay(300);

}


void setup() {
  // initialize serial communication

  lcd.begin(20, 4);

  // initialize SD card

  pinMode (2 , INPUT);

}

void loop() {


  push_button();
  check_sd();
  read_write_sd();




  // wait for a second
  //delay(1000);
}
void read_write_sd(void)
{
  // open the file for writing
  File dataFile = SD.open("data.txt", FILE_WRITE);

  // if the file is available, write to it
  if (dataFile) {
    dataFile.println("data");
    dataFile.close();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Data written to SD card.");
    //delay(300);
  }

  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error opening file.");
    //delay(300);
  }
}
