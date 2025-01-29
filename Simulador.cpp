#include "Simulador.h"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <set>

Simulador::Simulador()
    : emFase1(true),
      moedas(500),
      turnos(1), combatesVencidos(0),
      proximoIdNormal(0),
      proximoIdBarbaro(100),
      proximoIdSecreta(20),
      instantesEntreNovosItens(10),
      duracaoItem(10),
      maxItens(5),
      precoVendaMercadoria(2),
      precoCompraMercadoria(1),
      precoCaravana(100),
      precoCaravanaSecreta(300),
      instantesEntreNovosBarbaros(40),
      duracaoBarbaros(60),
      mapa(std::make_shared<Mapa>()) {
}

Simulador::~Simulador() {
    // Liberta automaticamente os recursos geridos por smart pointers
    // Nao e necessário código extra, pois smart pointers cuidam disso
}

int Simulador::getMoedas() const { return moedas; }

void Simulador::setMoedas(int valor) { moedas = valor; }

std::shared_ptr<Mapa> &Simulador::getMapa() { return mapa; }

std::vector<std::shared_ptr<Caravana> > &Simulador::getCaravanas() { return caravanas; }

void Simulador::setFase1(bool estado) {
    emFase1 = estado;
}

bool Simulador::estaEmFase1() const {
    return emFase1;
}

void Simulador::saveBuffer(const std::string &nome, const Buffer &buffer) {
    try {
        if (savedBuffers.contains(nome)) {
            std::cout << "Aviso: Ja existe um buffer com o nome '" << nome << "'.\n";
            std::cout << "Deseja substituir o ficheiro antigo? (s/n): ";

            char resposta;
            std::cin >> resposta;
            std::cin.ignore(); // Limpa o '\n' do buffer de entrada

            if (resposta != 's' && resposta != 'S') {
                std::cout << "Operacao cancelada. O buffer nao foi guardado.\n";
                return;
            }
        }

        auto savedBuffer = std::make_shared<Buffer>(buffer);
        savedBuffers[nome] = move(savedBuffer);
        std::cout << "Buffer guardado com o nome: " << nome << "\n";
    } catch (const std::exception &e) {
        std::cout << "Erro ao guardar o buffer: " << e.what() << "\n";
    }
}

void Simulador::loadBuffer(const std::string &nome) const {
    auto it = savedBuffers.find(nome);
    if (it != savedBuffers.end()) {
        std::cout << "Mapa antigo do buffer guardado (" << nome << "):\n";
        std::cout << *(it->second); // Usa o operador `<<` do Buffer
    } else {
        std::cout << "Erro: Buffer com o nome \"" << nome << "\" nao encontrado.\n";
    }
}

void Simulador::deleteBuffer(const std::string &nome) {
    if (savedBuffers.erase(nome)) {
        std::cout << "Buffer \"" << nome << "\" apagado com sucesso.\n";
    } else {
        std::cout << "Erro: Buffer com o nome \"" << nome << "\" nao encontrado.\n";
    }
}

void Simulador::carregarMapa(const std::string &ficheiro) {
    std::cout << "Iniciando carregamento do mapa a partir do ficheiro: " << ficheiro << "\n";

    if (!mapa->loadFromFile(ficheiro)) {
        throw std::runtime_error("Erro ao carregar o mapa.");
    }

    std::ifstream file(ficheiro);
    if (!file.is_open()) {
        throw std::runtime_error("Erro ao abrir o ficheiro de configuracao.");
    }

    //std::cout << "Ficheiro aberto com sucesso.\n";

    std::string linha;
    getline(file, linha);
    std::cout << "Linha 1 (linhas): " << linha << "\n";
    getline(file, linha);
    std::cout << "Linha 2 (colunas): " << linha << "\n";

    // Percorrer o mapa linha a linha
    for (int i = 0; i < mapa->getLinhas(); ++i) {
        getline(file, linha);
        //std::cout << "Linha do mapa " << i << ": " << linha << "\n";
        for (int j = 0; j < linha.size(); ++j) {
            char simbolo = linha[j];
            switch (simbolo) {
                case '*':
                    mapa->colocarItem(std::make_shared<Mina>(i, j, duracaoItem));
                    std::cout << "Mina criada em (" << i << ", " << j << ").\n";
                    break;
                case '$':
                    mapa->colocarItem(std::make_shared<ArcaTesouro>(i, j, duracaoItem));
                    std::cout << "ArcaTesouro criada em (" << i << ", " << j << ").\n";
                    break;
                case 'J':
                    mapa->colocarItem(std::make_shared<Jaula>(i, j, duracaoItem));
                    std::cout << "Jaula criada em (" << i << ", " << j << ").\n";
                    break; // Adicionado o break
                case 'P':
                    mapa->colocarItem(std::make_shared<CaixaPandora>(i, j, duracaoItem));
                    std::cout << "Caixa de Pandora criada em (" << i << ", " << j << ").\n";
                    break;
                case '?':
                    mapa->colocarItem(std::make_shared<Surpresa>(i, j, duracaoItem));
                    std::cout << "Surpresa criada em (" << i << ", " << j << ").\n";
                    break;
                default:
                    break;
            }
        }
    }

    std::cout << "Mapa carregado. Iniciando leitura da configuracao.\n";

    // Continuar a carregar a configuração
    while (getline(file, linha)) {
        //std::cout << "Linha lida: " << linha << "\n";
        std::istringstream iss(linha);
        std::string chave;
        int valor;
        if (iss >> chave >> valor) {
            //std::cout << "Chave: " << chave << ", Valor: " << valor << "\n";
            if (chave == "moedas") {
                moedas = valor;
                //std::cout << "Moedas carregadas: " << moedas << "\n";
            } else if (chave == "instantes_entre_novos_itens") {
                instantesEntreNovosItens = valor;
                //std::cout << "Instantes entre novos itens carregados: " << instantesEntreNovosItens << "\n";
            } else if (chave == "duração_item") {
                duracaoItem = valor;
                //std::cout << "Duracao do item carregada: " << duracaoItem << "\n";
            } else if (chave == "max_itens") {
                maxItens = valor;
                //std::cout << "Maximo de itens carregado: " << maxItens << "\n";
            } else if (chave == "preço_venda_mercadoria") {
                precoVendaMercadoria = valor;
                //std::cout << "Preco de venda da mercadoria carregado: " << precoVendaMercadoria << "\n";
            } else if (chave == "preço_compra_mercadoria") {
                precoCompraMercadoria = valor;
                //std::cout << "Preco de compra da mercadoria carregado: " << precoCompraMercadoria << "\n";
            } else if (chave == "preço_caravana") {
                precoCaravana = valor;
                //std::cout << "Preco da caravana carregado: " << precoCaravana << "\n";
            } else if (chave == "instantes_entre_novos_barbaros") {
                instantesEntreNovosBarbaros = valor;
                //std::cout << "Instantes entre novos barbaros carregados: " << instantesEntreNovosBarbaros << "\n";
            } else if (chave == "duração_barbaros") {
                duracaoBarbaros = valor;
                //std::cout << "Duracao dos barbaros carregada: " << duracaoBarbaros << "\n";
            } else {
                // std::cout << "Aviso: Chave desconhecida no ficheiro: " << chave << "\n";
            }
        } else {
            std::cout << "Erro ao ler chave e valor da linha: " << linha << "\n";
        }
    }

    std::cout << "Configuracao carregada. Inicializando caravanas do mapa.\n";

    inicializarCaravanasDoMapa();
    file.close();
    std::cout << "Mapa e itens carregados com sucesso!\n";
}

