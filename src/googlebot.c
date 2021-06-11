#include <googlebot.h>
#include <stdlib.h>

/*
    Struct google_bot:
        Registro utilizado para armazenar as arvores de site e de palavras
    @Campos:
        AVL* arvoreSites - avl de sites que funciona como o banco de dados de sites do programa
        AVL* arvorePalavras - avl de palavras que armazena as palavras chaves dos sites e os sites em que elas aparecem
*/
struct google_bot {
    AVL* arvoreSites;
    AVL* arvorePalavras;
};

/*
	Função criar_google_bot:
        Função de inicialização do registro google_bot
	@Parametros:
        void
	@Retorno:
        Ponteiro para o registro google_bot criado
*/
GOOGLE_BOT* criar_google_bot(){
    GOOGLE_BOT* google_bot = (GOOGLE_BOT*) malloc(sizeof(GOOGLE_BOT));
    google_bot->arvoreSites = NULL;
    google_bot->arvorePalavras = NULL;
    return google_bot;
}

/*
	Função googlebot_carregar_dados:
        Le o arquivo que contem os dados dos sites, cria uma avl para os sites e uma para as palavras e aloca os dados em ambas
	@Parametros:
        GOOGLE_BOT* google_bot - registro google_bot onde as avl serão inseridas
	@Retorno:
        TRUE com o sucesso do carregamento de dados
*/
boolean googlebot_carregar_dados(GOOGLE_BOT* google_bot) {
    AVL* arvoreSites = avl_criar(comparar_sites, comparar_chave_site, imprimir_site, remover_site);
    AVL* arvorePalavras = avl_criar(comparar_palavra_chave, comparar_chave_palavra_chave, imprimir_palavra_chave, remover_palavra_chave);
    
    PALAVRA_CHAVE* palavra_chave = NULL;
    char* palavra = NULL; 
    SITE* site = NULL;
    FILE* fp = fopen("res/googlebot.txt", "r");
    char auxiliar;

    while (auxiliar = fgetc(fp), auxiliar != EOF) {
        fseek(fp, -1L, SEEK_CUR);
        // Armazena a estutura SITE da linha do arquivo
        site = ler_site_arquivo(fp);
        // Insere essa esturura na avl de sites
        avl_inserir(arvoreSites, site);
        
        for (int i = 0; i < getNumeroPalavraChave(site); i++) {
            palavra= getPalavraChaveComIndice(site, i);
            palavra_chave = avl_buscar(arvorePalavras, palavra);
            if(palavra_chave == NULL){
                palavra_chave = criar_palavra_chave(palavra);
                // Insere a palavra chave na avl de palavras
                avl_inserir(arvorePalavras, palavra_chave);
            }
            // Insere o site na avl de sites dentro do nó da avl de palavras
            avl_inserir(palavraChaveGetSites(palavra_chave), site);
        }
    }

    fclose(fp);
    
    google_bot->arvoreSites = arvoreSites;
    google_bot->arvorePalavras = arvorePalavras;

    return TRUE;
}

/*
	Função google_bot_inserir_site:
        Função que realiza a inserção de um site na avl de sites, recolhe as palavras chave desse site e as insere
        na avl de palavras. Por fim, insere esse site na avl de sites dentro do no da avl de palavras
	@Parametros:
        GOOGLE_BOT* google_bot - registro que contem as avl nas quais a inserção ocorrerá
        SITE* site - site a ser inserido
	@Retorno:
        TRUE ao fim da inserção
*/
boolean google_bot_inserir_site(GOOGLE_BOT* google_bot, SITE* site) {

    PALAVRA_CHAVE* palavra_chave = NULL;
    
    for (int i = 0; i < getNumeroPalavraChave(site); i++)
    {
        char* palavra = getPalavraChaveComIndice(site, i);
        
        palavra_chave = avl_buscar(google_bot->arvorePalavras, palavra);
        if(palavra_chave == NULL){
            palavra_chave = criar_palavra_chave(palavra);
            avl_inserir(google_bot->arvorePalavras, palavra_chave);
        }

        avl_inserir(palavraChaveGetSites(palavra_chave), site);
    }

    avl_inserir(google_bot->arvoreSites, site);
    return TRUE;
}

