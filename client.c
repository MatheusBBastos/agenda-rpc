/* Arquivo client.c: um cliente RPC simples */
 
#include <stdio.h>
#include "agenda.h"
 
/* função que chama a RPC consulta_1 */
consulta_res *consulta_contato(CLIENT *clnt, str_t nome) {
    consulta_res *res = consulta_1(&nome,clnt);
    if (res == NULL) {
        printf ("Problemas ao chamar a função remota\n");
        exit (1);
    }

    return res;
}

int insere_contato(struct contato_t *contato, CLIENT *clnt) {
    int *res = insere_1(contato, clnt);
    if (res == NULL) {
        printf("Problemas ao chamar a função remota\n");
        exit(1);
    }

    return *res;
}

int altera_contato(struct contato_t *contato, CLIENT *clnt) {
    int *res = altera_1(contato, clnt);
    if (res == NULL) {
        printf("Problemas ao chamar a função remota\n");
        exit(1);
    }

    return *res;
}

int remove_contato(str_t nome, CLIENT *clnt) {
    int *res = remove_1(&nome, clnt);
    if (res == NULL) {
        printf("Problemas ao chamar a função remota\n");
        exit(1);
    }

    return *res;
}
 
int main( int argc, char *argv[]) {
    int op;

    CLIENT *clnt; 
    /* verifica se o cliente foi chamado corretamente */
    if (argc != 2) {
        fprintf (stderr, "Usage: %s hostname\n", argv[0]);
        exit(1);
    }

    /* cria uma struct CLIENT que referencia uma interface RPC */
    clnt = clnt_create(argv[1], AGENDA_PROG, AGENDA_VERSION, "udp");

    /* verifica se a referência foi criada */
    if (clnt == (CLIENT *) NULL) {
        clnt_pcreateerror(argv[1]);
        exit(1);
    }

    struct contato_t contatoins = { "Nomezaum", "Endereçozaum", "Telefonezaum" };

    do {
        printf("AGENDA RPC\n");
        printf("Selecione uma opcao: ");
        printf("1 - Consultar\n");
        printf("2 - Inserir\n");
        printf("3 - Alterar\n");
        printf("4 - Remover\n");
        printf("5 - Sair\n");

        scanf("%d", &op);

        if(op == 1){
            char nome[255];
            
            printf("Nome: ");
            scanf("%s", nome);

            consulta_res *res = consulta_contato(clnt, nome);

            contato_p contato = res->consulta_res_u.contato;
            if (contato != NULL){
                printf("Nome: %s\n", contato->nome);
                printf("Endereço: %s\n", contato->endereco);
                printf("Telefone: %s\n", contato->telefone);
            }else{
                printf("Contato nao encontrado.\n");
            }    
        }
        else if (op == 2){
            char nome[255];
            char endereco[255];
            char telefone[255];

            printf("Nome: ");
            scanf("%s", nome);
            printf("Endereço: ");
            scanf("%s", endereco);
            printf("Telefone: ");
            scanf("%s", telefone);

            struct contato_t contatoins = { nome, endereco, telefone };
            int insere_res = insere_contato(&contatoins, clnt);

            if (insere_res == 1)
                printf("Contato inserido com sucesso.\n");
            else
                printf("Já existe um contato com esse nome.\n");
        }
        else if (op == 3){
            char nome[255];
            char endereco[255];
            char telefone[255];

            printf("Nome: ");
            scanf("%s", nome);
            printf("Endereço: ");
            scanf("%s", endereco);
            printf("Telefone: ");
            scanf("%s", telefone);

            struct contato_t contatoins = { nome, endereco, telefone };
            int altera_res = altera_contato(&contatoins, clnt);

            if (altera_res == 1)
                printf("Contato alterado com sucesso.\n");
            else
                printf("Não existe um contato com esse nome.\n");
        }
        else if (op == 4){
            char nome[255];
            
            printf("Nome: ");
            scanf("%s", nome);

            int remove_res = remove_contato(nome, clnt);

            if (remove_res == 1)
                printf("Contato removido com sucesso.\n");
            else
                printf("Não existe um contato com esse nome.\n");
        }
    }while(op != 5);

    
    return 0;
}