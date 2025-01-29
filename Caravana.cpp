#include "Caravana.h"

#include <algorithm>
#include <cstdlib>  // Para rand e srand
#include <iostream>
#include "Buffer.h"
#include "Mapa.h"

Caravana::Caravana(int id,
                   int linha,
                   int coluna,
                   int maxTripulacao,
                   int maxMercadoria,
                   int maxAgua,
                   std::shared_ptr<Simulador> simulador)
    : id(id),
      linha(linha),
      coluna(coluna),
      maxTripulacao(maxTripulacao),
      maxMercadoria(maxMercadoria),
      maxAgua(maxAgua),
      tripulacaoAtual(maxTripulacao),
      mercadoriaAtual(0),
      aguaAtual(maxAgua),
      turnosSemTripulacao(0),
      tipo('C'),
      simulador(std::move(simulador)) {
    turnoCriacao = getSimulador()->getTurnos();
}
std::shared_ptr<Simulador> Caravana::getSimulador() const { return simulador; }

void Caravana::consumirRecursos() {
    if (aguaAtual > 0 && tripulacaoAtual > 0) {
        aguaAtual -= tripulacaoAtual;
        if (aguaAtual < 0) aguaAtual = 0;
    } else if (tripulacaoAtual > 0) {
        tripulacaoAtual = std::max(0, tripulacaoAtual - 1);
    } else {
        // Incrementa turnos sem tripulacao diretamente
        turnosSemTripulacao++;
    }
}

void Caravana::faltaAguaAtacaTripulacao() {
    if (getTipo() != 'B' && getAgua() <= 0) {
        // Apenas caravanas não bárbaras
        setTripulacao(std::max(0, getTripulacao() - 1)); // Perde um tripulante
        std::cout << "Caravana " << getId() << " ficou sem agua e perdeu um tripulante.\n";
    }
}

void Caravana::mudaCelula(std::shared_ptr<Mapa> &mapa) {
    tripulacaoAtual = 0;
    aguaAtual = 0;
    mercadoriaAtual = 0;

    // Atualiza o mapa para refletir a destruição
    mapa->setCell(linha, coluna, '.');

    std::cout << "Caravana " << id << " foi destruida.\n";
}

bool Caravana::terTripulacao() const { return tripulacaoAtual != 0; }

void Caravana::incrementaTurnosSemTripulacao() {
    if (!terTripulacao()) {
        turnosSemTripulacao++;
    }
}

int Caravana::getTurnosSemTripulacao() const { return turnosSemTripulacao; }

// Caravana Comercial
char CaravanaComercial::getSimbolo() const {
    return static_cast<char>(getId() + '0'); // Convertendo ID em char
}
char CaravanaComercial::getTipo() const {
    return 'C'; // simbolo para a caravana de comércio
}

void CaravanaComercial::move(
    const std::string &direcao,
    std::shared_ptr<Mapa> &map,
    const std::vector<std::shared_ptr<Caravana> > &caravanas) {
    int linhaAnterior = getLinha();
    int colunaAnterior = getColuna();

    if (!terTripulacao()) {
        // Movimento aleatório quando sem tripulação
        int novaLinha = linhaAnterior + (rand() % 3 - 1);
        int novaCol = colunaAnterior + (rand() % 3 - 1);
        map->wrapPosition(novaLinha, novaCol);

        if (map->posicaoValida(novaLinha, novaCol)) {
            map->setCell(linhaAnterior, colunaAnterior, '.'); // Limpa a celula anterior
            setLinha(novaLinha);
            setColuna(novaCol);
            map->setCell(novaLinha, novaCol, getSimbolo()); // Marca a nova celula
            std::cout << "\nCaravana" << getId() << " do tipo " << getTipo() << " movida aleatoriamente para a " <<
                    "posicao:(" << getLinha() << "," << getColuna() << ")";
        } else {
            std::cout << "\nCaravana " << getId() << " nao se pode mover aleatoriamente devido a obstaculos.\n";
        }
        return;
    }

    // Movimento normal (com tripulação)
    int steps = (getMercadoria() > getMaxMercadoria() / 2)
                    ? 1
                    : 2; // Movimentos dependem da carga

    for (int i = 0; i < steps; ++i) {
        // Verifica se o limite de movimentos no turno foi atingido
        if (getMovimentosNoTurno() >= steps) {
            std::cout << "\nCaravana " << getId() << " atingiu o limite de movimentos no turno.\n";
            break;
        }

        linhaAnterior = getLinha();
        colunaAnterior = getColuna();

        int novaLinha = linhaAnterior;
        int novaCol = colunaAnterior;

        calcularNovaPosicao(direcao, novaLinha, novaCol);

        // Ajusta as posições conforme as regras do mapa
        map->wrapPosition(novaLinha, novaCol);

        if (map->posicaoValida(novaLinha, novaCol)) {
            // Limpa a celula anterior
            map->setCell(linhaAnterior, colunaAnterior, '.');

            // Atualiza a posição
            setLinha(novaLinha);
            setColuna(novaCol);
            incrementarMovimentosNoTurno(); // Incrementa o contador de movimentos
            std::cout << "\nCaravana" << getId() << " do tipo " << getTipo() << " movida na direcao " << direcao <<
                    " posicao:(" << getLinha() << "," << getColuna() << ")";

            // Marca a nova celula no mapa
            map->setCell(novaLinha, novaCol, getSimbolo());
        } else {
            std::cout << "\nCaravana " << getId() << " nao se pode mover na direcao "
                    << direcao << " devido a obstaculos.\n";
            break; // Interrompe o movimento se a posição não for válida
        }
    }
}
void CaravanaComercial::consumirRecursos() {
    int rate = (getTripulacao() < getMaxTripulacao() / 2)
                   ? 1
                   : 2; // Reduz consumo com tripulacao menor
    setAgua(std::max(0, getAgua() - rate));
}

