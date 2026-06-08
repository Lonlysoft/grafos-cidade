#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define BRANCO 0
#define CINZA 1
#define PRETO 2

#define INF INT_MAX
int noZero;
typedef struct No{
	int chave;
	int custo;
	struct No* prox;
} No;
No* LISTA__inserir(No* n, int chave, int custo){
	No* no = n;
	No* novo = malloc(sizeof(No));
	novo->chave = chave;
	novo->custo = custo;
	novo->prox = no;
	n = novo;
	return n;
}
void LISTA__remover(No* n, int chave){
	No* no = n;
	No* ant;
	if(no == NULL && no->chave == chave){
		n = no->prox;
		free(no);
	}
	while(no!=NULL && no->chave != chave){
		ant = no;
		no = no->prox;
	}
	if(no == NULL) return;
	ant->prox = no->prox;
	free(no);
}

typedef struct Grapho{
	No* adjacencias;
	int direcionado;
	int numVertices;
}Grapho;
void GRAPHO__printf(Grapho* g){
	for (int i = 0; i < g->numVertices; i++){
		if(g->adjacencias[i].chave == 0) continue;
		printf("%d → ", i+noZero);
		No* n = g->adjacencias[i].prox;
		while (n != NULL){
			printf("(%d, %d) ", n->chave+noZero, n->custo);
			n = n->prox;
		}
		printf("\n");
	}
}
Grapho* GRAPHO__iniciar(int numVertices, int direcionado){
	Grapho* novo = malloc(sizeof(Grapho));
	novo->direcionado = direcionado;
	novo->adjacencias = malloc(sizeof(No) * numVertices);
	for (int i = 0; i < numVertices; ++i){
		novo->adjacencias[i].chave = 1; //quer dizer que eles está ativo
		novo->adjacencias[i].prox = NULL;
	}
	novo->numVertices = numVertices;
	return novo;
}
void GRAPHO__armazenar(Grapho* g, int origem, int destino, int custo){
	if(g->direcionado){
        g->adjacencias[origem].prox = LISTA__inserir(g->adjacencias[origem].prox, destino, custo);
        return;
    }
    g->adjacencias[origem].prox = LISTA__inserir(g->adjacencias[origem].prox, destino, custo);
	g->adjacencias[destino].prox = LISTA__inserir(g->adjacencias[destino].prox, origem, custo);
}
void GRAPHO__BFSsubConjunto(Grapho* g, int origem, int*corAux){
    int* fila = malloc(sizeof(int)* g->numVertices);
    int* d = malloc(sizeof(int)* g->numVertices);
    int* pi = malloc(sizeof(int)* g->numVertices);
    int* cor = malloc(sizeof(int)* g->numVertices);
    int frente = 0, tras = 0;
    for(int i = 0; i < g->numVertices; i++){
        d[i] = -1;
        pi[i] = -1;
        cor[i] = BRANCO;
    }
    fila[tras++] = origem;
    d[origem] = 0;
    pi[origem] = -1;
    cor[origem] = CINZA;
    
    while(frente < tras){
        int atual = fila[frente++];
        for(No* i = g->adjacencias[atual].prox; i != NULL; i = i->prox){
            if(cor[i->chave] == BRANCO){
                cor[i->chave] = CINZA;
                d[i->chave] = d[atual]+1;
                fila[tras++] = i->chave;
                pi[i->chave] = atual;
            }
        }
        cor[atual] = PRETO;
    }
	for(int i = 0; i < g->numVertices; i++){
		if(cor[i] == PRETO){
			corAux[i] = cor[i];
			printf("%d ", i+noZero);
		}
	}
	free(pi); free(d); free(cor);
}
// a partir de um bairro de origem verifica quais bairros são alcançáveis, quantos e quais conjuntos de bairros estão isolados
int GRAPHO__conectividade(Grapho* g, int origem){
    //usar um BFS
	printf("Análise de conectividade: \n\t");
    int* fila = malloc(sizeof(int)* g->numVertices);
    int* d = malloc(sizeof(int)* g->numVertices);
    int* pi = malloc(sizeof(int)* g->numVertices);
    int* cor = malloc(sizeof(int)* g->numVertices);
    int frente = 0, tras = 0;
    for(int i = 0; i < g->numVertices; i++){
        d[i] = -1;
        pi[i] = -1;
        cor[i] = BRANCO;
    }
    fila[tras++] = origem;
    d[origem] = 0;
    pi[origem] = -1;
    cor[origem] = CINZA;
    
    while(frente < tras){
        int atual = fila[frente++];
        for(No* i = g->adjacencias[atual].prox; i != NULL; i = i->prox){
            if(cor[i->chave] == BRANCO){
                cor[i->chave] = CINZA;
                d[i->chave] = d[atual]+1;
                fila[tras++] = i->chave;
                pi[i->chave] = atual;
            }
        }
        cor[atual] = PRETO;
    }
    printf("A partir do bairro %d os bairros alcançáveis são:\n\t", origem+noZero);
    for(int i = 0; i < g->numVertices; i++){
        if(i == origem) continue;
        if(cor[i] == PRETO) printf("%d ", i+noZero);
    }
    printf("\n\t");
    int totalmenteConectada = 1;
    for(int i = 0; i < g->numVertices; i++){
        if(d[i] == -1){
            totalmenteConectada = 0;
			break;
		}
    }
    free(pi); free(d); free(cor);
    //TODO: fazer impressão conjunto por conjunto, provavelmente vamos fazer BFS dos outros vertices como uma pseudo recursão
    if(totalmenteConectada){
		printf("a cidade é toda conectada\n\tNão há bairros isolados\n\n");
	} else {
		int conjuntos = 1;
		printf("a cidade não é toda conectada\n\tConjuntos de bairros");
		
		printf("\n\t\tconjunto %d: ", conjuntos);
	    for(int i = 0; i < g->numVertices; i++){
			if(cor[i] == PRETO){
				printf("%d ", i+noZero);
			}
		}
		conjuntos++;
	    for(int i = 0; i < g->numVertices; i++){
			if(cor[i] == BRANCO){
				printf("\n\t\tconjunto %d: ", conjuntos);
				GRAPHO__BFSsubConjunto(g, i, cor);
				conjuntos++;
			}
		}
		printf("\n\n");
	}
    return totalmenteConectada;
}
int GRAPHO__conectividade_semImpressao(Grapho* g, int origem){
    //usar um BFS
    int* fila = malloc(sizeof(int)* g->numVertices);
    int* d = malloc(sizeof(int)* g->numVertices);
    int* pi = malloc(sizeof(int)* g->numVertices);
    int* cor = malloc(sizeof(int)* g->numVertices);
    int frente = 0, tras = 0;
    for(int i = 0; i < g->numVertices; i++){
        d[i] = -1;
        pi[i] = -1;
        cor[i] = BRANCO;
    }
    fila[tras++] = origem;
    d[origem] = 0;
    pi[origem] = -1;
    cor[origem] = CINZA;
    
    while(frente < tras){
        int atual = fila[frente++];
        for(No* i = g->adjacencias[atual].prox; i != NULL; i = i->prox){
            if(cor[i->chave] == BRANCO){
                cor[i->chave] = CINZA;
                d[i->chave] = d[atual]+1;
                fila[tras++] = i->chave;
                pi[i->chave] = atual;
            }
        }
        cor[atual] = PRETO;
    }
    int totalmenteConectada = 1;
    for(int i = 0; i < g->numVertices; i++){
        if(d[i] == -1){
            totalmenteConectada = 0;
			break;
		}
    }
    free(pi); free(d); free(cor);
    return totalmenteConectada;
}
//2. Caminhos Minimos: algoritimo de dijkstra
typedef struct ElementoHeap{
    int vertice;
    int custo;
} ElementoHeap;
typedef struct Heap{
    ElementoHeap* elementos;
	int* posicao;
    int tamanho;
    int capacidade;
} Heap;
Heap* HEAP__criar(int capacidade){
    Heap* nova = (Heap*)malloc(sizeof(Heap));
    nova->elementos = malloc(capacidade* sizeof(ElementoHeap));
	nova->posicao = malloc(capacidade* sizeof(int));
    nova->tamanho = 0;
    nova->capacidade = capacidade;
	for(int i = 0; i < capacidade; i++){
		nova->posicao[i] = -1;
	}
    return nova;
}
void HEAP__trocarElementos(Heap* h, int a, int b){
    ElementoHeap temp = h->elementos[a];
    h->elementos[a] = h->elementos[b];
    h->elementos[b] = temp;

	h->posicao[h->elementos[a].vertice] = a;
	h->posicao[h->elementos[b].vertice] = b;
}
void HEAP__heapifyDown(Heap* h, int idx){
    int menor = idx;
    int esquerdo = 2*idx+1;
    int direito = 2*idx+2;
    if(esquerdo < h->tamanho && h->elementos[esquerdo].custo < h->elementos[menor].custo){
        menor = esquerdo;
    }
    if(direito < h->tamanho && h->elementos[direito].custo < h->elementos[menor].custo){
        menor = direito; 
    }
    if(menor!=idx){
        HEAP__trocarElementos(h, idx, menor);
        HEAP__heapifyDown(h, menor);
    }
}
void HEAP__heapifyUp(Heap* h, int idx){
    while(idx > 0){
        int pai = (idx - 1)/2;
        if(h->elementos[pai].custo > h->elementos[idx].custo){
            HEAP__trocarElementos(h, pai, idx);
            idx = pai;
        } else{
            break;
        }
    }
}
void HEAP__inserir(Heap* h, int vertice, int custo){
	if(h->tamanho >= h->capacidade){
		return;
	}
	h->elementos[h->tamanho].vertice = vertice;
	h->elementos[h->tamanho].custo = custo;
	h->posicao[vertice] = h->tamanho;
	HEAP__heapifyUp(h, h->tamanho);
	h->tamanho++;
}
bool HEAP__encontrou(Heap* h, int vertice){
	return h->posicao[vertice] != -1;
}
ElementoHeap HEAP__extrairMin(Heap* h){
    ElementoHeap min = h->elementos[0];
	h->posicao[min.vertice] = -1;
    h->tamanho--;
	if(h->tamanho > 0){
    	h->elementos[0] = h->elementos[h->tamanho-1];
		h->posicao[h->elementos[0].vertice] = 0;
		HEAP__heapifyDown(h, 0);
	}
    return min;
}
void HEAP__diminuirCusto(Heap* h, int vertice, int novaChave){
	int idx = h->posicao[vertice];
	if(idx == -1){
		return;
	}
	h->elementos[idx].custo = novaChave;
}