void Simulador::inicializarCaravanasDoMapa() {
    auto self = shared_from_this();
    int idEstacionadas = 0; // Inicializa com zero
    int maiorIdNormal = 0; // Guarda o maior ID normal encontrado

    for (int i = 0; i < mapa->getLinhas(); ++i) {
        for (int j = 0; j < mapa->getcolunas(); ++j) {
            char simbolo = mapa->getCell(i, j);

            // Criação de caravanas estacionadas (letra minúscula)
            if (simbolo >= 'a' && simbolo <= 'z') {
                // Caravana de Comércio
                caravanas.push_back(std::make_shared<CaravanaComercial>(
                    --idEstacionadas,
                    // ID negativo para caravanas estacionadas
                    i,
                    j,
                    self));

                // Caravana Militar
                caravanas.push_back(std::make_shared<CaravanaMilitar>(
                    --idEstacionadas,
                    // ID negativo para caravanas estacionadas
                    i,
                    j,
                    self));
                // Caravana Secreta
                caravanas.push_back(std::make_shared<CaravanaSecreta>(
                    --idEstacionadas,
                    // ID negativo para caravanas estacionadas
                    i,
                    j,
                    self));
            }

            // Criação de caravanas do utilizador (ID positivo)
            if (simbolo >= '1' && simbolo <= '9') {
                int id = simbolo - '0'; // Converte o símbolo para ID
                maiorIdNormal = std::max(maiorIdNormal, id); // Atualiza o maior ID

                int tipoAleatorio = std::rand() % 2;

                // Decide o tipo de caravana com base no ID
                if (tipoAleatorio == 0) {
                    caravanas.push_back(std::make_shared<CaravanaComercial>(id, i, j, self));
                } else if (tipoAleatorio == 1) {
                    caravanas.push_back(std::make_shared<CaravanaMilitar>(id, i, j, self));
                }
            }

            // Criação de caravanas bárbaras (símbolo '!')
            if (simbolo == '!') {
                caravanas.push_back(std::make_shared<CaravanaBarbara>(
                    proximoIdBarbaro++,
                    // Usa o próximo ID bárbaro disponível
                    i,
                    j,
                    self));
            }
        }
    }

    // Define o próximo ID normal com base no maior ID encontrado
    proximoIdNormal = maiorIdNormal + 1;

    std::cout << "Caravanas iniciais configuradas com sucesso!\n";
}

int Simulador::obterProximoId(std::map<int, bool> &idsDisponiveis, int &proximoId, int limiteInferior, int limiteSuperior) {
    // Verifica se há IDs disponíveis no intervalo especificado
    for (auto it = idsDisponiveis.begin(); it != idsDisponiveis.end(); ++it) {
        int id = it->first;
        if (id >= limiteInferior && id <= limiteSuperior) {
            idsDisponiveis.erase(it);
            return id;
        }
    }

    // Se não houver IDs disponíveis, retorna o próximo ID válido
    while (proximoId < limiteInferior || proximoId > limiteSuperior) {
        proximoId++;
    }
    return proximoId++;
}

int Simulador::obterProximoIdNormal() {
    if (proximoIdNormal > 9) {
        return -1; // Retorna -1 para indicar que o limite de IDs foi atingido
    }
    return obterProximoId(idsNormaisDisponiveis, proximoIdNormal, 0, 9);
}

int Simulador::obterProximoIdBarbaro() {
    return obterProximoId(idsBarbarosDisponiveis, proximoIdBarbaro, 101, 150);
}

int Simulador::obterProximoIdSecreta() {
    return obterProximoId(idsNormaisDisponiveis, proximoIdSecreta, 20, 29);
}

void Simulador::adicionarCaravanaBarbara(int linha, int coluna) {
    // Verifica se as coordenadas são válidas no mapa
    if (!mapa->posicaoValida(linha, coluna)) {
        std::cout << "Erro: Coordenadas (" << linha << ", " << coluna << ") sao invalidas.\n";
        return;
    }

    // Cria uma nova caravana bárbara na posicao especificada
    caravanas.push_back(std::make_shared<CaravanaBarbara>(
        obterProximoIdBarbaro(),
        // Usa o próximo ID bárbaro disponível
        linha,
        coluna,
        shared_from_this()));

    std::cout << "Caravana Barbara criada na posicao (" << linha << ", " << coluna << ").\n";
}

void Simulador::adicionarCaravana(char tipo, char cidade) {
    int custoCaravana = 0;

    // Define o custo com base no tipo de caravana
    if (tipo == 'C' || tipo == 'M') {
        custoCaravana = precoCaravana;
    } else if (tipo == 'S') {
        custoCaravana = precoCaravanaSecreta;
    } else {
        std::cout << "Erro: Tipo de caravana inválido.\n";
        return;
    }

    // Verifica se o jogador tem moedas suficientes
    if (moedas < custoCaravana) {
        std::cout << "Erro: Nao ha moedas suficientes para comprar uma caravana.\n";
        return;
    }

    int linha = 0, coluna = 0;

    // Encontra as coordenadas da cidade especificada
    if (!mapa->encontrarCidade(cidade, linha, coluna)) {
        std::cout << "Erro: Cidade " << cidade << " nao encontrada.\n";
        return;
    }

    // Procura caravanas disponíveis (ID negativo)
    for (auto &caravana : caravanas) {
        if (caravana->getLinha() == linha && caravana->getColuna() == coluna &&
            caravana->getTipo() == tipo && caravana->getId() < 0) {
            int novoId;
            if (tipo == 'S') {
                novoId = obterProximoIdSecreta(); // Caravanas secretas não têm restrição de IDs
            } else {
                novoId = obterProximoIdNormal(); // Caravanas normais (C e M) têm IDs de 0 a 9
                if (novoId == -1) {
                    std::cout << "Erro: Limite de caravanas Comerciais e Militares atingido (IDs de 0 a 9).\n";
                    return;
                }
            }
            // Atribui o novo ID a caravana
            caravana->setId(novoId);
            moedas -= custoCaravana; // Subtrai o custo da caravana
            std::cout << "Caravana do tipo " << tipo
                    << " comprada com sucesso em " << cidade
                    << " (ID: " << caravana->getId() << ").\n";
            return;
        }
    }

    std::cout << "Erro: Nenhuma caravana do tipo " << tipo
            << " disponivel para compra em " << cidade << ".\n";
}

