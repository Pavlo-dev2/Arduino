#include <Wire.h>
#include <math.h>
#include <LiquidCrystal_I2C.h>
#define LINE_WITH 4
#define LINE_HIGHT 2
#define CHAR_WITH 5
#define CHAR_HIGHT 8

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

//display pixels for program logic
byte display[LINE_WITH][LINE_HIGHT*CHAR_HIGHT] = {{B0, B0}, {B11111111, B0}};

byte checkcords(int x, int y)
{
  byte xline = display[(x - (x%CHAR_WITH))/CHAR_WITH][y];
  //Serial.println(xline);
  int exact_x = x%CHAR_WITH;
  for (int i = CHAR_WITH - exact_x - 1; i > 0; i--)
  {
    xline /= 2;
  }
  //Serial.println(xline);
  for (int i = 0; i < 7; i++)
  {
    //Serial.print("In loop: ");
    //Serial.println(xline);
    xline *= 2;
  }
  //Serial.println(xline);
  switch (xline)
  {
    case B10000000:
      return B1;
      break;
    case B00000000:
      return B0;
      break;
  }
  return;
}

void setpixel(int x, int y, byte value)
{
  int exact_x = (8-CHAR_WITH) - ((x%CHAR_WITH) - 1);
  Serial.println(exact_x);
  if (checkcords(x, y) == value)
  {
    return;
  }
  switch (value)
  {
    case B0:
      display[(x - (x%CHAR_WITH))/CHAR_WITH][y] = display[(x - (x%CHAR_WITH))/CHAR_WITH][y]-pow(2, exact_x);
      return;
    case B1:
      display[(x - (x%CHAR_WITH))/CHAR_WITH][y] = display[(x - (x%CHAR_WITH))/CHAR_WITH][y]+pow(2, exact_x);
      return;
  }
}

void fillwithvalue(byte value)
{
  for (int y = 0; y < LINE_HIGHT*CHAR_HIGHT; y++)
  {
    for (int x = 0; x < LINE_WITH; x++)
    {
      display[x][y] = value;
    }
  }
}

//draws the displey
void draw(byte display[LINE_WITH][LINE_HIGHT*CHAR_HIGHT])
{
  int slot = 0;
  int charpos_x, charpos_y;//possisin of charecter you are working on at the moment
  for (charpos_y = 0; charpos_y < LINE_HIGHT; charpos_y++)
  {
    for (charpos_x = 0; charpos_x < LINE_WITH; charpos_x++)
    {
      //workin on 1 character fild
      byte customChar[8] = {B0};
      int pixelpos_x, pixelpos_y;//possision of pixel you are working on at the moment
      for (pixelpos_y = 0; pixelpos_y < CHAR_HIGHT; pixelpos_y++)
      {
        customChar[pixelpos_y] = display[charpos_x][charpos_y*CHAR_HIGHT+pixelpos_y];
      }
      lcd.createChar(slot, customChar);
      lcd.setCursor(charpos_x, charpos_y);
      lcd.write(byte(slot));
      slot++;
    }
  }
}
/*********************************************************/
void setup()
{
  lcd.init();  //initialize the lcd
  lcd.backlight();  //open the backlight
  Serial.begin(9600);
}
/*********************************************************/
void loop()
{
  Serial.println(checkcords(5, 1));
  lcd.clear();
  fillwithvalue(B0);
  setpixel(19, 15, B1);
  draw(display);
  delay(1000);
}
/************************************************************/