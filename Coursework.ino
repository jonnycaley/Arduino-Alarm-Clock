#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#include <TimeLib.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

int x = 0;
int y = 0;
int z = 0;
int button = 0;
int cursorPosition = 0;
int secondHour = 0;
int secondMin = 0;
int secondSecond = 0;
int setAlarm = 0;
int yesNo = 0;
int snoozeyesno = 0;
int snoozeSet = 0;
int NOOOO = 0;
int noChosen = 0;
int alarmMet = 0;

void setup()  {
  Serial.begin(9600);
  setTime(x, y, z, 0, 0, 0);
  lcd.begin(16, 2);
}
void loop() {
  uint8_t buttons = lcd.readButtons();

  lcd.setCursor(0, 0);
  digitalClockDisplay();
  lcd.print(" - Clock");
  delay(50);

  if (button == 0) {
    hourLoop();
    minLoop();
    secLoop();
  }
  if (button == 1 && noChosen == 0) {
    lcd.setCursor(0, 1);
    lcd.print("Set Alarm? Y/N");
    setAlarmY();
    setAlarmN();
  }
  if (button == 2) {
    hourLoop2();
    minLoop2();
    secLoop2();
  }
  if (alarmMet == 1 && button == 2) {
      secondSecond = second();
  }
  if (setAlarm == 1 && snoozeSet == 0) {
    if (secondHour == hour() && secondMin == minute() && secondSecond == second()) {
      alarmMet = 1;
      lcd.setBacklight(TEAL);
      delay(300);
      lcd.setBacklight(RED);
      lcd.setCursor(0, 1);
      lcd.print("Snooze? YES/NO  ");
      snoozeLoop(yesNo);
    }
    else {
      lcd.setCursor(0, 1);
      printDigitsHour(secondHour);
      printDigits(secondMin);
      printDigits(secondSecond);
      lcd.print(" - Set! ");
    }
  }
  if (snoozeSet == 1 && secondHour == hour() && secondMin == minute() && secondSecond == second()) {
    snoozeSet = 0;
  }

  if (buttons) {
    if (buttons & BUTTON_UP) {
      if (button == 0) {
        if (cursorPosition == 0) {
          if (x < 23) {
            setTime(++x, y, z, 0, 0, 0);
          }
          else {
            x = 0;
            setTime(x, y, z, 0, 0, 0);
          }
        }
        if (cursorPosition == 1) {
          if (y < 59) {
            setTime(x, ++y, z, 0, 0, 0);
          }
          else {
            y = 0;
            setTime(x, y, z, 0, 0, 0);
          }
        }
        if (cursorPosition == 2) {
          if (z < 59) {
            setTime(x, y, ++z, 0, 0, 0);
          }
          else {
            z = 0;
            setTime(x, y, z, 0, 0, 0);
          }
        }
        if (cursorPosition == 3) {
          cursorPosition == 2;
        }
      }
      if (button == 2) {
        if (cursorPosition == 0) {
          if (secondHour < 23) {
            ++secondHour;
          }
          else {
            secondHour = 0;
          }
        }
        if (cursorPosition == 1) {
          if (secondMin < 59) {
            ++secondMin;
          }
          else {
            secondMin = 0;
          }
        }
        if (cursorPosition == 2) {
          if (secondSecond < 59) {
            ++secondSecond;
          }
          else {
            secondSecond = 0;
          }
        }
      }
    }
    if (buttons & BUTTON_DOWN) {
      if (button == 0) {
        if (cursorPosition == 0) {
          if (x > 0) {
            setTime(--x, y, z, 0, 0, 0);
          }
          else {
            x = 23;
            setTime(x, y, z, 0, 0, 0);
          }
        }
        if (cursorPosition == 1) {
          if (y > 0) {
            setTime(x, --y, z, 0, 0, 0);
          }
          else {
            y = 59;
            setTime(x, y, z, 0, 0, 0);
          }
        }
        if (cursorPosition == 2) {
          if (z > 0) {
            setTime(x, y, --z, 0, 0, 0);
          }
          else {
            z = 59;
            setTime(x, y, z, 0, 0, 0);
          }
        }
        if (cursorPosition == 3) {
          cursorPosition == 2;
        }
      }
      if (button == 2) {
        if (cursorPosition == 0) {
          if (secondHour < 1) {
            secondHour = 23;
          }
          else {
            --secondHour;
          }
        }
        if (cursorPosition == 1) {
          if (secondMin > 1) {
            --secondMin;
          }
          else {
            secondMin = 59;
          }
        }
        if (cursorPosition == 2) {
          if (secondSecond > 1) {
            --secondSecond;
          }
          else {
            secondSecond = 59;
          }
        }
      }
    }

    if (buttons & BUTTON_LEFT) {
      --cursorPosition;
      if (setAlarm == 1) {
        yesNo = 0;
      }
      if (button == 0 && cursorPosition == -1) {
        cursorPosition = 0;
      }
      if (button == 2 && cursorPosition == -1) {
        cursorPosition = 0;
      }
    }
    if (buttons & BUTTON_RIGHT) {
      ++cursorPosition;
      if (button == 0 && cursorPosition == 3) {
        cursorPosition = 2;
      }
      if (button == 2 && cursorPosition == 3) {
        cursorPosition = 2;
      }

      if (setAlarm == 1) {
        yesNo = 1;
        snoozeyesno = 0;
        NOOOO = 1;
      }

    }
    if (buttons & BUTTON_SELECT) {
      if (snoozeyesno == 1) {
        lcd.setCursor(0, 1);
        secondMin = secondMin + 1;
        alarmMet = 0;
        printDigitsHour(secondHour);
        printDigits(secondMin);
        printDigits(secondSecond);
        lcd.print(" Snooze Set");
        snoozeSet = 1;
        lcd.setBacklight(WHITE);
      }
      if (button == 2) {
        cursorPosition = 4;
        setAlarm = 1;
        snoozeyesno = 1;
      }
      if (button == 1) {
        if (cursorPosition == 0 || ((cursorPosition != 1) && !(cursorPosition % 2))) {
          button = 2;
          cursorPosition = 0;
        }
        else {
          noAlarmSet();
          noChosen = 1;
          button = 1;
        }
      }
      if (button == 0) {
        button = 1;
        cursorPosition = 0;
      }
      if (NOOOO == 1) {
        secondHour = 0;
        secondMin = 0;
        secondSecond = 0;
        button = 1;
        setAlarm = 0;
        cursorPosition = 0;
        setAlarm = 0;
        yesNo = 0;
        snoozeyesno = 0;
        snoozeSet = 0;
        NOOOO = 0;
        lcd.setBacklight(WHITE);
      }
    }
  }
}