void Simulador::verificarEfeitosItensAdjacentes(std::shared_ptr<Caravana> &caravana) const {
    // Ignorar caravanas secretas
    if (caravana->getTipo() == 'S') {
        return;
    }
    int linha = caravana->getLinha();
    int coluna = caravana->getColuna();

    std::vector<std::pair<int, int> > adjacentes = {
        {linha - 1, coluna}, // Acima
        {linha + 1, coluna}, // Abaixo
        {linha, coluna - 1}, // Esquerda
        {linha, coluna + 1} // Direita
    };

    for (auto [l, c] : adjacentes) {
        auto item = mapa->getItemAt(l, c);
        if (item) {
            std::cout << "\nItem encontrado: " << item->getSimbolo() << "\n"; // Verifica o item

            item->aplicarEfeito(caravana);
            mapa->removerItem(item);
            std::cout << "Caravana " << caravana->getId()
                    << " ativou um item adjacente na posicao (" << l << ", " << c << ").\n";
        }
    }
}

void Simulador::avancarTurno(int n) {
    try {
        for (int i = 0; i < n; ++i) {
            // Reinicia o contador de movimentos para todas as caravanas
            for (auto &caravana : caravanas) {
                caravana->resetMovimentosNoTurno();
            }
            executarComportamentosAutomaticos(); // 1. Movimento e gestão automática
            gerarEventos(); // 2. Geração de itens/barbaros
            mapa->removerItensExpirados(); // 3. Remover itens que expiraram

            // 4. Consumir recursos (e verificar destruição por falta de tripulação)
            for (auto &caravana : caravanas) {
                if (caravana->getId() >= 0) {
                    // Apenas caravanas com ID >= 0 (do jogador)
                    caravana->consumirRecursos();
                    caravana->faltaAguaAtacaTripulacao();
                }
            }

            condicoesCombate(); // 5. Resolver combates
            removerCaravanasSemTripulacao(); // 6. Remover caravanas que ficaram sem tripulação

            // **Só depois de tudo isso, remover bárbaros antigos**
            removerBarbarosAntigos(); // 7. Desaparecimento por tempo de vida

            // Limpa a tempestade de areia após o turno
            mapa->limparTempestadeAreia();

            ++turnos;

            // **Verificação para terminar a simulação**
            bool temCaravanasAtivas = false;
            for (const auto &caravana : caravanas) {
                if (caravana->getId() > 0) {
                    temCaravanasAtivas = true;
                    break;
                }
            }

            if (!temCaravanasAtivas && moedas < precoCaravana) {
                std::cout << "\nSimulacao terminada: Sem caravanas ativas e moedas insuficientes para comprar novas.\n";
                mostrarPontuacaoFinal();
                exit(0);
            }
        }
    } catch (const std::exception &e) {
        std::cout << "Erro ao avancar turno: " << e.what() << std::endl;
    }
}

void Simulador::removerBarbarosAntigos() {
    std::vector<int> idsParaRemover;
    for (auto &caravana : caravanas) {
        if (caravana->getTipo() == 'B') {
            int idade = turnos - caravana->getTurnoCriacao();
            if (idade >= duracaoBarbaros) {
                std::cout << "Caravana Barbara " << caravana->getId()
                        << " desapareceu apos " << idade << " turnos.\n";
                idsParaRemover.push_back(caravana->getId());
            }
        }
    }
    for (int id : idsParaRemover) {
        removeCaravanaDoVetorEMapa(id);
    }
}

void Simulador::removeCaravanaDoVetorEMapa(int id) {
    for (auto it = caravanas.begin(); it != caravanas.end();) {
        if ((*it)->getId() == id) {
            // Adiciona o ID ao mapa de IDs disponíveis para reutilização
            if (id < 20) {
                idsNormaisDisponiveis[id] = true;
            } else if (id >= 20 && id < 100) {
                // IDs secretos (20 a 99)
                idsNormaisDisponiveis[id] = true; // Ou crie um mapa específico para IDs secretos
            } else if (id >= 100) {
                idsBarbarosDisponiveis[id] = true;
            }

            // Chamar o método mudaCelula para limpar o estado da caravana (e o mapa)
            (*it)->mudaCelula(mapa);

            // Remove efetivamente do vector e atualiza o iterador
            it = caravanas.erase(it);

            std::cout << "Removida a caravana com ID " << id << " do vetor.\n";
            return;
        } else {
            ++it;
        }
    }

    std::cout << "Nao foi encontrada nenhuma caravana com ID " << id << " no vetor.\n";
}

void Simulador::mostrarConteudoCidade(char simboloCidade) const {
    // Verifica se o char esta entre 'a' e 'z'
    if (simboloCidade < 'a' || simboloCidade > 'z') {
        std::cout << "Erro: O simbolo da cidade deve ser uma letra minuscula.\n";
        return;
    }

    int linha = 0, coluna = 0;
    if (!mapa->encontrarCidade(simboloCidade, linha, coluna)) {
        std::cout << "Cidade '" << simboloCidade << "' nao encontrada no mapa.\n";
        return;
    }

    std::cout << "Cidade '" << simboloCidade << "' encontrada nas coordenadas ("
            << linha << ", " << coluna << ").\n";

    // Verifica caravanas na cidade
    bool encontrouCaravanas = false;
    for (const auto &caravana : caravanas) {
        if (caravana->getLinha() == linha && caravana->getColuna() == coluna) {
            // Verifica se o ID e negativo e imprime mensagem apropriada
            if (caravana->getId() < 0) {
                std::cout << " - Caravana ID " << caravana->getId() << " (tipo "
                        << caravana->getTipo() << ") esta pronta para ser comprada.\n";
            } else {
                std::cout << " - Caravana ID " << caravana->getId() << " (tipo "
                        << caravana->getTipo() << ") esta presente na cidade.\n";
            }
            encontrouCaravanas = true;
        }
    }
    if (!encontrouCaravanas) {
        std::cout << "Nao ha caravanas nesta cidade.\n";
    }
}

