#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <site.h>

/*
    Struct site:
        Estrutura que armazena as informações de um site
    @Campos:
        int siteId - codigo do site (valor unico para cada objeto)
        char *nome - ponteiro para heap do nome do site
        int relevancia - valor de relevancia (0 a 1.000)
        char *link - pontoeiro para heap do link do site
        char **palavrasChave - ponteiro duplo para as palavras chave do site (uma matriz de letras)
        int numeroPalavrasChave - quantidade de palavras chave relacionadas a esse site
 */
struct site {
    int siteId;
    char* nome;
    int relevancia;
    char* link;
    char** palavrasChave;
    int numeroPalavrasChave;
};

/*
    Função criar_novo_site:
        Aloca um ponteiro para a estrutura SITE, inicializa suas variaveis e o retorna
    @Parametros:
        void
    @Retorno
        Ponteiro para uma estrutura SITE
 */
SITE* criar_novo_site() {
    SITE* novoSite = malloc(sizeof(SITE));

    if (novoSite != NULL) {
        novoSite->siteId = 0;
        novoSite->relevancia = 0;
        novoSite->nome = NULL;
        novoSite->link = NULL;
        novoSite->palavrasChave = NULL;
        novoSite->numeroPalavrasChave = 0;
    }

    return novoSite;
}

/*
    Função criar_site:
        Recebe os valores das variaveis da estrutura SITE digitados pelo usuario, aloca um novo SITE e atribui as suas
        variaveis os valores desejados pelo usuario. Devolve o ponteiro para essa estrutura
    @Parametros:
        int siteId - codigo de identificação
        char *nome - nome do site
        int relevancia - valor de relevancia do site
        char *link - link do site
        char **palavrasChave - matriz de palavras chave
        int numeroPalavrasChave - quantidade de palavras chave
    @Retorno:
        Ponteiro para essa estrutura SITE
 */
SITE* criar_site(int siteId, char* nome, int relevancia, char* link, char** palavrasChave, int numeroPalavrasChave) {
    SITE* novoSite = malloc(sizeof(SITE));

    if (novoSite != NULL) {
        novoSite->siteId = siteId;
        novoSite->relevancia = relevancia;
        novoSite->nome = nome;
        novoSite->link = link;
        novoSite->palavrasChave = palavrasChave;
        novoSite->numeroPalavrasChave = numeroPalavrasChave;
    }

    return novoSite;
}

/*
    Funçãoo ler_site_arquivo:
        Realiza a leitura por linha do arquivo de banco de dados .txt de sites, atribui os valores do banco de dados em uma estrutura SITE na memoria
        heap e retorna seu ponteiro.
    @Parametros:
        FILE *fp - ponteiro para o arquivo .txt
    @Retorno:
        Ponteiro para a estrutura SITE lida e alocada na memoria
 */
SITE* ler_site_arquivo(FILE* fp) {
    SITE* site = criar_novo_site();
    char nomeTemp[50];
    char linkTemp[100];
    char auxiliar;        // variavel responsavel por atuar como um buffer para as virgulas do arquivo

    fscanf(fp, "%d%c%[^,]%c%d%c%[^,\n]", &site->siteId, &auxiliar, nomeTemp, &auxiliar, &site->relevancia, &auxiliar, linkTemp);

    int k = 0;
    int counter1 = 0;
    while (nomeTemp[k] != '\0') {
        counter1++;
        k++;
    }
    int counter2 = 0;
    k = 0;
    while (linkTemp[k] != '\0') {
        counter2++;
        k++;
    }

    nomeTemp[counter1] = '\0';
    linkTemp[counter2] = '\0';

    site->nome = (char*)calloc(counter1 + sizeof(char), sizeof(char));
    site->link = (char*)calloc(counter2 + sizeof(char), sizeof(char));

    strcpy(site->nome, nomeTemp);
    strcpy(site->link, linkTemp);

    int i = 0;
    auxiliar = fgetc(fp);

    if (auxiliar != ',') {
        site->palavrasChave = NULL;
        site->numeroPalavrasChave = 0;
        return site;
    }

    char palavraTemp[10][50];
    int counter = 0;

    while (auxiliar != '\n' && auxiliar != EOF && auxiliar != '\r') {
        fscanf(fp, "%[a-zA-Z]", palavraTemp[i]);
        auxiliar = fgetc(fp);
        i++;
        counter++;
    }

    site->palavrasChave = malloc(counter * sizeof(char*));

    for (int j = 0; j < i; j++) {
        int size = 0;
        int k = 0;

        while (palavraTemp[j][k] != '\0') {
            size++;
            k++;
        }

        site->palavrasChave[j] = calloc((size + 1), sizeof(char));
        strcpy(site->palavrasChave[j], palavraTemp[j]);
    }

    site->numeroPalavrasChave = i;

    return site;
}

/*
    Função remover_site:
        Realiza a liberação de uma estrutura SITE da memoria heap
    @Parametros:
        void *site - ponteiro duplo para a estrutura SITE que se deseja desalocar
    @Retorno:
        void
 */
void remover_site(void* item) {
    SITE* site = (SITE*) item;
    if (site != NULL) {
        for (int i = 0; i < site->numeroPalavrasChave; i++) {
            free(site->palavrasChave[i]);
        }
        free(site->palavrasChave);
        free(site->nome);
        free(site->link);
        free(site);
    }
}

