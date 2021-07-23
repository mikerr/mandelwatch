

#include "config.h"

TTGOClass *ttgo;

#define TFT_W 240
#define TFT_H 240

void setup()
{
    // Setup the LCD
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->tft->setTextFont(1);
}

int changed;
double scale = 1./128;
double cx = -.6, cy = 0;

void mandel(int skip) {
  int width = TFT_W;
  int height = TFT_H;

  int iter, max_iter = 256;
  double x, y, zx, zy, zx2, zy2;
  for (int i = 0; i < height; i += skip) {
    y = (i - height/2) * scale + cy;
    for (int j = 0; j  < width; j += skip) {
      x = (j - width/2) * scale + cx;
      zx = hypot(x - (double).25, y);
      if (x < zx - 2 * zx * zx + (double).25) continue;
      if ((x + 1)*(x + 1) + y * y < 1/16) continue;

      zx = zy = zx2 = zy2 = 0;
      for (iter = 0; iter < max_iter && zx2 + zy2 < 4; iter++) {
        zy = 2 * zx * zy + y;
        zx = zx2 - zy2 + x;
        zx2 = zx * zx;
        zy2 = zy * zy;
      }
      if (iter < max_iter) {     
               ttgo->tft->fillRect(j,i,skip,skip,iter*10);
               if (skip < 2) {   int16_t tx, ty;
                      if (ttgo->getTouch(tx,ty)) break;
                      }
      }
    }
  }
}

void loop()
{  
  static int resolution=5;     
  if (resolution) mandel(resolution--);

  int16_t x, y;
  if (ttgo->getTouch(x, y)) {
      if (x > TFT_W - 60) cx +=0.1;
      if (x < 60) cx -=0.1;
      if (y < 60) cy -=0.1;

      if (y > TFT_H-60) {
        if (x < 60) scale *= 1.1;
        if (x > TFT_W-60) scale /= 1.1;
        if ( abs(x-120) < 60) cy +=0.1;
      }
      resolution = 5;
      ttgo->tft->fillScreen(TFT_BLACK);
  }
}