void CaravanaComercial::renderToBuffer(Buffer &buffer) const {
    buffer.moveCursor(getLinha(), getColuna());
    buffer << 'C';
}



// CaravanaMilitar
char CaravanaMilitar::getSimbolo() const {
    return static_cast<char>(getId() + '0'); // Convertendo ID em char
}

char CaravanaMilitar::getTipo() const {
    return 'M'; // simbolo para a caravana militar
}

void CaravanaMilitar::move(
    const std::string &direcao,
    std::shared_ptr<Mapa> &map,
    const std::vector<std::shared_ptr<Caravana> > &caravanas) {
    int linhaAnterior = getLinha();
    int colunaAnterior = getColuna();

    int novaLinha = linhaAnterior;
    int novaCol = colunaAnterior;

    if (!terTripulacao()) {
        // Movimento contínuo na última direção antes de perder tripulação
        if (!ultimaDirecao.empty()) {
            for (char dir : ultimaDirecao) {
                linhaAnterior = getLinha();
                colunaAnterior = getColuna();

                novaLinha = linhaAnterior;
                novaCol = colunaAnterior;

                if (dir == 'C')
                    novaLinha--;
                else if (dir == 'B')
                    novaLinha++;
                else if (dir == 'E')
                    novaCol--;
                else if (dir == 'D')
                    novaCol++;

                map->wrapPosition(novaLinha, novaCol);

                if (map->posicaoValida(novaLinha, novaCol)) {
                    map->setCell(linhaAnterior, colunaAnterior, '.'); // Limpa a posição anterior
                    setLinha(novaLinha);
                    setColuna(novaCol);
                    map->setCell(novaLinha, novaCol, getSimbolo()); // Marca a nova posição
                    std::cout << "\nCaravana" << getId() << " do tipo " << getTipo() << " movida na direcao " << direcao
                            << " posicao:(" << getLinha() << "," << getColuna() << ")";
                } else {
                    std::cout << "\nCaravana " << getId() << " nao se pode mover na direcao "
                            << dir << " devido a obstaculos.\n";
                    break; // Interrompe se encontrar obstáculo
                }
            }
        }
        return;
    }

    // Movimento normal: até 3 passos
    for (int i = 0; i < 3; ++i) {
        // Verifica se o limite de movimentos no turno foi atingido
        if (getMovimentosNoTurno() >= 3) {
            std::cout << "\nCaravana " << getId() << " atingiu o limite de movimentos no turno.\n";
            break;
        }

        linhaAnterior = getLinha();
        colunaAnterior = getColuna();

        novaLinha = linhaAnterior;
        novaCol = colunaAnterior;

        calcularNovaPosicao(direcao, novaLinha, novaCol);
        map->wrapPosition(novaLinha, novaCol);

        if (map->posicaoValida(novaLinha, novaCol)) {
            map->setCell(linhaAnterior, colunaAnterior, '.'); // Limpa a posição anterior
            setLinha(novaLinha);
            setColuna(novaCol);
            incrementarMovimentosNoTurno(); // Incrementa o contador de movimentos
            std::cout << "\nCaravana" << getId() << " do tipo " << getTipo() << " movida na direcao " << direcao <<
                    " posicao:(" << getLinha() << "," << getColuna() << ")";

            // Marca a nova posição no mapa
            map->setCell(novaLinha, novaCol, getSimbolo());
        } else {
            std::cout << "\nCaravana " << getId() << " nao se pode mover na direcao "
                    << direcao << " devido a obstaculos.\n";
            break; // Interrompe o movimento se encontrar obstáculo
        }
    }
}

