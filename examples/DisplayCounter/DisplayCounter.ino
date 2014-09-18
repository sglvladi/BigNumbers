/*
  Display_Counter.ino - Example for displaying large numbers on LCD displays using the HD44780 driver.
  Displays the number of seconds since last boot on the display.
  	
  The circuit:
  * LCD RS pin to digital pin 12
  * LCD Enable pin to digital pin 11
  * LCD D4 pin to digital pin 5
  * LCD D5 pin to digital pin 4
  * LCD D6 pin to digital pin 3
  * LCD D7 pin to digital pin 2
  * LCD R/W pin to ground
  * LCD V0 pin to digital pin 9
  * LCD backlight Anode pin to digital pin 10
 
  Copyright (C) 2014 Sean Auffinger
 
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <LiquidCrystal.h>
#include <BigNumbers.h>

const int lcdD7Pin = 2; // LCD D7 pin
const int lcdD6Pin = 3; // LCD D6 pin
const int lcdD5Pin = 4; // LCD D5 pin
const int lcdD4Pin = 5; // LCD D4 pin
const int lcdEPin = 11; // LCD E Pin
const int lcdRSPin = 12; // LCD RS pin

LiquidCrystal lcd(lcdRSPin, lcdEPin, lcdD4Pin, lcdD5Pin, lcdD6Pin, lcdD7Pin);
BigNumbers bigNum(&lcd);

void setup()
{
  TCCR1B = TCCR1B & 0b11111000 | 0x01; // increase Timer 1 PWM freq to ~32 kHz
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  analogWrite(9,50); // set LCD contrast with PWM
  analogWrite(10,127); // set LCD backlight with PWM
  lcd.begin(16,2);
  lcd.clear();
}

void loop()
{
  int currentTime = millis()/100; // assigns the current time since boot in tenths of a second to currentTime
  // extracts the digits from the number into an array
  byte timeString[5];
  timeString[4] = currentTime % 10;
  currentTime /= 10;
  timeString[3] = currentTime % 10;
  currentTime /= 10;
  timeString[2] = currentTime % 10;
  currentTime /= 10;
  timeString[1] = currentTime % 10;
  currentTime /= 10;
  timeString[0] = currentTime % 10;
  // print the digits before the decimal point as large numbers without preceding zeros
  boolean significantZero = false;
  for (int i = 0; i < 4; i++)
  {
    if( timeString[i]==0 && significantZero==false && i<3)
    {
      bigNum.clearLargeNumber(i*3);
    }
    else
    {
      bigNum.displayLargeNumber(timeString[i],i*3);
      significantZero = true;
    }
  }
  // print out the decimal point and the digit after it
  lcd.setCursor(12,1);
  lcd.print(".");
  lcd.print(timeString[4]);
}
