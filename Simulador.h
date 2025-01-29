#ifndef PROJETOPOO2024_SIMULADOR_H
#define PROJETOPOO2024_SIMULADOR_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Caravana.h"
#include "Item.h"
#include "Mapa.h"

// ---------- Forward declarations -----------
class Mapa;
class Caravana;
class Item;
class Buffer;
// --------------------------------------------

class Simulador : public std::enable_shared_from_this<Simulador> {
    private:
        bool emFase1;

        int moedas; // Moedas do jogador
        int turnos; // Número de turnos passados
        int turnoAtiva; // turno em que a caravana secreta e ativada
        int combatesVencidos; // Número de combates vencidos
        int proximoIdNormal; // IDs para caravanas normais (C, M) a partir de 0
        int proximoIdBarbaro; // IDs para bárbaros a partir de 100
        int proximoIdSecreta; // IDs para caravanas secretas a partir de 20

        std::map<int, bool> idsNormaisDisponiveis; // IDs normais disponíveis para reutilização
        std::map<int, bool> idsBarbarosDisponiveis; // IDs bárbaros disponíveis para reutilização

        // Configurações personalizáveis
        int instantesEntreNovosItens; // Intervalo entre o surgimento de novos itens
        int duracaoItem; // Duração de cada item no mapa
        int maxItens; // Máximo de itens simultâneos
        int precoVendaMercadoria; // Preco por tonelada de venda de mercadorias
        int precoCompraMercadoria; // Preco por tonelada de compra de mercadorias
        int precoCaravana; // Preco para comprar uma caravana
        int precoCaravanaSecreta;
        int instantesEntreNovosBarbaros; // Intervalo para surgimento de novas
        // caravanas bárbaras
        int duracaoBarbaros; // Duração de vida das caravanas bárbaras

        std::shared_ptr<Mapa> mapa;
        std::vector<std::shared_ptr<Caravana> > caravanas;
        std::vector<std::shared_ptr<Item> > itens; // Alteração aqui
        std::map<std::string, std::shared_ptr<Buffer> > savedBuffers;

    public:
        Simulador();
        ~Simulador();

        void setFase1(bool estado);
        bool estaEmFase1() const;
        // Métodos de acesso
        int getMoedas() const;
        void setMoedas(int valor);
        std::shared_ptr<Mapa> &getMapa(); // Getter para o mapa
        std::vector<std::shared_ptr<Caravana> > &getCaravanas();
        const std::map<std::string, std::shared_ptr<Buffer> > &getSavedBuffers() const {
            return savedBuffers;
        }

        void saveBuffer(const std::string &nome, const Buffer &buffer);
        void loadBuffer(const std::string &nome) const;
        void deleteBuffer(const std::string &nome);

        //Carregamento do jogo
        void carregarMapa(const std::string &ficheiro);
        void inicializarCaravanasDoMapa();
        void inicializarItens(); // Inicializa os itens iniciais no mapa
        int obterProximoId(std::map<int, bool> &idsDisponiveis, int &proximoId, int limiteInferior, int limiteSuperior);
        int obterProximoIdNormal();
        int obterProximoIdBarbaro();
        int obterProximoIdSecreta();

        // Funções do Simulador que alteram o estado do jogo
        void adicionarCaravanaBarbara(int linha, int coluna);
        void adicionarCaravana(char tipo, char cidade);
        void verificarEfeitosItensAdjacentes(std::shared_ptr<Caravana> &caravana) const;
        void avancarTurno(int n);
        void removerBarbarosAntigos();
        void removeCaravanaDoVetorEMapa(int id);
        void mostrarConteudoCidade(char simboloCidade) const;

        int getTurnos() const { return turnos; }

        void executarCombate(Caravana &c1, Caravana &c2); // Resolve um combate entre duas caravanas
        int calcularForcaCombate(const Caravana &caravana);
        void resolverVitoria(Caravana &vencedora, Caravana &derrotada);
        void transferirRecursos(Caravana &origem, Caravana &destino);
        void verificarDestruicao(Caravana &caravana, Caravana &vencedora);
        void condicoesCombate(); // Verifica se ha condições para o combate e executa-o

        void mostrarEstado() const;
        void gerarEventos(); // Gera eventos aleatórios como tempestades e bárbaros
        void mostrarPontuacaoFinal() const; // Exibe pontuação final ao terminar
        void exibirPrecos() const; // Exibe os precos das caravanas e mercadorias

        void executarComportamentosAutomaticos(); // Movimentos automáticos
        void executarAutoGestaoComercial(const std::shared_ptr<Caravana> &caravana);
        void executarAutoGestaoMilitar(const std::shared_ptr<Caravana> &caravana);
        void executarAutoGestaoSecreta(const std::shared_ptr<Caravana> &caravana);
        void executarAutoGestaoCaravana(const std::shared_ptr<Caravana> &caravana);

        void descreveCaravana(int id) const;
        std::shared_ptr<Caravana> obterCaravanaPorId(int id) const;
        bool caravanaEstaEmCidade(int id) const;
        void comprarMercadorias(int id, int quantidade);
        void adicionarMoedas(int valor);
        void moverCaravana(int id, const std::string &direcao);
        void removerCaravanasSemTripulacao();
        void processarComando(const std::string &comando);
        void reset();

        // métodos para as caravanas secretas
        void secretaAutoDestroi(int id1, int id2);
        void secretaTransportaMercadoria(int id, char destino);

        void moverCaravanaParaCidade(const std::shared_ptr<Caravana> &caravana, int linhaCidade, int colunaCidade);
        void criarTempestadeAreia(int l, int c, int _linha);
        void aplicarEfeitoTempestade();
};

#endif  // SIMULADOR_H
