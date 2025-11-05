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

/*
 * Struct: Jogador
 * 
 * Representa um jogador no sistema de batalha estratégica:
 * - nome: nome do jogador (até 29 caracteres + '\0')
 * - cor: cor que representa o jogador no mapa (até 9 caracteres + '\0')
 * - missao: ponteiro para string da missão (alocada dinamicamente)
 * - ativo: flag indicando se o jogador ainda está no jogo
 * - territoriosControlados: contador de territórios sob controle
 */
typedef struct {
    char nome[30];           // Nome do jogador
    char cor[10];           // Cor do jogador
    char *missao;           // Missão alocada dinamicamente
    bool ativo;             // Status do jogador (ativo/eliminado)
    int territoriosControlados; // Número de territórios controlados
} Jogador;

// ============================================================================
// CONSTANTES
// ============================================================================

#define BUFFER_SIZE 100          // Tamanho padrão para buffers de entrada
#define MIN_TERRITORIOS 5        // Mínimo de territórios para batalha estratégica
#define MAX_TERRITORIOS 20       // Máximo de territórios suportados
#define MAX_NOME 30             // Tamanho máximo para nomes
#define MAX_JOGADORES 6         // Máximo de jogadores simultâneos
#define MIN_JOGADORES 2         // Mínimo de jogadores para o jogo
#define TOTAL_MISSOES 8         // Total de missões disponíveis
#define MAX_MISSAO 200          // Tamanho máximo da string de missão
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
int obterNumeroJogadores(void);

// Funções de alocação e liberação de memória
Territorio* alocarTerritorios(int quantidade);
Jogador* alocarJogadores(int quantidade);
void liberarMemoriaCompleta(Territorio* mapa, Jogador* jogadores, int numJogadores);

// Funções de missões estratégicas
void inicializarMissoes(char missoes[][MAX_MISSAO]);
void atribuirMissao(char* destino, char missoes[][MAX_MISSAO], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanho, const char* corJogador);
void exibirMissao(const char* missao, const char* nomeJogador);
void exibirTodasMissoes(Jogador* jogadores, int numJogadores);

// Funções de jogadores
void cadastrarJogadores(Jogador* jogadores, int numJogadores, char missoes[][MAX_MISSAO]);
void distribuirTerritorios(Territorio* mapa, int numTerritorios, Jogador* jogadores, int numJogadores);
void atualizarEstatisticasJogadores(Jogador* jogadores, int numJogadores, Territorio* mapa, int numTerritorios);
int verificarVencedor(Jogador* jogadores, int numJogadores, Territorio* mapa, int numTerritorios);

// Funções de cadastro e exibição de territórios
void cadastrarTerritorio(Territorio *t, int numero);
void exibirTerritorio(const Territorio *t, int numero);
void exibirTodosTeritorios(const Territorio *territorios, int total);
void exibirMapaSimplificado(const Territorio *territorios, int total);

// Funções de batalha e simulação
bool atacar(Territorio* atacante, Territorio* defensor);
int simularDado(void);
void executarBatalhaMultiplayer(Territorio *mapa, int numTerritorios, Jogador* jogadores, int numJogadores);
bool validarAtaque(const Territorio* atacante, const Territorio* defensor);

// Funções utilitárias
void limparTela(void);
void aguardarEnter(void);
void exibirEstatisticas(Territorio *mapa, int numTerritorios);

// ============================================================================
// FUNÇÃO PRINCIPAL
// ============================================================================

/**
 * Função principal - Sistema de Batalha Estratégica com Missões
 * 
 * Implementa o sistema completo de guerra estratégica entre múltiplos
 * jogadores, cada um com sua missão específica para vencer. Utiliza
 * alocação dinâmica, sistema de missões, e validação de ataques.
 * 
 * Fluxo do programa:
 *   1. Inicialização (semente aleatória e sistema de missões)
 *   2. Configuração (número de jogadores e territórios)
 *   3. Alocação dinâmica de memória
 *   4. Cadastro de jogadores e atribuição de missões
 *   5. Cadastro e distribuição automática de territórios
 *   6. Loop principal de batalhas com verificação de vitória
 *   7. Liberação completa de memória
 * 
 * @return 0 se execução foi bem-sucedida, 1 em caso de erro
 */
