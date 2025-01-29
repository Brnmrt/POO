#include "Mapa.h"
#include <algorithm>
#include <cstdlib>  // Para rand e srand
#include <fstream>
#include <iostream>

Mapa::Mapa() : linhas(0), colunas(0), grid(nullptr) {
}

Mapa::~Mapa() { clear(); }

int Mapa::getLinhas() const { return linhas; }
int Mapa::getcolunas() const { return colunas; }

char Mapa::getCell(int linha, int coluna) const {
    if (linha >= 0 && linha < linhas && coluna >= 0 && coluna < colunas) {
        return grid[linha * colunas + coluna];
    }
    return ' ';
}

bool Mapa::loadFromFile(const std::string &filename) {
    clear(); // Limpa o mapa antigo

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Erro: nao foi possível abrir o ficheiro de configuracao.\n";
        return false;
    }

    // Ler "linhas" e o numero de linhas
    std::string label;
    file >> label >> linhas;
    if (label != "linhas" || linhas <= 0) {
        std::cout << "Erro: Dimensao 'linhas' invalida ou mal formatada.\n";
        return false;
    }

    // Ler "colunas" e o numero de colunas
    file >> label >> colunas;
    if (label != "colunas" || colunas <= 0) {
        std::cout << "Erro: Dimensao 'colunas' invalida ou mal formatada.\n";
        return false;
    }

    file.ignore(); // Ignora o restante da linha

    // Verifica as dimensões do mapa
    grid = std::make_unique<char[]>(linhas * colunas); // Aloca memória para o mapa

    // Ler cada linha do mapa
    for (int i = 0; i < linhas; ++i) {
        std::string line;
        getline(file, line);

        if (line.size() !=
            static_cast<size_t>(colunas)) {
            // Verificar tamanho da linha
            std::cout << "Erro: Linha " << i + 1
                    << " do mapa tem um tamanho diferente do esperado.\n";
            return false;
        }

        for (int j = 0; j < colunas; ++j) {
            grid[i * colunas + j] = line[j]; // Preenche o grid
        }
    }

    // O ficheiro continua com os valores configuráveis que serão ignorados aqui.
    // Estes valores são processados no Simulador::carregarMapa.
    file.close();
    return true;
}

void Mapa::renderToBuffer(
    Buffer &buffer,
    const std::vector<std::shared_ptr<Caravana> > &caravanas) const {
    buffer.clear();

    // Renderizar a grelha base do mapa
    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            buffer.moveCursor(i, j);
            buffer << grid[i * colunas + j]; // Renderiza o mapa
        }
    }

    // Renderizar as caravanas
    for (const auto &caravana : caravanas) {
        int linha = caravana->getLinha();
        int coluna = caravana->getColuna();

        // Apenas renderiza a caravana se nao houver cidade na mesma posicao
        if (!(grid[linha * colunas + coluna] >= 'a' && grid[linha * colunas + coluna] <= 'z')) {
            buffer.moveCursor(linha, coluna);
            buffer << caravana->getSimbolo(); // Renderiza as caravanas
        }
    }
}

bool Mapa::posicaoValida(int linha, int coluna) const {
    if (!grid) {
        std::cout << "Erro: O mapa nao esta carregado.\n";
        return false;
    }

    // Verifica se a posicao está dentro dos limites, nao e montanha, caravana (0-9) ou bárbara (!)
    return linha >= 0 && linha < linhas &&
            coluna >= 0 && coluna < colunas &&
            grid[linha * colunas + coluna] != '+' && // Nao permite montanhas
            (grid[linha * colunas + coluna] < '0' || grid[linha * colunas + coluna] > '9') && // Evita 0-9 (caravanas)
            grid[linha * colunas + coluna] != '!'; // Evita bárbaros
}

bool Mapa::encontrarCidade(char simbolo, int &linha, int &coluna) const {
    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            if (grid[i * colunas + j] == simbolo) {
                // Verifica o simbolo na celula
                linha = i;
                coluna = j;
                return true; // Cidade encontrada
            }
        }
    }
    return false; // Cidade nao encontrada
}

