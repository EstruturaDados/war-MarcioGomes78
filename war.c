/*
 * ============================================================================
 * Programa: Sistema de Batalha entre Territórios com Alocação Dinâmica
 * Autor: Marcio Gomes
 * Data: 2025-11-05
 * Descrição: Sistema completo para cadastrar territórios e simular batalhas
 *            entre eles, utilizando alocação dinâmica de memória e simulação
 *            de ataques com dados aleatórios.
 * 
 * Funcionalidades Principais:
 *   - Alocação dinâmica de memória para territórios
 *   - Cadastro interativo de territórios
 *   - Simulação de batalhas com dados aleatórios
 *   - Transferência de territórios entre exércitos
 *   - Atualização automática de tropas após batalhas
 *   - Gerenciamento adequado de memória
 * 
 * Conceitos Aplicados:
 *   - Alocação dinâmica (malloc/calloc)
 *   - Ponteiros e referências
 *   - Simulação com números aleatórios
 *   - Estruturas de dados (structs)
 *   - Modularização de código
 *   - Gerenciamento de memória (free)
 * ============================================================================
 */

#include <stdio.h>      // Para funções de entrada/saída
#include <stdlib.h>     // Para alocação dinâmica e números aleatórios
#include <string.h>     // Para manipulação de strings
#include <time.h>       // Para semente de números aleatórios
#include <ctype.h>      // Para conversão de caracteres
#include <stdbool.h>    // Para usar tipo bool, true e false

// ============================================================================
// DEFINIÇÃO DA ESTRUTURA
// ============================================================================

/*
 * Struct: Territorio
 * 
 * Representa um território no sistema de batalha com suas características:
 * - nome: identificação do território (até 29 caracteres + '\0')
 * - dono: nome do comandante/exército que controla (até 29 caracteres + '\0')
 * - cor: cor do exército ocupante (até 9 caracteres + '\0')  
 * - tropas: quantidade de soldados presentes no território
 */
typedef struct {
    char nome[30];   // Nome do território
    char dono[30];   // Nome do dono/comandante
    char cor[10];    // Cor do exército ocupante
    int tropas;      // Número de tropas
} Territorio;

// ============================================================================
// CONSTANTES
// ============================================================================

#define BUFFER_SIZE 100          // Tamanho padrão para buffers de entrada
#define MIN_TERRITORIOS 2        // Mínimo de territórios para batalha
#define MAX_TERRITORIOS 20       // Máximo de territórios suportados
#define MAX_NOME 30             // Tamanho máximo para nomes
#define DADO_MIN 1              // Valor mínimo do dado de batalha
#define DADO_MAX 6              // Valor máximo do dado de batalha

// Macros utilitárias
#define LIMPAR_BUFFER while(getchar() != '\n')  // Limpar buffer de entrada

// ============================================================================
// PROTÓTIPOS DE FUNÇÕES
// ============================================================================

// Funções de inicialização e configuração
void exibirCabecalho(void);
void inicializarSistema(void);
int obterNumeroTerritorios(void);

// Funções de alocação e liberação de memória
Territorio* alocarTerritorios(int quantidade);
void liberarMemoria(Territorio* mapa);

// Funções de cadastro e exibição
void cadastrarTerritorio(Territorio *t, int numero);
void exibirTerritorio(const Territorio *t, int numero);
void exibirTodosTeritorios(const Territorio *territorios, int total);
void exibirMapaSimplificado(const Territorio *territorios, int total);

// Funções de batalha e simulação
bool atacar(Territorio* atacante, Territorio* defensor);
int simularDado(void);
void executarBatalha(Territorio *mapa, int numTerritorios);

// Funções utilitárias
void limparTela(void);
void aguardarEnter(void);
void exibirEstatisticas(Territorio *mapa, int numTerritorios);

// ============================================================================
// FUNÇÃO PRINCIPAL
// ============================================================================

/**
 * Função principal - coordena todo o fluxo do programa
 * 
 * Esta função implementa o fluxo completo do sistema de batalha entre
 * territórios, incluindo alocação dinâmica, cadastro, simulação de
 * batalhas e liberação adequada de memória.
 * 
 * Fluxo do programa:
 *   1. Inicialização do sistema (semente aleatória)
 *   2. Obtenção do número de territórios do usuário
 *   3. Alocação dinâmica de memória
 *   4. Cadastro interativo dos territórios
 *   5. Sistema de batalhas (loop interativo)
 *   6. Liberação de memória e encerramento
 * 
 * @return 0 se execução foi bem-sucedida, 1 em caso de erro
 */
