#include "Elemento.h"

#include <stdexcept>  // Para invalid_argument

// Implementação da classe base Elemento
Elemento::Elemento(int x, int y, char c) : Pixel(x, y, c) {
}

char Elemento::getSimbolo() const { return c; }
// Implementação da classe Deserto
Deserto::Deserto(int x, int y) : Elemento(x, y, '.') {
}

char Deserto::getSimbolo() const { return '.'; }

// Implementação da classe Montanha
Montanha::Montanha(int x, int y) : Elemento(x, y, '+') {
}

char Montanha::getSimbolo() const { return '+'; }

// Implementação da classe Cidade
Cidade::Cidade(int x, int y, char letra) : Elemento(x, y, letra) {
  if (letra < 'a' || letra > 'z') {
    throw std::invalid_argument(
      "Caracter invalido para Cidade. Deve ser uma letra minuscula.");
  }
}

char Cidade::getSimbolo() const { return c; }
