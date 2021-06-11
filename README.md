# AVL-Tree
AVL Tree implementation in C


Grupo:
Rodrigo Inui Miyake - 11218834
Rodrigo Anes Sena de Araújo - 9763064

Compilador: gcc (Ubuntu) 9.3.0

Instruções:

O programa inicia carregando uma base de dados de 79 itens. Logo em seguida, é impressa a interface do
programa e seus comandos. O usuário pode realizar quantas ações quiser e depois sair do programa.
Ao iniciar o Mini Googlebot, o usuário poderá realizar 8 ações (digitando o valor de 1 - 8):
    1. Inserir um site novo a lista com os componentes desejados pelo usuário
    2. Remover um site da lista a partir de seu código de identificação
    3. Inserir uma nova palavra chave digitada pelo usuário a um site do banco de dados identificado pelo
    código informado pelo usuário
    4. Modificar a relevancia de um site para um valor inserido pelo usuário
    5. Imprimir a lista inteira do banco de dados de sites
    6. Buscar por palavra-chave
    7. Sugestão de sites
    8. Finalizar o programa

Justificativa da TAD implementada:

Neste projeto foi utilizada uma lista encadeada. Ela está armazenada na heap e permite economia de memória.
Esta implementação não sofre problemas de espaços de memória inutilizados. 
A TAD é ordenada de forma crescente pelas chaves dos sites.
A ordenação da lista otimiza os processos de busca, inserção e remoção, reduzindo pela metade o tempo médio de tais operações.
As velocidades de busca, inserção e remoção são de complexidade O(n). Sendo n a quantidade de itens na lista.
