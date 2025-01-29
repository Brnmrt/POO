#include "GestorComandos.h"
// Classe GestorComandos
//construtor sem argumentos
#include <fstream>

#include "Comando.h"
// Construtor com argumentos
GestorComandos::GestorComandos(const std::shared_ptr<Simulador> &sim) : simulador(sim) {
    // Inicializa o vetor com todas as classes derivadas

    comandos.emplace_back(std::make_shared<ComandoConfig>(simulador));
    comandos.emplace_back(std::make_shared<ComandoSair>(simulador));
    comandos.emplace_back(std::make_shared<ComandoExec>(simulador));
    comandos.emplace_back(std::make_shared<ComandoProx>(simulador));
    comandos.emplace_back(std::make_shared<ComandoCompraC>(simulador));
    comandos.emplace_back(std::make_shared<ComandoPrecos>(simulador));
    comandos.emplace_back(std::make_shared<ComandoCidade>('a', simulador));
    comandos.emplace_back(std::make_shared<ComandoCaravana>(simulador));
    comandos.emplace_back(std::make_shared<ComandoCompra>(simulador));
    comandos.emplace_back(std::make_shared<ComandoVende>(simulador));
    comandos.emplace_back(std::make_shared<ComandoMove>(simulador));
    comandos.emplace_back(std::make_shared<ComandoAuto>(simulador));
    comandos.emplace_back(std::make_shared<ComandoStop>(simulador));
    comandos.emplace_back(std::make_shared<ComandoBarbaro>(simulador));
    comandos.emplace_back(std::make_shared<ComandoAreia>(simulador));
    comandos.emplace_back(std::make_shared<ComandoMoedas>(simulador));
    comandos.emplace_back(std::make_shared<ComandoTripul>(simulador));
    comandos.emplace_back(std::make_shared<ComandoSaves>(simulador));
    comandos.emplace_back(std::make_shared<ComandoLoads>(simulador));
    comandos.emplace_back(std::make_shared<ComandoLists>(simulador));
    comandos.emplace_back(std::make_shared<ComandoDels>(simulador));
    comandos.emplace_back(std::make_shared<ComandoTerminar>(simulador));
    comandos.emplace_back(std::make_shared<ComandoListaCaravanas>(simulador));
    comandos.emplace_back(std::make_shared<ComandoSecretaDestroi>(simulador));
    comandos.emplace_back(std::make_shared<ComandoSecretaMercadoria>(simulador));
}

// Getter para o simulador
std::shared_ptr<Simulador> GestorComandos::getSimulador() const {
    return simulador;
}

// Getter para os comandos
const std::vector<std::shared_ptr<Comando> > &GestorComandos::getComandos() const {
    return comandos;
}

// Método para execução de comandos
void GestorComandos::executar() {
    std::string input;
    std::cout << "\nInicio da simulacao" << std::endl;
    while (true) {
        std::cout << "\nIntroduza um comando: " << std::flush;;
        std::getline(std::cin, input);

        // Valida o comando antes de seguir
        if (!validarFase(input)) {
            // Exibe o prompt novamente após erro
            continue;
        }

        bool comandoEncontrado = false;
        for (const auto &comando : comandos) {
            if (comando->validar(input)) {
                comandoEncontrado = true;
                if (input.find("exec") == 0) {
                    comandoEncontrado = true;
                    nomeFicheiro = input.substr(5);
                    executarComandos(nomeFicheiro);
                    continue; // Salta para o próximo ciclo de comandos sem exibir "comando inválido"
                }

                comando->executar();
                //tratamento excecao para o comando nao encontrado

                if (input.find("config") == 0) {
                    simulador->setFase1(false);
                }
                if (input == "terminar") {
                    simulador->setFase1(true);
                }
                break;
            }
        }
        if (!comandoEncontrado) {
            std::cout << "Erro: Comando invalido. Tente novamente.\n";
        }
    }
}

bool GestorComandos::validarFase(const std::string &input) const {
    auto config = std::make_shared<ComandoConfig>(simulador);
    auto sair = std::make_shared<ComandoSair>(simulador);

    if (simulador->estaEmFase1() && !config->validar(input) && !sair->validar(input)) {
        std::cout << "Erro: Apenas 'config' e 'sair' sao permitidos na fase 1.\n";
        return false;
    }
    return true;
}

void GestorComandos::executarComandos(const std::string &nomeFicheiro) const {
    std::cout << "A executar comandos do ficheiro: " << nomeFicheiro << std::endl;

    // Abrir o ficheiro
    std::ifstream ficheiro(nomeFicheiro);
    if (!ficheiro.is_open()) {
        std::cout << "Erro: Nao foi possível abrir o ficheiro " << nomeFicheiro << std::endl;
        return;
    }

    // Vetor para armazenar as linhas do ficheiro
    std::vector<std::string> linhas;
    std::string linha;

    // Lê todas as linhas para o vetor
    while (std::getline(ficheiro, linha)) {
        linhas.push_back(linha);
    }

    // Fecha o ficheiro após a leitura completa
    ficheiro.close();

    // Processa cada comando armazenado no vetor
    for (const auto &input : linhas) {
        bool comandoValido = false;

        // Valida e executa cada comando
        for (const auto &c : comandos) {
            if (c->validar(input)) {
                comandoValido = true;
                c->executar();
                break;
            }
        }
    }
}

