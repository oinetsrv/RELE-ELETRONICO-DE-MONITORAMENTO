/*
    LCD_I2C - Arduino library to control a 16x2 LCD via an I2C adapter based on PCF8574

    Copyright(C) 2020 Blackhack <davidaristi.0504@gmail.com>

    This program is free software : you can redistribute it and /or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.If not, see < https://www.gnu.org/licenses/>.
*/

#include <LCD_I2C.h>

LCD_I2C lcd(0x27, 16, 2); // Default address of most PCF8574 modules, change according
LCD_I2C lcd_2(0x26, 16, 2); // Default address of most PCF8574 modules, change according

void setup()
{
    lcd.begin(); // If you are using more I2C devices using the Wire library use lcd.begin(false)
                 // this stop the library(LCD_I2C) from calling Wire.begin()
    lcd.backlight();
    lcd_2.begin(); 
    lcd_2.backlight();
}

void loop()
{
    lcd.print("     Hello"); // You can make spaces using well... spaces
    lcd.setCursor(5, 1); // Or setting the cursor in the desired position.
    lcd.print("END 27!");
    delay(500);
    lcd_2.print("     Hello"); // You can make spaces using well... spaces
    lcd_2.setCursor(5, 1); // Or setting the cursor in the desired position.
    lcd_2.print("END 26!");
    delay(500);

    // Flashing the backlight
    for (int i = 0; i < 5; ++i)
    {
        lcd.backlight();
        delay(50);
        lcd.noBacklight();
        delay(50);
        lcd_2.backlight();
        delay(50);
        lcd_2.noBacklight();
        delay(50);
    }

    lcd.backlight();
    lcd.clear();
    delay(500);
    lcd_2.backlight();
    lcd_2.clear();
    delay(500);
}