void GRAPHO__printfRotas(int* pi, int destino){
	if(pi[destino] == -1){
		printf("%d ", destino+noZero);
		return;
	}
	GRAPHO__printfRotas(pi, pi[destino]);
	printf(" -> %d", destino+noZero);
}
void GRAPHO__rotasBairro(Grapho* g, int origem, int destino){
	printf("Rotas bairro:");
    int* d = malloc(sizeof(int)*g->numVertices);
    int* pi = malloc(sizeof(int)*g->numVertices);
    bool* processado = malloc(sizeof(bool)*g->numVertices);

	for(int i = 0; i < g->numVertices; i++){
		d[i] = INF;
		pi[i] = -1;
		processado[i] = false;
	}
	d[origem] = 0;

	Heap* h = HEAP__criar(g->numVertices);
	HEAP__inserir(h, origem, 0);
	while(h->tamanho != 0){
		ElementoHeap atual = HEAP__extrairMin(h);
		int u = atual.vertice;
		int custoAtual = atual.custo;
		if(processado[u]) continue;
		if(custoAtual > d[u]) continue;
		if(u == destino) break;
		processado[u] = true;
		for(No* n = g->adjacencias[u].prox; n != NULL; n = n->prox){
			int v = n->chave;
			if(!processado[v] && d[u] != INF && d[u] + n->custo < d[v]){
				d[v] = d[u] + n->custo;
				pi[v] = u;
				HEAP__inserir(h, v, d[v]);
			}
		}
	}
	
	printf("\n");
	if(d[destino] == INF){
		printf("\tnão existe caminho entre o bairro %d e %d", origem+noZero, destino+noZero);
	}else{
		printf("\t");
		GRAPHO__printfRotas(pi, destino);
		printf("\n\tcusto: %d", d[destino]);
	}
	printf("\n");
	free(d); free(pi); free(processado); free(h->elementos); free(h);
}