int main(void) {
    // Variáveis principais do jogo
    Territorio *mapa = NULL;        // Array de territórios (alocação dinâmica)
    Jogador *jogadores = NULL;      // Array de jogadores (alocação dinâmica)
    int numTerritorios = 0;         // Número total de territórios
    int numJogadores = 0;           // Número total de jogadores
    
    // Array de missões disponíveis (alocação estática)
    char missoes[TOTAL_MISSOES][MAX_MISSAO];
    
    // ========================================================================
    // FASE 1: INICIALIZAÇÃO DO SISTEMA
    // ========================================================================
    limparTela();
    exibirCabecalho();
    inicializarSistema();
    
    // ========================================================================
    // FASE 2: INICIALIZAÇÃO DO SISTEMA DE MISSÕES
    // ========================================================================
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║               SISTEMA DE MISSÕES ESTRATÉGICAS             ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    inicializarMissoes(missoes);
    aguardarEnter();
    
    // ========================================================================
    // FASE 3: CONFIGURAÇÃO DE JOGADORES E TERRITÓRIOS
    // ========================================================================
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                  CONFIGURAÇÃO DO JOGO                     ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    // Obter configurações do usuário
    numJogadores = obterNumeroJogadores();
    numTerritorios = obterNumeroTerritorios();
    
    // Validar configurações
    if (numTerritorios < numJogadores) {
        printf("❌ Erro: Número de territórios deve ser >= número de jogadores!\n");
        printf("   💡 Ajuste: %d territórios para %d jogadores.\n", numJogadores, numJogadores);
        numTerritorios = numJogadores + 2; // Mínimo viável
    }
    
    // ========================================================================
    // FASE 4: ALOCAÇÃO DINÂMICA DE MEMÓRIA
    // ========================================================================
    printf("\n📊 Alocando memória dinamicamente...\n");
    
    // Alocar territórios
    mapa = alocarTerritorios(numTerritorios);
    if (mapa == NULL) {
        printf("❌ Falha crítica na alocação de territórios!\n");
        return 1;
    }
    
    // Alocar jogadores
    jogadores = alocarJogadores(numJogadores);
    if (jogadores == NULL) {
        printf("❌ Falha crítica na alocação de jogadores!\n");
        free(mapa); // Limpar memória já alocada
        return 1;
    }
    
    printf("✅ Alocação bem-sucedida!\n");
    printf("   🏰 Territórios: %d (%zu bytes)\n", numTerritorios, numTerritorios * sizeof(Territorio));
    printf("   � Jogadores: %d (%zu bytes)\n", numJogadores, numJogadores * sizeof(Jogador));
    
    // ========================================================================
    // FASE 5: CADASTRO DE JOGADORES E ATRIBUIÇÃO DE MISSÕES
    // ========================================================================
    aguardarEnter();
    limparTela();
    
    cadastrarJogadores(jogadores, numJogadores, missoes);
    
    printf("\n🎯 Exibindo missões atribuídas:\n");
    exibirTodasMissoes(jogadores, numJogadores);
    
    // ========================================================================
    // FASE 6: CADASTRO E DISTRIBUIÇÃO DE TERRITÓRIOS
    // ========================================================================
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║               CADASTRO DE TERRITÓRIOS                     ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    // Cadastro básico dos territórios (apenas nomes)
    for (int i = 0; i < numTerritorios; i++) {
        printf("🏰 Nome do território %d: ", i + 1);
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        
        // Remove quebra de linha
        size_t len = strlen(mapa[i].nome);
        if (len > 0 && mapa[i].nome[len - 1] == '\n') {
            mapa[i].nome[len - 1] = '\0';
        }
    }
    
    // Distribuição automática entre jogadores
    distribuirTerritorios(mapa, numTerritorios, jogadores, numJogadores);
    
    // ========================================================================
    // FASE 7: EXIBIÇÃO DO ESTADO INICIAL DO JOGO
    // ========================================================================
    aguardarEnter();
    limparTela();
    
    printf("\n🗺️ MAPA INICIAL DO JOGO:\n");
    exibirTodosTeritorios(mapa, numTerritorios);
    atualizarEstatisticasJogadores(jogadores, numJogadores, mapa, numTerritorios);
    
    // ========================================================================
    // FASE 8: LOOP PRINCIPAL DE BATALHAS COM VERIFICAÇÃO DE MISSÕES
    // ========================================================================
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║              INÍCIO DA GUERRA ESTRATÉGICA                 ║\n");
    printf("║                                                            ║\n");
    printf("║  � Cada jogador tem uma missão específica para vencer    ║\n");
    printf("║  🎲 Batalhas decididas por dados (1-6)                    ║\n");
    printf("║  ⚔️  Atacante vence: transfere cor e metade das tropas     ║\n");
    printf("║  🛡️  Defensor vence: atacante perde 1 tropa               ║\n");
    printf("║  🚫 Só pode atacar territórios inimigos                   ║\n");
    printf("║                                                            ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    aguardarEnter();
    
    // Loop principal do jogo
    int turno = 1;
    int vencedor = -1;
    char continuar;
    
    do {
        limparTela();
        printf("🔄 ═══════════════════════════════════════════════════════════\n");
        printf("                        TURNO %d\n", turno);
        printf("═══════════════════════════════════════════════════════════🔄\n");
        
        // Mostrar estado atual
        printf("\n📊 SITUAÇÃO ATUAL DOS JOGADORES:\n");
        for (int i = 0; i < numJogadores; i++) {
            if (jogadores[i].ativo) {
                printf("👤 %s (%s): %d territórios\n", 
                       jogadores[i].nome, jogadores[i].cor, jogadores[i].territoriosControlados);
            }
        }
        
        // Executar uma rodada de batalha
        executarBatalhaMultiplayer(mapa, numTerritorios, jogadores, numJogadores);
        
        // Atualizar estatísticas
        atualizarEstatisticasJogadores(jogadores, numJogadores, mapa, numTerritorios);
        
        // Verificar se alguém cumpriu sua missão
        vencedor = verificarVencedor(jogadores, numJogadores, mapa, numTerritorios);
        
        if (vencedor != -1) {
            // Anunciar vencedor
            printf("\n🏆 ═══════════════════════════════════════════════════════════\n");
            printf("                      TEMOS UM VENCEDOR!\n");
            printf("═══════════════════════════════════════════════════════════🏆\n");
            printf("🎉 %s cumpriu sua missão e venceu o jogo!\n", jogadores[vencedor].nome);
            printf("🎯 Missão: %s\n", jogadores[vencedor].missao);
            printf("🏰 Territórios controlados: %d\n", jogadores[vencedor].territoriosControlados);
            break;
        }
        
        // Verificar se ainda há jogadores ativos
        int jogadoresAtivos = 0;
        for (int i = 0; i < numJogadores; i++) {
            if (jogadores[i].ativo) jogadoresAtivos++;
        }
        
        if (jogadoresAtivos <= 1) {
            printf("\n🏁 Jogo terminado - apenas 1 jogador restante!\n");
            break;
        }
        
        // Perguntar se quer continuar
        printf("\n🎮 Continuar para o próximo turno? (s/N): ");
        while (getchar() != '\n'); // Limpar buffer
        continuar = getchar();
        turno++;
        
    } while (continuar == 's' || continuar == 'S');
    
    // ========================================================================
    // FASE 9: RELATÓRIO FINAL E LIBERAÇÃO DE MEMÓRIA
    // ========================================================================
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                    RELATÓRIO FINAL                        ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    printf("🏁 Jogo finalizado após %d turnos\n", turno - 1);
    printf("📊 Estado final dos jogadores:\n");
    
    for (int i = 0; i < numJogadores; i++) {
        printf("👤 %s (%s): %s - %d territórios\n", 
               jogadores[i].nome, jogadores[i].cor, 
               jogadores[i].ativo ? "ATIVO" : "ELIMINADO", 
               jogadores[i].territoriosControlados);
    }
    
    // Liberação completa de memória
    printf("\n");
    liberarMemoriaCompleta(mapa, jogadores, numJogadores);
    
    printf("\n� Obrigado por jogar o Sistema de Guerra Estratégica!\n");
    printf("📚 Este programa demonstra:\n");
    printf("   • Alocação dinâmica com malloc/calloc\n");
    printf("   • Sistema de missões com ponteiros\n");
    printf("   • Validação de ataques entre inimigos\n");
    printf("   • Gerenciamento completo de memória (free)\n");
    printf("   • Passagem por valor e referência\n");
    printf("   • Modularização e organização de código\n");
    
    return 0;  // Sucesso
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
        printf("   %s conquista %s!\n", atacante->dono, defensor->nome);
        
        // Calcular transferência de tropas (metade das tropas do atacante)
        int tropasTranferidas = atacante->tropas / 2;
        if (tropasTranferidas == 0) tropasTranferidas = 1; // Mínimo 1 tropa
        
        // Transferir cor e tropas conforme especificado
        strcpy(defensor->cor, atacante->cor);
        strcpy(defensor->dono, atacante->dono);
        defensor->tropas = tropasTranferidas;
        atacante->tropas -= tropasTranferidas;
        
        printf("   🔄 Transferindo controle...\n");
        printf("   📊 %s transferiu %d tropas para %s\n", 
               atacante->nome, tropasTranferidas, defensor->nome);
        printf("   🏴 %s mantém %d tropas\n", atacante->nome, atacante->tropas);
        
        return true;
    } 
    else if (dadoDefensor > dadoAtacante) {
        // Defensor vence - atacante perde uma tropa
        printf("\n🛡️ VITÓRIA DO DEFENSOR!\n");
        printf("   %s defendeu com sucesso!\n", defensor->nome);
        
        if (atacante->tropas > 1) {
            atacante->tropas--;
            printf("   💀 %s perde 1 tropa (restam: %d)\n", 
                   atacante->nome, atacante->tropas);
        }
        
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
    printf("  ⏎  Pressione ENTER para continuar...");
    LIMPAR_BUFFER;
}

// ============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES - SISTEMA DE MISSÕES ESTRATÉGICAS
// ============================================================================

/**
 * Inicializa o vetor de missões pré-definidas
 * 
 * Esta função preenche um vetor com diferentes tipos de missões
 * estratégicas que serão sorteadas para os jogadores. Cada missão
 * representa um objetivo específico que deve ser cumprido para vencer.
 * 
 * @param missoes Array de strings onde serão armazenadas as missões
 */
void inicializarMissoes(char missoes[][MAX_MISSAO]) {
    strcpy(missoes[0], "CONQUISTADOR: Controle pelo menos 5 territórios simultaneamente");
    strcpy(missoes[1], "DOMINAÇÃO TOTAL: Elimine completamente 1 jogador (capture todos seus territórios)");
    strcpy(missoes[2], "ESTRATEGISTA: Mantenha 3 territórios com mais de 5 tropas cada por 2 turnos");
    strcpy(missoes[3], "EXPANSIONISTA: Conquiste 4 territórios em sequência sem perder nenhum");
    strcpy(missoes[4], "GENERAL SUPREMO: Acumule mais de 30 tropas distribuídas em seus territórios");
    strcpy(missoes[5], "LIBERTADOR: Conquiste territórios de pelo menos 3 jogadores diferentes");
    strcpy(missoes[6], "FORTALEZA: Defenda com sucesso 5 ataques consecutivos sem perder território");
    strcpy(missoes[7], "IMPERADOR: Controle mais da metade de todos os territórios do mapa");
    
    printf("🎯 Sistema de missões inicializado com %d objetivos estratégicos!\n", TOTAL_MISSOES);
}

/**
 * Atribui uma missão aleatória para um jogador
 * 
 * Sorteia uma missão do vetor de missões disponíveis e aloca
 * dinamicamente memória para armazenar a string da missão do jogador.
 * Utiliza malloc e strcpy conforme especificado nos requisitos.
 * 
 * @param destino Ponteiro para onde será armazenado o endereço da missão
 * @param missoes Array de missões disponíveis
 * @param totalMissoes Número total de missões no array
 */
void atribuirMissao(char* destino, char missoes[][MAX_MISSAO], int totalMissoes) {
    if (destino == NULL || missoes == NULL || totalMissoes <= 0) {
        printf("❌ Erro: Parâmetros inválidos para atribuição de missão!\n");
        return;
    }
    
    // Sorteia uma missão aleatória
    int indiceSorteado = rand() % totalMissoes;
    
    // Aloca memória dinamicamente para a missão
    char* missaoAlocada = (char*)malloc(MAX_MISSAO * sizeof(char));
    if (missaoAlocada == NULL) {
        printf("❌ Erro: Falha na alocação de memória para missão!\n");
        return;
    }
    
    // Copia a missão sorteada usando strcpy
    strcpy(missaoAlocada, missoes[indiceSorteado]);
    
    // Atribui o ponteiro para a missão alocada
    *(char**)&destino = missaoAlocada;
    
    printf("🎯 Missão sorteada e atribuída: Índice %d\n", indiceSorteado);
}

/**
 * Verifica se um jogador cumpriu sua missão
 * 
 * Analisa o estado atual do mapa e verifica se as condições
 * da missão do jogador foram satisfeitas. Implementa lógicas
 * específicas para diferentes tipos de objetivos.
 * 
 * @param missao String da missão a ser verificada
 * @param mapa Array de territórios do jogo
 * @param tamanho Número de territórios no mapa
 * @param corJogador Cor que identifica os territórios do jogador
 * @return 1 se missão foi cumprida, 0 caso contrário
 */
int verificarMissao(char* missao, Territorio* mapa, int tamanho, const char* corJogador) {
    if (missao == NULL || mapa == NULL || corJogador == NULL || tamanho <= 0) {
        return 0; // Parâmetros inválidos
    }
    
    // Contadores para análise do mapa
    int territoriosControlados = 0;
    int tropasTotais = 0;
    int territoriosComMais5Tropas = 0;
    
    // Análise do estado atual do jogador
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0) {
            territoriosControlados++;
            tropasTotais += mapa[i].tropas;
            if (mapa[i].tropas > 5) {
                territoriosComMais5Tropas++;
            }
        }
    }
    
    // Verificação baseada no conteúdo da missão (lógica simples inicial)
    if (strstr(missao, "CONQUISTADOR") != NULL) {
        return territoriosControlados >= 5;
    }
    else if (strstr(missao, "GENERAL SUPREMO") != NULL) {
        return tropasTotais > 30;
    }
    else if (strstr(missao, "ESTRATEGISTA") != NULL) {
        return territoriosComMais5Tropas >= 3;
    }
    else if (strstr(missao, "IMPERADOR") != NULL) {
        return territoriosControlados > (tamanho / 2);
    }
    else if (strstr(missao, "EXPANSIONISTA") != NULL) {
        return territoriosControlados >= 4;
    }
    
    // Missões mais complexas retornam false por enquanto (lógica simplificada)
    return 0;
}