void Simulador::executarCombate(Caravana &c1, Caravana &c2) {
    // Verifica se alguma caravana já está sem tripulantes antes do combate
    if (c1.getTripulacao() == 0 || c2.getTripulacao() == 0) {
        std::cout << "Erro: Uma das caravanas já está sem tripulação antes do combate.\n";
        return;
    }

    // Sorteio de força com base nos tripulantes
    int forca1 = calcularForcaCombate(c1);
    int forca2 = calcularForcaCombate(c2);

    std::cout << "Combate entre Caravana " << c1.getId() << " e Barbara "
            << c2.getId() << "!\n";
    std::cout << "Forca da Caravana " << c1.getId() << ": " << forca1
            << ", Forca da Barbara " << c2.getId() << ": " << forca2 << "\n";

    // Determina a caravana vencedora e aplica penalizações
    if (forca1 > forca2) {
        // Caravana do utilizador vence
        resolverVitoria(c1, c2);
    } else {
        // Caravana barbara vence
        resolverVitoria(c2, c1);
    }

    // Destruição imediata de caravanas sem tripulação
    verificarDestruicao(c1, c2);
    verificarDestruicao(c2, c1);

    // Incrementa o número de combates vencidos (apenas para caravanas do utilizador)
    if (forca1 > forca2) {
        combatesVencidos++;
    }
}

int Simulador::calcularForcaCombate(const Caravana &caravana) {
    return (caravana.getTripulacao() > 0) ? rand() % (caravana.getTripulacao() + 1) : 0;
}

void Simulador::resolverVitoria(Caravana &vencedora, Caravana &derrotada) {
    int perdaVencedora = vencedora.getTripulacao() / 5; // 20% da tripulação da vencedora
    int perdaDerrotada = 2 * perdaVencedora; // 40% da tripulação da perdedora

    vencedora.setTripulacao(vencedora.getTripulacao() - perdaVencedora);
    derrotada.setTripulacao(std::max(0, derrotada.getTripulacao() - perdaDerrotada));

    std::cout << "Caravana " << vencedora.getId() << " venceu! Perdeu " << perdaVencedora
            << " tripulantes.\n";
}

void Simulador::transferirRecursos(Caravana &origem, Caravana &destino) {
    // Transferência de agua
    int aguaTransferida = std::min(origem.getAgua(), destino.getMaxAgua() - destino.getAgua());
    destino.setAgua(destino.getAgua() + aguaTransferida);
    origem.setAgua(origem.getAgua() - aguaTransferida);
    std::cout << "Agua transferida para a Caravana " << destino.getId() << ": "
            << aguaTransferida << " litros.\n";

    // Transferência de mercadoria
    int mercadoriaTransferida = std::min(origem.getMercadoria(), destino.getMaxMercadoria() - destino.getMercadoria());
    destino.setMercadoria(destino.getMercadoria() + mercadoriaTransferida);
    origem.setMercadoria(origem.getMercadoria() - mercadoriaTransferida);
    std::cout << "Mercadoria transferida para a Caravana " << destino.getId() << ": "
            << mercadoriaTransferida << " toneladas.\n";
}

void Simulador::verificarDestruicao(Caravana &caravana, Caravana &vencedora) {
    if (caravana.getTripulacao() == 0) {
        std::cout << "Caravana " << caravana.getId() << " foi destruida durante o combate.\n";
        // Transferência de agua e mercadoria da caravana destruida para a vencedora
        if (vencedora.getTripulacao() > 0) {
            transferirRecursos(caravana, vencedora);
        }
        caravana.mudaCelula(mapa);
        removeCaravanaDoVetorEMapa(caravana.getId());
    }
}

void Simulador::condicoesCombate() {
    // chamado as Caravanas pelo respetivo vetor
    std::set<std::pair<int, int> > combatesRealizados; // Rastreia combates únicos

    for (size_t i = 0; i < caravanas.size(); ++i) {
        auto &caravana1 = caravanas[i];

        // Ignorar caravanas sem tripulação, bárbaras ou secretas
        if (!caravana1->terTripulacao() || caravana1->getTipo() == 'B' || caravana1->getTipo() == 'S') {
            continue;
        }

        for (size_t j = 0; j < caravanas.size(); ++j) {
            if (i == j) continue; // Ignorar a mesma caravana

            auto &caravana2 = caravanas[j];

            // Apenas combate com bárbaras
            if (caravana2->getTipo() != 'B' || !caravana2->terTripulacao()) {
                continue;
            }

            // Verificar adjacência
            int distLinha = abs(caravana1->getLinha() - caravana2->getLinha());
            int distColuna = abs(caravana1->getColuna() - caravana2->getColuna());
            if (distLinha + distColuna == 1) {
                // Adjacent
                auto combateKey = std::minmax(caravana1->getId(), caravana2->getId());
                if (!combatesRealizados.contains(combateKey)) {
                    executarCombate(*caravana1, *caravana2);
                    combatesRealizados.insert(combateKey); // Marca como realizado
                }
            }
        }
    }

    // Remover caravanas destruídas após os combates
    for (auto it = caravanas.begin(); it != caravanas.end();) {
        auto &caravana = *it;
        if (caravana->getTripulacao() == 0 && caravana->getTurnosSemTripulacao() >= 5) {
            caravana->mudaCelula(mapa); // Apenas atualiza estado interno e mapa
            it = caravanas.erase(it); // Remove a caravana do vetor
        } else {
            ++it;
        }
    }
}

void Simulador::mostrarEstado() const {
    std::cout << "\nTurno: " << turnos;
    std::cout << "\nMoedas: " << moedas;
    std::cout << "\nCombates vencidos: " << combatesVencidos << "\n";

    Buffer buffer(mapa->getLinhas(), mapa->getcolunas());

    mapa->renderToBuffer(buffer, caravanas);
    mapa->renderItemsToBuffer(buffer); // Adiciona esta linha
    std::cout << buffer; // Renderiza o conteúdo usando o operador <<
}

void Simulador::gerarEventos() {
    if (turnos % instantesEntreNovosItens == 0 && itens.size() < maxItens) {
        mapa->adicionarItemAleatorio();
        std::cout << "Novo item adicionado ao mapa!\n";
    }

    if (turnos % instantesEntreNovosBarbaros == 0) {
        int linha = rand() % mapa->getLinhas();
        int coluna = rand() % mapa->getcolunas();
        if (mapa->posicaoValida(linha, coluna)) {
            caravanas.push_back(std::make_shared<CaravanaBarbara>(
                proximoIdBarbaro + 1,
                linha,
                coluna,
                shared_from_this()));
            std::cout << "Nova caravana barbara<< adicionada em (" << linha << ", "
                    << coluna << ").\n";
            proximoIdBarbaro++;
        }
    }
}

void Simulador::mostrarPontuacaoFinal() const {
    std::cout << "Simulacao encerrada.\n";
    std::cout << "Turnos jogados: " << turnos << "\n";
    std::cout << "Combates vencidos: " << combatesVencidos << "\n";
    std::cout << "Moedas restantes: " << moedas << "\n";
}

void Simulador::exibirPrecos() const {
    std::cout << "Precos:\n";
    std::cout << " - Mercadoria (compra): 10 moedas/tonelada\n";
    std::cout << " - Mercadoria (venda): 20 moedas/tonelada\n";
}

