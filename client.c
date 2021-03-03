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

    int insere_res = insere_contato(&contatoins, clnt);
    printf("Resultado da inserção: %d\n", insere_res);

    int altera_res = altera_contato(&contatoins, clnt);
    printf("Resultado da alteração: %d\n", altera_res);

    int remove_res = remove_contato("Teste", clnt);
    printf("Resultado da remoção: %d\n", insere_res);


    consulta_res *res = consulta_contato(clnt, "Teste");

    if (res->err != 0) {
        printf("Erro recebido: %d\n", res->err);
        exit(1);
    }

    contato_p contato = res->consulta_res_u.contato;
    printf("Nome: %s\n", contato->nome);
    printf("Endereço: %s\n", contato->endereco);
    printf("Telefone: %s\n", contato->telefone);

    return 0;
}