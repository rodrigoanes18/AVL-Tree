#include <stdlib.h>
#include <avl.h>
#include <stdio.h>

typedef struct no NO;

/*
	Struct no:
		Estrutura do nó utilizado na avl
	@Campos:
		void* item - estrutura genérica do item que será inserido na avl
		NO* filhoEsquerdo - ponteiro para o filho esquerdo da raiz
		NO* filhoDireito - ponteiro para o filho direito da raiz
		int altura - altura do no
*/
struct no
{
	void* item;
	NO* filhoEsquerdo;
	NO* filhoDireito;
	int altura;
};

/*
	Struct avl:
		Estrutura da avl
	@Campos:
		NO* raiz - no raiz da arvore
		int profundidade - profundidade da arvore
		int (*cmp) (void*, void*) - campo genérico para a função de comparação entre dois ponteiros para void
		int (*cmp_chave) (void*, void*) - campo genérico para a função de comparação entre dois ponteiros para void, neste caso de um item para uma chave
		void (*imprimir) (void*) - campo genérico para a função de impressão de um item
		void (*free_item) (void*) - campo genérico para a função de remoção de um item
*/
struct avl
{
	NO* raiz;
	int profundidade;
	int (*cmp) (void*, void*);
	int (*cmp_chave) (void*, void*);
	void (*imprimir) (void*);
	void (*free_item) (void*);
};

/*
	Função avl_criar:
		Cria a arvore, aloca seu espaço na heap e preenche suas variáveis, formando uma avl de raiz nula, profundidade -1 e com as funções
		passadas pelo desenvolvedor
	@Parametros:
		int (*_cmp) (void*, void*) - função de comparação entre dois items
		int (*_cmp_chave) (void*, void*) - função de comparação entre item e chave
		void (* _imprimir) (void*) - função de impressão de um item
		void (*_free_item) (void*) - função de desalocação de memória de um item
	@Retorno:
		Ponteiro para a estrutura AVL criada
*/
AVL* avl_criar(int (*_cmp) (void*, void*), int (*_cmp_chave) (void*, void*), void (* _imprimir) (void*), void (*_free_item) (void*))
{
	AVL* arvore = malloc(sizeof(AVL));
	if (arvore != NULL)
	{
		arvore->raiz = NULL;
		arvore->profundidade = -1;
		arvore->cmp = _cmp;
		arvore->cmp_chave = _cmp_chave;
		arvore->imprimir = _imprimir;
		arvore->free_item = _free_item;
	}
	return arvore;
}

/*
	Função avl_apagar_aux:
		Método de auxilio para a remoção de uma arvore. Percorre-a em pós ordem de maneira recursiva 
		e realiza o free do item
	@Parametros:
		AVL* arvore - ponteiro para a avl a se apagar
		NO** raiz - ponteiro duplo para o nó raiz que se encontra o algoritmo
	@Retorno:
		void
*/
void avl_apagar_aux(AVL* arvore, NO** raiz)
{
	if (*raiz != NULL)
	{
		// Percorre em pós ordem
		avl_apagar_aux(arvore, &((*raiz)->filhoEsquerdo));
		avl_apagar_aux(arvore, &((*raiz)->filhoDireito));
		// Utiliza a função free passada a arvore ao se criar
		arvore->free_item((*raiz)->item);
		free(*raiz);
	}
}

/*
	Função avl_apagar:
		Função de remoção de uma árvore visível para o usuário. Apenas chama a função recursiva
		auxiliar e da o free no ponteiro para avl passado
	@Parametros:
		AVL** arvore - ponteiro duplo para a arvore que se deseja excluir
	@Retorno:
		void
*/
void avl_apagar(AVL** arvore)
{
	avl_apagar_aux((*arvore), &(*arvore)->raiz);
	free(*arvore);
	*arvore = NULL;
}

/*
	Função avl_altura_no:
		Retorna a altura do no passado
	@Parametros:
		NO* raiz - no que se deseja saber a altura
	@Retorno:
		-1 caso o no não exista ou sua altura (int) caso exista
*/
int avl_altura_no(NO* raiz)
{
	if (raiz == NULL)
	{
		return -1;
	}
	else
	{
		return raiz->altura;
	}
}

