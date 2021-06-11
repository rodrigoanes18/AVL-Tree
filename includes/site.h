#ifndef SITE_H
#define SITE_H

#include <stdio.h>
typedef struct site SITE;

#define FALSE 0
#define TRUE 1

SITE* ler_site_arquivo(FILE* fp);

SITE* criar_site(int siteId, char* nome, int relevancia, char* link, char** palavras_chave, int numero_palavras_chave);
void remover_site(void* item);
void imprimir_site(void* item);
void imprimir_site_reduzido(void* item);
int comparar_sites(void* item1, void* item2);
int comparar_chave_site(void* k, void* item);
int inserir_nova_palavra_chave(SITE* site, char* novaPalavraChave);
int mudar_relevancia(SITE* site, int novaRelevancia);

int getSiteId(SITE* site);
char* getNome(SITE* site);
int getRelevancia(SITE* site);
char* getLink(SITE* site);
char** getPalavrasChave(SITE* site);
int getNumeroPalavraChave(SITE* site);
char* getPalavraChaveComIndice(SITE* site, int indice);

#endif
