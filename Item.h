#ifndef PROJETOPOO2024_ITEM_H
#define PROJETOPOO2024_ITEM_H

#include "Caravana.h"
#include "Pixel.h"

class Caravana;
// Classe base Item
class Item : public Pixel, std::enable_shared_from_this<Item> {
  private:
    int duracao; // Duração em instantes até desaparecer
  public:
    Item(int x, int y, char simbolo, int duracao);
    ~Item() override = default;

    char getSimbolo() const override;
    int getDuracao() const;
    void reduzirDuracao();
    virtual void aplicarEfeito(std::shared_ptr<Caravana> &caravana) = 0;
};

// Classe derivada CaixaPandora
class CaixaPandora : public Item {
  public:
    CaixaPandora(int x, int y, int duracao);
    void aplicarEfeito(std::shared_ptr<Caravana> &caravana) override;
};

// Classe derivada ArcaTesouro
class ArcaTesouro : public Item {
  public:
    ArcaTesouro(int x, int y, int duracao);
    void aplicarEfeito(std::shared_ptr<Caravana> &caravana) override;
};

// Classe derivada Jaula
class Jaula : public Item {
  public:
    Jaula(int x, int y, int duracao);
    void aplicarEfeito(std::shared_ptr<Caravana> &caravana) override;
};

// Classe derivada Mina
class Mina : public Item {
  public:
    Mina(int x, int y, int duracao);
    void aplicarEfeito(std::shared_ptr<Caravana> &caravana) override;
};

// Classe derivada Surpresa
class Surpresa : public Item {
  public:
    Surpresa(int x, int y, int duracao);
    void aplicarEfeito(std::shared_ptr<Caravana> &caravana) override;
};

#endif  // PROJETOPOO2024_ITEM_H