void Simulador::executarComportamentosAutomaticos() {
    for (auto it = caravanas.begin(); it != caravanas.end();) {
        auto caravana = *it;
        bool removida = false;

        if (caravana->isAutoGestao()) {
            char tipo = caravana->getTipo();

            if (tipo == 'B') {
                if (turnos - caravana->getTurnoCriacao() >= duracaoBarbaros) {
                    it = caravanas.erase(it);
                    std::cout << "Caravana Barbara " << caravana->getId()
                            << " desapareceu por fim de tempo de vida.\n";
                    continue;
                }
                caravana->move("", mapa, caravanas); // Move autonomamente
            } else if (tipo == 'C') {
                executarAutoGestaoComercial(caravana);
            } else if (tipo == 'M') {
                executarAutoGestaoMilitar(caravana);
            }

            // Verifica efeitos de itens adjacentes (como minas)
            verificarEfeitosItensAdjacentes(caravana);
            // Reabastecimento ao entrar numa cidade durante autogestão
            if (mapa->getCell(caravana->getLinha(), caravana->getColuna()) >= 'a' &&
                mapa->getCell(caravana->getLinha(), caravana->getColuna()) <= 'z') {
                // Apenas reabastece se não for uma caravana bárbara
                if (caravana->getTipo() != 'B') {
                    caravana->setAgua(caravana->getMaxAgua());
                    std::cout << "Caravana " << caravana->getId()
                            << " reabasteceu agua automaticamente ao entrar na cidade.\n";
                }
            }
            // Se a caravana foi destruida, remove imediatamente do vetor
            if (std::find(caravanas.begin(), caravanas.end(), caravana) == caravanas.end()) {
                it = caravanas.begin(); // Reinicia o iterador após remoção
                removida = true;
            }
        }

        // Se nao foi removida, continua para a próxima caravana
        if (!removida) {
            ++it;
        }
    }
}

// Lógica de auto-gestao para caravanas comerciais
void Simulador::executarAutoGestaoComercial(const std::shared_ptr<Caravana> &caravana) {
    int linha = caravana->getLinha();
    int coluna = caravana->getColuna();

    // Verifica se a caravana está numa cidade
    if (mapa->getCell(linha, coluna) >= 'a' && mapa->getCell(linha, coluna) <= 'z') {
        int espacoDisponivel = caravana->getMaxMercadoria() - caravana->getMercadoria();
        int custo = espacoDisponivel * 10;

        // Compra mercadorias se houver espaço e moedas suficientes
        if (moedas >= custo) {
            caravana->setMercadoria(caravana->getMaxMercadoria());
            moedas -= custo;
            std::cout << "Caravana Comercial " << caravana->getId() << " comprou mercadorias.\n";
        }

        // Vende mercadorias na cidade
        moedas += caravana->getMercadoria() * 10;
        caravana->setMercadoria(0);
        std::cout << "Caravana Comercial " << caravana->getId() << " vendeu mercadorias.\n";
        return;
    }

    // Prioridade: Movimento em direcao a itens próximos (até 2 posições)
    int melhorLinha = linha;
    int melhorColuna = coluna;
    int menorDistancia = 3; // Máximo de 2 posições (linha + coluna)

    for (const auto &item : mapa->getItens()) {
        int distLinha = abs(item->getX() - linha);
        int distColuna = abs(item->getY() - coluna);
        int distancia = distLinha + distColuna;

        if (distancia <= 2 && distancia < menorDistancia) {
            melhorLinha = item->getX();
            melhorColuna = item->getY();
            menorDistancia = distancia;
        }
    }

    // Se nao houver itens próximos, alinhar-se a outras caravanas
    if (menorDistancia > 2) {
        for (const auto &outraCaravana : caravanas) {
            if (outraCaravana->getId() != caravana->getId()) {
                int distLinha = abs(outraCaravana->getLinha() - linha);
                int distColuna = abs(outraCaravana->getColuna() - coluna);
                int distancia = distLinha + distColuna;

                if (distancia <= 2 && distancia < menorDistancia) {
                    melhorLinha = outraCaravana->getLinha();
                    melhorColuna = outraCaravana->getColuna();
                    menorDistancia = distancia;
                }
            }
        }
    }

    // Movimento na direcao do item ou caravana próxima
    if (menorDistancia <= 2) {
        int deslocLinha = (melhorLinha > linha) ? 1 : (melhorLinha < linha) ? -1 : 0;
        int deslocColuna = (melhorColuna > coluna) ? 1 : (melhorColuna < coluna) ? -1 : 0;

        std::string direcao;
        if (deslocLinha == -1 && deslocColuna == 0) direcao = "C";
        else if (deslocLinha == 1 && deslocColuna == 0) direcao = "B";
        else if (deslocLinha == 0 && deslocColuna == -1) direcao = "E";
        else if (deslocLinha == 0 && deslocColuna == 1) direcao = "D";

        if (!direcao.empty()) {
            caravana->move(direcao, mapa, caravanas);
            std::cout << "Caravana Comercial " << caravana->getId()
                    << " moveu-se na direcao de um item ou outra caravana.\n";
            return;
        }
    }

    // Movimento aleatório se nao houver itens/caravanas próximas
    std::string direcoes[] = {"C", "B", "E", "D"};
    caravana->move(direcoes[rand() % 4], mapa, caravanas);
}

// Lógica de auto-gestao para caravanas militares
void Simulador::executarAutoGestaoMilitar(const std::shared_ptr<Caravana> &caravana) {
    int linha = caravana->getLinha();
    int coluna = caravana->getColuna();
    int melhorLinha = linha;
    int melhorColuna = coluna;
    bool encontrouBarbaro = false;

    // Verifica a presença de bárbaros a até 6 posições
    for (const auto &outraCaravana : caravanas) {
        if (outraCaravana->getTipo() == 'B') {
            int distLinha = abs(outraCaravana->getLinha() - linha);
            int distColuna = abs(outraCaravana->getColuna() - coluna);
            if (distLinha + distColuna <= 6) {
                melhorLinha = outraCaravana->getLinha();
                melhorColuna = outraCaravana->getColuna();
                encontrouBarbaro = true;
                break;
            }
        }
    }

    // Movimenta-se apenas se encontrar um bárbaro a 6 posições
    if (encontrouBarbaro) {
        int deslocLinha = (melhorLinha > linha) ? 1 : (melhorLinha < linha) ? -1 : 0;
        int deslocColuna = (melhorColuna > coluna) ? 1 : (melhorColuna < coluna) ? -1 : 0;

        std::string direcao;
        if (deslocLinha == -1) direcao = "C";
        else if (deslocLinha == 1) direcao = "B";
        else if (deslocColuna == -1) direcao = "E";
        else if (deslocColuna == 1) direcao = "D";

        caravana->move(direcao, mapa, caravanas);
        std::cout << "Caravana Militar " << caravana->getId()
                << " moveu-se para perseguir um bárbaro.\n";
    } else {
        std::cout << "Caravana Militar " << caravana->getId() << " ficou parada.\n";
    }
}