void CaravanaMilitar::consumirRecursos() {
    int rate = (getTripulacao() < getMaxTripulacao() / 2) ? 1 : 3; // Reduz consumo com tripulacao menor
    setAgua(std::max(0, getAgua() - rate));
}

void CaravanaMilitar::renderToBuffer(Buffer &buffer) const {
    buffer.moveCursor(getLinha(), getColuna());
    buffer << 'M';
}



// Caravana Secreta
char CaravanaSecreta::getSimbolo() const {
    return ':';
}

char CaravanaSecreta::getTipo() const {
    return 'S'; // simbolo para a caravana secreta
}

void CaravanaSecreta::move(
    const std::string &direcao,
    std::shared_ptr<Mapa> &map,
    const std::vector<std::shared_ptr<Caravana> > &caravanas) {
    int linhaAnterior = getLinha();
    int colunaAnterior = getColuna();
    int novaLinha = linhaAnterior;
    int novaCol = colunaAnterior;

    // Movimento "secreto": direcao específica ou aleatória
    calcularNovaPosicao(direcao, novaLinha, novaCol);
    map->wrapPosition(novaLinha, novaCol);

    if (map->posicaoValida(novaLinha, novaCol)) {
        map->setCell(linhaAnterior, colunaAnterior, '.');
        setLinha(novaLinha);
        setColuna(novaCol);
        map->setCell(novaLinha, novaCol, getSimbolo());
        std::cout << "Caravana " << getId() << " movida na direcao " << direcao << ".\n";
    } else {
        std::cout << "\nCaravana " << getId() << " nao se pode mover na direcao "
                << direcao << " devido a obstaculos.\n";
    }
}
void CaravanaSecreta::consumirRecursos() {
    // Caravana secreta não consome recursos
}

void CaravanaSecreta::renderToBuffer(Buffer &buffer) const {
    buffer.moveCursor(getLinha(), getColuna());
    buffer << 'S';
}


// Caravana Barbara
char CaravanaBarbara::getSimbolo() const { return '!'; }

char CaravanaBarbara::getTipo() const { return 'B'; }

