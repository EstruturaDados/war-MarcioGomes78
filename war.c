/*
 * ============================================================================
 * Programa: Sistema de Cadastro de Territórios
 * Autor: Marcio Gomes
 * Data: 2025-10-30
 * Descrição: Sistema para cadastrar e exibir informações de territórios
 *            utilizando estruturas de dados compostas (structs).
 * ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

// ============================================================================
// DEFINIÇÃO DA ESTRUTURA
// ============================================================================

/*
 * Struct: Territorio
 * 
 * Representa um território no jogo/sistema com suas características principais:
 * - nome: identificação do território (até 29 caracteres + '\0')
 * - cor: cor do exército que ocupa o território (até 9 caracteres + '\0')
 * - tropas: quantidade de soldados presentes no território
 */
typedef struct {
    char nome[30];   // Nome do território
    char cor[10];    // Cor do exército ocupante
    int tropas;      // Número de tropas
} Territorio;

// ============================================================================
// CONSTANTES
// ============================================================================

#define TOTAL_TERRITORIOS 5      // Número total de territórios a cadastrar
#define LIMPAR_BUFFER while(getchar() != '\n')  // Macro para limpar buffer

// ============================================================================
// PROTÓTIPOS DE FUNÇÕES
// ============================================================================

void exibirCabecalho(void);
void cadastrarTerritorio(Territorio *t, int numero);
void exibirTerritorio(const Territorio *t, int numero);
void exibirTodosTeritorios(const Territorio territorios[], int total);
void limparTela(void);
void aguardarEnter(void);

// ============================================================================
// FUNÇÃO PRINCIPAL
// ============================================================================

int main(void) {
    // Declaração do vetor de structs para armazenar os territórios
    Territorio territorios[TOTAL_TERRITORIOS];
    
    // Exibir cabeçalho inicial
    limparTela();
    exibirCabecalho();
    
    // ========================================================================
    // FASE 1: CADASTRO DOS TERRITÓRIOS
    // ========================================================================
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║          CADASTRO DE TERRITÓRIOS - FASE DE ENTRADA        ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    /*
     * Laço de repetição para cadastrar todos os territórios.
     * Utiliza índice i para iterar de 0 até (TOTAL_TERRITORIOS - 1).
     * O endereço de cada território é passado para a função de cadastro.
     */
    for (int i = 0; i < TOTAL_TERRITORIOS; i++) {
        cadastrarTerritorio(&territorios[i], i + 1);
        printf("\n");
    }
    
    // ========================================================================
    // FASE 2: EXIBIÇÃO DOS TERRITÓRIOS CADASTRADOS
    // ========================================================================
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║              TERRITÓRIOS CADASTRADOS COM SUCESSO           ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    aguardarEnter();
    limparTela();
    
    // Exibir todos os territórios cadastrados
    exibirTodosTeritorios(territorios, TOTAL_TERRITORIOS);
    
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                    FIM DO PROGRAMA                         ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    return 0;
}

// ============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES
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