void Mapa::colocarItem(std::shared_ptr<Item> item) {
    if (item && posicaoValida(item->getY(), item->getX())) {
        items.push_back(move(item)); // Mover para o vetor
    }
}

void Mapa::adicionarItemAleatorio() {
    const int maxTentativas = 100;
    for (int i = 0; i < maxTentativas; ++i) {
        int y = rand() % linhas;
        int x = rand() % colunas;

        // Só permite adicionar itens em desertos ('.')
        if (grid[y * colunas + x] == '.' && !getItemAt(y, x)) {
            auto novoItem = sortearItemAleatorio(y, x); // Cria o item
            colocarItem(move(novoItem));
            break;
        }
    }
}

void Mapa::setCell(int linha, int coluna, char simbolo) {
    if (linha >= 0 && linha < linhas && coluna >= 0 && coluna < colunas) {
        // Proteção para evitar sobrescrita de cidades
        if (grid[linha * colunas + coluna] < 'a' || grid[linha * colunas + coluna] > 'z') {
            grid[linha * colunas + coluna] = simbolo;
        }
    }
}

void Mapa::removerItensExpirados() {
    items.erase(remove_if(items.begin(),
                          items.end(),
                          [](const std::shared_ptr<Item> &item) {
                              return item->getDuracao() <=
                                      0; // Remove os itens expirados
                          }),
                items.end());
}

std::shared_ptr<Item> Mapa::getItemAt(int linha, int coluna) const {
    for (const auto &item : items) {
        if (item->getY() == coluna && item->getX() == linha) {
            return item; // Retorna diretamente o shared_ptr
        }
    }
    return nullptr; // Se nao encontrar, retorna nullptr (shared_ptr vazio)
}

void Mapa::renderItemsToBuffer(Buffer &buffer) const {
    if (!grid) return;

    for (const auto &item : items) {
        // auto& para iterar sobre shared_ptr
        buffer.moveCursor(item->getX(), item->getY());
        buffer << item->getSimbolo();
    }
}

void Mapa::removerItem(std::shared_ptr<Item> item) {
    if (item) {
        items.erase(remove_if(items.begin(),
                              items.end(),
                              [item](const std::shared_ptr<Item> &ptr) {
                                  return ptr.get() == item.get(); // Comparação correta
                              }),
                    items.end());
        setCell(item->getX(), item->getY(), '.');
    }
}

void Mapa::clear() {
    grid.reset(); // Liberta automaticamente a memória de grid
    items.clear(); // Liberta automaticamente os itens do vetor
}

void Mapa::wrapPosition(int &linha, int &coluna) const {
    linha = (linha + linhas) % linhas;
    coluna = (coluna + colunas) % colunas;
}

std::shared_ptr<Item> Mapa::sortearItemAleatorio(int linha, int coluna) const {
    int tipo = rand() % 5;
    switch (tipo) {
        case 0:
            return std::make_shared<CaixaPandora>(linha, coluna, 5);
        case 1:
            return std::make_shared<ArcaTesouro>(linha, coluna, 10);
        case 2:
            return std::make_shared<Jaula>(linha, coluna, 8);
        case 3:
            return std::make_shared<Mina>(linha, coluna, 6);
        case 4:
            return std::make_shared<Surpresa>(linha, coluna, 4);
        default:
            return nullptr;
    }
}

void Mapa::reconstruirMapa(const Buffer &buffer) {
    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            char simbolo = getCell(i, j);
            setCell(i, j, simbolo); // Atualiza a celula do mapa
        }
    }
}

void Mapa::limparTempestadeAreia() {
    if (!grid) {
        std::cout << "Erro: O mapa nao esta carregado.\n";
        return;
    }

    for (int i = 0; i < linhas; ++i) {
        for (int j = 0; j < colunas; ++j) {
            if (grid[i * colunas + j] == '~') {
                grid[i * colunas + j] = '.'; // Restaura a celula ao estado original
            }
        }
    }
}