/*
	Função google_bot_verificar_site:
        Verifica se o site com um Id escolhido pelo usuario existe na avl de sites
	@Parametros:
        GOOGLE_BOT* google_bot - registro que contem a arvore de sites a ser buscada
        int siteId - Id do site procurado
	@Retorno:
        TRUE caso o site exista, FALSE caso seja inexistente
*/
boolean google_bot_verificar_site(GOOGLE_BOT* google_bot, int siteId) {
    SITE* siteAuxiliar = avl_buscar(google_bot->arvoreSites, &siteId);

    if (siteAuxiliar == NULL)
        return FALSE;
    
    return TRUE;
}

/*
	Função google_bot_buscar_site:
        Busca um site em um avl de sites de acordo com um Id desejado
	@Parametros:
        GOOGLE_BOT* google_bot - registro que armazena a avl de sites utilizada na busca
        int siteId - Id do site procurado
	@Retorno:
        Ponteiro para o site buscado (SITE*)
*/
SITE* google_bot_buscar_site(GOOGLE_BOT* google_bot, int siteId) {
    SITE* siteBuscado = avl_buscar(google_bot->arvoreSites, &siteId);

    return siteBuscado;
}


/*
	Função google_bot_remover_site:
        Remove um site desejado da avl de sites e dos respectivos nós da avl de palavras nos quais esse site aparece
	@Parametros:
        GOOGLE_BOT* google_bot - registro que contem as avl nas quais serao realizadas as remoções
        int siteId - Id do site que se deseja remover
	@Retorno:
        TRUE ao fim da remoção
*/
boolean google_bot_remover_site(GOOGLE_BOT* google_bot, int siteId) {
    PALAVRA_CHAVE* palavraChave = NULL;
    SITE* site = google_bot_buscar_site(google_bot, siteId);
    
    for (int i = 0; i < getNumeroPalavraChave(site); i++)
    {
        char* palavra = getPalavraChaveComIndice(site, i);
        
        palavraChave = avl_buscar(google_bot->arvorePalavras, palavra);
        
        // Remove o site da avl de site dentro do no da palavra chave
        avl_remover(palavraChaveGetSites(palavraChave), site);
    }

    // Remove o site da avl de sites
    avl_remover(google_bot->arvoreSites, &siteId);

    return TRUE;
}

/*
	Função google_bot_inserir_palavra_chave:
        Realiza a inserção de um palavra chave escolhida pelo usuário no registro do site indicado. Em seguida 
        na avl de palavras e por fim, insere o site na avl de site dentro do no da palavra chava na avl de palavras
	@Parametros:
        GOOGLE_BOT* google_bot - registro que armazena a avl de site e a avl de palavras nas quais sera realizada a insercao
        SITE* site - site escolhido pelo usuario a ser editado
        char* nova_palavra - nova palavra chave que sera inserida no site
	@Retorno:
        TRUE se a inserção for correta, FALSE caso ocorra um erro
*/
boolean google_bot_inserir_palavra_chave(GOOGLE_BOT* google_bot, SITE* site, char* nova_palavra){
    PALAVRA_CHAVE* palavra_chave = NULL;
    // Inserir palavra chave no site     
    // Buscar nova_palavra em arvorePalavras
    // Se não existir, inserir palavra em arvorePalavras
    // Inserir o site no nó palavra da nova_palavra. 

    if(inserir_nova_palavra_chave(site, nova_palavra)){
        palavra_chave = (PALAVRA_CHAVE*) avl_buscar(google_bot->arvorePalavras, nova_palavra);
        if(palavra_chave == NULL){
            palavra_chave = (PALAVRA_CHAVE*) criar_palavra_chave(nova_palavra);
            avl_inserir(google_bot->arvorePalavras, palavra_chave);
        }
        avl_inserir(palavraChaveGetSites(palavra_chave), site);        
    }else{
        return FALSE;
    }

    return TRUE;
}

