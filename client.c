/* Arquivo client.c: um cliente RPC simples */
 
#include <stdio.h>
#include <sys/time.h>
#include "agenda.h"

void ler_string(char *string) {
    fgets(string, 255, stdin);
    string[strcspn(string, "\n")] = 0;
}
 
/* função que chama a RPC consulta_1 */
consulta_res *consulta_contato(CLIENT *clnt, str_t nome) {
    struct rpc_err err;

    consulta_res *res;
    do {
        res = consulta_1(&nome,clnt);
        /* retransmitir se ocorrer um timeout */
    } while (res == NULL && (clnt_geterr(clnt, &err), err.re_status == RPC_TIMEDOUT));

    if (res == NULL) {
        printf ("Problemas ao chamar a função remota\n");
        exit (1);
    }

    return res;
}

int insere_contato(struct contato_t *contato, CLIENT *clnt) {
    struct rpc_err err;

    int *res;
    do {
        res = insere_1(contato, clnt);
        /* retransmitir se ocorrer um timeout */
    } while (res == NULL && (clnt_geterr(clnt, &err), err.re_status == RPC_TIMEDOUT));

    if (res == NULL) {
        printf("Problemas ao chamar a função remota\n");
        exit(1);
    }

    return *res;
}

int altera_contato(struct contato_t *contato, CLIENT *clnt) {
    struct rpc_err err;

    int *res;
    do {
        res = altera_1(contato, clnt);
        /* retransmitir se ocorrer um timeout */
    } while (res == NULL && (clnt_geterr(clnt, &err), err.re_status == RPC_TIMEDOUT));

    if (res == NULL) {
        printf("Problemas ao chamar a função remota\n");
        exit(1);
    }

    return *res;
}

int remove_contato(str_t nome, CLIENT *clnt) {
    struct rpc_err err;
    
    int *res;
    do {
        res = remove_1(&nome, clnt);
        /* retransmitir se ocorrer um timeout */
    } while (res == NULL && (clnt_geterr(clnt, &err), err.re_status == RPC_TIMEDOUT));
    
    if (res == NULL) {
        printf("Problemas ao chamar a função remota\n");
        exit(1);
    }

    return *res;
}

contatos_t *lista_contatos(CLIENT *clnt) {
    struct rpc_err err;

    contatos_t *res;
    do {
        res = lista_1(NULL, clnt);
        /* retransmitir se ocorrer um timeout */
    } while (res == NULL && (clnt_geterr(clnt, &err), err.re_status == RPC_TIMEDOUT));
    
    if (res == NULL) {
        printf("Problemas ao chamar a função remota\n");
        exit(1);
    }

    return res;
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

    /* definir timeout para 10 segundos */
    struct timeval timeout = { 10, 0 };
    clnt_control(clnt, CLSET_TIMEOUT, &timeout);

    do {
        printf("========== AGENDA RPC ==========\n");
        printf("| 1 - Consultar                |\n");
        printf("| 2 - Inserir                  |\n");
        printf("| 3 - Alterar                  |\n");
        printf("| 4 - Remover                  |\n");
        printf("| 5 - Listar                   |\n");
        printf("| 6 - Sair                     |\n");

        printf("| Selecione uma opcao: ");
        scanf("%d%*c", &op);

        printf("================================\n");

        if(op == 1){
            char nome[255];

            printf("| Consultar contato\n");
            
            printf("| Nome: ");
            ler_string(nome);

            consulta_res *res = consulta_contato(clnt, nome);

            contato_p contato = res->consulta_res_u.contato;
            if (contato != NULL){
                printf("| Endereço: %s\n", contato->endereco);
                printf("| Telefone: %s\n", contato->telefone);
            }else{
                printf("| Contato nao encontrado.\n");
            }    
        }
        else if (op == 2){
            char nome[255];
            char endereco[255];
            char telefone[255];

            printf("| Inserir contato\n");

            printf("| Nome: ");
            ler_string(nome);
            printf("| Endereço: ");
            ler_string(endereco);
            printf("| Telefone: ");
            ler_string(telefone);

            struct contato_t contato = { nome, endereco, telefone };
            int insere_res = insere_contato(&contato, clnt);

            if (insere_res == 1)
                printf("| Contato inserido com sucesso.\n");
            else
                printf("| Já existe um contato com esse nome.\n");
        }
        else if (op == 3){
            char nome[255];
            char endereco[255];
            char telefone[255];

            printf("| Alterar contato\n");

            printf("| Nome: ");
            ler_string(nome);
            printf("| Endereço: ");
            ler_string(endereco);
            printf("| Telefone: ");
            ler_string(telefone);

            struct contato_t contato = { nome, endereco, telefone };
            int altera_res = altera_contato(&contato, clnt);

            if (altera_res == 1)
                printf("| Contato alterado com sucesso.\n");
            else
                printf("| Não existe um contato com esse nome.\n");
        }
        else if (op == 4){
            char nome[255];

            printf("| Remover contato\n");
            
            printf("| Nome: ");
            ler_string(nome);

            int remove_res = remove_contato(nome, clnt);

            if (remove_res == 1)
                printf("| Contato removido com sucesso.\n");
            else
                printf("| Não existe um contato com esse nome.\n");
        }
        else if (op == 5) {
            contatos_t *contatos = lista_contatos(clnt);

            printf("| Lista de contatos\n");

            if (contatos->contatos_t_len == 0) {
                printf("| Não há nenhum contato cadastrado...\n");
            } else {
                for (int i = 0; i < contatos->contatos_t_len; i++) {
                    printf("| -> %s\n", contatos->contatos_t_val[i].nome);
                }
            }

            xdr_free((xdrproc_t) xdr_contatos_t, contatos);
        }
        
        printf("\n");
    } while(op != 6);

    
    return 0;
}