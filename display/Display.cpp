#include <TFT_eSPI.h>
#include <SPI.h>

// Display definition
TFT_eSPI tft = TFT_eSPI();

void
ST7735_Setup()
{
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.setCursor(0, 0, 2);
  tft.println("Hello World!");
}