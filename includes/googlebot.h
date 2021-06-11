#ifndef GOOGLEBOT_H
#define GOOGLEBOT_H
#include <avl.h>
#include <palavra_chave.h>
#include <site.h>

typedef int boolean;
typedef struct google_bot GOOGLE_BOT;

GOOGLE_BOT* criar_google_bot();
boolean googlebot_carregar_dados(GOOGLE_BOT*);
boolean google_bot_inserir_site(GOOGLE_BOT*, SITE*);
boolean google_bot_verificar_site(GOOGLE_BOT*, int);
SITE* google_bot_buscar_site(GOOGLE_BOT*, int);
boolean google_bot_remover_site(GOOGLE_BOT*, int);
boolean google_bot_inserir_palavra_chave(GOOGLE_BOT*, SITE*, char*);
void google_bot_mudar_relevancia_site(GOOGLE_BOT*, SITE*, int);
void google_bot_apagar(GOOGLE_BOT*);
boolean google_bot_busca_palavra(GOOGLE_BOT*, char*);
boolean google_bot_sugerir_sites(GOOGLE_BOT*, char*);

void google_bot_debug(GOOGLE_BOT*);


AVL* google_bot_get_avl_site(GOOGLE_BOT*);
AVL* google_bot_get_avl_palavras(GOOGLE_BOT*);

#endif