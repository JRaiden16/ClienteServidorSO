/**
* Modificacion de la version original del libro
* Unix: Programación avanzada
* Autor: Francisco Marquez Garcia
* Alfaomega, 2005
*
* Version adaptada con fork para soporte de procesos padre-hijo
* Programa 9.4 Simulacion de conversaciones por radio (responder-a.c)
*
* El programa exige que la variable de entorno LOGNAME este correctamente
* inicializada con el nombre del usuario. Esto se puede conseguir incluyendo las
* dos lineas siguientes en el fichero. progile de configuracion de nuestro
* interprete de ordenes:
*         LOGNAME = 'logname'
*         export LOGNAME
*/
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "utmp.h"
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
    int tty;
    int status;
    char terminal[MAX], *logname, *getenv();
    struct utmp *utmp, *getutent();
    void fin_de_transmision();

    //Analisis de los argumentos se la linea de ordenes
    if (argc !=2){
        fprintf(stderr, "Forma de uso: %s usuario\n", argv [0]);
        exit(-1);
    }
    //Lectura de nuestro nombre de usuario
    logname = getenv ("LOGNAME");
    //Comprobacion para que un usuario no se llame a si mismo
    if(EQ(logname, argv [1])){
        fprintf(stderr, "Comunicacion con uno mismo no permitida\n");
        exit(-1);
    }

    //Consultamos si el usuario ha iniciado una sesion
    while((utmp= getutent ())!= NULL &&
          strncmp (utmp->ut_user, argv[1], sizeof(utmp->ut_user))!=0);
    if(utmp == NULL){
        printf("EL USUARIO %s NO HA INICIADO SESIÓN.\n", argv[1]);
        exit(0);
    }

    //Formulacion de los nombres de las tuberias de comunicacion
    sprintf (nombre_fifo_12, "/tmp/%s_%s", logname, argv[1]);
    sprintf (nombre_fifo_21, "/tmp/%s_%s", argv[1], logname);

    //Creacion y apertura de las tuberias
    //Primero borramos las tuberias, para que la llamada a mknod no falle
    unlink(nombre_fifo_12);
    unlink(nombre_fifo_21);

    //Cambiamos la mascara de perimiso por defecto para este proceso.
    //Esto permitira crear las tuberias con los permisos rw-rw-rw-=0666
    umask(~0666);

    //Creamos las tuberias para que la llamada a open no falle
    if (mkfifo (nombre_fifo_12, 0666)==-1){
        perror(nombre_fifo_12);
        exit (-1);
    }
    if (mkfifo (nombre_fifo_21, 0666)==-1){
        perror(nombre_fifo_21);
        exit (-1);
    }

    //Apertura terminal de usurio
    sprintf(terminal, "/dev/%s", utmp->ut_line);
    if((tty = open (terminal, O_WRONLY))==-1){
        perror (terminal);
        exit(-1);
    }

    //Aviso al usuario con el que queremos comunicarnos.
    sprintf(mensaje,
            "\n\t\tLLAMADA PROCEDENTE DEL USUARIO %s\07\07\07\n"
            "\t\tRESPONDER ESCRIBIENDO: responder-a %s\n\n", logname, logname);
    write(tty, mensaje, strlen(mensaje)+1);
    close(tty);
    printf("Esperando respuesta...\n");
    //Apertura de las tuberias. Una de ellas para escribir mensajes y la otra para leerlos
    if((fifo_12= open(nombre_fifo_12, O_WRONLY))==-1||
       (fifo_21= open(nombre_fifo_21, O_RDONLY))==-1){
       if(fifo_12==-1)
        perror(nombre_fifo_12);
       else
        perror(nombre_fifo_21);
        exit(-1);
       }
    //A este punto llegamos cuando nuestro interlocutor responde a nuestra llamada
    printf("LLAMADA ATENDIDA. \07\07\07\n");
    //Armamos el manejador de la señal SIGINT. Esta señal se genera al pulsar Ctrl+C
    signal(SIGINT, fin_de_transmision);
        do{
            printf("<== ");
            fgets(mensaje, sizeof(mensaje), stdin);
            write (fifo_12, mensaje, strlen(mensaje)+1);
// esta parte podria ponerse en otro proceso, eliminando el if para no tener la version secuencial de un solo proceso
 	    if(EQ(mensaje, "cambio\n"))
		do{
			printf("==> "); fflush(stdout);
			read(fifo_21, mensaje, MAX);
			printf("%s", mensaje);
		}while(!EQ(mensaje, "cambio\n") && !EQ(mensaje, "corto\n"));
// hasta aqui podria ponerse en otro proceso
        }while(!EQ(mensaje,"corto\n"));
        close(fifo_12);
        close(fifo_12);
        exit(0);
}

void fin_de_transmision(int sig){
        sprintf(mensaje, "corto\n");
        write(fifo_12, mensaje, strlen(mensaje)+1);
        printf("FIN DE TRANSMISION.\n");
        close(fifo_12);
        close(fifo_12);
    exit(0);
}