/*
	Função avl_criar_no:
		Metódo generico para a criação de um nó para a arvore, independente da estrutura de dados
		recebida
	@Parametros:
		void* _item - ponteiro para uma estrutura generica que se deseja armazenar
	@Retorno:
		Ponteiro para o nó criado
*/
NO* avl_criar_no(void* _item)
{
	NO* no = malloc(sizeof(NO));
	if (no != NULL)
	{
		no->altura = 0;
		no->filhoDireito = NULL;
		no->filhoEsquerdo = NULL;
		no->item = _item;
	}
	return no;
}

/*
	Função rodar_direita:
		Realiza a rotação simple para direita de uma subarvore de raiz a
	@Parametros:
		NO* a - raiz da subarvore a ser rotacionada
	@Retorno:
		Ponteiro para a nova raiz (NO*)
*/
NO* rodar_direita(NO* a)
{
	NO* b = a->filhoEsquerdo;
	a->filhoEsquerdo = b->filhoDireito;
	b->filhoDireito = a;

	a->altura = max(avl_altura_no(a->filhoEsquerdo), avl_altura_no(a->filhoDireito)) + 1;
	b->altura = max(avl_altura_no(b->filhoEsquerdo), a->altura) + 1;

	return b;
}

/*
	Função rodar_esquerda:
		Realiza a rotação simple para esquerda de uma subarvore de raiz a
	@Parametros:
		NO* a - raiz da subarvore a ser rotacionada
	@Retorno:
		Ponteiro para a nova raiz (NO*)
*/
NO* rodar_esquerda(NO* a)
{
	NO* b = a->filhoDireito;
	a->filhoDireito = b->filhoEsquerdo;
	b->filhoEsquerdo = a;

	a->altura = max(avl_altura_no(a->filhoEsquerdo), avl_altura_no(a->filhoDireito)) + 1;
	b->altura = max(avl_altura_no(b->filhoDireito), a->altura) + 1;

	return b;
}

/*
	Função rodar_esquerda_direita:
		Realiza a rotação dupla a direita da subarvore de raiz a
	@Parametros:
		NO* a - raiz da subarvore a ser rotacionada
	@Retorno:
		Ponteiro para a nova raiz (NO*)
*/
NO* rodar_esquerda_direita(NO* a)
{
	a->filhoEsquerdo = rodar_esquerda(a->filhoEsquerdo);
	return rodar_direita(a);
}

/*
	Função rodar_direita_esquerda:
		Realiza a rotação dupla a esquerda da subarvore de raiz a
	@Parametros:
		NO* a - raiz da subarvore a ser rotacionada
	@Retorno:
		Ponteiro para a nova raiz (NO*)
*/
NO* rodar_direita_esquerda(NO* a)
{
	a->filhoDireito = rodar_direita(a->filhoDireito);
	return rodar_esquerda(a);
}

/*
	Função avl_inserir_no:
		Função recursiva em pré ordem que realiza a inserção de um nó na arvore avl desejada.
	@Parametros:
		AVL* arvore - arvore na qual se realizara a inserção
		NO* raiz - raiz consultada
		void* item - estrutura de dados generica que sera inserida
	@Retorno:
		Ponteiro para a raiz da arvore que se realizou a insercao
*/
NO* avl_inserir_no(AVL* arvore, NO* raiz, void* item)
{
	// Percurso em pré ordem
	if (raiz == NULL)
	{
		raiz = avl_criar_no(item);
	}
	else if (arvore->cmp(item, raiz->item) > 0)
	{
		raiz->filhoDireito = avl_inserir_no(arvore, raiz->filhoDireito, item);
	}
	else if (arvore->cmp(item, raiz->item) < 0)
	{
		raiz->filhoEsquerdo = avl_inserir_no(arvore, raiz->filhoEsquerdo, item);
	}

	// Função para setar a altura do nó inserido
	raiz->altura = max(avl_altura_no(raiz->filhoEsquerdo), avl_altura_no(raiz->filhoDireito)) + 1;

	//Ajustes caso após a inserção, a arvore perca a propriedade de avl
	if (avl_altura_no(raiz->filhoEsquerdo) - avl_altura_no(raiz->filhoDireito) == -2)
		if (arvore->cmp(item, raiz->filhoDireito->item) > 0)
			raiz = rodar_esquerda(raiz);
		else
			raiz = rodar_direita_esquerda(raiz);

	if (avl_altura_no(raiz->filhoEsquerdo) - avl_altura_no(raiz->filhoDireito) == 2)
		if (arvore->cmp(item, raiz->filhoEsquerdo->item) < 0)
			raiz = rodar_direita(raiz);
		else
			raiz = rodar_esquerda_direita(raiz);

	return raiz;
}

