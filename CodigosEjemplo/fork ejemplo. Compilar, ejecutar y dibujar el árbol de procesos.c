// fork ejemplo. Compilar, ejecutar y dibujar el árbol de procesos. Durante la clase del 12 de junio 2020 se pedirá la evidencia.

#include  <stdio.h>
#include  <string.h>
#include  <sys/types.h>
#include <unistd.h>

#include <sys/wait.h>

#define   MAX_COUNT  2
#define   BUF_SIZE   100

void  main(void)
{
     pid_t  pidh, pidp;
     int    i;
     char   buf[BUF_SIZE];
     printf("\nprintf antes del fork1... el pid = %d parent_pid es = %d guau es el pid del BASH! \n", getpid(), getppid());
     pid_t pidfork = fork();
     printf("\nprintf justo despues del fork1 pid= %d parent_pid= %d \n", getpid(), getppid());

     if(pidfork==0){
     pid_t parent = getppid();
     pid_t hijoA = getpid();
     printf("\nhijo %d parent %d \n", hijoA, parent);

       if(fork()==0){
       pidh = getpid();
       pid_t papapid= getppid();
     	for (i = 1; i <= MAX_COUNT; i++) {
        	  fprintf(stdout, "\nEsta linea viene del proceso con pid %d parentpid=%d,  value = %d\n", pidh, papapid, i);
        	  //write(0, buf, strlen(buf));
		  //sleep(1);
     	} 
      }else{
	printf("Yo soy mas flojo no hare el for\n");
      }
     }else{
       pidp = getpid();
       pid_t parpid= getppid();
     	for (i = 1; i <= MAX_COUNT; i++) {
        	  fprintf(stdout, "Conteo dentro del proceso con pid %d parentpid= %d, value = %d\n", pidp, parpid, i);
        	  //write(0, buf, strlen(buf));
    	} 
     }
     printf("aqui %d\n", getpid());
}
