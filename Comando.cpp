//
// Created by Utilizador on 18/12/2024.
//

#include "Comando.h"

#include "Simulador.h"
#include "Mapa.h"
#include <iostream>
#include <sstream>
#include <regex>
#include <fstream>
#include <memory>

// 1. Implementação de 'config <ficheiro>'
// 1. executar 'config <ficheiro>'
bool ComandoConfig::validar(const std::string &input) {
  // Corrigir o regex para capturar o nome do ficheiro após 'config'
  std::regex pattern(R"(config\s+(\S+))");
  std::smatch match;

  if (std::regex_match(input, match, pattern)) {
    nomeFicheiro = match[1].str(); // Captura o nome do ficheiro
    return true;
  }
  return false;
}

// 1. executar 'config <ficheiro>'
void ComandoConfig::executar() {
  try {
    simulador->carregarMapa(nomeFicheiro);
    simulador->mostrarEstado();
  } catch (const std::exception &e) {
    std::cout << "Erro ao executar o comando 'config': " << e.what() << std::endl;
  }
}

// 2. Implementação de Sair
// 2. Validar 'sair'
bool ComandoSair::validar(const std::string &input) {
  return input == "sair";
}
// 2. executar 'sair'
void ComandoSair::executar() {
  std::cout << "A sair do programa." << std::endl;
  exit(0); // O programa termina aqui
}

// 3. Implementação de 'exec <ficheiro>'
// 3. Validar 'exec <ficheiro>'
bool ComandoExec::validar(const std::string &input) {
  std::regex pattern(R"(exec\s+(\S+))");
  std::smatch match;
  if (std::regex_match(input, match, pattern)) {
    nomeFicheiro = match[1];
    return true;
  }
  return false;
}
// 3. Executar 'exec <ficheiro>'
void ComandoExec::executar() {

}

// 4. Implementação de Prox
// 4. Validar 'prox [<n>]'
bool ComandoProx::validar(const std::string &input) {
  std::regex pattern(R"(prox(\s+(\d+))?)");
  std::smatch match;
  if (std::regex_match(input, match, pattern)) {
    if (match[2].matched) {
      n = std::stoi(match[2]);
    }
    return true;
  }
  return false;
}
// 4.Executar 'prox [<n>]'
void ComandoProx::executar() {
  std::cout << "A avancar " << n << " turnos." << std::endl;

  try {
    // Avançar o numero de turnos especificado
    simulador->avancarTurno(n);

    // Mostrar o estado atual do simulador após avançar os turnos
    simulador->mostrarEstado();
  } catch (const std::exception &e) {
    std::cout << "Erro ao avancar os turnos: " << e.what() << std::endl;
  }
}

// 5. Implementação de CompraC
// 5. Validar 'comprac <cidade> <tipo>'
bool ComandoCompraC::validar(const std::string &input) {
  std::regex pattern(R"(comprac\s+([a-zA-Z])\s+([CMS]))");
  std::smatch match;
  if (std::regex_match(input, match, pattern)) {
    cidade = match[1].str()[0];
    tipo = match[2].str()[0];
    return true;
  }
  return false;
}
// 5. Executar 'comprac <cidade> <tipo>'
void ComandoCompraC::executar() {
  std::cout << "A comprar uma caravana do tipo " << tipo << " na cidade " << cidade << std::endl;

  try {
    // Usa o método adicionaCaravana do Simulador para adicionar a caravana
    simulador->adicionarCaravana(tipo, cidade);
    simulador->mostrarEstado();
  } catch (const std::exception &e) {
    std::cout << "Erro ao comprar a caravana: " << e.what() << std::endl;
  }
}

// 6. Implementação de Precos
// 6. Validar 'precos'
bool ComandoPrecos::validar(const std::string &input) {
  return input == "precos";
}
// 6. Executar 'precos'
void ComandoPrecos::executar() {
  std::cout << "A listar os precos das mercadorias." << std::endl;

  try {
    simulador->exibirPrecos();
  } catch (const std::exception &e) {
    std::cout << "Erro ao listar os precos: " << e.what() << std::endl;
  }
}