/**
 * Exibe a missão de um jogador específico
 * 
 * Função para exibição que recebe a missão por valor,
 * conforme especificado nos requisitos não funcionais.
 * 
 * @param missao String da missão (passada por valor)
 * @param nomeJogador Nome do jogador dono da missão
 */
void exibirMissao(const char* missao, const char* nomeJogador) {
    if (missao == NULL || nomeJogador == NULL) {
        printf("❌ Erro: Dados inválidos para exibição de missão!\n");
        return;
    }
    
    printf("\n🎯 ═══════════════════════════════════════════════════════════\n");
    printf("                    MISSÃO ESTRATÉGICA\n");
    printf("═══════════════════════════════════════════════════════════🎯\n");
    printf("👤 Jogador: %s\n", nomeJogador);
    printf("🏆 Objetivo: %s\n", missao);
    printf("═════════════════════════════════════════════════════════════\n");
}

/**
 * Exibe todas as missões dos jogadores ativos
 * 
 * @param jogadores Array de jogadores
 * @param numJogadores Número de jogadores
 */
void exibirTodasMissoes(Jogador* jogadores, int numJogadores) {
    printf("\n📋 ═══════════════════════════════════════════════════════════\n");
    printf("                  MISSÕES DOS JOGADORES\n");
    printf("═══════════════════════════════════════════════════════════📋\n");
    
    for (int i = 0; i < numJogadores; i++) {
        if (jogadores[i].ativo && jogadores[i].missao != NULL) {
            printf("👤 %s (%s): %s\n", 
                   jogadores[i].nome, jogadores[i].cor, jogadores[i].missao);
            printf("─────────────────────────────────────────────────────────────\n");
        }
    }
}

