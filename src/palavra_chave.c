#include <palavra_chave.h>
#include <string.h>
#include <stdlib.h>

/*
    Struct palavra_chave:
        Registro de palavras utilizado na avl de palavras, armazena a palavra chave 
        e uma avl com os sites relacionados a essa palavra
    @Campos:
        char* palavra - palavra chave
        AVL* sites - avl dos sites relacionados a essa palavra
*/
struct palavra_chave{
    char* palavra;
    AVL* sites;
};

/*
	Função criar_palavra_chave:
        Inicializa um registro PALAVRA_CHAVE com uma palavra chave informada e uma avl de sites inicializada
	@Parametros:
        char* _palavra - palavra chave que sera a chave desse registro
	@Retorno:
        Ponteiro para a estrutura inicializada (PALAVRA_CHAR*)
*/
PALAVRA_CHAVE* criar_palavra_chave(char* _palavra){
    PALAVRA_CHAVE* p = (PALAVRA_CHAVE*) malloc(sizeof(PALAVRA_CHAVE));
    p->palavra = (char*) malloc(sizeof(char*)*(strlen(_palavra) + 1));
    strcpy(p->palavra, _palavra);
    p->sites = avl_criar(comparar_site2, comparar_site_chave2, imprimir_site, remover_site2);
}

/*
	Função criar_palavra_chave_reduzida:
        Inicializa um registro PALAVRA_CHAVE com uma palavra chave informada e uma avl de sites vazia (NULL)
	@Parametros:
        char* _palavra - palavra chave que sera a chave desse registro
	@Retorno:
        Ponteiro para a estrutura inicializada (PALAVRA_CHAR*)
*/
PALAVRA_CHAVE* criar_palavra_chave_reduzida(char* _palavra){
    PALAVRA_CHAVE* p = (PALAVRA_CHAVE*) malloc(sizeof(PALAVRA_CHAVE));
    p->palavra = (char*) malloc(sizeof(char*)*(strlen(_palavra) + 1));
    strcpy(p->palavra, _palavra);
    p->sites = NULL;
}

/*
	Função remover_palavra_chave:
        Esta função recebe um item genérico (void*), ela realiza um casting
        para mudar seu tipo para (PALAVRA_CHAVE*) e desaloca a memória dos
        campos necessários e em seguida desaloca a própria struct.
	@Parametros:
        void* item - item genérico que deve ser desalocado
	@Retorno:
        void
*/
void remover_palavra_chave(void* item){
    PALAVRA_CHAVE* p = (PALAVRA_CHAVE*) item;
    if(p->sites != NULL) avl_apagar(&p->sites);
    free(p->palavra);
    free(p);
}

/*
	Função imprimir_palavra_chave:
        
	@Parametros:
        void* item - 
	@Retorno:
        void
*/
void imprimir_palavra_chave(void* item){
    PALAVRA_CHAVE* p = (PALAVRA_CHAVE*) item;    
    printf("Palavra Chave: %s\n", p->palavra);
    avl_imprimir(p->sites);
}

/*
	Função comparar_palavra_chave:
        
	@Parametros:
        void* item - 
	@Retorno:
        int
*/
int comparar_palavra_chave(void* item1, void* item2){
    PALAVRA_CHAVE* p1 = (PALAVRA_CHAVE*) item1;
    PALAVRA_CHAVE* p2 = (PALAVRA_CHAVE*) item2;

    return strcmp(p1->palavra, p2->palavra);
}

/*
	Função comparar_chave_palavra_chave:
        
	@Parametros:
        void* k - 
        void* item - 
	@Retorno:
        int
*/
int comparar_chave_palavra_chave(void* k, void* item){
    char* s = (char*) k;
    PALAVRA_CHAVE* p = (PALAVRA_CHAVE*) item;
    return strcmp(s, p->palavra);
}

/*
	Função palavraChaveGetSites:
        Recebe um registro de PALAVRA_CHAVE e retorna a avl de sites que ela armazena
	@Parametros:
        PALAVRA_CHAVE* palavra_chave - registro de palavras chave que armazena a avl de sites
	@Retorno:
        Ponteiro para a arvore de sites
*/
AVL* palavraChaveGetSites(PALAVRA_CHAVE* palavra_chave){
    return palavra_chave->sites;
}

/*
	Função palavraChaveGetNome
        Retorna a palavra chave armazenada em um registro PALAVRA_CHAVE
	@Parametros:
        PALAVRA_CHAVE* palavra_chave - registro que se deseja saber a palavra chave armazenada
	@Retorno:
        Ponteiro para a palavra chave (char*)
*/
char* palavraChaveGetNome(PALAVRA_CHAVE* palavra_chave){
    return palavra_chave->palavra;
}

// as funções a seguir são usadas somente como referência para a árvore AVL de sites dentro da struct PALAVRA_CHAVE
// os nomes dessas funções específicas tem o número 2 ao fim, para diferenciá-las das funções contidas no arquivo site.h
int comparar_site2(void* item1, void* item2){
    SITE* site1 = (SITE*) item1;
    SITE* site2 = (SITE*) item2;

    if(getRelevancia(site1) != getRelevancia(site2))
        return (getRelevancia(site2) - getRelevancia(site1));
    else
        return (getSiteId(site1) - getSiteId(site2));
}

int comparar_site_chave2(void* k, void* i){
    SITE* site1 = (SITE*) k;
    SITE* site2 = (SITE*) i;

    if(getRelevancia(site1) != getRelevancia(site2))
        return (getRelevancia(site2) - getRelevancia(site1));
    else
        return (getSiteId(site1) - getSiteId(site2));
}

// não é necessário realizar free no item referenciado, pois ele receberá free na árvore de sites, quando removido
void remover_site2(void* item){
    return;
}