/*
	Função avl_inserir:
		Função que o usuário pode acessar para requisitar a inserção de um nó em uma avl
	@Parametros:
		AVL* arvore - avl na qual sera feita a inserção
		void* item - ponteiro genérico para a estrutura de dados que será inserida
	@Retorno:
		Retorna TRUE caso a inserção seja feita com sucesso, caso contrario retorna FALSE
*/
boolean avl_inserir(AVL* arvore, void* item)
{
	return ((arvore->raiz = avl_inserir_no(arvore, arvore->raiz, item)) != NULL);
}

/*
	Função troca_max_esquerda:
		Função de auxilio, que realiza a troca do nó da subarvore esquerda para o seu nó de maior indice, para realizar a remoção de um no
		caso o nó que se deseja remover possui dois filhos
	@Parametros:
		AVL* arvore - arvore que sera realizada o ajuste
		NO* troca - nó a ser trocado
		NO* raiz - raiz da subarvore a ser trocada
		NO* anterior - ponteiro para guardar o nó anterior consultado
	@Retorno:
		void
*/
void troca_max_esquerda(AVL* arvore, NO* troca, NO* raiz, NO* anterior)
{
	if (troca->filhoDireito != NULL)
	{
		troca_max_esquerda(arvore, troca->filhoDireito, raiz, troca);
		troca->altura = max(avl_altura_no(troca->filhoEsquerdo), avl_altura_no(troca->filhoDireito)) + 1;
		return;
	}

	if (raiz == anterior)
		anterior->filhoEsquerdo = troca->filhoEsquerdo;
	else
		anterior->filhoDireito = troca->filhoEsquerdo;

	arvore->free_item(raiz->item);
	raiz->item = troca->item;
	free(troca);
	troca = NULL;
}

/*
	Função avl_remover_aux:
		Função recursiva em pré ordem que realiza a remoção de um nó com uma chave desejada de uma avl
	@Parametros:
		AVL* arvore - avl na qual será realizada a remoção
		NO** raiz - nó raiz que esta sendo consultado
		void* chave - chave do nó que se deseja remover
	@Retorno:
		Ponteiro para a raiz da subarvore onde a remoção ocorreu
*/
NO* avl_remover_aux(AVL* arvore, NO** raiz, void* chave)
{
	NO* noAuxiliar;

	// Percuros em pré ordem para a remoção
	if (*raiz == NULL)
		return NULL;
	else if (arvore->cmp_chave(chave, (*raiz)->item) == 0)
	{
		if ((*raiz)->filhoEsquerdo == NULL || (*raiz)->filhoDireito == NULL)
		{
			noAuxiliar = *raiz;
			if ((*raiz)->filhoEsquerdo == NULL)
				*raiz = (*raiz)->filhoDireito;
			else
				*raiz = (*raiz)->filhoEsquerdo;
			arvore->free_item(noAuxiliar->item);
			free(noAuxiliar);
			noAuxiliar = NULL;
		}

		else
			// Ajuste caso o nó a ser removido possua dois filhos
			troca_max_esquerda(arvore, (*raiz)->filhoEsquerdo, (*raiz), (*raiz));
	}
	else if (arvore->cmp_chave(chave,(*raiz)->item) < 0)
		(*raiz)->filhoEsquerdo = avl_remover_aux(arvore, &(*raiz)->filhoEsquerdo, chave);
	else if (arvore->cmp_chave(chave,(*raiz)->item) > 0)
		(*raiz)->filhoDireito = avl_remover_aux(arvore, &(*raiz)->filhoDireito, chave);

	// Ajuste das alturas e, caso necessario, a realização de rotações com a finalidade de manter a arvore como avl
	if (*raiz != NULL)
	{
		(*raiz)->altura = max(avl_altura_no((*raiz)->filhoEsquerdo), avl_altura_no((*raiz)->filhoDireito)) + 1;
		if (avl_altura_no((*raiz)->filhoEsquerdo) - avl_altura_no((*raiz)->filhoDireito) == -2)
		{
			if (avl_altura_no((*raiz)->filhoDireito->filhoEsquerdo) - avl_altura_no((*raiz)->filhoDireito->filhoDireito) <= 0)
			{
				*raiz = rodar_esquerda(*raiz);
			}
			else
			{
				*raiz = rodar_direita_esquerda(*raiz);
			}
		}
		if (avl_altura_no((*raiz)->filhoEsquerdo) - avl_altura_no((*raiz)->filhoDireito) == 2)
		{
			if (avl_altura_no((*raiz)->filhoEsquerdo->filhoEsquerdo) - avl_altura_no((*raiz)->filhoEsquerdo->filhoDireito) >= 0)
			{
				*raiz = rodar_direita(*raiz);
			}
			else
			{
				*raiz = rodar_esquerda_direita(*raiz);
			}
		}
	}

	return *raiz;
}

