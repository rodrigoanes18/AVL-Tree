#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <googlebot.h>

/*
    Função readLine:
        Le os caracteres de um fluxo de informações atá uma quebra de linha ou o fim de um arquivo,
        aloca esses caracteres na heap e retorna seu ponteiro
    @Parametros:
        FILE *stream - ponteiro para a stream que será lida (ex. stdin)
    @Retorno:
        Ponteiro para os caracteres lidos
 */
char* readLine(FILE* stream) {
    char* str = calloc(1, sizeof(char)), c;

    c = getc(stream);

    while (c != '\n' && c != EOF) {
        if (c != '\r') {
            str = realloc(str, sizeof(char) * (strlen(str) + 1) + sizeof(char));
            strncat(str, &c, 1);
        }
        c = getc(stream);
    }
    strncat(str, "\0", 1);
    return str;
}

/*
    Função interface_inserir_site:
        Pedirá ao usuário para inserir os valores dos componentes para a inserção de um novo site
        na avl de sites e para a inserção da palavra na avl de palavras
    @Parametros:
        GOOGLE_BOT* google_bot - ponteiro para a estrutura que armazena a avl de sites e a avl de palavras chave
    @Retorno
        void
 */
void interface_inserir_site(GOOGLE_BOT* google_bot) {
    int siteId, relevancia, numeroPalavrasChave;
    char* nome, * link, ** palavrasChave;

    printf("Insira um novo site\n");
    printf("\nCodigo (ex. 0002): ");
    scanf("%d%*c", &siteId);

    while (google_bot_verificar_site(google_bot, siteId))
    {
        printf("\nCodigo inválido, insira outro\n ");
        scanf("%d%*c", &siteId);
    }

    printf("\nNome do site: ");
    nome = readLine(stdin);
    printf("\nRelevancia (0 - 1000): ");
    scanf("%d%*c", &relevancia);
    printf("\nLink: ");
    link = readLine(stdin);
    printf("\nNumero de palavras chave: ");
    scanf("%d%*c", &numeroPalavrasChave);

    if (numeroPalavrasChave > 0) {
        palavrasChave = calloc(1, sizeof(char*));
        for (int i = 0; i < numeroPalavrasChave; i++) {
            if (i != 0) palavrasChave = realloc(palavrasChave, sizeof(char*) * (i + 1));
            palavrasChave[i] = readLine(stdin);
        }
    }

    SITE* novoSite = criar_site(siteId, nome, relevancia, link, palavrasChave, numeroPalavrasChave);
    // Aqui ocorre a chamada para a função de inserção
    boolean confirmacaoSite = google_bot_inserir_site(google_bot, novoSite);

    if (confirmacaoSite) {
        printf("Site inserido com sucesso.\n");
    }
    else {
        printf("ERRO: site nao pode ser inserido.\n");
    }
}

/*
    Função interface_remover_site:
        Pedirá ao usuário o código do site que se deseja remover e realizará a sua remoção da avl de sites
        e da avl de sites dentro da avl de palavras, por fim realiza a desalocação de sua memória
    @Parametros:
        GOOGLE_BOT* google_bot - ponteiro para a estrutura que armazena a avl de sites e a avl de palavras chave
    @Retorno:
        void
 */
void interface_remover_site(GOOGLE_BOT* google_bot) {
    int codigo;

    printf("Insira o codigo do site que deseja remover:\n");
    scanf("%d", &codigo);

    if (google_bot_remover_site(google_bot, codigo))
        printf("Site removido com sucesso\n");
    else
        printf("Falha de remoção\n");
}

/*
    Função interface_inserir_palavra_chave:
        Pedirá ao usuário o código que se deseja inserir a palavra chave, a palavra chave em si e realizará
        a sua inserção na estrutura SITE e na avl de palavras
    @Parametros:
        GOOGLE_BOT* google_bot - ponteiro para a estrutura que armazena a avl de sites e a avl de palavras chave
    @Retorno:
        void
 */
void interface_inserir_palavra_chave(GOOGLE_BOT* google_bot) {
    int codigo;
    char* palavra = NULL;
    SITE* site = NULL;

    printf("Insira o codigo do site que deseja editar:\n");
    scanf("%d%*c", &codigo);

    // Faz a busca pela estrutura SITE na lista de acordo com o seu codigo
    site = google_bot_buscar_site(google_bot, codigo);
    if (site == NULL) {
        printf("O site nao foi encontrado");
        return;
    }

    printf("Insira a nova palavra chave:\n");
    palavra = readLine(stdin);
    // Insere a palavra chave
    google_bot_inserir_palavra_chave(google_bot, site, palavra);
    return;
}

/*
    Função interface_atualizar_relevancia:
        Pede ao usuário o código do site que se deseja modificar, o valor da nova relevancia, faz
        a busca deste site na avl de sites e realiza a alteração da sua relevancia
    @Parametros:
        GOOGLE_BOT* google_bot - ponteiro para a estrutura que armazena a avl de sites e a avl de palavras chave
    @Retorno:
        void
 */
