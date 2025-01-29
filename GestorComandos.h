#ifndef PROJETOPOO2024_GESTORCOMANDOS_H
#define PROJETOPOO2024_GESTORCOMANDOS_H

#include "Comando.h"

class GestorComandos : public std::enable_shared_from_this<GestorComandos> {
  private:
    std::vector<std::shared_ptr<Comando> > comandos; // Vetor de comandos
    std::shared_ptr<Simulador> simulador; // Ponteiro para o simulador
    std::string nomeFicheiro;

  public:
    // Construtor
    GestorComandos(); // Construtor vazio
    GestorComandos(const std::shared_ptr<Simulador> &sim);
    bool validarFase(const std::string &input) const;
    void executarComandos(const std::string &nomeFicheiro) const;
    void setNomeFicheiro(const std::string &_nomeFicheiro) {
      nomeFicheiro = _nomeFicheiro;
    };

    std::shared_ptr<Simulador> getSimulador() const;
    const std::vector<std::shared_ptr<Comando> > &getComandos() const;

    // Método para execução de comandos
    void executar();
};

#endif //PROJETOPOO2024_GESTORCOMANDOS_H