void digitalClockDisplay() {
  if (button == 0) {
    printDigitsHour(hour(0));
    printDigits(minute(0));
    printDigits(second(0));
  }
  if (button == 2 || button == 1) {
    //clock display of the time
    printDigitsHour(hour());
    printDigits(minute());
    printDigits(second());
  }
}

void printDigits(int digits) {
  // function for digital clock display, prints preceding colon and 0
  lcd.print(":");
  if (digits < 10)
    lcd.print('0');
  lcd.print(digits);
}

void printDigitsHour(int digits) {
  // function for digital clock display, prints preceding colon and
  if (digits < 10)
    lcd.print('0');
  lcd.print(digits);
}

void hourLoop() {
  if (cursorPosition == 0) {
    lcd.setCursor(0, 0);
    lcd.print("  ");
    delay(250);
  }
}

void minLoop() {
  if (cursorPosition == 1) {
    lcd.setCursor(3, 0);
    lcd.print("  ");
    delay(250);
  }
}
void secLoop() {
  if (cursorPosition == 2) {
    lcd.setCursor(6, 0);
    lcd.print("  ");
    delay(250);
  }
}

void hourLoop2() {
  if (cursorPosition == 0) {
    lcd.setCursor(0, 1);
    printDigitsHour(secondHour);
    printDigits(secondMin);
    printDigits(secondSecond);
    lcd.print(" - Alarm   ");
    delay(250);
    lcd.setCursor(0, 1);
    lcd.print("  ");
    delay(250);
  }
}

void minLoop2() {
  if (cursorPosition == 1) {
    lcd.setCursor(0, 1);
    printDigitsHour(secondHour);
    printDigits(secondMin);
    printDigits(secondSecond);
    lcd.print(" - Alarm");
    delay(250);
    lcd.setCursor(3, 1);
    lcd.print("  ");
    delay(250);
  }
}

void secLoop2() {
  if (cursorPosition == 2) {
    lcd.setCursor(0, 1);
    printDigitsHour(secondHour);
    printDigits(secondMin);
    printDigits(secondSecond);
    lcd.print(" - Alarm");
    delay(250);
    lcd.setCursor(6, 1);
    lcd.print("  ");
    delay(250);
  }
}

void snoozeLoop(int x) {
  if (x == 0) {
    lcd.setCursor(8, 1);
    lcd.print("   /NO");
    delay(250);
    lcd.setCursor(8, 1);
    lcd.print("YES/NO");
    delay(250);
  }
  if (x == 1) {
    lcd.setCursor(8, 1);
    lcd.print("YES/  ");
    delay(250);
    lcd.setCursor(8, 1);
    lcd.print("YES/NO");
    delay(250);
  }
}

void setAlarmY() {
  if (!(cursorPosition % 2)) {
    lcd.setCursor(11, 1);
    lcd.print(" ");
    delay(250);
  }
}
void setAlarmN() {
  if (cursorPosition % 2) {
    lcd.setCursor(13, 1);
    lcd.print(" ");
    delay(250);
  }
}
void noAlarmSet() {
  lcd.setCursor(0, 1);
  lcd.print("No Alarm Set :(");
}