// 7. Implementação de 'cidade <cidade>'
// 7. Validar 'cidade <cidade>'
bool ComandoCidade::validar(const std::string &input) {
  std::regex pattern(R"(cidade\s+([a-zA-Z]))");
  std::smatch match;
  if (std::regex_match(input, match, pattern)) {
    cidade = match[1].str()[0];
    return true;
  }
  return false;
}
// 7. Executar 'cidade <cidade>'
void ComandoCidade::executar() {
  simulador->mostrarConteudoCidade(cidade);
}

// 8. Implementação de 'caravana <C>'
// 8. Validar 'caravana <C>'
bool ComandoCaravana::validar(const std::string &input) {
  std::regex pattern(R"(caravana\s+(\d+))");
  std::smatch match;

  if (std::regex_match(input, match, pattern)) {
    // Converte o dígito capturado para inteiro
    id = std::stoi(match[1].str());
    return true;
  }
  return false;
}

// 8. Executar 'caravana <C>'
void ComandoCaravana::executar() {
  try {
    // Valida se o identificador da caravana foi fornecido corretamente
    if (id < 0) {
      throw std::invalid_argument("ID da caravana invalido.");
    }

    // Usa o ponteiro inteligente para chamar o método descreveCaravana do simulador
    simulador->descreveCaravana(id);
  } catch (const std::exception &e) {
    std::cout << "Erro ao executar o comando caravana: " << e.what() << std::endl;
  }
}

// 9. Implementação de 'compra <caravanaId> <quantidade>'
// 9. Validar 'compra <caravanaId> <quantidade>'
bool ComandoCompra::validar(const std::string &input) {
  std::regex pattern(R"(compra\s+(\d+)\s+(\d+))");
  std::smatch match;
  if (std::regex_match(input, match, pattern)) {
    // Converte os valores capturados para inteiros
    caravanaId = std::stoi(match[1].str());
    quantidade = std::stoi(match[2].str());

    // Verifica se os valores são válidos
    if (caravanaId <= 0 || quantidade <= 0) {
      throw std::invalid_argument("ID ou quantidade nao pode ser menor ou igual a zero.");
    }

    return true; // Comando válido
  } else {
    return false;
  }
}

// 9. Executar 'compra <caravanaId> <quantidade>'
void ComandoCompra::executar() {
  try {
    // Verifica se os valores de ID e quantidade são válidos
    if (caravanaId <= 0 || quantidade <= 0) {
      throw std::invalid_argument("ID da caravana ou quantidade invalidos.");
    }

    // Verifica se a caravana esta numa cidade e tenta realizar a compra
    auto caravana = simulador->obterCaravanaPorId(caravanaId);
    if (!caravana) {
      throw std::runtime_error("Caravana com o ID fornecido nao encontrada.");
    }

    if (!simulador->caravanaEstaEmCidade(caravanaId)) {
      throw std::runtime_error("A caravana nao esta numa cidade.");
    }

    // Realiza a compra chamando o método do simulador
    simulador->comprarMercadorias(caravanaId, quantidade);
    std::cout << "Compra realizada: " << quantidade << " toneladas de mercadorias para a caravana " << caravanaId <<
        ".\n";
  } catch (const std::exception &e) {
    std::cout << "Erro ao executar o comando de compra: " << e.what() << std::endl;
  }
}