// Lógica genérica para auto-gestao de caravanas
void Simulador::executarAutoGestaoCaravana(const std::shared_ptr<Caravana> &caravana) {
    if (!caravana) {
        std::cout << "Erro: Caravana invalida (nullptr).\n";
        return;
    }

    // Obtém o simbolo da caravana para determinar o tipo
    char simbolo = caravana->getSimbolo();

    if (simbolo == 'C') {
        // Caravana Comercial
        executarAutoGestaoComercial(caravana);
    } else if (simbolo == 'M') {
        // Caravana Militar
        executarAutoGestaoMilitar(caravana);
    } else {
        // Caso o tipo de caravana nao seja reconhecido
        std::cout << "Erro: Tipo de caravana desconhecido (ID: "
                << caravana->getId() << ", Simbolo: " << simbolo << ").\n";
    }
}

void Simulador::descreveCaravana(int id) const {
    for (const auto &caravana : caravanas) {
        char simboloCidade = mapa->getCell(caravana->getLinha(), caravana->getColuna());
        std::string cidade = (simboloCidade >= 'a' && simboloCidade <= 'z')
                                 ? std::string(1, simboloCidade)
                                 : "Nenhuma. Deserto.";

        // Itera sobre as caravanas
        if (caravana->getId() == id) {
            // Verifica se o ID corresponde
            std::cout << "ID: " << id
                    << ", Tipo: " << caravana->getTipo() // Obtém o tipo da caravana
                    << ", Posicao: (" << caravana->getLinha() << ", "
                    << caravana->getColuna() << ")"
                    << ", Tripulantes: " << caravana->getTripulacao()
                    << ", Agua: " << caravana->getAgua()
                    << ", Carga: " << caravana->getMercadoria()
                    << ", Cidade: " << cidade << "\n";
            return;
        }
    }

    // Caso o ID nao seja encontrado
    std::cout << "Caravana com ID " << id << " nao encontrada.\n";
}

std::shared_ptr<Caravana> Simulador::obterCaravanaPorId(int id) const {
    for (const auto &caravana : caravanas) {
        if (caravana->getId() == id) {
            return std::shared_ptr<Caravana>(caravana.get(),
                                             [](Caravana *) {
                                             }); // Cria um shared_ptr sem gerir o ciclo de vida
        }
    }
    return nullptr;
}

bool Simulador::caravanaEstaEmCidade(int id) const {
    for (const auto &caravana : caravanas) {
        if (caravana->getId() == id) {
            return mapa->getCell(caravana->getLinha(), caravana->getColuna()) >= 'a' &&
                    mapa->getCell(caravana->getLinha(), caravana->getColuna()) <= 'z';
        }
    }
    return false;
}

void Simulador::comprarMercadorias(int id, int quantidade) {
    auto caravana = obterCaravanaPorId(id);
    if (!caravana) {
        throw std::runtime_error("Caravana com o ID fornecido nao encontrada.");
    }

    int custoTotal = quantidade * 10; // Supondo que o custo e 10 moedas por tonelada
    if (moedas < custoTotal) {
        throw std::runtime_error("Moedas insuficientes para a compra.");
    }

    if (caravana->getMercadoria() + quantidade > caravana->getMaxMercadoria()) {
        throw std::runtime_error("A caravana nao pode transportar essa quantidade de mercadorias.");
    }

    caravana->setMercadoria(caravana->getMercadoria() + quantidade);
    moedas -= custoTotal;

    std::cout << "Compra realizada: " << quantidade << " toneladas de mercadorias.\n";
}

void Simulador::adicionarMoedas(int lucro) {
    if (lucro < 0) {
        throw std::invalid_argument("O valor do lucro nao pode ser negativo.");
    }
    moedas += lucro; // Incrementa as moedas com o valor do lucro
    std::cout << "Saldo atualizado: " << moedas << " moedas.\n";
}

void Simulador::moverCaravana(int id, const std::string &direcao) {
    auto caravana = obterCaravanaPorId(id);
    if (!caravana) {
        throw std::runtime_error("Caravana com o ID fornecido nao encontrada.");
    }

    if (caravana->getTipo() == 'B') {
        std::cout << "Caravanas barbaras nao podem ser movidas manualmente.\n";
        return;
    }
    int linhaAnterior = caravana->getLinha();
    int colunaAnterior = caravana->getColuna();

    // Move a caravana para a nova posicao
    caravana->move(direcao, mapa, caravanas);

    // Verifica se a caravana entrou numa cidade
    if (mapa->getCell(caravana->getLinha(), caravana->getColuna()) >= 'a' &&
        mapa->getCell(caravana->getLinha(), caravana->getColuna()) <= 'z') {
        if (caravana->getTipo() != 'B') {
            caravana->setAgua(caravana->getMaxAgua()); // Reabastece a agua
            std::cout << "Caravana " << caravana->getId()
                    << " reabasteceu agua ao entrar na cidade.\n";
        }
    }

    // Após o movimento, verifica se apanha um item adjacente
    verificarEfeitosItensAdjacentes(caravana);

    // Limpa a posicao anterior no mapa
    mapa->setCell(linhaAnterior, colunaAnterior, '.');
}

void Simulador::removerCaravanasSemTripulacao() {
    for (auto it = caravanas.begin(); it != caravanas.end();) {
        if ((*it)->getTripulacao() == 0) {
            char tipo = (*it)->getTipo();

            // Aplica limite de turnos apenas a comerciais e militares
            int limiteTurnos = (tipo == 'C') ? 5 : (tipo == 'M') ? 7 : (tipo == 'B') ? 0 : -1;

            // Se o limite for atingido, remove a caravana
            if (limiteTurnos != -1 && (*it)->getTurnosSemTripulacao() >= limiteTurnos) {
                std::cout << "Caravana " << tipo << " (ID: " << (*it)->getId()
                        << ") desapareceu após " << limiteTurnos << " turnos sem tripulacao.\n";

                removeCaravanaDoVetorEMapa((*it)->getId());

                // Reinicia o iterador para evitar invalidar o loop
                it = caravanas.begin();
                continue;
            }

            // Para caravanas bárbaras (tipo B), removemos imediatamente
            if (tipo == 'B') {
                std::cout << "Caravana Barbara (ID: " << (*it)->getId()
                        << ") desapareceu imediatamente ao perder a tripulacao.\n";

                removeCaravanaDoVetorEMapa((*it)->getId());
                it = caravanas.begin();
                continue;
            }
        }
        ++it;
    }
}