/*
	Função google_bot_mudar_relevancia_site:
        Primeiro a função recolhe todas as palavras chave desse site que se deseja mmudar a relevancia. Faz a busca por elas na avl de palavras e remove o site de seus respectivos nos.
        Então muda a relevancia do site e o insere novamente nos nós das palavras chave na avl de palavras.
	@Parametros:
        GOOGLE_BOT* google_bot - registro que contém a avl de sites e a avl de palavras
        SITE* site - site a ser alterado a relevancia
        int nova_relevancia - novo fator de relevancia escolhido pelo usuario
	@Retorno:
        void
*/
void google_bot_mudar_relevancia_site(GOOGLE_BOT* google_bot, SITE* site, int nova_relevancia){
    for (int i = 0; i < getNumeroPalavraChave(site); i++){
        char* palavra = getPalavraChaveComIndice(site, i);
        PALAVRA_CHAVE* palavra_chave = avl_buscar(google_bot->arvorePalavras, palavra);
        avl_remover(palavraChaveGetSites(palavra_chave), site);
    }

    mudar_relevancia(site, nova_relevancia);
    
    for (int i = 0; i < getNumeroPalavraChave(site); i++){
        char* palavra = getPalavraChaveComIndice(site, i);
        PALAVRA_CHAVE* palavra_chave = avl_buscar(google_bot->arvorePalavras, palavra);
        avl_inserir(palavraChaveGetSites(palavra_chave), site);
    }
}

/*
	Função google_bot_get_avl_site:
        Retorna a arovre de sites do registro google_bot
	@Parametros:
        GOOGLE_BOT* google_bot - registro onde a arvore deseja esta armazenada
	@Retorno:
        Ponteiro para a arvore de sites (AVL*)
*/
AVL* google_bot_get_avl_site(GOOGLE_BOT* google_bot) {
    return google_bot->arvoreSites;
}

/*
	Função google_bot_get_avl_palavras:
        Retorna a arovre de palavras do registro google_bot
	@Parametros:
        GOOGLE_BOT* google_bot - registro onde a arvore deseja esta armazenada
	@Retorno:
        Ponteiro para a arvore de palavras (AVL*)
*/
AVL* google_bot_get_avl_palavras(GOOGLE_BOT* google_bot) {
    return google_bot->arvorePalavras;
}

/*
	Função google_bot_apagar:
        Função que realiza a desalocação de memoria das avl de sites e de palavras do registro google_bot. Por fim,
        desaloca memoria do registro google_bot
	@Parametros:
        GOOGLE_BOT* google_bot - registro que sera desalocado a memoria
	@Retorno:
        void
*/
void google_bot_apagar(GOOGLE_BOT* google_bot) {
    avl_apagar(&(google_bot->arvorePalavras));
    avl_apagar(&(google_bot->arvoreSites));
    free(google_bot);
}

/*
	Função google_bot_debug:
        Função auxiliar que imprime a avl de palavras em modo DEBUG
	@Parametros:
        GOOGLE_BOT* google_bot - registro onde a arvore de palavras esta armazenada
	@Retorno:
        void
*/
void google_bot_debug(GOOGLE_BOT* google_bot){
    printf("\n|| INICIO DEBUG|| \n");
    avl_imprimir(google_bot->arvorePalavras);
    printf("\n|| FIM DEBUG|| \n");
}

/*
	Função google_bot_busca_palavra:
        Realiza a busca da palavra chave inserida pelo usuario na avl de palavras e imprime os sites (apenas nomes e links)
        em ordem decrescente em relação ao seu nivel de relevancia que estao relacionados com a palavra chave escolhida
	@Parametros:

	@Retorno:
*/
boolean google_bot_busca_palavra(GOOGLE_BOT* google_bot, char* palavra) {
    PALAVRA_CHAVE* palavra_chave = NULL;

    palavra_chave = (PALAVRA_CHAVE*) avl_buscar(google_bot->arvorePalavras, palavra);
    if(palavra_chave == NULL) return FALSE;

    // Altera a funcao imprimir da avl de sites para imprimir apenas o nome e o link
    setFuncaoImprimir(palavraChaveGetSites(palavra_chave), imprimir_site_reduzido);

    avl_imprimir(palavraChaveGetSites(palavra_chave));

    // Retorna para a função de impressão completa
    setFuncaoImprimir(palavraChaveGetSites(palavra_chave), imprimir_site);

}

