#include "Pixel.h"

Pixel::Pixel(int _x, int _y, char _c) : x(_x), y(_y), c(_c) {
}

int Pixel::getX() const {
  //linha
  return x;
}

int Pixel::getY() const {
  //coluna
  return y;
}

char Pixel::getC() const {
  return c;
}