// 10. Implementação de 'vende <numero>'
// 10. Validar 'vende <numero>'
bool ComandoVende::validar(const std::string &input) {
  std::regex pattern(R"(vende\s+(\d+))");
  std::smatch match;
  if (std::regex_match(input, match, pattern)) {
    caravanaId = std::stoi(match[1]);
    return true;
  }
  return false;
}
// 10. Executar 'vende <numero>'
void ComandoVende::executar() {
  try {
    // Verifica se o ID da caravana e válido
    if (caravanaId <= 0) {
      throw std::invalid_argument("ID da caravana invalido.");
    }

    // Obtém a caravana pelo ID
    auto caravana = simulador->obterCaravanaPorId(caravanaId);
    if (!caravana) {
      throw std::runtime_error("Caravana com o ID fornecido nao encontrada.");
    }

    // Verifica se a caravana esta numa cidade
    if (!simulador->caravanaEstaEmCidade(caravanaId)) {
      throw std::runtime_error("A caravana nao esta numa cidade e nao pode vender mercadorias.");
    }

    // Calcula os lucros com a venda
    int quantidade = caravana->getMercadoria();
    int lucro = quantidade * 10; // Supondo que cada tonelada vale 10 moedas

    if (quantidade <= 0) {
      throw std::runtime_error("A caravana nao tem mercadorias para vender.");
    }

    // Realiza a venda
    caravana->setMercadoria(0); // Remove toda a mercadoria da caravana
    simulador->adicionarMoedas(lucro); // Adiciona o lucro ao simulador

    std::cout << "Venda realizada: " << quantidade << " toneladas de mercadorias vendidas pela caravana "
        << caravanaId << ". Lucro: " << lucro << " moedas.\n";
  } catch (const std::exception &e) {
    std::cout << "Erro ao executar o comando de venda: " << e.what() << std::endl;
  }
}

// 11. Implementação de 'move <numero> <direcao>'
// 11. Validar 'move <numero> <direcao>'
bool ComandoMove::validar(const std::string &input) {
  std::regex pattern(R"(move\s+(\d+)\s+([A-Za-z]+))");
  std::smatch match;
  if (std::regex_match(input, match, pattern)) {
    caravanaId = std::stoi(match[1]);
    direcao = match[2];
    return true;
  }
  return false;
}
// 11. Executar 'move <numero> <direcao>'
void ComandoMove::executar() {
  try {
    // Valida se o ID da caravana e a direcao são válidos
    if (caravanaId <= 0 || direcao.empty()) {
      throw std::invalid_argument("ID da caravana ou direcao invalidos.");
    }

    // Obtém a caravana pelo ID
    auto caravana = simulador->obterCaravanaPorId(caravanaId);
    if (!caravana) {
      throw std::runtime_error("Caravana com o ID fornecido nao encontrada.");
    }

    // Move a caravana na direcao especificada
    simulador->moverCaravana(caravanaId, direcao);
    simulador->mostrarEstado();
  } catch (const std::exception &e) {
    std::cout << "Erro ao executar o comando de movimento: " << e.what() << std::endl;
  }
}

// 12. Implementação de 'auto'
// 12. Validar 'auto <caravanaId>'
bool ComandoAuto::validar(const std::string &input) {
  std::regex pattern(R"(auto\s+(\d+))");
  std::smatch match;
  if (std::regex_match(input, match, pattern)) {
    caravanaId = std::stoi(match[1]);
    return true;
  }
  return false;
}

// 12. Executar 'auto <caravanaId>'
void ComandoAuto::executar() {
  try {
    // Obtém a caravana pelo ID
    auto caravana = simulador->obterCaravanaPorId(caravanaId);
    if (!caravana) {
      throw std::runtime_error("Caravana com o ID fornecido nao encontrada.");
    }

    // Ativa o modo de auto-gestao
    caravana->setAutoGestao(true);
    std::cout << "Caravana " << caravanaId << " colocada em modo automático.\n";
  } catch (const std::exception &e) {
    std::cout << "Erro ao ativar o modo automático para a caravana: " << e.what() << std::endl;
  }
}

// 13. Implementação de 'stop <caravanaId>'
// 13. Validar 'stop <numero>'
bool ComandoStop::validar(const std::string &input) {
  std::regex pattern(R"(stop\s+(\d+))");
  std::smatch match;
  if (std::regex_match(input, match, pattern)) {
    caravanaId = std::stoi(match[1]);
    return true;
  }
  return false;
}
// 13. Executar 'stop <numero>'
void ComandoStop::executar() {
  try {
    // Obtém a caravana pelo ID
    auto caravana = simulador->obterCaravanaPorId(caravanaId);
    if (!caravana) {
      throw std::runtime_error("Caravana com o ID fornecido nao encontrada.");
    }

    // Desativa o modo de auto-gestao
    caravana->setAutoGestao(false);
    std::cout << "Auto-gestao desativada para a caravana " << caravanaId << ".\n";
  } catch (const std::exception &e) {
    std::cout << "Erro ao desativar o modo automático para a caravana: " << e.what() << std::endl;
  }
}