// ============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES - GERENCIAMENTO DE JOGADORES
// ============================================================================

/**
 * Obtém o número de jogadores que participarão do jogo
 * 
 * @return Número válido de jogadores
 */
int obterNumeroJogadores(void) {
    int numero;
    bool entradaValida = false;
    
    printf("\n👥 Quantos jogadores irão participar? ");
    printf("(min: %d, máx: %d): ", MIN_JOGADORES, MAX_JOGADORES);
    
    do {
        if (scanf("%d", &numero) != 1) {
            printf("❌ Entrada inválida! Digite apenas números.\n");
            printf("👥 Quantos jogadores? (min: %d, máx: %d): ", 
                   MIN_JOGADORES, MAX_JOGADORES);
            while (getchar() != '\n');
            continue;
        }
        
        if (numero < MIN_JOGADORES || numero > MAX_JOGADORES) {
            printf("❌ Número inválido! Deve estar entre %d e %d.\n", 
                   MIN_JOGADORES, MAX_JOGADORES);
            printf("👥 Quantos jogadores? (min: %d, máx: %d): ", 
                   MIN_JOGADORES, MAX_JOGADORES);
            continue;
        }
        
        entradaValida = true;
    } while (!entradaValida);
    
    while (getchar() != '\n'); // Limpar buffer
    return numero;
}

