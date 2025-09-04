// Compilar:  gcc index.c -o batalha.exe
// Executar:  ./batalha.exe

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// -------------------- Configurações do tabuleiro e navios --------------------
#define TAM      10     // tamanho do tabuleiro (10x10)
#define NAVIO_T  3      // tamanho fixo do navio (3 células)
#define MAR      0      // representa água
#define NAVIO    3      // representa parte de navio

// Valor para "área afetada por habilidade"
#define HABIL    5

// -------------------- Configurações das habilidades --------------------
// tamanho da máscara (ímpar para ter centro bem definido)
#define MASK 5      

// Posições de origem (linha, coluna) para aplicar as habilidades no tabuleiro
#define ORIG_CONE_L  3
#define ORIG_CONE_C  5

#define ORIG_CRUZ_L  7
#define ORIG_CRUZ_C  2

#define ORIG_OCTA_L  6
#define ORIG_OCTA_C  6

// -------------------- Coordenadas iniciais dos 4 navios (base 0) --------------------
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

// -------------------- Utilitárias do tabuleiro --------------------
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
        if (ori == HORIZONTAL)      { r = linha;     c = col + k; }
        else if (ori == VERTICAL)   { r = linha + k; c = col;     }
        else if (ori == DIAG_SE)    { r = linha + k; c = col + k; }
        else /* DIAG_NE */          { r = linha - k; c = col + k; }

        if (tab[r][c] != MAR) return false;
    }
    return true;
}

// Copia o valor NAVIO nas células alvo
bool posicionarNavio(int tab[TAM][TAM], int linha, int col, Orientacao ori) {
    if (!cabeNoTabuleiro(linha, col, ori))       return false;
    if (!semSobreposicao(tab, linha, col, ori))  return false;

    for (int k = 0; k < NAVIO_T; k++) {
        int r = linha, c = col;
        if (ori == HORIZONTAL)      { r = linha;     c = col + k; }
        else if (ori == VERTICAL)   { r = linha + k; c = col;     }
        else if (ori == DIAG_SE)    { r = linha + k; c = col + k; }
        else /* DIAG_NE */          { r = linha - k; c = col + k; }
        tab[r][c] = NAVIO;
    }
    return true;
}

// Impressão do tabuleiro
void imprimirTabuleiro(int tab[TAM][TAM]) {
    printf("Legenda: 0=água  3=navio  5=habilidade\n\n");
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

// -------------------- Construção dinâmica das máscaras (0/1) --------------------

void construirCone(int m[MASK][MASK]) {
    int c = MASK / 2;
    for (int i = 0; i < MASK; i++) {
        for (int j = 0; j < MASK; j++) {
            if (i < c) {
                m[i][j] = 0;
            } else {
                int raio = i - c;         
                m[i][j] = (abs(j - c) <= raio) ? 1 : 0;
            }
        }
    }
}

// Cruz: linha e coluna centrais
void construirCruz(int m[MASK][MASK]) {
    int c = MASK / 2;
    for (int i = 0; i < MASK; i++) {
        for (int j = 0; j < MASK; j++) {
            m[i][j] = (i == c || j == c) ? 1 : 0;
        }
    }
}

// Octaedro (losango): |i-c| + |j-c| <= c
void construirOctaedro(int m[MASK][MASK]) {
    int c = MASK / 2;
    for (int i = 0; i < MASK; i++) {
        for (int j = 0; j < MASK; j++) {
            m[i][j] = (abs(i - c) + abs(j - c) <= c) ? 1 : 0;
        }
    }
}

// -------------------- Sobreposição no tabuleiro --------------------
void aplicarHabilidadeCentralizada(int tab[TAM][TAM],
                                   int mask[MASK][MASK],
                                   int orig_l, int orig_c)
{
    int c = MASK / 2;
    for (int i = 0; i < MASK; i++) {
        for (int j = 0; j < MASK; j++) {
            if (mask[i][j] == 1) {
                int r = orig_l + (i - c);
                int k = orig_c + (j - c);               
                if (r >= 0 && r < TAM && k >= 0 && k < TAM) {
                    if (tab[r][k] == MAR) {
                        tab[r][k] = HABIL;
                    }                    
                }
            }
        }
    }
}

// -------------------- Principal --------------------
int main(void) {
    int tab[TAM][TAM];
    inicializarTabuleiro(tab);

    // 1) Posiciona os quatro navios
    if (!posicionarNavio(tab, H_INICIO_LINHA, H_INICIO_COL, HORIZONTAL)) {
        fprintf(stderr, "ERRO: navio horizontal fora dos limites ou sobreposto.\n");
        return EXIT_FAILURE;
    }
    if (!posicionarNavio(tab, V_INICIO_LINHA, V_INICIO_COL, VERTICAL)) {
        fprintf(stderr, "ERRO: navio vertical fora dos limites ou sobreposto.\n");
        return EXIT_FAILURE;
    }
    if (!posicionarNavio(tab, DSE_INICIO_LINHA, DSE_INICIO_COL, DIAG_SE)) {
        fprintf(stderr, "ERRO: navio diagonal SE fora dos limites ou sobreposto.\n");
        return EXIT_FAILURE;
    }
    if (!posicionarNavio(tab, DNE_INICIO_LINHA, DNE_INICIO_COL, DIAG_NE)) {
        fprintf(stderr, "ERRO: navio diagonal NE fora dos limites ou sobreposto.\n");
        return EXIT_FAILURE;
    }

    // 2) Constrói as máscaras (0/1) das habilidades
    int cone[MASK][MASK];
    int cruz[MASK][MASK];
    int octa[MASK][MASK];

    construirCone(cone);
    construirCruz(cruz);
    construirOctaedro(octa);

    // 3) Aplica as habilidades centralizadas nos pontos de origem
    aplicarHabilidadeCentralizada(tab, cone, ORIG_CONE_L, ORIG_CONE_C);
    aplicarHabilidadeCentralizada(tab, cruz, ORIG_CRUZ_L, ORIG_CRUZ_C);
    aplicarHabilidadeCentralizada(tab, octa, ORIG_OCTA_L, ORIG_OCTA_C);

    // 4) Exibe o resultado
    imprimirTabuleiro(tab);

    return EXIT_SUCCESS;
}