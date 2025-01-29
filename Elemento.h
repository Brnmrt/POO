#ifndef PROJETOPOO2024_ELEMENTO_H
#define PROJETOPOO2024_ELEMENTO_H

#include "Pixel.h"

// Classe base Elemento
class Elemento : public Pixel {
  public:
    Elemento(int x, int y, char c);
    ~Elemento() override = default;

    [[nodiscard]] char getSimbolo() const override;
};

// Classe derivada Deserto
class Deserto : public Elemento {
  public:
    Deserto(int x, int y);
    [[nodiscard]] char getSimbolo() const override;
};

// Classe derivada Montanha
class Montanha : public Elemento {
  public:
    Montanha(int x, int y);
    [[nodiscard]] char getSimbolo() const override;
};

// Classe derivada Cidade
class Cidade : public Elemento {
  public:
    Cidade(int x, int y, char letra);
    [[nodiscard]] char getSimbolo() const override;
};

#endif  // PROJETOPOO2024_ELEMENTO_H
