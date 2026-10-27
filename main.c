/*
    * REPRESENTACAO DE GRAFOS - Versao 2025/2
    *- Gustavo Augusto Alves Pivatto (10401400)
    *- Giovanna Micher Santana (10401330)
    *- Marina Miki Sinzato (10401880)
    *- Jefferson da Conceição Dantas (10401327)
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <memory.h>
#include <string.h>

#define BRANCO 0
#define CINZA  1
#define PRETO  2

#define MAXN 100
#define INF 1e9f

/*
 * Estrutura de dados para representar grafos
 */
typedef struct a{ /* Celula de uma lista de arestas */
    int    extremo2;
    struct a *prox;
} Arest;

typedef struct v{  /* Cada vertice tem um ponteiro para uma lista de arestas incidentes nele */
    int nome;
    int cor;
    Arest *prim;
} Vert;

/* imprime o índice interno do vértice como 1-based */
void printVert(int v) {
    printf("%d", v + 1);
}

/*
 * Declaracoes das funcoes para manipulacao de grafos
 */
void criaGrafo(Vert **G, int ordem);
void destroiGrafo(Vert **G, int ordem);
int  acrescentaAresta(Vert G[], int ordem, int v1, int v2);
void imprimeGrafo(Vert G[], int ordem);

/*
 * Criacao de um grafo com ordem predefinida (passada como argumento),
 *   e, inicialmente, sem nenhuma aresta
 */
void criaGrafo(Vert **G, int ordem){
    int i;
    *G= (Vert*) malloc(sizeof(Vert)*ordem); /* Alocacao dinamica de um vetor de vertices */
    if (*G == NULL){
        fprintf(stderr, "Erro de alocacao\n");
        exit(EXIT_FAILURE);
    }

    for(i=0; i<ordem; i++){
        (*G)[i].nome= i;
        (*G)[i].cor= BRANCO;
        (*G)[i].prim= NULL;    /* Cada vertice sem nenhuma aresta incidente */
    }
}

/*
 * Desaloca a memoria dinamica usada para armazenar um grafo.
 */
void destroiGrafo(Vert **G, int ordem){
    int i;
    Arest *a, *n;

    if (*G == NULL) return;
    for(i=0; i<ordem; i++){ /* Remove lista de adjacencia de cada vertice */
        a= (*G)[i].prim;
        while (a!= NULL){
              n= a->prox;
              free(a);
              a= n;
        }
    }
    free(*G);  /* Remove o vetor de vertices */
    *G = NULL;
}

/*
 * Acrescenta uma nova aresta em um grafo previamente criado.
 * Como o grafo nao e orientado, para aresta (v1,v2) sao criadas entradas em v1 e v2.
 */
int acrescentaAresta(Vert G[], int ordem, int v1, int v2){
    Arest * A1, *A2;

    if (v1<0 || v1 >= ordem) /* Testo se vertices sao validos */
       return 0;
    if (v2<0 || v2 >= ordem)
       return 0;

    /* Acrescento aresta na lista do vertice v1 */
    A1= (Arest *) malloc(sizeof(Arest));
    if (!A1) return 0;
    A1->extremo2= v2;
    A1->prox= G[v1].prim;
    G[v1].prim= A1;

    if (v1 == v2) return 1; /* Aresta e um laco */

    /* Acrescento aresta na lista do vertice v2 se v2 != v1 */
    A2= (Arest *) malloc(sizeof(Arest));
    if (!A2) return 0;
    A2->extremo2= v1;
    A2->prox= G[v2].prim;
    G[v2].prim= A2;

    return 1;
}
// Função para imprimir o grafo e esconder as localidades na lista de adjacência
void imprimeGrafo(Vert G[], int ordem){
    int i;
    Arest *aux;

    int ordemVisivel = 0;
    for (i = 0; i < ordem; i++) {
        if (i < 49) ordemVisivel++;
    }

    printf("\n====== Ordem: %d", ordemVisivel);
    printf(" =======");
    printf("\n\n====== Lista de Adjacência =======\n");

    for (i = 0; i < 49; i++) {

        printf("\n    v");
        printVert(i);
        printf(": ");

        aux = G[i].prim;

        for (; aux != NULL; aux = aux->prox) {
            if (aux->extremo2 >= 49) continue;

            printf("  v");
            printVert(aux->extremo2);
        }
    }
    printf("\n\n");
}

