#include "Display.h"

// Display definition
TFT_eSPI tft = TFT_eSPI();

void
ST7735_Setup()
{
  tft.init();
  tft.setRotation(ST7735_ROTATION);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(ST7735_TEXT_SIZE);
}

void
ST7735_Text_Center(const char* fmt, ...)
{
  char buf[256];
  va_list args;

  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);

  va_end(args);

  tft.setTextDatum(MC_DATUM);
  tft.drawString(buf, tft.width() / 2, tft.height() / 2);
}