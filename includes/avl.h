#ifndef AVL_H
#define AVL_H


typedef struct avl AVL;

#define TRUE 1
#define FALSE 0
#define max(a, b) ((a > b) ? a : b)

typedef int boolean;

AVL* avl_criar(int (*_cmp) (void*, void*), int (*_cmp_chave) (void*, void*), void (* _imprimir) (void*), void (*_free_item) (void*));
void avl_apagar(AVL** arvore);
boolean avl_inserir(AVL* arvore, void* item);
boolean avl_remover(AVL* arvore, void* chave);
void* avl_buscar(AVL* arvore, void* chave);
void avl_imprimir(AVL* arvore);
void** avl_n_primeiros(AVL* arvore, int* n);
void** avl_lista_ordenada(AVL* arvore, int* tamanho);
void setFuncaoImprimir(AVL* arvore, void (* _imprimir) (void*));

#endif