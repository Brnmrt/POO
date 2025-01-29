#ifndef PROJETOPOO2024_CARAVANA_H
#define PROJETOPOO2024_CARAVANA_H

#include <memory>
#include <string>
#include <vector>

#include "Simulador.h"

class Mapa;
class Buffer;
class Simulador;
// Classe base Caravana
class Caravana {
  private:
    int id;
    int linha, coluna;
    int maxTripulacao, maxMercadoria, maxAgua;
    int tripulacaoAtual, mercadoriaAtual, aguaAtual;
    int turnosSemTripulacao; // Turnos sem tripulacao
    int turnoCriacao;
    char tipo;
    bool autoGestao = false;
    int movimentosNoTurno = 0; // Contador de movimentos no turno atual

  protected:
    std::string
    ultimaDirecao; // Última direcao de movimento (para caravanas militares)
    std::shared_ptr<Simulador> simulador;
  public:
    Caravana(int id,
             int linha,
             int coluna,
             int maxTripulacao,
             int maxMercadoria,
             int maxAgua,
             std::shared_ptr<Simulador> simulador);
    virtual ~Caravana() = default;

    virtual void move(
      const std::string &direcao,
      std::shared_ptr<Mapa> &,
      const std::vector<std::shared_ptr<Caravana> > &caravanas) = 0;
    // Métodos para controlar o contador de movimentos
    void resetMovimentosNoTurno() {
      movimentosNoTurno = 0;
    }

    int getMovimentosNoTurno() const {
      return movimentosNoTurno;
    }

    void incrementarMovimentosNoTurno() {
      movimentosNoTurno++;
    }



    static void calcularNovaPosicao(const std::string &direcao,
                                    int &novaLinha,
                                    int &novaCol);
    virtual void renderToBuffer(Buffer &buffer) const = 0;
    virtual void consumirRecursos() = 0;
    void faltaAguaAtacaTripulacao();

    void setAutoGestao(bool estado) { autoGestao = estado; }
    [[nodiscard]] bool isAutoGestao() const { return autoGestao; }
    [[nodiscard]] std::shared_ptr<Simulador> getSimulador() const;
    void mudaCelula(std::shared_ptr<Mapa> &mapa);
    // Métodos para gestao de tripulacao
    [[nodiscard]] bool terTripulacao() const;
    void incrementaTurnosSemTripulacao();
    [[nodiscard]] int getTurnosSemTripulacao() const;

    // Getters
    [[nodiscard]] int getTurnoCriacao() const { return turnoCriacao; }
    [[nodiscard]] int getId() const { return id; }
    [[nodiscard]] int getLinha() const { return linha; }
    [[nodiscard]] int getColuna() const { return coluna; }
    [[nodiscard]] int getTripulacao() const { return tripulacaoAtual; }
    [[nodiscard]] int getMercadoria() const { return mercadoriaAtual; }
    [[nodiscard]] int getAgua() const { return aguaAtual; }
    [[nodiscard]] int getMaxTripulacao() const { return maxTripulacao; }
    [[nodiscard]] int getMaxMercadoria() const { return maxMercadoria; }
    [[nodiscard]] int getMaxAgua() const { return maxAgua; }
    [[nodiscard]] virtual char getTipo() const { return tipo; }
    [[nodiscard]] virtual char getSimbolo() const = 0;

    // Setters
    void setId(int _id) { id = _id; };
    void setTripulacao(int tripulacao) { tripulacaoAtual = tripulacao; }
    void setMercadoria(int carga) { mercadoriaAtual = carga; }
    void setAgua(int agua) { aguaAtual = agua; }
    void setLinha(int _linha) { linha = _linha; }
    void setColuna(int c) { coluna = c; }
};

class CaravanaComercial : public Caravana {
  public:
    CaravanaComercial(int id,
                      int linha,
                      int coluna,
                      std::shared_ptr<Simulador> simulador)
      : Caravana(id, linha, coluna, 20, 40, 200, std::move(simulador)) {
    }

    void move(const std::string &direcao,
              std::shared_ptr<Mapa> &mapa,
              const std::vector<std::shared_ptr<Caravana> > &caravanas) override;
    void renderToBuffer(Buffer &buffer) const override;
    void consumirRecursos() override;


    [[nodiscard]] char getSimbolo() const override;
    [[nodiscard]] char getTipo() const override;
};

class CaravanaMilitar : public Caravana {
  public:
    CaravanaMilitar(int id,
                    int linha,
                    int coluna,
                    std::shared_ptr<Simulador> simulador)
      : Caravana(id, linha, coluna, 40, 5, 400, std::move(simulador)) {
    }

    void move(const std::string &direcao,
              std::shared_ptr<Mapa> &map,
              const std::vector<std::shared_ptr<Caravana> > &caravanas) override;
    void renderToBuffer(Buffer &buffer) const override;
    void consumirRecursos() override;

    [[nodiscard]] char getSimbolo() const override;
    [[nodiscard]] char getTipo() const override;
};

class CaravanaSecreta : public Caravana {
  public:
    CaravanaSecreta(int id,
                    int linha,
                    int coluna,
                    std::shared_ptr<Simulador> simulador)
      : Caravana(id, linha, coluna, 10, 10, 100, std::move(simulador)) {
    }

    void move(const std::string &direcao,
              std::shared_ptr<Mapa> &map,
              const std::vector<std::shared_ptr<Caravana> > &caravanas) override;
    void renderToBuffer(Buffer &buffer) const override;
    void consumirRecursos() override;



    [[nodiscard]] char getSimbolo() const override;
    [[nodiscard]] char getTipo() const override;
};

class CaravanaBarbara : public Caravana {
  public:
    CaravanaBarbara(int id,
                    int linha,
                    int coluna,
                    std::shared_ptr<Simulador> simulador)
      : Caravana(id, linha, coluna, 40, 25, 100, std::move(simulador)) {
      setTripulacao(40);
      setAutoGestao(true); // Barbara esta sempre em auto-gestao
    }
    void move(
      const std::string &direcao,
      std::shared_ptr<Mapa> &map,
      const std::vector<std::shared_ptr<Caravana> > &caravanas) override;

    void renderToBuffer(Buffer &buffer) const override;
    void consumirRecursos() override;


    [[nodiscard]] char getSimbolo() const override;
    [[nodiscard]] char getTipo() const override;
};
#endif  // PROJETOPOO2024_CARAVANA_H