/*
	Função
        Essa função sugere os 5 sites mais relevantes que possuem alguma relação com a
        palavra buscada. 
	@Parametros:
        GOOGLE_BOT* google_bot - registro que contém as TADs e dados necessárias para operações
        simplificadas de um buscador do google.
        char* palavra - palavra que deseja ser pesquisada
	@Retorno:
        boolean - retorna false se a palavra não existe na base de dados do google_bot, caso 
        contrário, retorna true.
*/
boolean google_bot_sugerir_sites(GOOGLE_BOT* google_bot, char* palavra){
    int tamanho = 0, tamanho2 = 0;
    PALAVRA_CHAVE* palavra_chave = NULL;
    PALAVRA_CHAVE** lista_palavras_chave = NULL;
    SITE** lista_sites = NULL;
    SITE* site = NULL;
    // aux1 para arvore de palavras
    AVL* aux1 = avl_criar(comparar_palavra_chave, comparar_chave_palavra_chave, imprimir_palavra_chave, remover_palavra_chave);
    // aux2 para arvore de sites por relevancia
    AVL* aux2 = avl_criar(comparar_site2, comparar_site_chave2, imprimir_site, remover_site2);
    

    
    //Instrucoes:
    //buscar palavra_chave de palavra
    palavra_chave = (PALAVRA_CHAVE*) avl_buscar(google_bot->arvorePalavras, palavra);
    if(palavra_chave == NULL) return FALSE;
   
    //percorrer cada site da arvore de sites da palavra

    lista_sites = (SITE**) avl_lista_ordenada(palavraChaveGetSites(palavra_chave), &tamanho);
    
    //para cada site, percorrer lista de palavras_chave
    //para cada palavra_chave buscar na arvore auxiliar
    //se não estiver na árvore auxiliar, criar nova palavra_chave e inserir  
    for(int i = 0 ; i < tamanho; i++){
        for (int j = 0; j < getNumeroPalavraChave(lista_sites[i]); j++){
            palavra = getPalavraChaveComIndice(lista_sites[i], j);
            if(avl_buscar(aux1, palavra) == NULL){
                palavra_chave = criar_palavra_chave_reduzida(palavra);
                avl_inserir(aux1, palavra_chave);
            }
        }
    }
    free(lista_sites);
    lista_sites = NULL;

    lista_palavras_chave = (PALAVRA_CHAVE**) avl_lista_ordenada(aux1, &tamanho);
    //percorre as palavra_chave
        //percorre a arvoreSites
        //para cada site, buscar em uma arvore auxiliar 2 (arvoreSites, ordenada pela relevancia)
        //se nao tiver na arvore aux 2 e insere
    for(int i = 0 ; i < tamanho; i++){
        palavra_chave = (PALAVRA_CHAVE*) avl_buscar(google_bot->arvorePalavras, palavraChaveGetNome(lista_palavras_chave[i]));
        tamanho2 = 5;
        lista_sites = (SITE**) avl_n_primeiros(palavraChaveGetSites(palavra_chave), &tamanho2);
        for(int j = 0; j < tamanho2; j++){
            int id = getSiteId(lista_sites[j]);
            if(avl_buscar(aux2, &id) == NULL){
                avl_inserir(aux2, lista_sites[j]);
            }
        }
        free(lista_sites); 
        lista_sites = NULL;
    }
    free(lista_palavras_chave);

    tamanho2 = 5;
    lista_sites = (SITE**) avl_n_primeiros(aux2, &tamanho2);
    
    for(int i = 0; i < tamanho2; i++)
        imprimir_site_reduzido(lista_sites[i]);

    free(lista_sites);
    lista_sites = NULL;

    avl_apagar(&aux1);
    avl_apagar(&aux2);
    return TRUE;
}       
