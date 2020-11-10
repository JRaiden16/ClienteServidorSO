/**
Modificacion de la version original del libro
Unix: Programación avanzada
Autor: Francisco Marquez Garcia
Alfaomega, 2005

Version adaptada con fork para soporte de procesos padre-hijo
*/

#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#define MAX 256

//Macro para comparar dos cadenas de caracteres
#define EQ(str1, str2)(strcmp((str1),(str2))==0)
//Descriptores de ficheros de la tuberias con nombre mediante las cuales vamos a comunicarnos
int fifo_12, fifo_21;
char nombre_fifo_12[MAX], nombre_fifo_21[MAX];

//Array para leer los mensajes
char mensaje[MAX];
pid_t pid;

int main(int argc, char *argv[]){
    char *logname, *getenv();
    void fin_de_transmision();
    //Analisis de los argumentos se la linea de ordenes
    if (argc != 2){
        fprintf(stderr, "Forma de uso: %s usuario\n", argv [0]);
        exit(-1);
    }
    //Lectura de nuestro nombre de usuario
    logname = getenv ("LOGNAME");
    //Comprobacion para que un usuario no se responda a si mismo
    if(EQ(logname, argv [1])){
        fprintf(stderr, "Comunicacion con uno mismo no permitida\n");
        exit(0);
    }
    //Formulacion de los nombres de las tuberias de comunicacion
    sprintf (nombre_fifo_12, "/tmp/%s_%s", argv[1], logname);
    sprintf (nombre_fifo_21, "/tmp/%s_%s", logname, argv[1]);
    //Apertura de las tuberias. Una de ellas para escribir mensajes y la otra para leerlos
    if((fifo_12= open(nombre_fifo_12, O_RDONLY))==-1||
       (fifo_21=open(nombre_fifo_21, O_WRONLY))==-1){
        perror(nombre_fifo_21);
        exit(-1);
       }
    //Armamos el manejador de la señal SIGINT. Esta señal se genera al pulsar Ctrl+C
    signal(SIGINT, fin_de_transmision);
    //Bucle de recepcion de mensajes
        do{
            printf("==> "); fflush(stdout);
            read(fifo_12, mensaje, MAX);
            printf("%s", mensaje);
//Esta parte podria ponerse en otro proceso, sin el if para no tener una version secuencial
            if(EQ(mensaje, "cambio\n"))
		do{
			printf("<== ");
		        fgets(mensaje, sizeof(mensaje), stdin);
                        write (fifo_21, mensaje, strlen(mensaje)+1);
   		}while(!EQ(mensaje, "cambio\n") && !EQ(mensaje,"corto\n"));

        }while(!EQ(mensaje,"corto\n"));

	printf("FIN DE TRANSMISION. \n");
        close(fifo_12);
	close(fifo_21);

//    }
	exit(0);
}
void fin_de_transmision(int sig){
        sprintf(mensaje, "corto\n");
        write(fifo_21, mensaje, strlen(mensaje)+1);
        printf("FIN DE TRANSMISION.\n");
        close(fifo_21);
        close(fifo_21);
	exit(0);
}