/**
 * Aloca memória dinamicamente para array de jogadores
 * 
 * @param quantidade Número de jogadores a serem alocados
 * @return Ponteiro para array alocado, ou NULL se falhou
 */
Jogador* alocarJogadores(int quantidade) {
    if (quantidade <= 0) {
        printf("❌ Erro: Quantidade inválida de jogadores (%d).\n", quantidade);
        return NULL;
    }
    
    Jogador *jogadores = (Jogador*)calloc(quantidade, sizeof(Jogador));
    
    if (jogadores == NULL) {
        printf("❌ Erro crítico: Falha na alocação de memória para jogadores!\n");
        return NULL;
    }
    
    printf("👥 Alocação bem-sucedida: %d jogadores\n", quantidade);
    return jogadores;
}

/**
 * Cadastra todos os jogadores e atribui missões
 * 
 * @param jogadores Array de jogadores
 * @param numJogadores Número de jogadores
 * @param missoes Array de missões disponíveis
 */
void cadastrarJogadores(Jogador* jogadores, int numJogadores, char missoes[][MAX_MISSAO]) {
    const char* cores[] = {"Vermelho", "Azul", "Verde", "Amarelo", "Roxo", "Laranja"};
    
    printf("\n👤 ═══════════════════════════════════════════════════════════\n");
    printf("                  CADASTRO DE JOGADORES\n");
    printf("═══════════════════════════════════════════════════════════👤\n");
    
    for (int i = 0; i < numJogadores; i++) {
        printf("\n--- JOGADOR %d ---\n", i + 1);
        printf("Nome: ");
        fgets(jogadores[i].nome, sizeof(jogadores[i].nome), stdin);
        
        // Remove quebra de linha
        size_t len = strlen(jogadores[i].nome);
        if (len > 0 && jogadores[i].nome[len - 1] == '\n') {
            jogadores[i].nome[len - 1] = '\0';
        }
        
        // Atribui cor automaticamente
        strcpy(jogadores[i].cor, cores[i % 6]);
        
        // Inicializa status
        jogadores[i].ativo = true;
        jogadores[i].territoriosControlados = 0;
        
        // Aloca e atribui missão
        jogadores[i].missao = (char*)malloc(MAX_MISSAO * sizeof(char));
        if (jogadores[i].missao != NULL) {
            int indiceMissao = rand() % TOTAL_MISSOES;
            strcpy(jogadores[i].missao, missoes[indiceMissao]);
            printf("🎯 Missão atribuída: %s\n", jogadores[i].missao);
        }
        
        printf("🎨 Cor atribuída: %s\n", jogadores[i].cor);
        printf("✅ Jogador cadastrado!\n");
    }
}

