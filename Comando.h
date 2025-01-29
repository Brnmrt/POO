#ifndef PROJETOPOO2024_COMANDO_H
#define PROJETOPOO2024_COMANDO_H

#include <string>
#include <vector>
#include <memory>

#include "Simulador.h"

class Simulador;

// Classe base
class Comando {
    public:
        explicit Comando(std::shared_ptr<Simulador> simul): simulador(move(simul)) {
        };
        virtual ~Comando() = default;
        virtual bool validar(const std::string &input) = 0;
        virtual void executar() = 0;

    protected:
        std::shared_ptr<Simulador> simulador; // Ponteiro inteligente para o simulador
};

// Classes derivadas para os comandos
// Comando 1: config <ficheiro>
class ComandoConfig : public Comando, std::enable_shared_from_this<ComandoConfig> {
    std::string nomeFicheiro;

    public:
        // Construtor que aceita o simulador e o passa para a base
        explicit ComandoConfig(std::shared_ptr<Simulador> simulador) : Comando(move(simulador)) {
        }
        bool validar(const std::string &input) override;
        void executar() override;
};

// Comando 2: sair
class ComandoSair : public Comando, std::enable_shared_from_this<ComandoConfig> {
    public:
        // Construtor que aceita o simulador e o passa para a base
        explicit ComandoSair(std::shared_ptr<Simulador> simulador) : Comando(move(simulador)) {
        }
        bool validar(const std::string &input) override;
        void executar() override;
};

// Comando 3: exec <ficheiro>
class ComandoExec : public Comando {
    std::string nomeFicheiro;
    std::shared_ptr<Simulador> simulador;

    public:
        // Construtor que aceita o simulador e o passa para a base
        explicit ComandoExec(std::shared_ptr<Simulador> simulador) : Comando(move(simulador)) {
        }
        bool validar(const std::string &input) override;
        void executar() override;
};

// Comando 4: prox [<n>] (default: 1)
class ComandoProx : public Comando {
    int n = 1; // Número de turnos a avançar, valor padrão e 1
    public:
        // Construtor que aceita o simulador e o passa para a base
        explicit ComandoProx(std::shared_ptr<Simulador> simulador) : Comando(move(simulador)) {
        }

        // Implementação de métodos virtuais
        bool validar(const std::string &input) override;
        void executar() override;

        // Setter para o numero de turnos (opcional para uso direto)
        void setTurnos(int turnos) { n = turnos; }
};

// Comando 5: comprac <cidade> <tipo>
class ComandoCompraC : public Comando {
    char cidade; // Letra da cidade onde será comprada a caravana
    char tipo; // Tipo de caravana ('C', 'M', 'S')
    public:
        // Construtor que aceita o simulador e o passa para a base
        explicit ComandoCompraC(std::shared_ptr<Simulador> simulador) : Comando(move(simulador)) {
        }

        // Implementação de métodos virtuais
        bool validar(const std::string &input) override;
        void executar() override;

        // Setters para configurar os valores (opcional)
        void setCidade(char c) { cidade = c; }
        void setTipo(char t) { tipo = t; }
};

// Comando 6: precos
class ComandoPrecos : public Comando {
    public:
        explicit ComandoPrecos(std::shared_ptr<Simulador> simul) : Comando(move(simul)) {
        }
        bool validar(const std::string &input) override;
        void executar() override;
};

// Comando 7: cidade <cidade>
class ComandoCidade : public Comando {
    private:
        char cidade; // Símbolo da cidade

    public:
        // Construtor
        ComandoCidade(char c, std::shared_ptr<Simulador> simul) : Comando(move(simul)), cidade(c) {
        }

        // Implementação dos métodos virtuais
        bool validar(const std::string &input) override;
        void executar() override;
};

// Comando 8: caravana <tipo>
class ComandoCaravana : public Comando {
    int id;

    public:
        explicit ComandoCaravana(std::shared_ptr<Simulador> simul) : Comando(move(simul)) {
        }
        bool validar(const std::string &input) override;
        void executar() override;
};

// Comando 9: compra <caravanaId> <quantidade>
class ComandoCompra : public Comando {
    int caravanaId, quantidade;

    public:
        explicit ComandoCompra(std::shared_ptr<Simulador> simul) : Comando(move(simul)) {
        }
        bool validar(const std::string &input) override;
        void executar() override;
};

// Comando 10: vende <caravanaId>
class ComandoVende : public Comando {
    int caravanaId;

    public:
        explicit ComandoVende(std::shared_ptr<Simulador> simul) : Comando(move(simul)) {
        }
        bool validar(const std::string &input) override;
        void executar() override;
};