void Simulador::reset() {
    moedas = 500;
    turnos = 1;
    caravanas.clear();
    itens.clear();
    mapa = std::make_shared<Mapa>(); // Cria um novo mapa vazio
    savedBuffers.clear(); // Apaga todos os buffers guardados
    combatesVencidos = 0; // Reinicia contagem de combates
    proximoIdNormal = 0;
    proximoIdBarbaro = 100;
    proximoIdSecreta = 20;

    // Opcional: Reset de parâmetros de configuração para valores padrão
    instantesEntreNovosItens = 10;
    duracaoItem = 10;
    maxItens = 5;
    precoVendaMercadoria = 100;
    precoCompraMercadoria = 50;
    precoCaravana = 200;
    instantesEntreNovosBarbaros = 10;
    duracaoBarbaros = 60;

    std::cout << "\nSimulador reiniciado.\n";
}

void Simulador::secretaAutoDestroi(int id1, int id2) {
    auto &mapa = getMapa(); // Obter o mapa
    auto &caravanas = getCaravanas(); // Obter as caravanas
    auto secreta = obterCaravanaPorId(id1);
    auto alvo = obterCaravanaPorId(id2);

    // Verifica se ambas as caravanas existem
    if (!secreta || !alvo) {
        std::cout << "Erro: Caravana secreta ou alvo nao encontrada.\n";
        return;
    }

    // Verifica se a caravana e do tipo secreta
    if (secreta->getTipo() != 'S') {
        std::cout << "Erro: A caravana " << id1 << " nao e secreta.\n";
        return; // Retorna imediatamente se a caravana não for secreta
    }

    // Verifica se a caravana está inativa
    if (turnos < turnoAtiva) {
        int turnoAtual = getTurnos();
        int turnoAtiva = turnoAtual + secreta->getTurnosSemTripulacao();
        std::cout << "Caravana secreta " << id1 << " está inativa do turno " << turnoAtual << " ao turno " << turnoAtiva
                << ".\n";
        return;
    }

    // Guardar posição inicial (cidade)
    int linhaInicial = secreta->getLinha();
    int colunaInicial = secreta->getColuna();

    // Verifica se está numa cidade no início
    char simboloCidade = mapa->getCell(linhaInicial, colunaInicial);
    if (!(simboloCidade >= 'a' && simboloCidade <= 'z')) {
        std::cout << "Erro: A caravana secreta nao está numa cidade.\n";
        return;
    }

    // Verifica se o alvo já está adjacente
    int distLinha = abs(secreta->getLinha() - alvo->getLinha());
    int distColuna = abs(secreta->getColuna() - alvo->getColuna());
    if (distLinha <= 1 && distColuna <= 1) {
        // Destruir o alvo imediatamente
        removeCaravanaDoVetorEMapa(id2);
        std::cout << "Caravana secreta " << id1 << " destruiu a caravana " << id2 << ".\n";

        // Inibe a caravana secreta por 5 turnos
        turnoAtiva = turnos + 5;
        std::cout << "Caravana secreta " << id1 << " nao pode ser usada por 5 turnos.\n";

        // Retornar a cidade de origem
        moverCaravanaParaCidade(secreta, linhaInicial, colunaInicial);
        return;
    }

    // Movimento aleatório até estar adjacente ao alvo
    while (abs(secreta->getLinha() - alvo->getLinha()) > 1 || abs(secreta->getColuna() - alvo->getColuna()) > 1) {
        std::string direcoes[] = {"B", "C", "D", "E", "BD", "BE", "CD", "CE"};
        std::string direcao = direcoes[rand() % 8]; // Seleciona uma direção aleatória
        secreta->move(direcao, mapa, caravanas);
    }

    // Destruir o alvo
    removeCaravanaDoVetorEMapa(id2);
    std::cout << "Caravana secreta " << id1 << " destruiu a caravana " << id2 << ".\n";

    // Inibe a caravana secreta por 5 turnos
    turnoAtiva = turnos + 5;
    std::cout << "Caravana secreta " << id1 << " nao pode ser usada por 5 turnos.\n";

    // Retornar a cidade de origem
    moverCaravanaParaCidade(secreta, linhaInicial, colunaInicial);
}

// Função auxiliar para mover a caravana de volta a cidade
void Simulador::moverCaravanaParaCidade(const std::shared_ptr<Caravana> &caravana, int linhaCidade, int colunaCidade) {
    while (caravana->getLinha() != linhaCidade || caravana->getColuna() != colunaCidade) {
        int dx = linhaCidade - caravana->getLinha();
        int dy = colunaCidade - caravana->getColuna();

        std::string direcao;
        if (dx > 0 && dy > 0) direcao = "BD";
        else if (dx > 0 && dy < 0) direcao = "BE";
        else if (dx < 0 && dy > 0) direcao = "CD";
        else if (dx < 0 && dy < 0) direcao = "CE";
        else if (dx > 0) direcao = "B";
        else if (dx < 0) direcao = "C";
        else if (dy > 0) direcao = "D";
        else if (dy < 0) direcao = "E";

        caravana->move(direcao, mapa, caravanas);
    }
}

void Simulador::secretaTransportaMercadoria(int id, char destino) {
    auto &mapa = getMapa(); // Obter o mapa
    auto &caravanas = getCaravanas(); // Obter as caravanas
    auto secreta = obterCaravanaPorId(id);

    // Verifica se a caravana existe e e do tipo secreta
    if (!secreta || secreta->getTipo() != 'S') {
        std::cout << "Erro: Caravana secreta nao encontrada ou invalida.\n";
        return;
    }

    // Verifica se a caravana está inativa
    if (turnos < turnoAtiva) {
        int turnoAtual = getTurnos();
        int turnoAtiva = turnoAtual + secreta->getTurnosSemTripulacao();
        std::cout << "Caravana secreta " << id << " esta inativa do turno " << turnoAtual << " ao turno " << turnoAtiva
                << ".\n";
        return;
    }

    // Guardar posição inicial (cidade de origem)
    int linhaInicial = secreta->getLinha();
    int colunaInicial = secreta->getColuna();

    // Verificar se está numa cidade no início
    char simboloCidadeInicial = mapa->getCell(linhaInicial, colunaInicial);
    if (!(simboloCidadeInicial >= 'a' && simboloCidadeInicial <= 'z')) {
        std::cout << "Erro: A caravana secreta nao esta numa cidade.\n";
        return;
    }

    // Encontrar a cidade de destino por símbolo
    int linhaDestino = 0;
    int colunaDestino = 0;
    if (!mapa->encontrarCidade(destino, linhaDestino, colunaDestino)) {
        std::cout << "Erro: Cidade de destino nao encontrada.\n";
        return;
    }

    // Movimento até a cidade de destino
    while (secreta->getLinha() != linhaDestino || secreta->getColuna() != colunaDestino) {
        int dx = linhaDestino - secreta->getLinha();
        int dy = colunaDestino - secreta->getColuna();

        std::string direcao;
        if (dx > 0 && dy > 0) direcao = "BD";
        else if (dx > 0 && dy < 0) direcao = "BE";
        else if (dx < 0 && dy > 0) direcao = "CD";
        else if (dx < 0 && dy < 0) direcao = "CE";
        else if (dx > 0) direcao = "B";
        else if (dx < 0) direcao = "C";
        else if (dy > 0) direcao = "D";
        else if (dy < 0) direcao = "E";

        // Move a caravana na direção calculada
        secreta->move(direcao, mapa, caravanas);

        // Verifica se a caravana chegou ao destino após o movimento
        if (secreta->getLinha() == linhaDestino && secreta->getColuna() == colunaDestino) {
            break; // Sai do loop se a caravana chegou ao destino
        }
    }

    std::cout << "Caravana secreta " << id << " chegou a cidade " << destino << "\n";

    // Inibe a caravana secreta por 5 turnos
    turnoAtiva = turnos + 5;
    std::cout << "Caravana secreta " << id << " nao pode ser usada por 5 turnos.\n";
}

