Batalha Naval (C)

Projeto acadêmico em C que evolui em três níveis:

Novato – tabuleiro 10×10 e posicionamento de navios retos;

Aventureiro – adiciona navios diagonais com validação;

Mestre – adiciona habilidades especiais com áreas de efeito (cone, cruz e octaedro).

O foco é praticar vetores/matrizes, funções, validações de limite/sobreposição, loops aninhados e condicionais.

📦 Níveis
Nível Novato — Tabuleiro e 2 navios

Tabuleiro 10×10 inicializado com 0 (água).

Dois navios de tamanho 3: horizontal (→) e vertical (↓).

Navio representado por 3.

Valida limites e não sobreposição.

Impressão organizada do tabuleiro.

Nível Aventureiro — 4 navios (inclui diagonais)

Mantém o 10×10 e tamanho 3.

Acrescenta 2 navios diagonais:

↘ (SE): linha++ e coluna++.

↗ (NE): linha-- e coluna++.

Mesmas validações (limite + sobreposição) e impressão.

Nível Mestre — Habilidades (áreas de efeito)

Mantém os 4 navios.

Cria máscaras 5×5 (0/1) geradas dinamicamente com loops/condicionais:

Cone (apontado para baixo)

Cruz

Octaedro/Losango (|i-c| + |j-c| ≤ c)

Sobreposição ao tabuleiro centralizando na origem escolhida; marca 5 em células afetadas (sem sobrescrever navios).

Impressão final mostra: 0 = água, 3 = navio, 5 = habilidade.

🧠 Representação

0 → água (MAR)

3 → navio (NAVIO)

5 → área de habilidade (HABIL)

Tamanho do navio: 3 (NAVIO_T)

Tamanho das habilidades: 5×5 (MASK, ímpar para ter centro)