int main(void) {
    Territorio *mapa = NULL;  // Ponteiro para array de territórios alocado dinamicamente
    int numTerritorios = 0;   // Número total de territórios
    
    // ========================================================================
    // FASE 1: INICIALIZAÇÃO DO SISTEMA
    // ========================================================================
    limparTela();
    exibirCabecalho();
    inicializarSistema();
    
    // ========================================================================
    // FASE 2: CONFIGURAÇÃO E ALOCAÇÃO DINÂMICA
    // ========================================================================
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║            CONFIGURAÇÃO DO MAPA DE BATALHA                ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    // Obter número de territórios do usuário
    numTerritorios = obterNumeroTerritorios();
    
    // Alocar memória dinamicamente para os territórios
    mapa = alocarTerritorios(numTerritorios);
    if (mapa == NULL) {
        printf("\n❌ Erro: Não foi possível alocar memória para os territórios!\n");
        printf("   💡 Tente com um número menor de territórios.\n");
        return 1;  // Encerrar com código de erro
    }
    
    printf("\n✅ Memória alocada com sucesso para %d territórios!\n", numTerritorios);
    printf("   📊 Tamanho alocado: %zu bytes\n", numTerritorios * sizeof(Territorio));
    
    // ========================================================================
    // FASE 3: CADASTRO DOS TERRITÓRIOS
    // ========================================================================
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║          CADASTRO DE TERRITÓRIOS - FASE DE ENTRADA        ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    // Loop de cadastro usando ponteiros para acessar territórios alocados
    for (int i = 0; i < numTerritorios; i++) {
        cadastrarTerritorio(&mapa[i], i + 1);  // Passa endereço do território
        printf("\n");
    }
    
    // ========================================================================
    // FASE 4: EXIBIÇÃO INICIAL DOS TERRITÓRIOS
    // ========================================================================
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║              TERRITÓRIOS CADASTRADOS COM SUCESSO           ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    aguardarEnter();
    limparTela();
    
    // Exibir mapa inicial completo
    exibirTodosTeritorios(mapa, numTerritorios);
    exibirEstatisticas(mapa, numTerritorios);
    
    // ========================================================================
    // FASE 5: SISTEMA DE BATALHAS INTERATIVO
    // ========================================================================
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                  SISTEMA DE BATALHAS                      ║\n");
    printf("║                                                            ║\n");
    printf("║  🎲 As batalhas são decididas por dados (1-6)             ║\n");
    printf("║  ⚔️  Atacante vence: conquista território                  ║\n");
    printf("║  🛡️  Defensor vence: atacante perde 1 tropa               ║\n");
    printf("║                                                            ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    aguardarEnter();
    
    // Loop principal de batalhas
    executarBatalha(mapa, numTerritorios);
    
    // ========================================================================
    // FASE 6: LIBERAÇÃO DE MEMÓRIA E ENCERRAMENTO
    // ========================================================================
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                 ENCERRANDO SISTEMA                        ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    printf("🧹 Liberando memória alocada dinamicamente...\n");
    liberarMemoria(mapa);
    printf("✅ Memória liberada com sucesso!\n");
    
    printf("\n🏆 Obrigado por jogar o Sistema de Batalha entre Territórios!\n");
    printf("📚 Este programa demonstra:\n");
    printf("   • Alocação dinâmica de memória (malloc/calloc)\n");
    printf("   • Uso de ponteiros para manipulação de dados\n");
    printf("   • Simulação com números aleatórios\n");
    printf("   • Gerenciamento adequado de memória (free)\n");
    
    return 0;  // Encerramento bem-sucedido
}

// ============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES - SISTEMA E GERENCIAMENTO DE MEMÓRIA
// ============================================================================

/**
 * Inicializa o sistema de batalha
 * 
 * Configura a semente do gerador de números aleatórios baseado no
 * tempo atual, garantindo que cada execução tenha sequências
 * diferentes para simular batalhas mais realistas.
 */
void inicializarSistema(void) {
    srand(time(NULL));  // Semente baseada no tempo atual
    printf("🎲 Sistema de números aleatórios inicializado!\n");
    printf("   Cada batalha terá resultados únicos baseados no tempo.\n");
}

/**
 * Obtém o número de territórios que o usuário deseja cadastrar
 * 
 * Valida a entrada garantindo que seja um número válido dentro
 * dos limites estabelecidos (entre MIN_TERRITORIOS e MAX_TERRITORIOS).
 * 
 * @return Número válido de territórios escolhido pelo usuário
 */
int obterNumeroTerritorios(void) {
    int numero;
    bool entradaValida = false;
    
    printf("\n🗺️  Quantos territórios deseja cadastrar? ");
    printf("(min: %d, máx: %d): ", MIN_TERRITORIOS, MAX_TERRITORIOS);
    
    do {
        if (scanf("%d", &numero) != 1) {
            printf("❌ Entrada inválida! Digite apenas números.\n");
            printf("🗺️  Quantos territórios? (min: %d, máx: %d): ", 
                   MIN_TERRITORIOS, MAX_TERRITORIOS);
            // Limpar buffer de entrada
            while (getchar() != '\n');
            continue;
        }
        
        if (numero < MIN_TERRITORIOS || numero > MAX_TERRITORIOS) {
            printf("❌ Número inválido! Deve estar entre %d e %d.\n", 
                   MIN_TERRITORIOS, MAX_TERRITORIOS);
            printf("🗺️  Quantos territórios? (min: %d, máx: %d): ", 
                   MIN_TERRITORIOS, MAX_TERRITORIOS);
            continue;
        }
        
        entradaValida = true;
    } while (!entradaValida);
    
    // Limpar buffer restante
    while (getchar() != '\n');
    
    return numero;
}

/**
 * Aloca memória dinamicamente para array de territórios
 * 
 * Utiliza calloc para garantir que toda a memória seja inicializada
 * com zeros, evitando problemas com dados não inicializados.
 * 
 * @param quantidade Número de territórios a serem alocados
 * @return Ponteiro para array alocado, ou NULL se falhou
 */
Territorio* alocarTerritorios(int quantidade) {
    if (quantidade <= 0) {
        printf("❌ Erro: Quantidade inválida de territórios (%d).\n", quantidade);
        return NULL;
    }
    
    // Usar calloc para inicializar com zeros
    Territorio *mapa = (Territorio*)calloc(quantidade, sizeof(Territorio));
    
    if (mapa == NULL) {
        printf("❌ Erro crítico: Falha na alocação de memória!\n");
        printf("   Memória solicitada: %zu bytes\n", quantidade * sizeof(Territorio));
        return NULL;
    }
    
    printf("💾 Alocação bem-sucedida: %d territórios\n", quantidade);
    printf("   Endereço base: %p\n", (void*)mapa);
    
    return mapa;
}

/**
 * Libera a memória alocada dinamicamente
 * 
 * Função segura que verifica se o ponteiro é válido antes
 * de liberar e define o ponteiro como NULL após liberação.
 * 
 * @param mapa Ponteiro para o array de territórios a ser liberado
 */
void liberarMemoria(Territorio *mapa) {
    if (mapa != NULL) {
        printf("🗑️  Liberando memória do endereço: %p\n", (void*)mapa);
        free(mapa);
        mapa = NULL;  // Boa prática: evitar ponteiros soltos
        printf("✅ Memória liberada com segurança!\n");
    } else {
        printf("⚠️  Aviso: Tentativa de liberar ponteiro nulo.\n");
    }
}

// ============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES - SISTEMA DE BATALHAS
// ============================================================================

/**
 * Simula o resultado de um dado de 6 faces
 * 
 * Gera um número aleatório entre 1 e 6, simulando o lançamento
 * de um dado tradicional usado nas batalhas entre territórios.
 * 
 * @return Valor do dado (1-6)
 */
int simularDado(void) {
    return (rand() % 6) + 1;  // Gera número entre 1 e 6
}

/**
 * Executa uma batalha entre dois territórios
 * 
 * Implementa as regras de combate:
 * - Cada lado rola um dado (1-6)
 * - Maior valor vence a batalha
 * - Atacante vence: conquista o território
 * - Defensor vence: atacante perde 1 tropa
 * - Empate: nada acontece
 * 
 * @param atacante Ponteiro para território atacante
 * @param defensor Ponteiro para território defensor
 * @return true se ataque foi bem-sucedido, false caso contrário
 */
bool atacar(Territorio *atacante, Territorio *defensor) {
    if (atacante == NULL || defensor == NULL) {
        printf("❌ Erro: Ponteiros inválidos na batalha!\n");
        return false;
    }
    
    // Verificar se atacante tem tropas suficientes
    if (atacante->tropas <= 1) {
        printf("❌ %s não tem tropas suficientes para atacar!\n", atacante->nome);
        printf("   (Necessário: mín. 2 tropas, atual: %d)\n", atacante->tropas);
        return false;
    }
    
    printf("\n⚔️ ═══════════════════════════════════════════════════════════\n");
    printf("              BATALHA EM ANDAMENTO\n");
    printf("═══════════════════════════════════════════════════════════⚔️\n");
    printf("🏴 Atacante: %s (👥 %d tropas)\n", atacante->nome, atacante->tropas);
    printf("🏰 Defensor: %s (👥 %d tropas)\n", defensor->nome, defensor->tropas);
    
    // Simular dados de batalha
    int dadoAtacante = simularDado();
    int dadoDefensor = simularDado();
    
    printf("\n🎲 Lançamento dos dados:\n");
    printf("   🏴 %s rolou: %d\n", atacante->nome, dadoAtacante);
    printf("   🏰 %s rolou: %d\n", defensor->nome, dadoDefensor);
    
    // Determinar resultado da batalha
    if (dadoAtacante > dadoDefensor) {
        // Atacante vence - conquista território
        printf("\n🏆 VITÓRIA DO ATACANTE!\n");
        printf("   %s conquista %s!\n", atacante->nome, defensor->nome);
        
        // Transferir território
        strcpy(defensor->nome, atacante->nome);
        strcpy(defensor->dono, atacante->dono);
        defensor->tropas = atacante->tropas - 1;
        atacante->tropas = 1;  // Atacante fica com 1 tropa apenas
        
        printf("   🔄 Transferindo controle...\n");
        printf("   📊 %s agora tem %d tropas em %s\n", 
               defensor->dono, defensor->tropas, defensor->nome);
        
        return true;
    } 
    else if (dadoDefensor > dadoAtacante) {
        // Defensor vence - atacante perde tropa
        printf("\n🛡️ VITÓRIA DO DEFENSOR!\n");
        printf("   %s defendeu com sucesso!\n", defensor->nome);
        
        atacante->tropas--;
        printf("   💀 %s perde 1 tropa (restam: %d)\n", 
               atacante->nome, atacante->tropas);
        
        return false;
    } 
    else {
        // Empate - nada acontece
        printf("\n🤝 EMPATE!\n");
        printf("   Ambos os lados rolaram %d - nenhuma mudança!\n", dadoAtacante);
        return false;
    }
}

/**
 * Executa o loop principal de batalhas
 * 
 * Interface interativa que permite ao usuário escolher territórios
 * para batalhar, visualizar resultados e continuar jogando até
 * decidir parar.
 * 
 * @param mapa Array de territórios
 * @param numTerritorios Número total de territórios
 */
void executarBatalha(Territorio *mapa, int numTerritorios) {
    char continuar;
    int indiceAtacante, indiceDefensor;
    
    do {
        limparTela();
        printf("⚔️ ═══════════════════════════════════════════════════════════\n");
        printf("                    ARENA DE BATALHA\n");
        printf("═══════════════════════════════════════════════════════════⚔️\n\n");
        
        // Mostrar territórios disponíveis
        printf("🗺️ TERRITÓRIOS DISPONÍVEIS:\n");
        for (int i = 0; i < numTerritorios; i++) {
            printf("   [%d] %s - %s (👥 %d tropas)\n", 
                   i + 1, mapa[i].nome, mapa[i].dono, mapa[i].tropas);
        }
        
        // Escolher atacante
        printf("\n🏴 Escolha o território ATACANTE (1-%d): ", numTerritorios);
        while (scanf("%d", &indiceAtacante) != 1 || 
               indiceAtacante < 1 || indiceAtacante > numTerritorios) {
            printf("❌ Índice inválido! Escolha entre 1 e %d: ", numTerritorios);
            while (getchar() != '\n');  // Limpar buffer
        }
        indiceAtacante--;  // Converter para índice 0-based
        
        // Escolher defensor
        printf("🏰 Escolha o território DEFENSOR (1-%d): ", numTerritorios);
        while (scanf("%d", &indiceDefensor) != 1 || 
               indiceDefensor < 1 || indiceDefensor > numTerritorios) {
            printf("❌ Índice inválido! Escolha entre 1 e %d: ", numTerritorios);
            while (getchar() != '\n');  // Limpar buffer
        }
        indiceDefensor--;  // Converter para índice 0-based
        
        // Verificar se são territórios diferentes
        if (indiceAtacante == indiceDefensor) {
            printf("❌ Um território não pode atacar a si mesmo!\n");
            aguardarEnter();
            continue;
        }
        
        // Executar batalha
        atacar(&mapa[indiceAtacante], &mapa[indiceDefensor]);
        
        // Mostrar estado atual após batalha
        printf("\n📊 ESTADO ATUAL DOS TERRITÓRIOS:\n");
        exibirTodosTeritorios(mapa, numTerritorios);
        exibirEstatisticas(mapa, numTerritorios);
        
        // Perguntar se quer continuar
        printf("\n🎮 Deseja realizar outra batalha? (s/N): ");
        while (getchar() != '\n');  // Limpar buffer
        continuar = getchar();
        
    } while (continuar == 's' || continuar == 'S');
    
    printf("\n🏁 Fim das batalhas!\n");
}

/**
 * Exibe estatísticas do mapa de territórios
 * 
 * Calcula e mostra informações agregadas como total de tropas,
 * território com mais tropas, distribuição de donos, etc.
 * 
 * @param mapa Array de territórios
 * @param numTerritorios Número total de territórios
 */
void exibirEstatisticas(Territorio *mapa, int numTerritorios) {
    if (mapa == NULL || numTerritorios <= 0) {
        printf("❌ Dados inválidos para calcular estatísticas.\n");
        return;
    }
    
    int totalTropas = 0;
    int maxTropas = 0;
    char territorioMaisForte[MAX_NOME];
    char donoMaisForte[MAX_NOME];
    
    // Calcular estatísticas
    for (int i = 0; i < numTerritorios; i++) {
        totalTropas += mapa[i].tropas;
        if (mapa[i].tropas > maxTropas) {
            maxTropas = mapa[i].tropas;
            strcpy(territorioMaisForte, mapa[i].nome);
            strcpy(donoMaisForte, mapa[i].dono);
        }
    }
    
    double mediaTropas = (double)totalTropas / numTerritorios;
    
    printf("\n📊 ═══════════════════════════════════════════════════════════\n");
    printf("                      ESTATÍSTICAS DO MAPA\n");
    printf("═══════════════════════════════════════════════════════════📊\n");
    printf("🏗️  Total de territórios: %d\n", numTerritorios);
    printf("👥 Total de tropas: %d\n", totalTropas);
    printf("📈 Média de tropas por território: %.1f\n", mediaTropas);
    printf("🏆 Território mais forte: %s (%s) - %d tropas\n", 
           territorioMaisForte, donoMaisForte, maxTropas);
    printf("═══════════════════════════════════════════════════════════📊\n");
}

// ============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES ORIGINAIS
// ============================================================================

/*
 * Função: exibirCabecalho
 * Descrição: Exibe o cabeçalho inicial do programa com arte ASCII
 */
void exibirCabecalho(void) {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                                                            ║\n");
    printf("║        🏰  SISTEMA DE CADASTRO DE TERRITÓRIOS  🏰         ║\n");
    printf("║                                                            ║\n");
    printf("║         Organize suas conquistas e exércitos!             ║\n");
    printf("║                                                            ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
}

/*
 * Função: cadastrarTerritorio
 * Parâmetros:
 *   - t: ponteiro para a struct Territorio a ser preenchida
 *   - numero: número do território (1 a 5) para exibição
 * 
 * Descrição: Solicita ao usuário os dados de um território e os
 *            armazena na struct apontada por 't'.
 * 
 * Validações:
 *   - Tropas devem ser um número inteiro não-negativo
 *   - Remove espaços extras e quebras de linha
 */
void cadastrarTerritorio(Territorio *t, int numero) {
    char buffer[100];  // Buffer temporário para leitura
    int valido = 0;    // Flag de validação
    
    printf("┌────────────────────────────────────────────────────────────┐\n");
    printf("│  TERRITÓRIO #%d                                            │\n", numero);
    printf("└────────────────────────────────────────────────────────────┘\n");
    
    // ========================================================================
    // ENTRADA: NOME DO TERRITÓRIO
    // ========================================================================
    printf("  📍 Nome do território: ");
    
    /*
     * fgets é preferível a scanf para strings pois:
     * - Permite espaços no nome
     * - Limita o tamanho da entrada
     * - É mais seguro contra buffer overflow
     */
    fgets(t->nome, sizeof(t->nome), stdin);
    
    // Remove o caractere de nova linha '\n' se presente
    size_t len = strlen(t->nome);
    if (len > 0 && t->nome[len - 1] == '\n') {
        t->nome[len - 1] = '\0';
    }
    
    // ========================================================================
    // ENTRADA: NOME DO COMANDANTE/DONO
    // ========================================================================
    printf("  👑 Nome do comandante: ");
    fgets(t->dono, sizeof(t->dono), stdin);
    
    // Remove nova linha
    len = strlen(t->dono);
    if (len > 0 && t->dono[len - 1] == '\n') {
        t->dono[len - 1] = '\0';
    }
    
    // ========================================================================
    // ENTRADA: COR DO EXÉRCITO
    // ========================================================================
    printf("  🎨 Cor do exército: ");
    fgets(t->cor, sizeof(t->cor), stdin);
    
    // Remove nova linha
    len = strlen(t->cor);
    if (len > 0 && t->cor[len - 1] == '\n') {
        t->cor[len - 1] = '\0';
    }
    
    // Converte primeira letra para maiúscula (padronização)
    if (t->cor[0] != '\0') {
        t->cor[0] = toupper((unsigned char)t->cor[0]);
    }
    
    // ========================================================================
    // ENTRADA: NÚMERO DE TROPAS (COM VALIDAÇÃO)
    // ========================================================================
    do {
        printf("  ⚔️  Número de tropas: ");
        
        // Lê a entrada como string primeiro
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            // Tenta converter para inteiro
            int resultado = sscanf(buffer, "%d", &t->tropas);
            
            // Valida se a conversão foi bem-sucedida e se o valor é válido
            if (resultado == 1 && t->tropas >= 0) {
                valido = 1;  // Entrada válida
            } else {
                printf("  ❌ Erro: Digite um número inteiro não-negativo!\n");
                valido = 0;
            }
        }
    } while (!valido);  // Repete até entrada válida
    
    printf("  ✅ Território cadastrado!\n");
}

/*
 * Função: exibirTerritorio
 * Parâmetros:
 *   - t: ponteiro constante para a struct Territorio a ser exibida
 *   - numero: número do território para identificação
 * 
 * Descrição: Exibe os dados de um único território formatado
 */
void exibirTerritorio(const Territorio *t, int numero) {
    printf("┌────────────────────────────────────────────────────────────┐\n");
    printf("│  🏰 TERRITÓRIO #%d                                         │\n", numero);
    printf("├────────────────────────────────────────────────────────────┤\n");
    printf("│  📍 Nome:     %-43s │\n", t->nome);
    printf("│  👑 Dono:     %-43s │\n", t->dono);
    printf("│  🎨 Cor:      %-43s │\n", t->cor);
    printf("│  ⚔️  Tropas:   %-43d │\n", t->tropas);
    printf("└────────────────────────────────────────────────────────────┘\n");
}

/*
 * Função: exibirTodosTeritorios
 * Parâmetros:
 *   - territorios: array de structs Territorio
 *   - total: número total de territórios no array
 * 
 * Descrição: Percorre o array e exibe todos os territórios cadastrados.
 *            Também calcula e exibe estatísticas gerais.
 */
void exibirTodosTeritorios(const Territorio territorios[], int total) {
    int total_tropas = 0;  // Contador de tropas totais
    
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║              📊 RELATÓRIO DE TERRITÓRIOS                   ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    /*
     * Loop para exibir cada território.
     * Utiliza const para garantir que os dados não sejam modificados.
     */
    for (int i = 0; i < total; i++) {
        exibirTerritorio(&territorios[i], i + 1);
        total_tropas += territorios[i].tropas;  // Acumula tropas
        printf("\n");
    }
    
}

/*
 * Função: limparTela
 * Descrição: Limpa a tela do terminal (multiplataforma)
 */
void limparTela(void) {
    #ifdef _WIN32
        system("cls");    // Windows
    #else
        system("clear");  // Linux/macOS
    #endif
}

/*
 * Função: aguardarEnter
 * Descrição: Aguarda o usuário pressionar Enter para continuar
 */
void aguardarEnter(void) {
    printf("\n");
    printf("  ⏎  Pressione ENTER para visualizar os territórios...");
    LIMPAR_BUFFER;
}