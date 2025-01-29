#ifndef PROJETOPOO2024_MAPA_H
#define PROJETOPOO2024_MAPA_H

#include <memory>
#include <vector>

#include "Buffer.h"
#include "Caravana.h"
#include "Item.h"

class Caravana;
class Item;

class Mapa : public std::enable_shared_from_this<Mapa> {
        private:
                int linhas, colunas; // Dimensões do mapa
                std::unique_ptr<char[]> grid;
                std::vector<std::shared_ptr<Item> > items;

                std::shared_ptr<Item> sortearItemAleatorio(
                        int linha,
                        int coluna) const; // Sorteia um tipo de item aleatório

        public:
                Mapa();
                ~Mapa();

                bool loadFromFile(
                        const std::string &filename); // Carrega o mapa de um ficheiro

                void renderToBuffer(
                        Buffer &buffer,
                        const std::vector<std::shared_ptr<Caravana> > &caravanas) const;

                int getLinhas() const; // Retorna o numero de linhas
                int getcolunas() const; // Retorna o numero de colunas
                char getCell(int linha,
                             int coluna) const; // Retorna o conteúdo de uma celula
                void setCell(int linha, int coluna, char simbolo);

                bool posicaoValida(int linha,
                                   int coluna) const; // Verifica se uma posicao e válida
                void wrapPosition(int &linha,
                                  int &coluna)
                const; // Implementa o comportamento esférico do mapa

                bool encontrarCidade(char simbolo, int &linha, int &coluna) const;


                void colocarItem(std::shared_ptr<Item> item); // Adiciona um item ao mapa
                void adicionarItemAleatorio(); // Adiciona itens aleatórios ao mapa
                void removerItensExpirados(); // Remove itens cuja duração expirou
                void renderItemsToBuffer(Buffer &buffer) const; // Renderiza itens no buffer
                void removerItem(std::shared_ptr<Item> item); // Remove um item do mapa
                void clear(); // Limpa o mapa e remove todos os itens
                const std::vector<std::shared_ptr<Item> > &getItens() const { return items; } ;

                std::shared_ptr<Item> getItemAt(int linha, int coluna) const;

                void reconstruirMapa(const Buffer &buffer);
                void limparTempestadeAreia();
};

#endif  // PROJETOPOO2024_MAPA_H
