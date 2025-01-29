#include "Item.h"
#include <iostream>
#include "Simulador.h"

Item::Item(int x, int y, char simbolo, int duracao)
    : Pixel(x, y, simbolo), duracao(duracao) {
}

int Item::getDuracao() const { return duracao; }

void Item::reduzirDuracao() { --duracao; }

CaixaPandora::CaixaPandora(int x, int y, int duracao)
    : Item(x, y, 'P', duracao) {
}

void CaixaPandora::aplicarEfeito(std::shared_ptr<Caravana> &caravana) {
    int perda = static_cast<int>(caravana->getTripulacao() *
        0.2); // Perde 20% da tripulacao
    caravana->setTripulacao(std::max(0, caravana->getTripulacao() - perda));
    std::cout << "Caixa de Pandora: A caravana perdeu " << perda
            << " tripulantes.\n";
}

// Implementação da ArcaTesouro
ArcaTesouro::ArcaTesouro(int x, int y, int duracao)
    : Item(x, y, '$', duracao) {
}

void ArcaTesouro::aplicarEfeito(std::shared_ptr<Caravana> &caravana) {
    std::shared_ptr<Simulador> simulador =
            caravana->getSimulador();
    if (simulador) {
        int bonus = static_cast<int>(simulador->getMoedas() *
            0.1); // Ganha 10% a mais de moedas
        simulador->setMoedas(simulador->getMoedas() + bonus);
        std::cout << "Arca do Tesouro: O jogador ganhou " << bonus << " moedas.\n";
    }
}

Jaula::Jaula(int x, int y, int duracao) : Item(x, y, 'J', duracao) {
}

void Jaula::aplicarEfeito(std::shared_ptr<Caravana> &caravana) {
    int novosTripulantes = std::min(5, caravana->getMaxTripulacao() - caravana->getTripulacao());
    if (novosTripulantes > 0) {
        caravana->setTripulacao(caravana->getTripulacao() + novosTripulantes);
        std::cout << "Jaula: " << novosTripulantes << " prisioneiros juntaram-se a caravana.\n";
    } else {
        std::cout << "Jaula: A caravana já tem a tripulacao maxima.\n";
    }
}

Mina::Mina(int x, int y, int duracao) : Item(x, y, '*', duracao) {
}

void Mina::aplicarEfeito(std::shared_ptr<Caravana> &caravana) {
    std::shared_ptr<Simulador> simulador = caravana->getSimulador();
    if (simulador) {
        simulador->removeCaravanaDoVetorEMapa(caravana->getId());
    }
    std::cout << "Mina: A caravana foi destruida.\n";
}
Surpresa::Surpresa(int x, int y, int duracao) : Item(x, y, '?', duracao) {
}

void Surpresa::aplicarEfeito(std::shared_ptr<Caravana> &caravana) {
    // Selecionar aleatoriamente o efeito de outro item
    int tipoEfeito = rand() % 4; // 4 tipos de itens existentes

    std::cout << "Surpresa: Efeito aleatório aplicado!\n";

    switch (tipoEfeito) {
        case 0:
            std::cout << "Surpresa: Calhou uma Caixa de Pandora!\n"; {
                auto caixaPandora = std::make_shared<CaixaPandora>(getX(), getY(), getDuracao());
                caixaPandora->aplicarEfeito(caravana); // Aplica diretamente o efeito
            }
            break;
        case 1:
            std::cout << "Surpresa: Calhou uma Arca do Tesouro!\n"; {
                auto arcaTesouro = std::make_shared<ArcaTesouro>(getX(), getY(), getDuracao());
                arcaTesouro->aplicarEfeito(caravana); // Aplica diretamente o efeito
            }
            break;
        case 2:
            std::cout << "Surpresa: Calhou uma Jaula!\n"; {
                auto jaula = std::make_shared<Jaula>(getX(), getY(), getDuracao());
                jaula->aplicarEfeito(caravana); // Aplica diretamente o efeito
            }
            break;
        case 3:
            std::cout << "Surpresa: Calhou uma Mina!\n"; {
                auto mina = std::make_shared<Mina>(getX(), getY(), getDuracao());
                mina->aplicarEfeito(caravana); // Aplica diretamente o efeito
            }
            break;
    }
}

char Item::getSimbolo() const {
    return c; // Retorna o simbolo associado
}
