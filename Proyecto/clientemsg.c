#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[]))
{
	struct sockaddr_in serverAddress;
	
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("192.168.0.0");
	serverAddress.sin_port = htons(3550);

	int cnclient = socket(AF_INET, SOCK_STREAM, 0);
	
	if (connect(cnclient, (void*) &serverAddress, sizeof(serverAddress)) != 0)
	{
		perror("No se pudo conectar");
		return 1;
	}

	while (1)
	{
		char msg[250];
		scanf("%s", msg);
		send(cnclient, msg, strlen(msg), 0);
	}
	return 0;
}