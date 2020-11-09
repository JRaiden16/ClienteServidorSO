/** clientSocket.c wellknown code
* Documented by Oscar Herrera Nov2019 ia.azc.uam.mx
*/

/** standard input output buffered library
*/
#include <stdio.h>       

/** exit function to finalize
*/
#include <stdlib.h>

/** bzero function to write zeros
*/
#include <strings.h>

/** Socket support
*/   
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 3550 /* El puerto que será abierto menor a 1024 son puertos reservados*/
#define BACKLOG 2 /* El número de conexiones permitidas */

#include <string.h> 
main()
{

   int fd, fd2; /* los descriptores de archivo - todo flujo de E/S se ve como un descriptor de archivo*/

   struct sockaddr_in server; 
   /* para la información de la dirección del servidor */

   struct sockaddr_in client; 
   /* para la información de la dirección del cliente */

   int sin_size;

   /* A continuación la llamada a socket() */
   if ((fd=socket(AF_INET, SOCK_STREAM, 0)) == -1 ) {  
      printf("error en socket()\n");
      exit(-1);
   }

   server.sin_family = AF_INET;         
/**    From linux manual
*      htonl,  htons,  ntohl,  ntohs - convert values between host and network byte order
*/
   server.sin_port = htons(PORT); 

   server.sin_addr.s_addr = INADDR_ANY; 
   /* INADDR_ANY coloca nuestra dirección IP automáticamente */

   bzero(&(server.sin_zero),8); 
   /* escribimos ceros en el reto de la estructura */


   /* A continuación la llamada a bind() con el fd del socket */
   if(bind(fd,(struct sockaddr*)&server,
           sizeof(struct sockaddr))==-1) {
      printf("error en bind() \n");
      exit(-1);
   }     

   if(listen(fd,BACKLOG) == -1) {  /* llamada a listen() con el fd del socket */
      printf("error en listen()\n");
      exit(-1);
   }
   int nump=0;
   while(1) {
      sin_size=sizeof(struct sockaddr_in);
      /* A continuación la llamada a accept() que devuelve un fd2 una vez aceptada la conexion con los datos del cliente en la estructura*/
      if ((fd2 = accept(fd,(struct sockaddr *)&client,
                        &sin_size))==-1) {
         printf("error in accept()\n");
         exit(-1);
      }else{

/**     From linux manual
*       inet_aton,    inet_addr,    inet_network,   inet_ntoa,   inet_makeaddr,
*       inet_lnaof, inet_netof - Internet address manipulation routines
        Note the casting to char* since it returns an integer for IP
*/
      pid_t ph;
      if((ph=fork()==0)){

      char buf[64];
      sprintf(buf, "Welcome... served with a child process %d...\n", nump);
      
      //close(1);
      //dup(fd2);
      //printf("More data con printf...\n");
      //fprintf(stdout,"More data with fprintf stdout\n");
     send(fd2, buf,strlen(buf),0); 
//$ man 2 send --> ssize_t send(int sockfd, const void *buf, size_t len, int flags);

      /* que enviará el mensaje de bienvenida al cliente, mediante el fd2 del cliente */

	//nump++;
      //char buf2[64];
      //recv(fd2, buf2, sizeof (buf2), 0);
      //printf("leido %s \n", buf2);
      //close(0);
      //dup(teclado);
      close(fd2); /* cierra fd2 */
      break;
      }else{
      printf("A new connection from: %s\n",
             (char*)inet_ntoa(client.sin_addr) ); 
      /* que mostrará la IP del cliente */
	//close(
	nump++;
        close(fd2); /* el proceso padre cierra fd2 */
	printf("El proceso padre cerro el fd2...\n");
	//wait(NULL);
      }

      }
   }
}
