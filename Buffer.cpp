#include "Buffer.h"

// Construtor
Buffer::Buffer(int linhas, int colunas)
    : linhas(linhas),
      colunas(colunas),
      cursorRow(0),
      cursorCol(0),
      buffer(std::make_unique<char[]>(linhas * colunas)) {
    clear();
}

// Construtor de cópia
Buffer::Buffer(const Buffer &other)
    : linhas(other.linhas),
      colunas(other.colunas),
      cursorRow(other.cursorRow),
      cursorCol(other.cursorCol),
      buffer(std::make_unique<char[]>(other.linhas * other.colunas)) {
    for (int i = 0; i < linhas * colunas; ++i) {
        buffer[i] = other.buffer[i]; // Cópia manual elemento a elemento
    }
}

// Calcula o índice linear com base nas coordenadas
int Buffer::index(int linha, int coluna) const {
    return linha * colunas + coluna;
}

// Operador de atribuição
Buffer &Buffer::operator=(const Buffer &other) {
    if (this != &other) {
        // Evitar auto-atribuição
        linhas = other.linhas;
        colunas = other.colunas;
        cursorRow = other.cursorRow;
        cursorCol = other.cursorCol;
        buffer = std::make_unique<char[]>(other.linhas * other.colunas);

        for (int i = 0; i < linhas * colunas; ++i) {
            buffer[i] = other.buffer[i]; // Cópia elemento a elemento
        }
    }
    return *this;
}

// Limpa o buffer preenchendo com espaços (' ')
void Buffer::clear() {
    for (int i = 0; i < linhas * colunas; ++i) {
        buffer[i] = ' '; // Preenche o buffer manualmente
    }
}

// Move o cursor para uma linha e coluna específicas
void Buffer::moveCursor(int linha, int coluna) {
    if (linha >= 0 && linha < linhas && coluna >= 0 && coluna < colunas) {
        cursorRow = linha;
        cursorCol = coluna;
    }
}

// Escreve o conteúdo do buffer no ostream
std::ostream &operator<<(std::ostream &os, const Buffer &buffer) {
    for (int _linha = 0; _linha < buffer.linhas; ++_linha) {
        for (int c = 0; c < buffer.colunas; ++c) {
            os << buffer.buffer[buffer.index(_linha, c)];
        }
        os << '\n';
    }
    return os;
}

// Operador << para escrever strings no buffer
Buffer &operator<<(Buffer &buffer, const char *str) {
    while (*str) {
        if (buffer.cursorRow < buffer.linhas && buffer.cursorCol < buffer.colunas) {
            int idx = buffer.index(buffer.cursorRow, buffer.cursorCol);
            buffer.buffer[idx] = *str++;
            buffer.cursorCol++;
            if (buffer.cursorCol >= buffer.colunas) {
                buffer.cursorCol = 0;
                buffer.cursorRow++;
            }
        } else {
            break; // Evita escrever fora do limite
        }
    }
    return buffer;
}

// Operador << para escrever caracteres no buffer
Buffer &operator<<(Buffer &buffer, char ch) {
    if (buffer.cursorRow < buffer.linhas && buffer.cursorCol < buffer.colunas) {
        int idx = buffer.index(buffer.cursorRow, buffer.cursorCol);
        buffer.buffer[idx] = ch;
        buffer.cursorCol++;
        if (buffer.cursorCol >= buffer.colunas) {
            buffer.cursorCol = 0;
            buffer.cursorRow++;
        }
    }
    return buffer;
}

// Operador << para escrever inteiros no buffer
Buffer &operator<<(Buffer &buffer, int value) {
    char temp[20];
    int i = 0;
    if (value == 0) {
        temp[i++] = '0';
    } else {
        while (value > 0 && i < sizeof(temp) - 1) {
            temp[i++] = '0' + (value % 10);
            value /= 10;
        }
    }
    temp[i] = '\0';

    // Inverter a string para escrever corretamente
    for (int j = 0; j < i / 2; ++j) {
        std::swap(temp[j], temp[i - j - 1]);
    }
    buffer << temp;
    return buffer;
}