/**
 * Valida se um ataque é permitido (apenas contra territórios inimigos)
 * 
 * @param atacante Território atacante
 * @param defensor Território defensor  
 * @return true se ataque é válido, false caso contrário
 */
bool validarAtaque(const Territorio* atacante, const Territorio* defensor) {
    if (atacante == NULL || defensor == NULL) {
        printf("❌ Erro: Territórios inválidos!\n");
        return false;
    }
    
    // Verificar se são territórios de cores diferentes (inimigos)
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("❌ Ataque inválido: Não pode atacar território da mesma cor!\n");
        printf("   🏴 %s (%s) não pode atacar %s (%s)\n", 
               atacante->nome, atacante->cor, defensor->nome, defensor->cor);
        return false;
    }
    
    // Verificar se atacante tem tropas suficientes
    if (atacante->tropas <= 1) {
        printf("❌ Ataque inválido: Tropas insuficientes!\n");
        printf("   🏴 %s tem apenas %d tropa(s) - mínimo necessário: 2\n", 
               atacante->nome, atacante->tropas);
        return false;
    }
    
    return true;
}

/**
 * Distribui territórios entre os jogadores no início do jogo
 * 
 * @param mapa Array de territórios
 * @param numTerritorios Número de territórios
 * @param jogadores Array de jogadores
 * @param numJogadores Número de jogadores
 */
