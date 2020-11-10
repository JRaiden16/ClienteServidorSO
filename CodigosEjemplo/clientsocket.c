/** clientSocket.c wellknown code
* Documented by Oscar Herrera Nov2019 ia.azc.uam.mx
*/

/** exit function to finalize
*/
#include <stdlib.h>

/** standard input output buffered library
*/
#include <stdio.h>

/** bzero function to write zeros
*/
#include <strings.h>
#include <string.h>
/** Socket support
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/**
* netbd.h es necesitada por la estructura hostent 
*/

#include <netdb.h>        

/** PORT to be connected. The server counterpart opens the same PORT
*/
#define PORT 3550         

#define MAXDATASIZE 100   
/* El número máximo de datos en bytes */

int main(int argc, char *argv[])
{
   int fd, numbytes;       
   /* descriptor de archivo, en realidad un socket cliente */

   char buf[MAXDATASIZE];  
   /* en donde es almacenará el texto recibido */

   struct hostent *he;         
   /* estructura que recibirá información sobre el nodo remoto */

   struct sockaddr_in server;  
   /* información sobre la dirección del servidor */

   if (argc !=2) { 
      /* esto es porque nuestro programa sólo necesitará un
      argumento, (la IP) */
      printf("Usage: %s <Dirección IP>\n", argv[0]);
      exit(-1);
   }

   if ((he=gethostbyname(argv[1]))==NULL){       
      /* llamada a gethostbyname() */
      printf("gethostbyname() error\n");
      exit(-1);
   }

   if ((fd=socket(AF_INET, SOCK_STREAM, 0))==-1){  
      /* llamada a socket() */
      printf("socket() error\n");
      exit(-1);
   }

   server.sin_family = AF_INET;
   server.sin_port = htons(PORT); 
   /* htons() es necesaria nuevamente ;-o */
   server.sin_addr = *((struct in_addr *)he->h_addr);  
   /*he->h_addr pasa la información de ``*he'' a "h_addr" */
   bzero(&(server.sin_zero),8);

   if(connect(fd, (struct sockaddr *)&server,
      sizeof(struct sockaddr))==-1){ 
      /* llamada a connect() */
      printf("connect() error\n");
      exit(-1);
   }

   if ((numbytes=recv(fd,buf,MAXDATASIZE,0)) == -1){  
      /* llamada a recv() */
      printf("Error en recv() \n");
      exit(-1);
   }

   buf[numbytes]='\0';

   printf("Mensaje del Servidor: %s contenido en %d bytes\n", buf, numbytes); 
   /* muestra el mensaje de bienvenida del servidor =) */

   sprintf(buf, "Ahora el cliente escribe al servidor este mensaje\n");
   send(fd, buf,strlen(buf),0); 
   close(fd);   /* cerramos fd =) */

}

