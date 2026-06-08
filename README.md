# Grafos Cidade

Aqui apresenta um programa para calcular um grafo que representa uma cidade, vertices representam bairros e arestas representam as ruas que conectam esses bairros.

Operações básicas
-

- impressão do grafo (lista de adjacência)
- Cálculo de ciclovia
- Análise de conectividade
- Caminho mínimo entre dois bairros

Compilação e execução
-
Essa aplicação não tem interface gráfica, cole esse codigo no terminal mudando o conteudo entre colchetes para o caminho do arquivo com as entradas
```bash
  gcc graphos.c -o g.out
  ./g.out < [arquivo de entrada]
```

Estruturas de Dados usadas
-
Para o grafo usamos uma lista de adjacencia com a seguinte estrutura
```C
typedef struct No{
	int chave;
	int custo;
	struct No* prox;
} No;
typedef struct Grapho{
	No* adjacencias;
	int direcionado;
	int numVertices;
}Grapho;
```

Padrão de entrada
-

Ao declarar um novo grafo teremos
```plainText
[numero de vertices] [numero de arestas] [grafo é direcionado? [ 1 | 0 ]] [nome do nó inicial]
[origem] [destino] [custo]
.
.
.
```
Após isso pule uma linha e coloque as entradas para navegar pelo menu.

Melhorias
-
Algumas melhorias podem ser feitas como adição e remoção de vértices, calculo de floresta geradora mínima, utilização de mais algorítimos de AGM e caminhos mínimos!

Conclusão
-
