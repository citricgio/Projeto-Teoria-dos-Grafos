# Projeto Teoria dos Grafos – Partes I e II

Análise de trajetos entre estabelecimentos e pontos de interesse de Mogi das Cruzes (SP), modelando o bairro como um grafo e calculando rotas de menor distância com o **algoritmo de Dijkstra**, implementado em **C**.

## Integrantes

| Nome | RA |
|---|---|
| Giovanna Micher Santana | 10401330 |
| Gustavo Augusto Alves Pivatto | 10401400 |
| Jefferson Dantas | 10401327 |
| Marina Sinzato | 10401880 |

São Paulo, 2025.

## Problema

Analisar os trajetos mais frequentes realizados por moradores entre estabelecimentos comerciais e pontos de interesse na cidade de Mogi das Cruzes.

## Solução

A região é representada como um grafo: localidades e interseções viram **vértices**, e as ruas que as ligam viram **arestas**, com peso igual à distância em metros. Com isso, é possível estudar como os moradores se deslocam entre mercados, padarias, farmácias, faculdade e trabalho, e identificar padrões de mobilidade, rotas otimizadas e possíveis gargalos.

**Exemplo:** um morador sai de casa, passa na padaria, segue até o mercado e depois vai para a faculdade. O grafo permite ver quais sequências de locais e quais ruas são mais usadas, e qual é a rota mais curta.

## Área de estudo

- **Bairros:** Vila Mogilar e parte do Centro, em Mogi das Cruzes.
- **Localidades:** 20 pontos de interesse, entre hospitais, supermercados, farmácias, postos de gasolina, universidades, shopping, hotel, lanchonetes, banco e o Terminal Estudantes, além da "Minha casa".
- **Grafo:** 49 vértices, representando ruas e quadras da área selecionada.

### Localidades mapeadas

As localidades são mapeadas como novos vértices

| Vértice | Localidade |
|---|---|
| v50 | Minha casa |
| v51 | Colégio Mello Dante |
| v52 | Mogi Shopping |
| v53 | Terminal Estudantes |
| v54 | Universidade de Mogi das Cruzes |
| v55 | D'Avó Mogi |
| v56 | Hotel Ibis |
| v57 | Spock Burger |
| v58 | Clube de Campo |
| v59 | Amarelinho |
| v60 | Skate Parque |
| v61 | Centro Universitário Braz Cubas |
| v62 | Autoescola Shangai |
| v63 | Shibata Supermercado |
| v64 | Banco do Brasil |
| v65 | Carmela Restaurante e Pizzaria |
| v66 | Hospital Santa Casa |
| v67 | Hospital Previna 24h |
| v68 | Farmácia de Alto Custo |
| v69 | Posto Shell |
| v70 | Rosita Lanches |

## Por que Dijkstra?

- Todas as distâncias são positivas, então Dijkstra funciona com segurança e eficiência. Bellman–Ford só se justificaria com pesos negativos.
- Várias rotas são calculadas em sequência (casa → ponto 1 → ponto 2 → ... → volta), então o algoritmo precisa ser leve.
- Além da distância mínima, o sistema precisa exibir o **caminho completo**, incluindo as interseções. O vetor de predecessores do Dijkstra permite reconstruí-lo.
- A distância total é a soma das distâncias de cada trecho.

## Estrutura do código

O sistema usa:

- Representação do grafo por **listas de adjacência**
- Mapeamento entre nomes das localidades e vértices internos
- Cálculo de rotas com Dijkstra, reconstrução de caminhos e soma das distâncias

### Funções principais

| Função | Descrição |
|---|---|
| `nomeDaLocalidade(int v)` | Retorna o nome da localidade de um vértice. |
| `mostraLocalidades()` | Lista as localidades cadastradas e seus vértices. |
| `LocalidadeSolicitada(int v, int lista[], int n)` | Verifica se o vértice está entre os destinos escolhidos. A "Minha casa" é sempre exibida. |
| `menorDistancia(...)` | Auxiliar do Dijkstra: escolhe o vértice não visitado com menor distância atual. |
| `dijkstra(Vert G[], float dist[][], ...)` | Calcula o menor caminho, preenche as distâncias e o vetor de predecessores. |
| `reconstróiCaminhoEmVetor(...)` | Reconstrói o caminho da origem ao destino a partir dos predecessores. |
| `imprimeCaminho(int pi[], int origem, int destino)` | Imprime o caminho recursivamente; informa se não existe caminho. |
| `main(void)` | Monta o grafo com `acrescentaAresta`, registra as distâncias na matriz `dist`, executa o Dijkstra e imprime o resultado. |

## Como usar

As rotas partem sempre de **"Minha casa"** e seguem a ordem definida no array `vertices[]`, dentro da `main()`. O exemplo do projeto usa os vértices **51, 57, 64 e 69** (Mogi Shopping, Clube de Campo, Carmela e Rosita). Para calcular outra rota, basta alterar o conteúdo desse array.

```c
int vertices[] = {51, 57, 64, 69};
```

Compilação e execução (ajuste o nome do arquivo-fonte):

```bash
gcc main.c -o grafos
./grafos
```

## Resultado

O programa calcula a rota completa entre as localidades escolhidas e exibe:

- a sequência exata de vértices visitados;
- os nomes das localidades correspondentes;
- a distância mínima de cada trecho;
- a distância total percorrida, incluindo o retorno.

## Caminhos

Os caminhos, em vértices, de cada trecho estão no [relatório em PDF](Grafos-Projeto-Parte-II.pdf).

## Documentação

O relatório completo, com o mapa da região, o output do código e os caminhos detalhados, está no PDF [**Grafos – Projeto Parte II**](Grafos-Projeto-Parte-II.pdf).