void PRIM__printfAGM(int* pi, int* chave, int numVertices){
	int custoTotal = 0;
	for(int i = 0; i < numVertices; i++){
		if(pi[i]!= -1){
			printf("(%d, %d) (%d)\n\t", pi[i]+noZero, i+noZero, chave[i]);
			custoTotal+=chave[i];
		}
	}
	printf("Custo total: %d\n", custoTotal);
}
//3. Arvore Geradora Minima, algoritimo de Prim
void GRAPHO__ciclovia(Grapho* g){
    int* pi = malloc(sizeof(int)*g->numVertices);
    int* chave = malloc(sizeof(int)*g->numVertices);
	bool* estaNaAGM = malloc(sizeof(bool)*g->numVertices);
	for(int i = 0; i < g->numVertices; i++){
		pi[i] = -1;
		chave[i] = INF;
		estaNaAGM[i] = false;
	}
	Heap* Q = HEAP__criar(g->numVertices);
	chave[0] = 0;
	HEAP__inserir(Q, 0, chave[0]);
	while(Q->tamanho != 0){
		ElementoHeap min = HEAP__extrairMin(Q);
		estaNaAGM[min.vertice] = true;
		for(No* i = g->adjacencias[min.vertice].prox; i != NULL; i = i->prox){
			int vizinho = i->chave;
			int custo = i->custo;
			if(!estaNaAGM[vizinho] && custo < chave[vizinho]){
				chave[vizinho] = custo;
				pi[vizinho] = min.vertice;
				if(HEAP__encontrou(Q, vizinho)){
					HEAP__diminuirCusto(Q, vizinho, custo);
				} else{
					HEAP__inserir(Q, vizinho, custo);
				}
			}
		}
	}
	PRIM__printfAGM(pi, chave, g->numVertices);
	free(estaNaAGM); free(pi); free(chave); free(Q->elementos); free(Q);
}

