#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
	struct sockaddr_in direccionServidor;
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = INADDR_ANY;
	direccionServidor.sin_port = htons(3550);

	int cnserver = socket(AF_INET, SOCK_STREAM, 0);

	int conexionactiva = 1;
	setsockopt(cnserver, SOL_SOCKET, SO_REUSEADDR, &conexionactiva, sizeof(conexionactiva));

	if (bind(cnserver, (void*) &direcciónServidor, sizeof(direccionServidor)) != 0) {
		perror("ERROR EN EL BIND");
		return -1;
	}
	
    /*if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(-1); */

	printf("Escuchando\n");
	listen(cnserver, 300);

	//------------------------------

	struct sockaddr_in direcciónCliente;
	unsigned int tamañoDirección;
	int cncliente = accept(cnserver, (void*) &direcciónCliente, &tamañoDirección);

	printf("Conexión de %d!!\n", cncliente);
	send(cncliente, "Bienvenido a ifgank!", 13, 0);
	send(cncliente, ":)\n", 4, 0);

	//------------------------------

	char* buffer = malloc(600);

	while (1) {
		int btsRecibidos = recv(cncliente, buffer, 600, 0);
		if (btsRecibidos <= 0) {
			perror("Cliente desconectado");
			return 1;
		}

		buffer[btsRecibidos] = '\0';
		printf("Total %d bytes con %s\n", btsRecibidos, buffer);
	}

	free(buffer);

	return 0;
}