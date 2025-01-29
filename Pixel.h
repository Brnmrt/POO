#ifndef PROJETOPOO2024_PIXEL_H
#define PROJETOPOO2024_PIXEL_H

class Pixel {
  protected:
    int x, y; // Posicao no mapa
    char c; // Caracter associado
  public:
    Pixel(int _x, int _y, char _c);
    virtual ~Pixel() = default;

    virtual char getSimbolo() const = 0; // Representação visual do elemento no mapa
    int getX() const;
    int getY() const;
    char getC() const;
};

#endif //PROJETOPOO2024_PIXEL_H