/*
	Função avl_remover:
		Função de remoção de um nó da avl
	@Parametros:
		AVL* arvore - arvore na qual sera realizada a remoção
		void* chave - chave do registro que sera removido
	@Retorno:
		TRUE caso a remoção seja feita com sucesso, caso contrario FALSE
*/
boolean avl_remover(AVL* arvore, void* chave)
{
	return ((arvore->raiz = avl_remover_aux(arvore, &arvore->raiz, chave)) != NULL);
}

/*
	Função avl_buscar_aux:
		Função em pré ordem que realiza a busca de um registro dentro de um avl
	@Parametros:
		AVL* arvore - arvore na qual sera realizada a busca
		NO* raiz - raiz consultada
		void* chave - indice do registro a ser removido
	@Retorno:
		Ponteiro para o registro generico encontrado
*/
void* avl_buscar_aux(AVL* arvore, NO* raiz, void* chave)
{
	if (raiz == NULL)
		return NULL;
	if (arvore->cmp_chave(chave, raiz->item) == 0)
		return	raiz->item;

	if (arvore->cmp_chave(chave, raiz->item) < 0)
		return avl_buscar_aux(arvore, raiz->filhoEsquerdo, chave);
	else
		return avl_buscar_aux(arvore, raiz->filhoDireito, chave);
}

/*
	Função avl_buscar:
		Função que realiza a busca de um registro em um avl
	@Parametros:
		AVL* arvore - avl onde sera realizada a busca
		void* chave - chave do registro generico que se deseja encontrar
	@Retorno:
		Ponteiro para o registro generico encontrado
*/
void* avl_buscar(AVL* arvore, void* chave)
{
	return avl_buscar_aux(arvore, arvore->raiz, chave);
}

/*
	Função avl_imprimir_aux:
		Função recursiva em ordem que realiza a impressão de um registro de um nó de toda a avl
	@Parametros:
		AVL* arvore - arvore que sera realizada a impressao
		NO** raiz - raiz que esta sendo consultada
	@Retorno:
		void
*/
void avl_imprimir_aux(AVL* arvore, NO** raiz)
{
	if (*raiz == NULL)
		return;

	avl_imprimir_aux(arvore, &(*raiz)->filhoEsquerdo);

	arvore->imprimir((*raiz)->item);

	avl_imprimir_aux(arvore, &(*raiz)->filhoDireito);
}

/*
	Função avl_imprimir:
		Função que chama a função de impressao dos registros dos nós de uma avl
	@Parametros:
		AVL* arvore - avl que sera impressa
	@Retorno:
		void
*/
void avl_imprimir(AVL* arvore)
{
	return avl_imprimir_aux(arvore, &arvore->raiz);
}

/*
	Função setFuncaoImprimir:
		Seta a função de imprimir de uma avl generica
	@Parametros:
		AVL* arvore - arvore que se deseja setar a função de impressao
		void (* _imprimir) (void*) - função de impressao desejada
	@Retorno:
		void
*/
void setFuncaoImprimir(AVL* arvore, void (* _imprimir) (void*))
{
	arvore->imprimir = _imprimir;
}

