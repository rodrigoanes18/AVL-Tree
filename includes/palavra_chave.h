#ifndef PALAVRA_CHAVE_H
#define PALAVRA_CHAVE_H

#include <avl.h>
#include <site.h>
#include <stdio.h>
typedef struct palavra_chave PALAVRA_CHAVE;

PALAVRA_CHAVE* criar_palavra_chave(char* _palavra);
PALAVRA_CHAVE* criar_palavra_chave_reduzida(char* _palavra);
void remover_palavra_chave(void* item);
void imprimir_palavra_chave(void* item);
int comparar_palavra_chave(void* item1, void* item2);
int comparar_chave_palavra_chave(void* k, void* item);

int comparar_site2(void* item1, void* item2);
int comparar_site_chave2(void* k, void* i);
void remover_site2(void* item);

AVL* palavraChaveGetSites(PALAVRA_CHAVE*);
char* palavraChaveGetNome(PALAVRA_CHAVE*);
#endif