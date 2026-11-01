#include <Wire.h>
#include <math.h>
#include <LiquidCrystal_I2C.h>
#define LINE_WITH 4
#define LINE_HIGHT 2
#define CHAR_WITH 5
#define CHAR_HIGHT 8
#define X_JOISTICK_PIN 3
#define Y_JOISTICK_PIN 2

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

//display pixels for program logic
byte display[LINE_WITH][LINE_HIGHT*CHAR_HIGHT] = {{B0, B0}, {B11111111, B0}};
int x = 0;
int y = 0;
int status = 1;

char get_x_direction()
{
  int x_val = map(analogRead(X_JOISTICK_PIN), 0, 1014, 1, 7);
  if (x_val > 5)
  {
    return 'r';
  }
  else if (x_val < 3)
  {
    return 'l';
  }
  else
  {
    return 'm';
  }
}

char get_y_direction()
{
  int y_val = map(analogRead(Y_JOISTICK_PIN), 0, 1014, 7, 1);
  if (y_val > 5)
  {
    return 'u';
  }
  else if (y_val < 3)
  {
    return 'd';
  }
  else
  {
    return 'm';
  }
}

byte checkcords(int x, int y)
{
  byte xline = display[(x - (x%CHAR_WITH))/CHAR_WITH][y];
  xline = xline >> (CHAR_WITH - (x%CHAR_WITH) - 1);
  xline = xline << 7;
  switch (xline)
  {
    case B10000000:
      return B1;
    case B00000000:
      return B0;
  }
}

void setpixel(int x, int y, byte value)
{
  int power_x = (8-CHAR_WITH) - ((x%CHAR_WITH) - 1);
  //Serial.println(exact_x);
  if (checkcords(x, y) == value)
  {
    return;
  }
  switch (value)
  {
    case B0:
      display[(x - (x%CHAR_WITH))/CHAR_WITH][y] = display[(x - (x%CHAR_WITH))/CHAR_WITH][y] & ~(1 << power_x);
      return;
    case B1:
      display[(x - (x%CHAR_WITH))/CHAR_WITH][y] = display[(x - (x%CHAR_WITH))/CHAR_WITH][y] | (1 << power_x);
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
  fillwithvalue(B0);
}
/*********************************************************/
void loop()
{
  char x_dir = get_x_direction();
  char y_dir = get_y_direction();
  Serial.print("X: ");
  Serial.print(x);
  Serial.print(" Y: ");
  Serial.println(y);
  if (!digitalRead(8))
  {
    status *= -1;
  }
  if (status == -1)
  {
    setpixel(x, y, B0);
  }
  switch (x_dir)
  {
    case 'r':
      if (x < LINE_WITH*CHAR_WITH-1)
      {
        x++;
      }
      break;
    case 'l':
      if (x > 0)
      {
        x--;
      }
      break;
  }

  switch (y_dir)
  {
    case 'u':
      if (y > 0)
      {
        y--;
      }
      break;
    case 'd':
      if (y < LINE_HIGHT*CHAR_HIGHT-1)
      {
        y++;
      }
      break;
  }
  setpixel(x, y, B1);
  draw(display);
  delay(250);
}
/************************************************************/