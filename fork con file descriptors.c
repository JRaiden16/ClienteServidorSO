//fork con file descriptors

#include  <stdio.h>
#include  <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
 
//    0: stdin
//    1: stdout
//    2: stderr


//     0123
//     dddddddddddddddddddddddddddddddddddddddddddddddddd
//     ccccccccccccccccccccccccccccccccccc
//current01234
//     cccccccccccccccccccccccccccccc
//current01234567
void  main(void) {
    int current=0;
    pid_t pa, pb;
    char buff[64];

    int fd = open("archivo.txt", O_RDONLY);
    if(fork()==0) {
        printf("El descriptor fd=%d\n", fd);//3
        for(int i=0; i<64; i++) {
            lseek(fd, current, SEEK_SET);
            int c= read(fd, buff, 3);
            buff[3+1]=0;
            current += c;
            if(c==0) break;
            printf("%s\n", buff);
        }
	close(fd);
    }else {
        close(fd);//close 3
	close(0); //se cerro el descriptor 0 de la esntrada estandar stdin
//		el primer descriptor a asignar es el 0
            int fdx = open("archivo.txt", O_RDONLY);
            printf("El descriptor fdx=%d\n", fdx);
            for(int i=0; i<6; i++) {
                //lseek(fdx, current, SEEK_SET);
//                int c= read(fdx, buff, 3);
		buff[0] = getc(stdin);
                buff[1]=0;
                //current += c;
                //if(c==0) break;
                printf("ESTO VIENE DE GETC %s\n", buff);

            }
            close(fdx);
    }

    }