// 14. Implementação de 'barbaro <l> <c>'
// 14. Validar 'barbaro <l> <c>'
bool ComandoBarbaro::validar(const std::string &input) {
  std::regex pattern(R"(barbaro\s+(\d+)\s+(\d+))");
  std::smatch match;
  if (std::regex_match(input, match, pattern)) {
    linha = std::stoi(match[1]);
    coluna = std::stoi(match[2]);
    return true;
  }
  return false;
}
// 14. Executar 'barbaro <l> <c>'
void ComandoBarbaro::executar() {
  try {
    // Verifica se as coordenadas fornecidas são válidas
    if (!simulador->getMapa()->posicaoValida(linha, coluna)) {
      throw std::invalid_argument("Coordenadas invalidas para criar uma caravana barbara.");
    }

    simulador->adicionarCaravanaBarbara(linha, coluna);
    simulador->mostrarEstado();
  } catch (const std::exception &e) {
    std::cout << "Erro ao criar caravana barbara: " << e.what() << std::endl;
  }
}

// 15. Implementação de 'areia <l> <c> <raio>'
// 15. Validar 'areia <l> <c> <raio>'
bool ComandoAreia::validar(const std::string &input) {
  std::regex pattern(R"(areia\s+(\d+)\s+(\d+)\s+(\d+))");
  std::smatch match;
  if (std::regex_match(input, match, pattern)) {
    linha = std::stoi(match[1]);
    coluna = std::stoi(match[2]);
    raio = std::stoi(match[3]);
    if (raio <= 0 || raio > simulador->getMapa()->getLinhas() || raio > simulador->getMapa()->getcolunas()) {
      std::cout << ("\nRaio invalido")<<std::endl;
      return false;
    }

    return true;
  }
  return false;
}
// 15. Executar 'areia <l> <c> <raio>'
void ComandoAreia::executar() {
  try {
    // Verifica se as coordenadas fornecidas são válidas no mapa
    if (!simulador->getMapa()->posicaoValida(linha, coluna)) {
      std::cout <<"Coordenadas invalidas para criar a tempestade de areia."<<std::endl;
    }

    // Cria a tempestade de areia usando o método do mapa
    simulador->criarTempestadeAreia(linha, coluna, raio);
    simulador->aplicarEfeitoTempestade();
    std::cout << "Tempestade de areia criada em (" << linha << ", " << coluna
        << ") com raio " << raio << ".\n";
    simulador->mostrarEstado();
  } catch (const std::exception &e) {
    std::cout << "Erro ao criar tempestade de areia: " << e.what() << std::endl;
  }
}

// 16. Implementação de 'moedas <quantidade>'
// 16. Validar 'moedas <quantidade>'
bool ComandoMoedas::validar(const std::string &input) {
  std::regex pattern(R"(moedas\s+(-?\d+))");
  std::smatch match;
  if (std::regex_match(input, match, pattern)) {
    quantidade = std::stoi(match[1]);
    return true;
  }
  return false;
}
// 16. Executar 'moedas <quantidade>'
void ComandoMoedas::executar() {
  try {
    // Atualiza o numero de moedas no simulador
    simulador->setMoedas(simulador->getMoedas() + quantidade);

    // Mostra o saldo atualizado
    std::cout << "Saldo atualizado: " << simulador->getMoedas() << " moedas.\n";
  } catch (const std::exception &e) {
    std::cout << "Erro ao atualizar moedas: " << e.what() << std::endl;
  }
}

