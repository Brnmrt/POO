#ifndef PROJETOPOO2024_BUFFER_H
#define PROJETOPOO2024_BUFFER_H

#include <memory>    // Para shared_ptr
#include <iostream>

class Buffer {
  private:
    int linhas, colunas; // Dimensões do buffer
    int cursorRow, cursorCol; // Coordenadas do cursor
    std::unique_ptr<char[]> buffer;

    // Calcula o índice linear com base nas coordenadas
    int index(int linha, int coluna) const;

  public:
    Buffer(int linhas, int colunas);
    Buffer(const Buffer &other); // Construtor de cópia
    Buffer &operator=(const Buffer &other); // Operador de atribuição
    ~Buffer() = default;

    void clear(); // Esvazia o buffer
    void moveCursor(int linha,
                    int coluna); // Move o cursor para uma posicao específica

    // Imprime o buffer final no ostream
    friend std::ostream &operator<<(std::ostream &os, const Buffer &buffer);

    // Escreve caracteres e valores no buffer (apenas modificam o buffer, nao
    // imprimem no cout)
    friend Buffer &operator<<(Buffer &buffer, const char *str);
    friend Buffer &operator<<(Buffer &buffer, int value);
    friend Buffer &operator<<(Buffer &buffer, char ch);
};

#endif  // PROJETOPOO2024_BUFFER_H
