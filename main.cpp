#include <memory>  // Para smartPointers
#include "Simulador.h"
#include "GestorComandos.h"

void simulacaoComClasseComandos() {
    try {
        // Configuracao inicial: Cria uma instância do simulador
        auto simulador = std::make_shared<Simulador>();
        // Inicializa o GestorComandos com o simulador
        GestorComandos gestor(simulador);

        // Executa o ciclo de comandos
        gestor.executar();
    } catch (const std::exception &e) {
        std::cout << "Erro fatal: " << e.what() << std::endl;
        return;
    }
}

int main() {
    simulacaoComClasseComandos();
    return 0;
}