/* Mapa de localidade -> vértice */
typedef struct {
    char nome[60];
    int vertice;
} LocalidadeMap;

LocalidadeMap mapa[] = {
    {"Minha casa", 49},
    {"Colegio Mello Dante", 50},
    {"Mogi Shopping", 51},
    {"Terminal Estudantes", 52},
    {"Universidade de Mogi das Cruzes", 53},
    {"Davo Mogi", 54},
    {"Hotel Ibis", 55},
    {"Spock Burger", 56},
    {"Clube de Campo", 57},
    {"Amarelinho", 58},
    {"Skate Parque", 59},
    {"Centro Universitario Braz Cubas", 60},
    {"Autoescola Shangai", 61},
    {"Shibata Supermercado", 62},
    {"Banco do Brasil", 63},
    {"Carmela Restaurante e Pizzaria", 64},
    {"Hospital Santa Casa", 65},
    {"Hospital Previna 24h", 66},
    {"Farmacia de Alto Custo", 67},
    {"Posto Shell", 68},
    {"Rosita Lanches", 69}
};

int TOTAL_LOCALIDADES = sizeof(mapa) / sizeof(mapa[0]);

const char* nomeDaLocalidade(int v) {
    for (int i = 0; i < TOTAL_LOCALIDADES; i++) {
        if (mapa[i].vertice == v)
            return mapa[i].nome;
    }
    return NULL;
}

void mostraLocalidades() {
    printf("\n===== LOCALIDADES REGISTRADAS =====\n\n");
    for (int i = 0; i < TOTAL_LOCALIDADES; i++) {
        printf("%s\n", mapa[i].nome);
    }
}

int LocalidadeSolicitada(int v, int lista[], int n) {
    if (v == 49) return 1;   /* Minha casa sempre aparece */
    for (int i = 0; i < n; i++) {
        if (lista[i] == v) return 1;
    }
    return 0;
}

/* retorna indice do vértice com menor distância ainda não visitado */
int menorDistancia(float dist[], int visitado[], int ordemG) {
    float min = INF;
    int minIndex = -1;
    for (int i = 0; i < ordemG; i++) {
        if (!visitado[i] && dist[i] < min) {
            min = dist[i];
            minIndex = i;
        }
    }
    return minIndex;
}

/* Dijkstra usando lista de adjacência (Vert G[]) e matriz de pesos distancias[u][v].
   pi[] é o predecessor (para reconstruir caminho). */
void dijkstra(Vert G[], float distancias[][MAXN], int ordemG,
              int origem, int destino, int pi[]) {
    int visitado[MAXN];
    float dist[MAXN];

    for (int i = 0; i < ordemG; i++) {
        dist[i] = INF;
        pi[i] = -1;
        visitado[i] = 0;
    }

    dist[origem] = 0.0f;

    for (int iter = 0; iter < ordemG; iter++) {
        int u = menorDistancia(dist, visitado, ordemG);
        if (u == -1) break;

        visitado[u] = 1;

        /* relaxar arestas de u */
        for (Arest *p = G[u].prim; p != NULL; p = p->prox) {
            int v = p->extremo2;
            float w = distancias[u][v];
            if (w <= 0.0f) continue; /* sem aresta ponderada definida */
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                pi[v] = u;
            }
        }
    }
}

/* Imprime caminho recursivamente usando pi[], já 1-based na saída */
void imprimeCaminho(int pi[], int origem, int destino) {
    if (destino == origem) {
        printVert(origem);
    } else if (pi[destino] == -1) {
        printf("Caminho inexistente.");
    } else {
        imprimeCaminho(pi, origem, pi[destino]);
        printf(" -> ");
        printVert(destino);
    }
}

int reconstróiCaminhoEmVetor(int pi[], int origem, int destino, int caminho[]) {
    int temp[MAXN];
    int tam = 0;
    int v = destino;

    /* Sobe pelos predecessores até chegar na origem ou ficar sem pai */
    while (v != -1) {
        temp[tam++] = v;
        if (v == origem) break;
        v = pi[v];
    }

    if (tam == 0 || temp[tam-1] != origem) {
        // não chegou na origem -> não existe caminho
        return 0;
    }

    // inverter para ficar origem -> ... -> destino
    for (int i = 0; i < tam; i++) {
        caminho[i] = temp[tam - 1 - i];
    }
    return tam;
}