void distribuirTerritorios(Territorio* mapa, int numTerritorios, Jogador* jogadores, int numJogadores) {
    printf("\n🗺️ ═══════════════════════════════════════════════════════════\n");
    printf("              DISTRIBUIÇÃO AUTOMÁTICA DE TERRITÓRIOS\n");
    printf("═══════════════════════════════════════════════════════════🗺️\n");
    
    // Distribui territórios de forma alternada entre jogadores
    for (int i = 0; i < numTerritorios; i++) {
        int jogadorAtual = i % numJogadores;
        
        // Atualizar cor e dono do território
        strcpy(mapa[i].cor, jogadores[jogadorAtual].cor);
        strcpy(mapa[i].dono, jogadores[jogadorAtual].nome);
        
        // Tropas iniciais aleatórias (2-6)
        mapa[i].tropas = (rand() % 5) + 2;
        
        printf("🏰 %s → %s (%s) - %d tropas\n", 
               mapa[i].nome, jogadores[jogadorAtual].nome, 
               jogadores[jogadorAtual].cor, mapa[i].tropas);
    }
    
    printf("✅ Distribuição concluída!\n");
}

/**
 * Atualiza estatísticas dos jogadores baseado no mapa atual
 * 
 * @param jogadores Array de jogadores
 * @param numJogadores Número de jogadores
 * @param mapa Array de territórios
 * @param numTerritorios Número de territórios
 */
void atualizarEstatisticasJogadores(Jogador* jogadores, int numJogadores, Territorio* mapa, int numTerritorios) {
    // Zera contadores
    for (int i = 0; i < numJogadores; i++) {
        jogadores[i].territoriosControlados = 0;
    }
    
    // Conta territórios por jogador
    for (int i = 0; i < numTerritorios; i++) {
        for (int j = 0; j < numJogadores; j++) {
            if (strcmp(mapa[i].cor, jogadores[j].cor) == 0) {
                jogadores[j].territoriosControlados++;
                break;
            }
        }
    }
    
    // Verifica se algum jogador foi eliminado
    for (int i = 0; i < numJogadores; i++) {
        if (jogadores[i].territoriosControlados == 0 && jogadores[i].ativo) {
            jogadores[i].ativo = false;
            printf("💀 %s foi eliminado do jogo!\n", jogadores[i].nome);
        }
    }
}

/**
 * Verifica se algum jogador cumpriu sua missão e venceu
 * 
 * @param jogadores Array de jogadores
 * @param numJogadores Número de jogadores
 * @param mapa Array de territórios
 * @param numTerritorios Número de territórios
 * @return Índice do jogador vencedor, ou -1 se ninguém venceu
 */
