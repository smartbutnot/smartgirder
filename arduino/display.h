#include <Adafruit_Protomatter.h>

#define MATRIX_WIDTH 256
#define MATRIX_HEIGHT 32
#define BIT_DEPTH 4

uint8_t rgbPins[]  = {7, 8, 9, 10, 11, 12};
uint8_t addrPins[] = {17, 18, 19, 20, 21};
uint8_t clockPin   = 14;
uint8_t latchPin   = 15;
uint8_t oePin      = 16;

//a bit depth of 4 was my sweet spot for colour range vs glitching
//double buffer set to true reduces refresh glitches
Adafruit_Protomatter display(
  MATRIX_WIDTH, BIT_DEPTH, 1, rgbPins, 4, addrPins, clockPin, latchPin, oePin, true);


void setupDisplay() {
  ProtomatterStatus pStatus = display.begin();
  Serial.print("Protomatter begin() status: ");
  Serial.println((int)pStatus);
  if(pStatus != PROTOMATTER_OK) {
    for(;;);
  }
}

void drawRightAlignedText(uint8_t xpos, uint8_t ypos, String text) {
  uint8_t characterWidth = 5;
  uint16_t textLength = text.length();
  uint16_t offset = xpos - textLength * (characterWidth + 1);
  display.setCursor(offset, ypos);
  display.print(text);
}


uint16_t FSFColorBrightness(uint16_t hexValue, int br){
  unsigned r = (hexValue & 0xF800) >> 8;       // rrrrr... ........ -> rrrrr000
  unsigned g = (hexValue & 0x07E0) >> 3;       // .....ggg ggg..... -> gggggg00
  unsigned b = (hexValue & 0x1F) << 3;         // ............bbbbb -> bbbbb000
  return display.color565(r*br/100,g*br/100,b*br/100);
}

uint16_t rgbColorBrightness(int r, int g, int b, int br){
  return display.color565(r*br/100,g*br/100,b*br/100);
}

void drawIcon(int x, int y, int width, int height, const uint16_t image[]) {
  int counter = 0;
  for (int yy = 0; yy < height; yy++) {
    for (int xx = 0; xx < width; xx++) {
      display.drawPixel(xx + x , yy + y, FSFColorBrightness(image[counter],brightness));
      counter++;
    }
  }
}