/*
    Função inserir_nova_palavra_chave:
        Recebe a palavra chave que sera inserida e a estrutra SITE que recebera a palavra e realiza
        a sua inserção
    @Parametros:
        SITE *site - ponteiro para estrutura SITE
        char *novaPalavraChave - palavra chave que sera inserida
    @Retorno:
        int 0 - inserção realizada com sucesso
        int 1 - inserção falhou
 */
int inserir_nova_palavra_chave(SITE* site, char* novaPalavraChave) {
    if (site == NULL) {
        printf("ERRO: site nao encontrado.\n");
        return FALSE;
    }

    if (site->numeroPalavrasChave >= 10) {
        printf("ERRO: numero de palavras chave lotado.\n");
        return FALSE;
    }

    site->numeroPalavrasChave++;
    site->palavrasChave = realloc(site->palavrasChave, sizeof(char*) * site->numeroPalavrasChave);
    site->palavrasChave[site->numeroPalavrasChave - 1] = calloc(strlen(novaPalavraChave) + sizeof(char), sizeof(char));

    strcpy(site->palavrasChave[site->numeroPalavrasChave - 1], novaPalavraChave);

    return TRUE;
}

/*
    Função mudar_relevancia:
        Função que altera a relevancia de um site para um valor desejado pelo usuario
    @Parametros:
        SITE *site - ponteiro para estrutura site que sofrera a mudança
        int novaRelevancia - valor da nova relevancia
    @Retorno:
        0 - alteração com sucesso
        1 - alteração falha
 */
int mudar_relevancia(SITE* site, int novaRelevancia) {
    if (site == NULL) {
        printf("ERRO: site nao encontrado.\n");
        return FALSE;
    }

    if (novaRelevancia < 0 || novaRelevancia > 1000) {
        printf("ERRO: nova relevancia deve ser um valor entre 0 e 1000.\n");
        return FALSE;
    }

    site->relevancia = novaRelevancia;
    return TRUE;
}

/*
    Função imprimir_site:
        Imprime em stdout os valores dos componentes de uma estrutura SITE
    @Parametros:
        SITE *site - ponteiro para a estrura a ser imprimida
    @Retorno:
        void
 */
void imprimir_site(void* item) {
    SITE* site = (SITE*) item;
    printf("============================\n");
    printf(">> Site: %s\n", site->nome);
    printf(">> Codigo: %d\n", site->siteId);
    printf(">> Link: %s\n", site->link);
    printf(">> Relevancia: %d\n", site->relevancia);
    printf(">> Palavras chave: ");
    for (int i = 0; i < site->numeroPalavrasChave; i++) {
        printf("%s | ", site->palavrasChave[i]);
    }
    printf("\n");
    printf("============================\n");
}

/*
    Função imprimir_site_reduzido:
        Imprime em stdout o nome e o link do site desejado
    @Parametros:
        SITE *site - ponteiro para a estrura a ser imprimida
    @Retorno:
        void   
*/
void imprimir_site_reduzido(void* item) {
    SITE* site = (SITE*) item;
    printf("============================\n");
    printf(">> Site: %s\n", site->nome);
    printf(">> Link: %s\n", site->link);
    printf("============================\n");
}

/*
    Função comparar_sites:
        Compara dois sites em relação ao seu id
    @Parametros:
        void* item1 - Ponteiros void de ambos itens a serem comparados
        void* item2
    @Retorno:
        Resultado da subtração entre o Id do primeiro site e o do segundo
*/
int comparar_sites(void* item1, void* item2) {
    SITE* site1 = (SITE*) item1;
    SITE* site2 = (SITE*) item2;

    return (site1->siteId - site2->siteId);
}

/*
    Função comparar_chave_site:
        Compara uma chave com um siteId
    @Parametros:
        void* item1 - Ponteiros void de ambos itens a serem comparados
        void* item2
    @Retorno:
        Resultado da subtração entre a chave e o Id do site        
*/
int comparar_chave_site(void* k, void* item) {
    SITE* site = (SITE*) item;
    int chave = *((int*)k);

    return (chave - site->siteId);
}

/*
    Embaixo encontram-se as funções de acesso de cada variavel de uma estrutura SITE.
    Cada uma recebendo um ponteiro para estrutura SITE a ser acessada e retornando o valor
    do seu respectivo componente
 */
int getSiteId(SITE* site) {
    if (site != NULL) {
        return site->siteId;
    }

    printf("ERRO: site nao encontrado.\n");
    return FALSE;
}

char* getNome(SITE* site) {
    if (site != NULL) {
        return site->nome;
    }

    printf("ERRO: site nao encontrado.\n");
    return NULL;
}

int getRelevancia(SITE* site) {
    if (site != NULL) {
        return site->relevancia;
    }

    printf("ERRO: site nao encontrado.\n");
    return FALSE;
}

char* getLink(SITE* site) {
    if (site != NULL) {
        return site->link;
    }

    printf("ERRO: site nao encontrado.\n");
    return NULL;
}

char** getPalavrasChave(SITE* site) {
    if (site != NULL) {
        return site->palavrasChave;
    }

    printf("ERRO: site nao encontrado.\n");
    return NULL;
}

/*
    Função get que retorna a palavra chave de um site em relação ao indice passado
*/
char* getPalavraChaveComIndice(SITE* site, int indice) {
    if (site == NULL) {
        printf("EERRO: site nao encontrado.\n");
        return NULL;
    }

    return site->palavrasChave[indice];
}

int getNumeroPalavraChave(SITE* site) {
    if (site == NULL) {
        printf("ERRO: site nao encontrado.\n");
        return -1;
    }

    return site->numeroPalavrasChave;
}