void interface_atualizar_relevancia(GOOGLE_BOT* google_bot) {
    int codigo;
    int relevancia;
    SITE* site = NULL;

    printf("Insira o codigo do site que deseja editar:\n");
    scanf("%d", &codigo);
    site = google_bot_buscar_site(google_bot, codigo);
    if (site == NULL) {
        printf("O site nao foi encontrado");
        return;
    }

    printf("Insira a nova relevancia:\n");
    scanf("%d", &relevancia);

    google_bot_mudar_relevancia_site(google_bot, site, relevancia);

    return;
}

/*
    Função interface_buscar_palavra_chave:
        Realiza a busca de sites que possuem a palavra chave inserida pelo usuário. Para isso, pede
        a ele que insira a palavra chave desejada. Por fim imprime a lista de sites encontrados
    @Parametros:
        GOOGLE_BOT* google_bot - ponteiro para a estrutura que armazena a avl de sites e a avl de palavras chave
    @Retorno:
        void
*/
void interface_buscar_palavra_chave(GOOGLE_BOT* google_bot) {
    char* palavra = NULL;

    printf("Insira a palavra chave que deseja buscar:\n");
    palavra = readLine(stdin);

    if(!google_bot_busca_palavra(google_bot, palavra))
        printf("A palavra chave não foi encontrada na base de dados\n");
    
    free(palavra);
    palavra = NULL;

    return;
}

/*
    Função interface_sugestao_sites:
        Realiza a busca de sites que possuem a palavra chave inserida pelo usuário, recolhe todas palavras chaves desses sites e 
        realiza novamente a busca de sites que possuem as novaa palavras chave recolhidas. Para isso, pede
        a ele que insira uma palavra chave desejada. Por fim imprime a lista dos 5 sites encontrados com maior indice de relevancia
    @Parametros:
        GOOGLE_BOT* google_bot - ponteiro para a estrutura que armazena a avl de sites e a avl de palavras chave
    @Retorno:
        void
*/
void interface_sugestao_sites(GOOGLE_BOT* google_bot) {
    char* palavra = NULL;

    printf("Insira a palavra chave:\n");
    palavra = readLine(stdin);

    if(!google_bot_sugerir_sites(google_bot, palavra))
        printf("A palavra chave não foi encontrada na base de dados\n");
    
    free(palavra);
    palavra = NULL;

    return;
}

/*
    Função interface_opcoes_usuario:
        Imprime em stdout um menu com as opções de ações possíveis e lê a escolha do usuário
    @Parametros:
        void
    @Retorno:
        int escolha - a escolha (int 1 - 6) do usuário
 */
int interface_opcoes_usuario() {
    int escolha;

    printf("Mini Googlebot\n");
    printf("1. Inserir um site\n");
    printf("2. Remover um site\n");
    printf("3. Inserir nova palavra-chave\n");
    printf("4. Atualizar relevancia\n");
    printf("5. Imprimir lista de sites\n");
    printf("6. Buscar por palavra-chave\n");
    printf("7. Sugestão de sites\n");
    printf("8. Sair\n\n");
    printf("Escolha uma acao(1 a 8): ");

    scanf("%d%*c", &escolha);
    while (escolha > 8 || escolha < 1) {
        printf("ERRO: opcao inexistente, escolha um valor entre 1 a 8.\n");
        scanf("%d", &escolha);
    }

    return escolha;
}

int main(int argc, char* argv[]) {
    GOOGLE_BOT* google_bot = criar_google_bot();
    //carregar_dados(arvore);
    int i = googlebot_carregar_dados(google_bot);

    int acaoUsuario;
    do {
        // LINHA DE CÓDIGO PARA DEBUGAR
        //google_bot_debug(google_bot);

        acaoUsuario = interface_opcoes_usuario();

        switch (acaoUsuario) {
        case 1:
            interface_inserir_site(google_bot);
            break;
        case 2:
            interface_remover_site(google_bot);
            break;
        case 3:
            interface_inserir_palavra_chave(google_bot);
            break;
        case 4:
            interface_atualizar_relevancia(google_bot);
            break;
        case 5:
            avl_imprimir(google_bot_get_avl_site(google_bot));
            break;
        case 6:
            interface_buscar_palavra_chave(google_bot);
            break;
        case 7:
            interface_sugestao_sites(google_bot);
            break;
        case 8:
            printf("Programa finalizado\n");
            break;
        default:
            break;
        }
    } while (acaoUsuario != 8);

    
    // Desaloca a memória da LISTA, de seus nós e das estruturas SITE

    google_bot_apagar(google_bot);
    google_bot = NULL;
    
    return EXIT_SUCCESS;
}