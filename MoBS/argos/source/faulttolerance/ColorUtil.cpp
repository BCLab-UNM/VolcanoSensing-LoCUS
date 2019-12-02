#include "ColorUtil.h"


CColor buildCColor(float red, float green, float blue) {
  return CColor(255.0 * red, 255.0 * green, 255.0 * blue);
}

CColor ColorUtil::HSVtoRGB(float hue, float saturation, float value) {
  // Conversion given by https://en.wikipedia.org/wiki/HSL_and_HSV#From_HSV
  float chroma = value * saturation;
  float band = fmod(360 * hue / 60, 6);
  float transition = chroma * (1 - fabs(fmod(band, 2) - 1));

  if(band < 0) {
    return buildCColor(0, 0, 0);
  } if(band < 1) {
    // Red to Yellow
    return buildCColor(chroma, transition, 0);
  } else if(band < 2) {
    // Yellow to Green
    return buildCColor(transition, chroma, 0);
  } else if(band < 3) {
    // Green to Cyan
    return buildCColor(0, chroma, transition);
  } else if(band < 4) {
    // Cyan to Blue
    return buildCColor(0, transition, chroma);
  } else if(band < 5) {
    // Blue to Purple
    return buildCColor(transition, 0, chroma);
  } else if(band < 6) {
    // Purple to Red
    return buildCColor(chroma, 0, transition);
  } else {
    return buildCColor(0, 0, 0);
  }
}