int main(int argc, char *argv[]) {

    int vertices[] = {51, 57, 64, 69};   // Mogi Shopping -> Clube de Campo -> Carmela -> Rosita
    int n = sizeof(vertices) / sizeof(vertices[0]);
    int i;
    Vert *G;
    int ordemG= 70; /* Vertices identificados internamente de 0 ate 69 */

    criaGrafo(&G, ordemG);
    mostraLocalidades();

    static float dist[MAXN][MAXN];
    for (i = 0; i < ordemG; i++)
        for (int j = 0; j < ordemG; j++)
            dist[i][j] = 0.0f;

    // acrescentaAresta(G, ordemG, 0, 1);
    // Vértice 1 até Minha Casa - Localidade 1
    acrescentaAresta(G, ordemG, 0, 49);
    dist[0][49] = 350.0; dist[49][0] = 350.0;

    acrescentaAresta(G, ordemG, 0, 1);
    dist[1][0] = 400.0; dist[0][1] = 800.0;

    // Minha casa até Colégio Mello Dante - Localidade 2
    acrescentaAresta(G, ordemG, 49, 50);
    dist[49][50] = 250.0; dist[50][49] = 250.0;

    // Colégio Mello Dante até Vértice 2
    acrescentaAresta(G, ordemG, 50, 1);
    dist[50][1] = 450.0; dist[1][50] = 450.0;

    // Sem localidades entre arestas
    acrescentaAresta(G, ordemG, 1, 17);
    dist[1][17] = 160.0; dist[17][1] = 160.0;

    acrescentaAresta(G, ordemG, 1, 2);
    dist[1][2] = 180.0; dist[2][1] = 180.0;

    acrescentaAresta(G, ordemG, 14, 2);
    dist[14][2] = 280.0; dist[2][14] = 280.0;

    acrescentaAresta(G, ordemG, 3, 2);
    dist[2][3] = 600.0; dist[3][2] = 600.0;

    acrescentaAresta(G, ordemG, 7, 2);
    dist[2][7] = 550.0; dist[7][2] = 550.0;

    // Vértice 3 até Mogi Shopping - Localidade 3
    acrescentaAresta(G, ordemG, 2, 51);
    dist[2][51] = 250.0; dist[51][2] = 250.0;

    // Mogi Shopping até Vértice 4
    acrescentaAresta(G, ordemG, 51, 3);
    dist[51][3] = 350.0; dist[3][51] = 350.0;

    // Vértice 3 até Terminal Estudantes - Localidade 4
    acrescentaAresta(G, ordemG, 2, 52);
    dist[2][52] = 200.0; dist[52][2] = 200.0;

    // Terminal Estudantes até Vértice 15
    acrescentaAresta(G, ordemG, 52, 14);
    dist[52][14] = 80.0; dist[14][52] = 80.0;

    // acrescentaAresta(G, ordemG, 2, 7);
    // Vértice 3 até Universidade de Mogi das Cruzes - Localidade 5
    acrescentaAresta(G, ordemG, 2, 53);
    dist[2][53] = 200.0; dist[53][2] = 200.0;

    // Universidade de Mogi das Cruzes até Vértice 3
    acrescentaAresta(G, ordemG, 53, 7);
    dist[53][7] = 350.0; dist[7][53] = 350.0;

    // Sem localidades entre arestas
    acrescentaAresta(G, ordemG, 3, 4);
    dist[3][4] = 190.0; dist[4][3] = 190.0;

    acrescentaAresta(G, ordemG, 4, 5);
    dist[4][5] = 160.0; dist[5][4] = 160.0;

    acrescentaAresta(G, ordemG, 5, 6);
    dist[5][6] = 250.0; dist[6][5] = 250.0;

    // Vértice 7 até Davó Mogi - Localidade 6
    acrescentaAresta(G, ordemG, 6, 54);
    dist[6][54] = 250.0; dist[54][6] = 250.0;

    // Davó Mogi até Vértice 8
    acrescentaAresta(G, ordemG, 54, 7);
    dist[54][7] = 100.0; dist[7][54] = 100.0;

    acrescentaAresta(G, ordemG, 6, 7);
    dist[6][7] = 350.0; dist[7][6] = 350.0;

    acrescentaAresta(G, ordemG, 7, 8);
    dist[7][8] = 86.0; dist[8][7] = 86.0;

    acrescentaAresta(G, ordemG, 7, 10);
    dist[10][7] = 160.0; dist[7][10] = 160.0;

    acrescentaAresta(G, ordemG, 7, 55);
    dist[7][55] = 60.0; dist[55][7] = 60.0;

    acrescentaAresta(G, ordemG, 55, 10);
    dist[55][10] = 100.0; dist[10][55] = 100.0;

    acrescentaAresta(G, ordemG, 8, 9);
    dist[8][9] = 130.0; dist[9][8] = 130.0;

     // Spock Burger até Vértice 9
    acrescentaAresta(G, ordemG, 8, 56);
    dist[8][56] = 90.0; dist[56][8] = 90.0;

    // Spock Burger até Vértice 10
    acrescentaAresta(G, ordemG, 56, 9);
    dist[56][9] = 40.0; dist[9][56] = 40.0;

    acrescentaAresta(G, ordemG, 9, 10);
    dist[9][10] = 39.0; dist[10][9] = 39.0;

    acrescentaAresta(G, ordemG, 9, 47);
    dist[9][47] = 76.0; dist[47][9] = 76.0;

    // Vértice 10 até Clube de Campo - Localidade 9
    acrescentaAresta(G, ordemG, 9, 57);
    dist[9][57] = 41.0; dist[57][9] = 41.0;

    // Clube de campo até Vértice 48
    acrescentaAresta(G, ordemG, 57, 47);
    dist[57][47] = 35.0; dist[47][57] = 35.0;

    acrescentaAresta(G, ordemG, 10, 11);
    dist[10][11] = 110.0; dist[11][10] = 110.0;

    acrescentaAresta(G, ordemG, 11, 42);
    dist[11][42] = 55.0; dist[42][11] = 55.0;

    // Vértice 12 até Amarelinho - Localidade 10
    acrescentaAresta(G, ordemG, 11, 58);
    dist[11][58] = 25.0; dist[58][11] = 25.0;

    // Amarelinho até Vértice 43
    acrescentaAresta(G, ordemG, 58, 42);
    dist[58][42] = 30.0; dist[42][58] = 30.0;

    acrescentaAresta(G, ordemG, 11, 46);
    dist[11][46] = 110.0; dist[46][11] = 110.0;

    acrescentaAresta(G, ordemG, 11, 12);
    dist[11][12] = 55.0; dist[12][11] = 55.0;

    acrescentaAresta(G, ordemG, 12, 42);
    dist[12][42] = 55.0; dist[42][12] = 55.0;

    acrescentaAresta(G, ordemG, 12, 13);
    dist[12][13] = 160.0; dist[13][12] = 160.0;

    acrescentaAresta(G, ordemG, 13, 14);
    dist[13][14] = 160.0; dist[14][13] = 160.0;

    acrescentaAresta(G, ordemG, 13, 20);
    dist[13][20] = 450.0; dist[20][13] = 450.0;

    // Vértice 14 até Skate Parque - Localidade 11
    acrescentaAresta(G, ordemG, 13, 59);
    dist[13][59] = 200.0; dist[59][13] = 200.0;

    // Skate Parque até Vértice 21
    acrescentaAresta(G, ordemG, 59, 20);
    dist[59][20] = 250.0; dist[20][59] = 250.0;

    acrescentaAresta(G, ordemG, 14, 15);
    dist[14][15] = 330.0; dist[15][14] = 350.0;

    acrescentaAresta(G, ordemG, 15, 16);
    dist[15][16] = 420.0; dist[16][15] = 100.0;

    acrescentaAresta(G, ordemG, 15, 20);
    dist[15][20] = 230.0; dist[20][15] = 230.0;

    acrescentaAresta(G, ordemG, 16, 18);
    dist[16][18] = 300.0; dist[18][16] = 300.0;

    acrescentaAresta(G, ordemG, 16, 17);
    dist[16][17] = 210.0; dist[17][16] = 450.0;

    // Vértice 17 até Centro Universitário Braz Cubas - Localidade 12
    acrescentaAresta(G, ordemG, 16, 60);
    dist[16][60] = 150.0; dist[60][16] = 150.0;

    // Centro Universitário Braz Cubas até Vértice 18
    acrescentaAresta(G, ordemG, 60, 17);
    dist[60][17] = 300.0; dist[17][60] = 300.0;

    acrescentaAresta(G, ordemG, 17, 18);
    dist[17][18] = 550.0; dist[18][17] = 550.0;

    acrescentaAresta(G, ordemG, 18, 19);
    dist[18][19] = 550.0; dist[19][18] = 550.0;

    acrescentaAresta(G, ordemG, 20, 21);
    dist[20][21] = 100.0; dist[21][20] = 100.0;

    acrescentaAresta(G, ordemG, 21, 37);
    dist[21][37] = 85.0; dist[37][21] = 85.0;

    acrescentaAresta(G, ordemG, 21, 22);
    dist[21][22] = 62.0; dist[22][21] = 62.0;

    // Vértice 22 até Autoescola Shangai - Localidade 13
    acrescentaAresta(G, ordemG, 21, 61);
    dist[21][61] = 5.0; dist[61][21] = 5.0;

    // Autoescola Shangai até Vértice 23
    acrescentaAresta(G, ordemG, 61, 22);
    dist[61][22] = 55.0; dist[22][61] = 55.0;

    acrescentaAresta(G, ordemG, 21, 39);
    dist[21][39] = 155.0; dist[39][21] = 155.0;

    acrescentaAresta(G, ordemG, 22, 23);
    dist[22][23] = 83.0; dist[23][22] = 83.0;

    acrescentaAresta(G, ordemG, 23, 24);
    dist[23][24] = 56.0; dist[24][23] = 56.0;

    acrescentaAresta(G, ordemG, 23, 37);
    dist[23][37] = 64.0; dist[37][23] = 64.0;

    acrescentaAresta(G, ordemG, 24, 36);
    dist[24][36] = 63.0; dist[36][24] = 63.0;

    acrescentaAresta(G, ordemG, 24, 25);
    dist[24][25] = 55.0; dist[25][24] = 55.0;

    acrescentaAresta(G, ordemG, 25, 26);
    dist[25][26] = 62.0; dist[26][25] = 62.0;

    acrescentaAresta(G, ordemG, 26, 27);
    dist[26][27] = 140.0; dist[27][26] = 140.0;

    // Vértice 27 até Shibata Supermercado - Localidade 14
    acrescentaAresta(G, ordemG, 26, 62);
    dist[26][62] = 80.0; dist[62][26] = 80.0;

    // Shibata Supermercado até Vértice 28
    acrescentaAresta(G, ordemG, 62, 27);
    dist[62][27] = 60.0; dist[27][62] = 60.0;

    acrescentaAresta(G, ordemG, 26, 36);
    dist[26][36] = 53.0; dist[36][26] = 53.0;

    acrescentaAresta(G, ordemG, 26, 33);
    dist[26][33] = 70.0; dist[33][26] = 70.0;

    acrescentaAresta(G, ordemG, 27, 34);
    dist[27][34] = 72.0; dist[34][27] = 72.0;

    acrescentaAresta(G, ordemG, 27, 35);
    dist[27][35] = 63.0; dist[35][27] = 63.0;

    // Vértice 28 até Banco do Brasil - Localidade 15
    acrescentaAresta(G, ordemG, 27, 63);
    dist[27][63] = 40.0; dist[63][27] = 40.0;

    // Banco do Brasil até Vértice 36
    acrescentaAresta(G, ordemG, 63, 35);
    dist[63][35] = 100.0; dist[35][63] = 100.0;

    acrescentaAresta(G, ordemG, 27, 28);
    dist[27][28] = 160.0; dist[28][27] = 160.0;

    acrescentaAresta(G, ordemG, 28, 29);
    dist[28][29] = 170.0; dist[29][28] = 170.0;

    acrescentaAresta(G, ordemG, 28, 32);
    dist[28][32] = 160.0; dist[32][28] = 160.0;

    acrescentaAresta(G, ordemG, 28, 44);
    dist[28][44] = 170.0; dist[44][28] = 170.0;

    acrescentaAresta(G, ordemG, 29, 34);
    dist[29][34] = 150.0; dist[34][29] = 150.0;

    acrescentaAresta(G, ordemG, 29, 30);
    dist[29][30] = 250.0; dist[30][29] = 250.0;

    acrescentaAresta(G, ordemG, 30, 32);
    dist[30][32] = 70.0; dist[32][30] = 70.0;

    acrescentaAresta(G, ordemG, 30, 31);
    dist[30][31] = 120.0; dist[31][30] = 120.0;

    acrescentaAresta(G, ordemG, 31, 48);
    dist[31][48] = 68.0; dist[48][31] = 68.0;

    acrescentaAresta(G, ordemG, 32, 43);
    dist[32][43] = 99.0; dist[43][32] = 99.0;

    // Vértice 33 até Carmela Restaurante e Pizzaria - Localidade 16
    acrescentaAresta(G, ordemG, 32, 64);
    dist[32][64] = 20.0; dist[64][32] = 20.0;

    // Carmela Restaurante e Pizzaria até Vértice 44
    acrescentaAresta(G, ordemG, 64, 43);
    dist[64][43] = 79.0; dist[43][64] = 79.0;

    acrescentaAresta(G, ordemG, 32, 48);
    dist[32][48] = 110.0; dist[48][32] = 110.0;

    acrescentaAresta(G, ordemG, 33, 34);
    dist[33][34] = 136.0; dist[34][33] = 136.0;

    acrescentaAresta(G, ordemG, 35, 36);
    dist[35][36] = 146.0; dist[36][35] = 146.0;

    // Vértice 36 até Hospital Santa Casa - Localidade 17
    acrescentaAresta(G, ordemG, 35, 65);
    dist[35][65] = 60.0; dist[65][35] = 60.0;

    // Hospital Santa Casa até Vértice 37
    acrescentaAresta(G, ordemG, 65, 36);
    dist[65][36] = 86.0; dist[36][65] = 86.0;

    acrescentaAresta(G, ordemG, 35, 38);
    dist[35][38] = 53.0; dist[38][35] = 53.0;

    acrescentaAresta(G, ordemG, 35, 45);
    dist[35][45] = 200.0; dist[45][35] = 200.0;

    // Vértice 36 até Hospital Previna 24h - Localidade 18
    acrescentaAresta(G, ordemG, 35, 66);
    dist[35][66] = 160.0; dist[66][35] = 160.0;

    // Hospital Previna 24h até Vértice 46
    acrescentaAresta(G, ordemG, 66, 45);
    dist[66][45] = 40.0; dist[45][66] = 40.0;

    acrescentaAresta(G, ordemG, 37, 38);
    dist[37][38] = 150.0; dist[38][37] = 150.0;

    acrescentaAresta(G, ordemG, 37, 36);
    dist[37][36] = 58.0; dist[36][37] = 58.0;

    acrescentaAresta(G, ordemG, 38, 39);
    dist[38][39] = 110.0; dist[39][38] = 110.0;

    acrescentaAresta(G, ordemG, 38, 40);
    dist[38][40] = 300.0; dist[40][38] = 300.0;
    // Vértice 39 até Farmácia de Alto Custo - Localidade 19
    acrescentaAresta(G, ordemG, 38, 67);
    dist[38][67] = 100.0; dist[67][38] = 100.0;

    // Farmácia de Alto Custo até Vértice 41
    acrescentaAresta(G, ordemG, 67, 40);
    dist[67][40] = 200.0; dist[40][67] = 200.0;

    acrescentaAresta(G, ordemG, 40, 41);
    dist[40][41] = 110.0; dist[41][40] = 110.0;

    // Vértice 41 até Posto Shell - Localidade 20
    acrescentaAresta(G, ordemG, 40, 68);
    dist[40][68] = 90.0; dist[68][40] = 90.0;

    // Posto Shell até Vértice 42
    acrescentaAresta(G, ordemG, 68, 41);
    dist[68][41] = 20.0; dist[41][68] = 20.0;

    acrescentaAresta(G, ordemG, 40, 45);
    dist[40][45] = 200.0; dist[45][40] = 200.0;

    acrescentaAresta(G, ordemG, 41, 42);
    dist[41][42] = 50.0; dist[42][41] = 50.0;

    acrescentaAresta(G, ordemG, 43, 44);
    dist[43][44] = 150.0; dist[44][43] = 150.0;

    acrescentaAresta(G, ordemG, 43, 42);
    dist[43][42] = 80.0; dist[42][43] = 80.0;

    // Vértice 44 até Rosita Lanches - Localidade 21
    acrescentaAresta(G, ordemG, 43, 69);
    dist[43][69] = 10.0; dist[69][43] = 10.0;

    // Rosita Lanches até Vértice 43
    acrescentaAresta(G, ordemG, 69, 42);
    dist[69][42] = 70.0; dist[42][69] = 70.0;

    acrescentaAresta(G, ordemG, 43, 46);
    dist[43][46] = 170.0; dist[46][43] = 170.0;

    acrescentaAresta(G, ordemG, 41, 45);
    dist[41][45] = 170.0; dist[45][41] = 170.0;

    acrescentaAresta(G, ordemG, 44, 45);
    dist[44][45] = 80.0; dist[45][44] = 80.0;

    acrescentaAresta(G, ordemG, 46, 47);
    dist[46][47] = 190.0; dist[47][46] = 190.0;

    acrescentaAresta(G, ordemG, 47, 48);
    dist[47][48] = 61.0; dist[48][47] = 61.0;

    /* Imprime para verificação */
    imprimeGrafo(G, ordemG);

printf("===== CAMINHO =====\n");

    int pi[MAXN];
    int rota[MAXN * 10];   // espaço para rota completa
    int tamRota = 0;
    float distTotal = 0.0f;

    for (int i = 0; i < n; i++) {
    int origem, destino;

        if (i == 0) {
            // primeiro trecho: sempre começa em Minha casa (49)
            origem  = 49;
            destino = vertices[0];
        } else {
            origem  = vertices[i-1];
            destino = vertices[i];
        }

        dijkstra(G, dist, ordemG, origem, destino, pi);

        int caminhoTmp[MAXN];
        int tamTmp = reconstróiCaminhoEmVetor(pi, origem, destino, caminhoTmp);

        for (int k = 0; k < tamTmp - 1; k++)
            distTotal += dist[caminhoTmp[k]][caminhoTmp[k+1]];

        int start = (i == 0 ? 0 : 1);
        for (int k = start; k < tamTmp; k++)
            rota[tamRota++] = caminhoTmp[k];
    }

    int origemFinal   = vertices[n-1];
    int destinoFinal  = 49;

    /* dijkstra filtrado que IGNORA vértices 50..69 */
    int visit2[MAXN], pi2[MAXN];
    float dist2[MAXN];

    for (int i = 0; i < ordemG; i++) {
        dist2[i] = INF;
        pi2[i]   = -1;
        visit2[i] = 0;
    }

    dist2[origemFinal] = 0.0f;

    /* Dijkstra modificado */
    for (int iter = 0; iter < ordemG; iter++) {

    /* escolhe u */
    int u = -1;
    float melhor = INF;
    for (int i = 0; i < ordemG; i++) {
        if (!visit2[i] && dist2[i] < melhor) {
            melhor = dist2[i];
            u = i;
        }
    }
    if (u == -1) break;

    visit2[u] = 1;

    for (Arest *p = G[u].prim; p != NULL; p = p->prox) {
        int v = p->extremo2;

        /* IGNORA LOCALIDADES (50..69) exceto destino (49) */
        if (v >= 50 && v != destinoFinal) continue;

        float w = dist[u][v];
        if (w <= 0) continue;

        if (dist2[v] > dist2[u] + w) {
            dist2[v] = dist2[u] + w;
            pi2[v]   = u;
        }
    }
}

/* reconstrói caminho filtrado */
int caminhoBack[MAXN];
int tamBack = reconstróiCaminhoEmVetor(pi2, origemFinal, destinoFinal, caminhoBack);

if (tamBack == 0) {
    printf("ERRO: Não há caminho de volta sem passar por localidades!\n");
    return 1;
}

/* acumula distância */
for (int k = 0; k < tamBack - 1; k++)
    distTotal += dist[caminhoBack[k]][caminhoBack[k+1]];

/* adiciona no vetor de rota FINAL */
for (int k = 1; k < tamBack; k++)
    rota[tamRota++] = caminhoBack[k];

    printf("Rota: ");
    int firstPrinted = 1;
    for (int i = 0; i < tamRota; i++) {
        int v = rota[i];
        const char* nomeLoc = nomeDaLocalidade(v);
        int isLocalidade = (nomeLoc != NULL);
        if (isLocalidade && !LocalidadeSolicitada(v, vertices, n)) {
            continue;
        }

        if (!firstPrinted) printf(" -> ");
        firstPrinted = 0;

        if (nomeLoc != NULL)
            printf("%s", nomeLoc);   // imprime nome da localidade
        else {
            printf("v");
            printVert(v);           // imprime vértice como 1-based
        }
    }

    printf("\n\n===== DISTÂNCIA =====\n");
    printf("Distância total do caminho: %.1f metros\n", distTotal);

    destroiGrafo(&G, ordemG);
    return 0;
}
