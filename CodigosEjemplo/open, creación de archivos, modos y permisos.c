//open, creación de archivos, modos y permisos
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]){

unsigned char buf[10];
/*
	Bitwise operators				
				0000 0000 --> 0
				1100 0000 --> 128+64 maskRDWR = maskRD | maskWR
				1000 0000 maskRD
				0100 0000 msakWR
				1111 1111 --> 255
*/
   remove(argv[2]);
   unlink(argv[2]);
   int fd;
   if( strncmp(argv[2], "S_IRWXU", 7) == 0) //la resta de las cadenas seria cero si son iguales
	fd = open(argv[1], O_WRONLY|O_CREAT , S_IRWXU);
   else if (strncmp(argv[2], "S", 7) == 0)
	fd = open(argv[1], O_WRONLY|O_CREAT , S_IRUSR);
   else if (strncmp(argv[2], "S_IWUSR", 7) == 0)
	fd = open(argv[1], O_WRONLY|O_CREAT , S_IWUSR);
   else if (strncmp(argv[2], "S_IXUSR", 7) == 0)
	fd = open(argv[1], O_WRONLY|O_CREAT , S_IXUSR);
    else{
	printf("Escribe bien las opciones\n");
	exit(-1);
   }

   if(fd<0){
	printf("El archivo no pudo crearse en modo escritura con modo de uso %s\n", argv[2]);
        exit(-1);
   }  
   printf("El archivo se pudo crear para escribir satisfactoriamente con el fd= %d \n", fd);


	printf("se escribió un byte");
   close(fd);
}