void Simulador::criarTempestadeAreia(int l, int c, int _linha) {
    if (!mapa) {
        std::cout << "Erro: O mapa nao esta carregado.\n";
        return;
    }
    if (_linha <= 0 || l < 0 || c < 0) {
        std::cout << "Erro: Parametros invalidos para a tempestade de areia.\n";
        return;
    }

    for (int i = l - _linha; i <= l + _linha; ++i) {
        for (int j = c - _linha; j <= c + _linha; ++j) {
            int dist = (i - l) * (i - l) + (j - c) * (j - c);
            if (dist <= _linha * _linha) {
                int wrappedRow = i;
                int wrappedCol = j;
                mapa->wrapPosition(wrappedRow, wrappedCol); // Ajusta as coordenadas para o mapa esférico

                // Verifica se a celula está dentro dos limites do mapa
                if (wrappedRow >= 0 && wrappedRow < mapa->getLinhas() && wrappedCol >= 0 && wrappedCol < mapa->
                    getcolunas()) {
                    // Marca a celula como afetada pela tempestade apenas se for um deserto ('.')
                    if (mapa->getCell(wrappedRow, wrappedCol) == '.') {
                        mapa->setCell(wrappedRow, wrappedCol, '~'); // Símbolo temporário para tempestade
                        //std::cout << "Tempestade de areia afetou a celula (" << wrappedRow << ", " << wrappedCol << ").\n";
                    }
                }
            }
        }
    }
}

void Simulador::aplicarEfeitoTempestade() {
    // Lista de IDs das caravanas a serem removidas
    std::vector<int> caravanasParaRemover;

    // Itera sobre todas as caravanas
    for (auto it = caravanas.begin(); it != caravanas.end(); ++it) {
        auto &caravana = *it;

        // Verifica se a caravana tem ID positivo (caravanas do jogador)
        if (caravana->getId() <= 0) {
            continue; // Ignora caravanas com ID não positivo
        }

        int linha = caravana->getLinha();
        int coluna = caravana->getColuna();

        // Verifica as células adjacentes (incluindo diagonais)
        bool afetadaPelaTempestade = false;
        for (int i = linha - 1; i <= linha + 1; ++i) {
            for (int j = coluna - 1; j <= coluna + 1; ++j) {
                if (i == linha && j == coluna) continue; // Ignora a própria celula da caravana

                // Ajusta as coordenadas para o mapa esférico
                int wrappedRow = i;
                int wrappedCol = j;
                mapa->wrapPosition(wrappedRow, wrappedCol);

                // Verifica se a celula adjacente está marcada como tempestade
                if (mapa->getCell(wrappedRow, wrappedCol) == '~') {
                    afetadaPelaTempestade = true;
                    break;
                }
            }
            if (afetadaPelaTempestade) break;
        }

        // Aplica o efeito da tempestade se a caravana estiver adjacente a uma tempestade
        if (afetadaPelaTempestade) {
            switch (caravana->getTipo()) {
                case 'C': {
                    // Caravana Comercial
                    if (rand() % 100 < ((caravana->getMercadoria() > caravana->getMaxMercadoria() / 2) ? 50 : 25)) {
                        std::cout << "Caravana Comercial " << caravana->getId()
                                << " foi destruida pela tempestade de areia.\n";
                        caravana->mudaCelula(mapa);
                        caravanasParaRemover.push_back(caravana->getId()); // Marca para remoção
                    } else {
                        caravana->setMercadoria(
                            std::max(0, caravana->getMercadoria() - (caravana->getMercadoria() / 4)));
                        std::cout << "Caravana Comercial " << caravana->getId()
                                << " perdeu 25% da carga devido a tempestade.\n";
                    }
                    break;
                }
                case 'M': {
                    // Caravana Militar
                    if (rand() % 100 < 33) {
                        std::cout << "Caravana Militar " << caravana->getId()
                                << " foi destruida pela tempestade de areia.\n";
                        caravana->mudaCelula(mapa);
                        caravanasParaRemover.push_back(caravana->getId()); // Marca para remoção
                    } else {
                        int perdaTripulacao = caravana->getTripulacao() / 10;
                        caravana->setTripulacao(std::max(0, caravana->getTripulacao() - perdaTripulacao));
                        std::cout << "Caravana Militar " << caravana->getId()
                                << " perdeu " << perdaTripulacao << " tripulantes na tempestade.\n";
                    }
                    break;
                }
                case 'B': {
                    // Caravana Bárbara
                    if (rand() % 100 < 25) {
                        std::cout << "Caravana Barbara " << caravana->getId()
                                << " foi completamente destruida pela tempestade de areia.\n";
                        caravana->mudaCelula(mapa);
                        caravanasParaRemover.push_back(caravana->getId()); // Marca para remoção
                    } else {
                        int perdaTripulacao = caravana->getTripulacao() / 10;
                        caravana->setTripulacao(std::max(0, caravana->getTripulacao() - perdaTripulacao));
                        std::cout << "Caravana Barbara " << caravana->getId()
                                << " perdeu " << perdaTripulacao << " tripulantes na tempestade.\n";

                        // Verifica se a caravana ficou sem tripulantes
                        if (caravana->getTripulacao() == 0) {
                            std::cout << "Caravana Barbara " << caravana->getId()
                                    << " desapareceu por falta de tripulantes.\n";
                            caravanasParaRemover.push_back(caravana->getId()); // Marca para remoção
                        }
                    }
                    break;
                }
                case 'S': {
                    // Caravana Secreta
                    std::cout << "Caravana Secreta " << caravana->getId()
                            << " não foi afetada pela tempestade.\n";
                    break;
                }
                default:
                    std::cout << "Erro: Tipo de caravana desconhecido.\n";
                    break;
            }
        }
    }

    // Remove as caravanas marcadas para remoção
    for (int id : caravanasParaRemover) {
        removeCaravanaDoVetorEMapa(id);
    }
}

