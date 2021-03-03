/* Arquivo server.c: um servidor RPC simples */
#include <stdio.h>
#include "agenda.h"
 
/* implementação da função consulta */
consulta_res * consulta_1_svc (str_t *nome, struct svc_req *rqstp) {
   static consulta_res res;
 
   printf("Recebeu consulta: %s\n", *nome);
   
   xdr_free(xdr_consulta_res, &res);

   contato_p *cp = &res.consulta_res_u.contato;
   contato_p cont = *cp = malloc(sizeof(struct contato_t));
   cont->nome = strdup("Mateusao");
   cont->endereco = strdup("Paranagua 800");
   cont->telefone = strdup("99845 3585");

   res.err = 0;

   return (&res);
}

int * insere_1_svc(struct contato_t *contato, struct svc_req *rqstp) {
   static int res;

   printf("Recebeu inserção: %s, %s, %s\n", contato->nome, contato->endereco, contato->telefone);

   res = 1;
   return (&res);
}

int * altera_1_svc(struct contato_t *contato, struct svc_req *rqstp) {
   static int res;

   printf("Recebeu alteração: %s, %s, %s\n", contato->nome, contato->endereco, contato->telefone);

   res = 1;
   return (&res);
}

int * remove_1_svc(str_t *nome, struct svc_req *rqstp) {
   static int res;

   printf("Recebeu remoção: %s\n", *nome);

   res = 1;
   return (&res);
}
