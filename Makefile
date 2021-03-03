INC_PATH = /usr/include/tirpc
LIB = tirpc
NAME = agenda

client: $(NAME)_clnt.c $(NAME)_xdr.c client.c
	gcc $(NAME)_clnt.c $(NAME)_xdr.c client.c -o client -I$(INC_PATH) -l$(LIB)

server: $(NAME)_svc.c $(NAME)_xdr.c server.c
	gcc $(NAME)_svc.c $(NAME)_xdr.c server.c -o server -I$(INC_PATH) -l$(LIB)

$(NAME)_svc.c $(NAME)_xdr.c $(NAME)_clnt.c: $(NAME).x
	rpcgen -C $(NAME).x