// Comando 11: move <numero> <direcao>
class ComandoMove : public Comando {
    int caravanaId;
    std::string direcao;

    public:
        explicit ComandoMove(std::shared_ptr<Simulador> simul) : Comando(move(simul)) {
        }
        bool validar(const std::string &input) override;
        void executar() override;
};

// Comando 12: auto <numero>
class ComandoAuto : public Comando {
    int caravanaId;

    public:
        explicit ComandoAuto(std::shared_ptr<Simulador> simul) : Comando(move(simul)) {
        }
        bool validar(const std::string &input) override;
        void executar() override;
};

// Comando 13: stop <numero>
class ComandoStop : public Comando {
    int caravanaId;

    public:
        explicit ComandoStop(std::shared_ptr<Simulador> simul) : Comando(move(simul)) {
        }
        bool validar(const std::string &input) override;
        void executar() override;
};

// Comando 14: barbaro <l> <c>
class ComandoBarbaro : public Comando {
    int linha, coluna;

    public:
        explicit ComandoBarbaro(std::shared_ptr<Simulador> simul) : Comando(move(simul)) {
        }
        bool validar(const std::string &input) override;
        void executar() override;
};

// Comando 15: areia <l> <c> <raio>
class ComandoAreia : public Comando {
    int linha, coluna, raio;

    public:
        explicit ComandoAreia(std::shared_ptr<Simulador> simul) : Comando(move(simul)) {
        }
        bool validar(const std::string &input) override;
        void executar() override;
};

// Comando 16: moedas <quantidade>
class ComandoMoedas : public Comando {
    int quantidade;

    public:
        explicit ComandoMoedas(std::shared_ptr<Simulador> simul) : Comando(move(simul)) {
        }
        bool validar(const std::string &input) override;
        void executar() override;
};

// Comando 17: tripul <numero> <tripulantes>
class ComandoTripul : public Comando {
    int numero, tripulantes;

    public:
        explicit ComandoTripul(std::shared_ptr<Simulador> simul) : Comando(move(simul)) {
        }
        bool validar(const std::string &input) override;
        void executar() override;
};

// Comando 18: saves <nome>
class ComandoSaves : public Comando {
    std::string nome;

    public:
        explicit ComandoSaves(std::shared_ptr<Simulador> simul) : Comando(move(simul)) {
        }
        bool validar(const std::string &input) override;
        void executar() override;
};

// Comando 19: load <nome>
class ComandoLoads : public Comando {
    std::string nome;

    public:
        explicit ComandoLoads(std::shared_ptr<Simulador> simul) : Comando(move(simul)) {
        }
        bool validar(const std::string &input) override;
        void executar() override;
};

// Comando 20: lists
class ComandoLists : public Comando {
    public:
        explicit ComandoLists(std::shared_ptr<Simulador> simul) : Comando(move(simul)) {
        }
        bool validar(const std::string &input) override;
        void executar() override;
};

// Comando 21: dels <nome>
class ComandoDels : public Comando {
    std::string nome;

    public:
        explicit ComandoDels(std::shared_ptr<Simulador> simul) : Comando(move(simul)) {
        }
        bool validar(const std::string &input) override;
        void executar() override;
};

// Comando 22: terminar
class ComandoTerminar : public Comando {
    public:
        explicit ComandoTerminar(std::shared_ptr<Simulador> simul) : Comando(move(simul)) {
        }
        bool validar(const std::string &input) override;
        void executar() override;
};

// Comando 23: listacaravanas
class ComandoListaCaravanas : public Comando {
    public:
        explicit ComandoListaCaravanas(std::shared_ptr<Simulador> simul) : Comando(move(simul)) {
        }
        bool validar(const std::string &input) override;
        void executar() override;
};
// Comando 24: secreta <id1> destroi <id2>
class ComandoSecretaDestroi : public Comando {
    int idSecreta;
    int idAlvo;

    public:
        explicit ComandoSecretaDestroi(std::shared_ptr<Simulador> simulador)
            : Comando(std::move(simulador)) {
        }

        bool validar(const std::string &input) override;
        void executar() override;
};

// Comandos 25: secreta <id> mercadoria <c>
// Declaração em Comandos.h
class ComandoSecretaMercadoria : public Comando {
    int idSecreta;
    char destino;

    public:
        explicit ComandoSecretaMercadoria(std::shared_ptr<Simulador> simulador)
            : Comando(std::move(simulador)) {
        }

        bool validar(const std::string &input) override;
        void executar() override;
};

#endif //PROJETOPOO2024_COMANDO_H