int verificarVencedor(Jogador* jogadores, int numJogadores, Territorio* mapa, int numTerritorios) {
    for (int i = 0; i < numJogadores; i++) {
        if (jogadores[i].ativo && jogadores[i].missao != NULL) {
            if (verificarMissao(jogadores[i].missao, mapa, numTerritorios, jogadores[i].cor)) {
                return i; // Retorna índice do vencedor
            }
        }
    }
    return -1; // Nenhum vencedor ainda
}

// ============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES - GERENCIAMENTO COMPLETO DE MEMÓRIA
// ============================================================================

/**
 * Libera toda a memória alocada dinamicamente (territórios e missões)
 * 
 * @param mapa Array de territórios
 * @param jogadores Array de jogadores
 * @param numJogadores Número de jogadores
 */
void liberarMemoriaCompleta(Territorio* mapa, Jogador* jogadores, int numJogadores) {
    printf("\n🧹 ═══════════════════════════════════════════════════════════\n");
    printf("              LIBERAÇÃO DE MEMÓRIA\n");
    printf("═══════════════════════════════════════════════════════════🧹\n");
    
    // Liberar territórios
    if (mapa != NULL) {
        printf("🗑️  Liberando memória dos territórios...\n");
        free(mapa);
        printf("✅ Territórios liberados!\n");
    }
    
    // Liberar missões e jogadores
    if (jogadores != NULL) {
        printf("🗑️  Liberando memória dos jogadores e missões...\n");
        
        for (int i = 0; i < numJogadores; i++) {
            if (jogadores[i].missao != NULL) {
                free(jogadores[i].missao);
                jogadores[i].missao = NULL;
            }
        }
        
        free(jogadores);
        printf("✅ Jogadores e missões liberados!\n");
    }
    
    printf("🎉 Liberação de memória concluída com sucesso!\n");
}

/**
 * Executa uma rodada de batalha no modo multiplayer
 * 
 * @param mapa Array de territórios
 * @param numTerritorios Número de territórios
 * @param jogadores Array de jogadores
 * @param numJogadores Número de jogadores
 */
void executarBatalhaMultiplayer(Territorio *mapa, int numTerritorios, Jogador* jogadores, int numJogadores) {
    int indiceAtacante, indiceDefensor;
    
    printf("\n⚔️ ═══════════════════════════════════════════════════════════\n");
    printf("                    RODADA DE BATALHA\n");
    printf("═══════════════════════════════════════════════════════════⚔️\n");
    
    // Mostrar territórios disponíveis com cores
    printf("\n🗺️ TERRITÓRIOS DISPONÍVEIS:\n");
    for (int i = 0; i < numTerritorios; i++) {
        printf("   [%d] %s - %s (%s) - %d tropas\n", 
               i + 1, mapa[i].nome, mapa[i].dono, mapa[i].cor, mapa[i].tropas);
    }
    
    // Escolher atacante
    printf("\n🏴 Escolha o território ATACANTE (1-%d): ", numTerritorios);
    while (scanf("%d", &indiceAtacante) != 1 || 
           indiceAtacante < 1 || indiceAtacante > numTerritorios) {
        printf("❌ Índice inválido! Escolha entre 1 e %d: ", numTerritorios);
        while (getchar() != '\n');
    }
    indiceAtacante--; // Converter para índice 0-based
    
    // Escolher defensor
    printf("🏰 Escolha o território DEFENSOR (1-%d): ", numTerritorios);
    while (scanf("%d", &indiceDefensor) != 1 || 
           indiceDefensor < 1 || indiceDefensor > numTerritorios) {
        printf("❌ Índice inválido! Escolha entre 1 e %d: ", numTerritorios);
        while (getchar() != '\n');
    }
    indiceDefensor--; // Converter para índice 0-based
    
    // Verificar se são territórios diferentes
    if (indiceAtacante == indiceDefensor) {
        printf("❌ Um território não pode atacar a si mesmo!\n");
        aguardarEnter();
        return;
    }
    
    // Validar ataque (só contra inimigos)
    if (!validarAtaque(&mapa[indiceAtacante], &mapa[indiceDefensor])) {
        aguardarEnter();
        return;
    }
    
    // Executar batalha
    bool sucesso = atacar(&mapa[indiceAtacante], &mapa[indiceDefensor]);
    
    if (sucesso) {
        printf("🎊 Território conquistado com sucesso!\n");
    }
    
    aguardarEnter();
}