/*
	Função
		Função recursiva que percorre em ordem os n primeiros nós da arvore. Inserindo o item
		de cada nó na lista_itens. A recursão finaliza quando se alcança o número n itens, ou
		quando a arvore não possui mais nós para percorrer.
	@Parametros:
		NO** raiz - no que da arvore com informações de seus filhos
		int* counter - contador passado por referência incrementado a cada item adicionado
		int n - quantidade máxima de itens desejados na lista
		void** lista_itens = lista de itens que será povoada com os n primeiros itens da arvore

	@Retorno:
		void
*/
void avl_n_primeiros_aux(NO** raiz, int* counter, int n, void** lista_itens)
{
	if (*raiz == NULL)
		return;

	avl_n_primeiros_aux(&(*raiz)->filhoEsquerdo, counter, n, lista_itens);

	if( (*counter) == n) return;
	else {
		(*counter)++;
		lista_itens[(*counter) - 1] = (*raiz)->item;
	}
	avl_n_primeiros_aux(&(*raiz)->filhoDireito, counter, n, lista_itens);
}

/*
	Função
		A função recebe uma árvore e uma quantidade n de itens que ela precisa retornar
		Ela percorre de forma recurssiva os nós usando uma função auxiliar e retorna 
		uma lista dos n primeiros itens. 
	@Parametros:
		AVL* arvore - Arvore da qual serão retirados os n primeiros itens
		int* n - Inteiro passado por referencia que inicialmente indica o 
		numero n de itens para passar à lista. Caso a arvore tenha menos 
		itens do que o valor n. O valor n é modificado por referência 
		para que seja equivalente à quantidade de itens da arvore

	@Retorno:
		void** - Lista de ponteiros para itens genéricos, contendo os n primeiros itens 
*/
void** avl_n_primeiros(AVL* arvore, int* n)
{
	int counter = 0;
	void** lista_itens = (void**) malloc(sizeof(void*)*(*n));

	avl_n_primeiros_aux(&arvore->raiz, &counter,(*n), lista_itens);
	(*n) = counter;
	return lista_itens;
}

/*
	Função
		Função auxiliar que percorre os nós de uma árvore recursivamente e
		em ordem, repassando os itens de cada nó para a lista_itens, que
		aloca espaço dinamicamente. 
	@Parametros:
		NO** raiz - Nó atual da recursão.
		int* tamanho - Inteiro passado por referência que contém o tamanho
		da lista de itens e é incrementado a cada etapa da recursão.
		void*** lista_itens - Ponteiro triplo que referencia o ponteiro duplo
		void** que contém a lista de ponteiros para uma estrutura genérica do tipo void
	@Retorno:
		void
*/
void avl_lista_ordenada_aux(NO** raiz, int* tamanho, void*** lista_itens){
	if (*raiz == NULL)
		return;

	avl_lista_ordenada_aux(&(*raiz)->filhoEsquerdo, tamanho, lista_itens);

	(*tamanho)++;
	(*lista_itens) = (void**) realloc((*lista_itens), sizeof(void*)*(*tamanho));
	(*lista_itens)[(*tamanho) - 1] = (*raiz)->item;


	avl_lista_ordenada_aux(&(*raiz)->filhoDireito, tamanho, lista_itens);
}

/*
	Função
		A função recebe uma árvore e um inteiro tamanho passado por referência, ela percorre
		recursivamente, através de uma função auxiliar os nós em ordem da árvore, 
		passando todos para uma lista de intens genéricos que é retornada ao final.  
	@Parametros:
		AVL* arvore - Arvore da qual serão retirados os n primeiros itens
		int* tamanho - Inteiro passado por referencia que inicia em 0. Ele será incrementado
		para cada item que for adicionado na lista_itens. Ao fim da recursão ele equivale à
		quantidade de nós da árvore. 

	@Retorno:
		void** - Lista de ponteiros para itens genéricos, contendo os n primeiros itens 
*/
void** avl_lista_ordenada(AVL* arvore, int* tamanho){
	void** lista_itens = NULL;
	(*tamanho) = 0;

	avl_lista_ordenada_aux(&arvore->raiz, tamanho, &lista_itens);
	return lista_itens;
}