// 17. Implementação de 'tripul <numero> <tripulantes>'
// 17. Validar 'tripul <numero> <tripulantes>'
bool ComandoTripul::validar(const std::string &input) {
  std::regex pattern(R"(tripul\s+(\d+)\s+(\d+))");
  std::smatch match;
  if (std::regex_match(input, match, pattern)) {
    numero = std::stoi(match[1]);
    tripulantes = std::stoi(match[2]);
    return true;
  }
  return false;
}
// 17. Executar 'tripul <numero> <tripulantes>'
void ComandoTripul::executar() {
  try {
    // Obtém a caravana pelo ID
    auto caravana = simulador->obterCaravanaPorId(numero);
    if (!caravana) {
      throw std::runtime_error("Caravana com o ID fornecido nao encontrada.");
    }

    // Verifica se a caravana esta numa cidade
    if (!simulador->caravanaEstaEmCidade(numero)) {
      throw std::runtime_error("A caravana nao esta numa cidade e nao pode recrutar tripulantes.");
    }

    // Calcula o custo para recrutar os tripulantes
    int custo = tripulantes * 5; // Supondo que cada tripulante custa 5 moedas
    if (simulador->getMoedas() < custo) {
      throw std::runtime_error("Moedas insuficientes para recrutar tripulantes.");
    }

    // Verifica se ha espaço suficiente na caravana
    int capacidadeDisponivel = caravana->getMaxTripulacao() - caravana->getTripulacao();
    if (tripulantes > capacidadeDisponivel) {
      throw std::runtime_error("Nao ha espaço suficiente na caravana para mais tripulantes.");
    }

    // Atualiza o estado: compra os tripulantes
    caravana->setTripulacao(caravana->getTripulacao() + tripulantes);
    simulador->setMoedas(simulador->getMoedas() - custo);

    // Mensagem de sucesso
    std::cout << "Foram recrutados " << tripulantes << " tripulantes para a caravana "
        << numero << ".\nSaldo atualizado: " << simulador->getMoedas() << " moedas.\n";
  } catch (const std::exception &e) {
    std::cout << "Erro ao recrutar tripulantes: " << e.what() << std::endl;
  }
}

// 18. Implementação de 'saves <nome>'
// 18. Validar 'saves <nome>'
bool ComandoSaves::validar(const std::string &input) {
  std::regex pattern(R"(saves\s+(\S+))");
  std::smatch match;
  if (std::regex_match(input, match, pattern)) {
    nome = match[1];
    return true;
  }
  return false;
}
// 18. Executar 'saves <nome>'
void ComandoSaves::executar() {
  try {
    // Cria um buffer com as dimensões do mapa
    Buffer buffer(simulador->getMapa()->getLinhas(), simulador->getMapa()->getcolunas());

    // Renderiza o estado atual do mapa e das caravanas no buffer
    simulador->getMapa()->renderToBuffer(buffer, simulador->getCaravanas());
    simulador->getMapa()->renderItemsToBuffer(buffer);

    // Guarda o estado visual no simulador com o nome fornecido
    simulador->saveBuffer(nome, buffer);

    // Mensagem de sucesso
    std::cout << "Estado do buffer guardado com o nome '" << nome << "'.\n";
  } catch (const std::exception &e) {
    std::cout << "Erro ao guardar o estado do buffer: " << e.what() << std::endl;
  }
}

// 19. Implementação de 'loads <nome>'
// 19. Validar 'loads <nome>'
bool ComandoLoads::validar(const std::string &input) {
  std::regex pattern(R"(loads\s+(\S+))");
  std::smatch match;
  if (std::regex_match(input, match, pattern)) {
    nome = match[1];
    return true;
  }
  return false;
}
// 19. Executar 'loads <nome>'
void ComandoLoads::executar() {
  try {
    // Tenta carregar o buffer guardado com o nome especificado
    simulador->loadBuffer(nome);

    // Mensagem de sucesso
    std::cout << "Buffer com o nome '" << nome << "' carregado e exibido.\n";
  } catch (const std::exception &e) {
    // Mensagem de erro
    std::cout << "Erro ao carregar o buffer: " << e.what() << std::endl;
  }
}

// 20. Implementação de Lists
// 20. Validar 'lists'
bool ComandoLists::validar(const std::string &input) {
  return input == "lists";
}
// 20. Executar 'lists'
void ComandoLists::executar() {
  try {
    // Obtém a lista de buffers guardados no simulador
    const auto &buffers = simulador->getSavedBuffers();

    // Verifica se existem buffers guardados
    if (buffers.empty()) {
      std::cout << "Nao existem buffers guardados.\n";
      return;
    }

    // Lista os nomes dos buffers guardados
    std::cout << "Buffers guardados:\n";
    for (const auto &buffer : buffers) {
      std::cout << "- " << buffer.first << "\n"; // buffer.first contém o nome
    }
  } catch (const std::exception &e) {
    std::cout << "Erro ao listar os buffers guardados: " << e.what() << std::endl;
  }
}

