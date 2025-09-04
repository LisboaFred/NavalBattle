// Compilar:  gcc index.c -o batalha.exe
// Executar:  ./batalha.exe

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// -------------------- Configurações --------------------
#define TAM     10      // tamanho do tabuleiro (10x10)
#define NAVIO_T 3       // tamanho fixo do navio (3 células)
#define MAR     0       // representa água
#define NAVIO   3       // representa parte de navio

// Coordenadas iniciais dos 4 navios (linha e coluna, base 0)
#define H_INICIO_LINHA 2
#define H_INICIO_COL   1   

#define V_INICIO_LINHA 5
#define V_INICIO_COL   7   

#define DSE_INICIO_LINHA 0
#define DSE_INICIO_COL   6  

#define DNE_INICIO_LINHA 8
#define DNE_INICIO_COL   1   

// -------------------- Tipos --------------------
typedef enum {
    HORIZONTAL = 0,  // →
    VERTICAL   = 1,  // ↓
    DIAG_SE    = 2,  // ↘
    DIAG_NE    = 3   // ↗
} Orientacao;

// -------------------- Utilitárias --------------------
void inicializarTabuleiro(int tab[TAM][TAM]) {
    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            tab[i][j] = MAR;
        }
    }
}

// Verifica se, a partir de (linha,col) e orientação, o navio de tamanho NAVIO_T cabe no tabuleiro
bool cabeNoTabuleiro(int linha, int col, Orientacao ori) {
    if (linha < 0 || col < 0 || linha >= TAM || col >= TAM) return false;

    switch (ori) {
        case HORIZONTAL: return (col + NAVIO_T - 1) < TAM;
        case VERTICAL:   return (linha + NAVIO_T - 1) < TAM;
        case DIAG_SE:    return (linha + NAVIO_T - 1) < TAM && (col + NAVIO_T - 1) < TAM;
        case DIAG_NE:    return (linha - (NAVIO_T - 1)) >= 0 && (col + NAVIO_T - 1) < TAM;
        default:         return false;
    }
}

// Checa se as células onde o navio ficaria estão livres (== MAR)
bool semSobreposicao(int tab[TAM][TAM], int linha, int col, Orientacao ori) {
    for (int k = 0; k < NAVIO_T; k++) {
        int r = linha, c = col;
        if (ori == HORIZONTAL)         { r = linha;        c = col + k; }
        else if (ori == VERTICAL)      { r = linha + k;    c = col;     }
        else if (ori == DIAG_SE)       { r = linha + k;    c = col + k; }
        else    { r = linha - k;    c = col + k; }

        if (tab[r][c] != MAR) return false;
    }
    return true;
}

// Copia o valor NAVIO nas células alvo
bool posicionarNavio(int tab[TAM][TAM], int linha, int col, Orientacao ori) {
    if (!cabeNoTabuleiro(linha, col, ori))  return false;
    if (!semSobreposicao(tab, linha, col, ori)) return false;

    for (int k = 0; k < NAVIO_T; k++) {
        int r = linha, c = col;
        if (ori == HORIZONTAL)         { r = linha;        c = col + k; }
        else if (ori == VERTICAL)      { r = linha + k;    c = col;     }
        else if (ori == DIAG_SE)       { r = linha + k;    c = col + k; }
        else { r = linha - k;    c = col + k; }
        tab[r][c] = NAVIO;
    }
    return true;
}

void imprimirTabuleiro(int tab[TAM][TAM]) {
    printf("    ");
    for (int j = 0; j < TAM; j++) printf("%2d ", j);
    printf("\n   ");
    for (int j = 0; j < TAM; j++) printf("---");
    printf("-\n");

    for (int i = 0; i < TAM; i++) {
        printf("%2d |", i);
        for (int j = 0; j < TAM; j++) printf(" %d ", tab[i][j]);
        printf("\n");
    }
}

// -------------------- Principal --------------------
int main(void) {
    int tab[TAM][TAM];
    inicializarTabuleiro(tab);

    // 1) Horizontal →
    if (!posicionarNavio(tab, H_INICIO_LINHA, H_INICIO_COL, HORIZONTAL)) {
        fprintf(stderr, "ERRO: navio horizontal fora dos limites ou sobreposto.\n");
        return EXIT_FAILURE;
    }
    // 2) Vertical ↓
    if (!posicionarNavio(tab, V_INICIO_LINHA, V_INICIO_COL, VERTICAL)) {
        fprintf(stderr, "ERRO: navio vertical fora dos limites ou sobreposto.\n");
        return EXIT_FAILURE;
    }
    // 3) Diagonal ↘
    if (!posicionarNavio(tab, DSE_INICIO_LINHA, DSE_INICIO_COL, DIAG_SE)) {
        fprintf(stderr, "ERRO: navio diagonal ↘ fora dos limites ou sobreposto.\n");
        return EXIT_FAILURE;
    }
    // 4) Diagonal ↗
    if (!posicionarNavio(tab, DNE_INICIO_LINHA, DNE_INICIO_COL, DIAG_NE)) {
        fprintf(stderr, "ERRO: navio diagonal ↗ fora dos limites ou sobreposto.\n");
        return EXIT_FAILURE;
    }

    imprimirTabuleiro(tab);
    return EXIT_SUCCESS;
}
