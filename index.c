// Desafio: Posicionando Navios no Tabuleiro (nível novato)
// Autor: Frederico Lisboa  |  
// Compilar:  gcc index.c -o batalha.exe
// Executar: ./batalha.exe

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// -------------------- Configurações fixas do nível básico --------------------
#define TAM     10      // tamanho do tabuleiro (10x10)
#define NAVIO_T 3       // tamanho de cada navio (3 células)
#define MAR     0       // valor para água
#define NAVIO   3       // valor para parte de navio

// Coordenadas iniciais (linha e coluna) definidas diretamente no código.
// Ajuste aqui se quiser testar outros casos.
#define H_INICIO_LINHA 2
#define H_INICIO_COL   1
#define V_INICIO_LINHA 5
#define V_INICIO_COL   7

// Orientação do navio
typedef enum {
    HORIZONTAL = 0,
    VERTICAL   = 1
} Orientacao;

// -------------------- Funções utilitárias --------------------

// zera todo o tabuleiro com MAR (0)
void inicializarTabuleiro(int tab[TAM][TAM]) {
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            tab[i][j] = MAR;
        }
    }
}

// checa se um navio cabe nos limites do tabuleiro, dadas a posição inicial
bool cabeNoTabuleiro(int linha, int col, Orientacao ori) {
    if (linha < 0 || linha >= TAM || col < 0 || col >= TAM) return false;

    if (ori == HORIZONTAL) {
        return (col + NAVIO_T - 1) < TAM;
    } else { 
        return (linha + NAVIO_T - 1) < TAM;
    }
}

// checa se as células onde o navio ficaria estão livres (== MAR)
bool semSobreposicao(int tab[TAM][TAM], int linha, int col, Orientacao ori) {
    if (ori == HORIZONTAL) {
        for (int k = 0; k < NAVIO_T; k++) {
            if (tab[linha][col + k] != MAR) return false;
        }
    } else { 
        for (int k = 0; k < NAVIO_T; k++) {
            if (tab[linha + k][col] != MAR) return false;
        }
    }
    return true;
}

// "copia" o conteúdo do vetor do navio para o tabuleiro (valor 3 em NAVIO_T células)
bool posicionarNavio(int tab[TAM][TAM], const int navio[NAVIO_T],
                     int linha, int col, Orientacao ori)
{
    // 1) validar limites
    if (!cabeNoTabuleiro(linha, col, ori)) {
        return false;
    }
    // 2) validar sobreposição
    if (!semSobreposicao(tab, linha, col, ori)) {
        return false;
    }
    // 3) posicionar (copiar os 3 "3"s do vetor para a matriz)
    if (ori == HORIZONTAL) {
        for (int k = 0; k < NAVIO_T; k++) {
            tab[linha][col + k] = navio[k];
        }
    } else { // VERTICAL
        for (int k = 0; k < NAVIO_T; k++) {
            tab[linha + k][col] = navio[k];
        }
    }
    return true;
}

// imprime o tabuleiro com espaçamento para visualização
void imprimirTabuleiro(int tab[TAM][TAM]) {
    printf("    ");
    for (int j = 0; j < TAM; j++) printf("%2d ", j);
    printf("\n");

    printf("   ");
    for (int j = 0; j < TAM; j++) printf("---");
    printf("-\n");

    for (int i = 0; i < TAM; i++) {
        printf("%2d |", i); 
        for (int j = 0; j < TAM; j++) {
            printf(" %d ", tab[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    // -------------------- 1) Tabuleiro --------------------
    int tabuleiro[TAM][TAM];
    inicializarTabuleiro(tabuleiro);

    // -------------------- 2) Navios como vetores --------------------
    // Cada navio tem 3 posições com o valor 3 (NAVIO)
    int navio_horizontal[NAVIO_T] = { NAVIO, NAVIO, NAVIO };
    int navio_vertical[NAVIO_T]   = { NAVIO, NAVIO, NAVIO };

    // -------------------- 3) Posicionamento --------------------
    // Posiciona o horizontal
    if (!posicionarNavio(tabuleiro, navio_horizontal,
                         H_INICIO_LINHA, H_INICIO_COL, HORIZONTAL)) {
        fprintf(stderr, "ERRO: navio horizontal fora dos limites ou sobreposto.\n");
        return EXIT_FAILURE;
    }

    // Posiciona o vertical
    if (!posicionarNavio(tabuleiro, navio_vertical,
                         V_INICIO_LINHA, V_INICIO_COL, VERTICAL)) {
        fprintf(stderr, "ERRO: navio vertical fora dos limites ou sobreposto.\n");
        return EXIT_FAILURE;
    }

    // -------------------- 4) Exibição --------------------
    imprimirTabuleiro(tabuleiro);

    return EXIT_SUCCESS;
}
