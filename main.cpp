#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
    lcd.init(); // initialize the lcd

    // Print a message to the LCD.
    lcd.backlight();
    lcd.setCursor(2, 1);
    lcd.printf("(o o)");

    /*set sleep
    lcd.setCursor(2, 1);
    lcd.printf("     zZ");
    lcd.setCursor(2, 2);
    lcd.printf("(- -)");*/
    /*lcd.setCursor(10, 1);
    lcd.printf("25°C");*/

    /*set hot
    lcd.setCursor(2, 1);
    lcd.printf("     !")
    lcd.setCursor(2, 2);
    lcd.printf("(= o=)");*/

    /*set cold
    lcd.setCursor(2, 1);
    lcd.printf("     !")
    lcd.setCursor(2, 2);
    lcd.printf("(- =-)");*/

    /*set wet
    lcd.setCursor(2, 1);
    lcd.printf("     !");
    lcd.setCursor(2, 2);
    lcd.printf("(o w o)");
    lcd.setCursor(10, 1);
    lcd.printf("25C");*/

    /*set dry
    lcd.setCursor(2, 1);
    lcd.printf("     !")
    lcd.setCursor(2, 2);
    lcd.printf("(o x o)");*/

    /*set dark
    lcd.setCursor(2, 1);
    lcd.printf("     !");
    lcd.setCursor(2, 2);
    lcd.printf("(0 0)");*/

    /*set sun
    lcd.setCursor(2, 2);
    lcd.printf("(@ u @");*/
}

void warningSign() {
  for(int i = 0; i < 9; i++) {
      lcd.setCursor(2, 1);
      lcd.printf("      ");
      lcd.printf("     !");
    }
}

void loop() {
  //show temp
  lcd.setCursor(10, 1);
  lcd.printf("%.2f C", temperature);

  //light status
  if (light < 500) {
    LINE.notify("dude, i think it's really dark here...");

    //emote
    warningSign();
    lcd.setCursor(2, 2);
    lcd.printf("(0 0)");
  } else {
    LINE.notify("hmmm, it's time to sunbathing! ^^");

    //emote
    lcd.setCursor(2, 2);
    lcd.printf("(@ u @");
  }

  if (temperature < 20) {
    LINE.notify("damn man, i'm too cool");
    //emote
    warningSign();
    lcd.setCursor(2, 2);
    lcd.printf("(- =-)");
  } else if (temperature >= 30) {
    LINE.notify("damn i'm too hot, too hot!");
    //emote
    warningSign();
    lcd.setCursor(2, 2);
    lcd.printf("(= o=)");
  } else {
    LINE.notify("hmmm.., not bad not bad ^^");
    
    //emote
    lcd.setCursor(2, 1);
    lcd.printf("(o o)");
  }

  //mois status
  if (moisture < wet) {
    LINE.notify("damn dude that's too wet, too wet");
    
    //emote
    warningSign();
    lcd.printf("(o w o)");
  } else if (moisture >= dry) {
    LINE.notify("damn dude that's too dry, give me some water");
    
    //emote
    warningSign();
    lcd.setCursor(2, 2);
    lcd.printf("(o x o)");
  } else {
    LINE.notify("That's perfect mois ^^");

    //emote
    for(int i = 0; i < 9; i++) {
      lcd.setCursor(2, 1);
      lcd.printf("     z ");
      lcd.printf("     zZ");
    }
    lcd.setCursor(2, 2);
    lcd.printf("(- -)");
  }
}