// 21. Implementação de 'dels <nome>'
// 21. Validar 'dels <nome>'
bool ComandoDels::validar(const std::string &input) {
  std::regex pattern(R"(dels\s+(\S+))");
  std::smatch match;
  if (std::regex_match(input, match, pattern)) {
    nome = match[1];
    return true;
  }
  return false;
}
// 21. Executar 'dels <nome>'
void ComandoDels::executar() {
  try {
    // Apaga o buffer guardado com o nome especificado
    simulador->deleteBuffer(nome);

    // Mensagem de sucesso
    std::cout << "Buffer com o nome '" << nome << "' apagado com sucesso.\n";
  } catch (const std::exception &e) {
    // Mensagem de erro
    std::cout << "Erro ao apagar o buffer: " << e.what() << std::endl;
  }
}

// 22. Implementação de 'terminar'
// 22. Validar 'terminar'
bool ComandoTerminar::validar(const std::string &input) {
  return input == "terminar";
}
// 22. Executar 'terminar'
void ComandoTerminar::executar() {
  try {

    // Exibe a pontuação final antes de encerrar a simulação
    simulador->mostrarPontuacaoFinal();
    std::cout << "\n-------------FIM-------------\n";
    simulador->reset(); // Reinicializa o simulador
    std::cout << "O simulador regressou a fase 1\n";
    std::cout << "Comandos Disponiveis:\nconfig <nomeFicheiro>\nsair\n";
  } catch (const std::exception &e) {
    std::cout << "Erro ao terminar a simulação: " << e.what() << std::endl;
  }
}

// 23. Implementação de 'listacaravanas'
// 23. Validar 'listacaravanas'
bool ComandoListaCaravanas::validar(const std::string &input) {
  return input == "listacaravanas";
}
// 23. Executar 'listacaravanas'
void ComandoListaCaravanas::executar() {
  try {
    // Obtém a lista de caravanas do simulador
    const auto &caravanas = simulador->getCaravanas();

    // Verifica se existem caravanas
    if (caravanas.empty()) {
      std::cout << "Nao existem caravanas.\n";
      return;
    }

    // Lista as caravanas
    int id = 0;
    std::cout << "Caravanas:\n";
    for (const auto &caravana : caravanas) {
      id = caravana->getId();
      if (id >= 0) {
        simulador->descreveCaravana(id);
      }
    }
  } catch (const std::exception &e) {
    std::cout << "Erro ao listar as caravanas: " << e.what() << std::endl;
  }
}

// 24. Implementação de 'secreta <id1> destroi <id2>'
// 24. Validação de 'secreta <id1> destroi <id2>'
bool ComandoSecretaDestroi::validar(const std::string &input) {
  std::regex pattern(R"(secreta\s+(\d+)\s+destroi\s+(\d+))");
  std::smatch match;

  if (std::regex_match(input, match, pattern)) {
    idSecreta = std::stoi(match[1].str());
    idAlvo = std::stoi(match[2].str());
    return true;
  }
  return false;
}

// 24. Execução do comando 'secreta <id1> destroi <id2>'
void ComandoSecretaDestroi::executar() {
  simulador->secretaAutoDestroi(idSecreta, idAlvo);

  std::cout << "Caravana secreta " << idSecreta << " regressou a cidade de origem.\n";
}

// 25. Implementação de 'secreta <id> mercadoria <c>'
// 25. Validação de 'secreta <id> mercadoria <c>'
bool ComandoSecretaMercadoria::validar(const std::string &input) {
  std::regex pattern(R"(secreta\s+(\d+)\s+mercadoria\s+([a-z]))");

  std::smatch match;

  if (std::regex_match(input, match, pattern)) {
    idSecreta = std::stoi(match[1].str());
    destino = match[2].str()[0]; // Pega o primeiro caractere da string
    return true;
  }
  return false;
}

// 25. Execução do comando 'secreta <id> mercadoria <c>'
void ComandoSecretaMercadoria::executar() {
  simulador->secretaTransportaMercadoria(idSecreta, destino);
}