void CaravanaBarbara::move(
    const std::string & /*direcao*/,
    std::shared_ptr<Mapa> &map,
    const std::vector<std::shared_ptr<Caravana> > &caravanas) {
    int linhaAnterior = getLinha();
    int colunaAnterior = getColuna();

    // Verifica se o limite de movimentos no turno foi atingido
    if (getMovimentosNoTurno() >= 1) {
        std::cout << "\nCaravana Barbara " << getId() << " atingiu o limite de movimentos no turno.\n";
        return;
    }

    // Se a caravana não tem tripulação, mova aleatoriamente
    if (!terTripulacao()) {
        int direcaoAleatoria = rand() % 4;
        std::string direcao;
        switch (direcaoAleatoria) {
            case 0: direcao = "C";
                break; // Cima
            case 1: direcao = "B";
                break; // Baixo
            case 2: direcao = "E";
                break; // Esquerda
            case 3: direcao = "D";
                break; // Direita
        }

        int novaLinha = getLinha();
        int novaCol = getColuna();
        calcularNovaPosicao(direcao, novaLinha, novaCol); // Chama calcularNovaPosicao

        map->wrapPosition(novaLinha, novaCol);
        if (map->posicaoValida(novaLinha, novaCol)) {
            map->setCell(linhaAnterior, colunaAnterior, '.');
            setLinha(novaLinha);
            setColuna(novaCol);
            incrementarMovimentosNoTurno(); // Incrementa o contador de movimentos
            map->setCell(novaLinha, novaCol, getSimbolo());
            std::cout << "Caravana Barbara " << getId() << " movida aleatoriamente para nova posicao.\n";
        } else {
            std::cout << "Caravana Barbara " << getId() << " nao se pode mover devido a obstaculos.\n";
        }
        return;
    }

    // Caso a caravana tenha tripulação, mova-se em direção a uma caravana próxima
    std::shared_ptr<Caravana> target = nullptr;
    int menorDistancia = 9;

    for (const auto &caravana : caravanas) {
        if (caravana->getId() > 0 && caravana->getId() < 100) {
            // Caravanas de jogadores
            int distLinha = abs(caravana->getLinha() - getLinha());
            int distColuna = abs(caravana->getColuna() - getColuna());
            int distancia = distLinha + distColuna;
            if (distancia < menorDistancia && distancia <= 8) {
                menorDistancia = distancia;
                target = caravana;
            }
        }
    }

    if (target) {
        int novaLinha = getLinha();
        int novaCol = getColuna();
        // Decide para qual direção mover com base na maior diferença
        if (abs(target->getLinha() - getLinha()) > abs(target->getColuna() - getColuna())) {
            if (getLinha() < target->getLinha()) novaLinha++;
            else if (getLinha() > target->getLinha()) novaLinha--;
        } else {
            if (getColuna() < target->getColuna()) novaCol++;
            else if (getColuna() > target->getColuna()) novaCol--;
        }

        map->wrapPosition(novaLinha, novaCol);
        if (map->posicaoValida(novaLinha, novaCol)) {
            map->setCell(linhaAnterior, colunaAnterior, '.');
            setLinha(novaLinha);
            setColuna(novaCol);
            incrementarMovimentosNoTurno(); // Incrementa o contador de movimentos
            map->setCell(novaLinha, novaCol, getSimbolo());
            std::cout << "\nCaravana Barbara " << getId() << " movida em direcao a melhor posicao encontrada.\n";
        } else {
            std::cout << "\nCaravana Barbara " << getId() << " nao se pode mover devido a obstaculos.\n";
        }
    } else {
        // Movimento aleatório se não encontrar caravana próxima
        int direcaoAleatoria = rand() % 4;
        std::string direcao;
        switch (direcaoAleatoria) {
            case 0: direcao = "C";
                break; // Cima
            case 1: direcao = "B";
                break; // Baixo
            case 2: direcao = "E";
                break; // Esquerda
            case 3: direcao = "D";
                break; // Direita
        }

        int novaLinha = getLinha();
        int novaCol = getColuna();
        calcularNovaPosicao(direcao, novaLinha, novaCol); // Chama calcularNovaPosicao

        map->wrapPosition(novaLinha, novaCol);
        if (map->posicaoValida(novaLinha, novaCol)) {
            map->setCell(linhaAnterior, colunaAnterior, '.');
            setLinha(novaLinha);
            setColuna(novaCol);
            incrementarMovimentosNoTurno(); // Incrementa o contador de movimentos
            map->setCell(novaLinha, novaCol, getSimbolo());
            std::cout << "Caravana Barbara " << getId() << " movida aleatoriamente para nova posicao.\n";
        } else {
            std::cout << "Caravana Barbara " << getId() << " nao se pode mover devido a obstaculos.\n";
        }
    }
}

void CaravanaBarbara::renderToBuffer(Buffer &buffer) const {
    buffer.moveCursor(getLinha(), getColuna());
    buffer << 'B';
}

void CaravanaBarbara::consumirRecursos() {
    // Bárbaros nao consomem recursos
}


void Caravana::calcularNovaPosicao(const std::string &direcao,
                                   int &novaLinha,
                                   int &novaCol) {
    if (direcao == "C")
        novaLinha--; // Cima
    else if (direcao == "B")
        novaLinha++; // Baixo
    else if (direcao == "E")
        novaCol--; // Esquerda
    else if (direcao == "D")
        novaCol++; // Direita
    else if (direcao == "CE") {
        novaLinha--;
        novaCol--;
    } // Cima-Esquerda
    else if (direcao == "CD") {
        novaLinha--;
        novaCol++;
    } // Cima-Direita
    else if (direcao == "BE") {
        novaLinha++;
        novaCol--;
    } // Baixo-Esquerda
    else if (direcao == "BD") {
        novaLinha++;
        novaCol++;
    } // Baixo-Direita
}