//imprimir informações para testes ràpidos
void GRAPHO__info(Grapho* g){
    int conectado = GRAPHO__conectividade(g, 0);
    GRAPHO__rotasBairro(g, 0, 4);
	printf("Ciclovia: \n\t");
    if(conectado)
        GRAPHO__ciclovia(g);
    else
        printf("a cidade não é conectada.\n\n");
}

void menu(Grapho* g){
	int option;
	int bairroInput;
	int bairroOrigem, bairroDestino;
	int conectado;
	while(true){
		scanf("%d", &option);
		switch(option){
			case 1: //imprimir grafo
				GRAPHO__printf(g);
			break;
			case 2: //calcular conectividade 
				scanf("%d", &bairroInput);
				GRAPHO__conectividade(g, bairroInput-noZero);
			break;
			case 3:
				scanf("%d %d", &bairroOrigem, &bairroDestino);
				GRAPHO__rotasBairro(g, bairroOrigem-noZero, bairroDestino-noZero);
			break;
			case 4:
				conectado = GRAPHO__conectividade_semImpressao(g, 0);
				printf("Ciclovia: \n\t");
				if(conectado)
        			GRAPHO__ciclovia(g);
    			else
        			printf("a cidade não é conectada.\n\n");
			break;
			default: return; break;
		}
	}
}

int main(){
    int vertices, ruas, direcionado;
    scanf("%d %d %d %d", &vertices, &ruas, &direcionado, &noZero);
    Grapho* this__cidade = GRAPHO__iniciar(vertices, direcionado);
    for(int i = 0; i < ruas; i++){
		int origem, destino, custo;
		scanf("%d %d %d", &origem, &destino, &custo);
		GRAPHO__armazenar(this__cidade, origem-noZero, destino-noZero, custo);
	}
	menu(this__cidade);
	return 0;
}