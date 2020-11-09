//Demostracion de memoria compartida
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>

int main()
{
/**
	programa:
		codigo   
		datos:  i  *number
		pila: parametros...

	RAM: 	*number      0 -----------------------------------------
		free(number) X XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

*/
    int child;
    //int *number = NULL;  //definicion del apuntador
    //number = (int*)malloc(1*sizeof(int));  //solicitud de memoria dinamica
    //number = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
   int i=0;  //memoria estatica para un entero

   // *number = 0;

    child = fork();

    if (child==-1){
        exit(1);
    }   else if (child==0)    {  //HIJ_0 O
        //int ih= *number;
        for(;i<10;i++) printf("ih= %d\n", i);
        /*{
            usleep(10);
            printf ("CHILD -- Number: %d\n", *number);
            *number=*number+1;
            if(*number >= 10)
            {
                exit(0);
            }
        }*/

    }
    else
    {
        //int ip= *number;
        for(;i<10;i++) write(0,&i, sizeof(int));

        /*{
            //usleep(200);
            printf ("PARENT -- Number: %d\n", *number);
	   if(*number >7)
            *number=*number+1;
            if(*number >= 10)
            {
                exit(0);
            }

        }*/
        wait(NULL);
        //free(number);
        //munmap(number, sizeof(int));

    }
}
