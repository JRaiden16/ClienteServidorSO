/* 
https://lwn.net/Articles/604697/
inotify_demo.c

   Demonstrate the use of the Linux inotify API to obtain filesystem
   event notifications.

   This program is Linux-specific.

   Copyright (C) 2014, Michael Kerrisk

   Licensed under the GNU General Public License, version 2 or later.
*/
#include<fcntl.h> //Para las descripciones de los directorios
#include <sys/inotify.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utmp.h"

#DEFINE MAXIMO 1024
static void     /* Display information from inotify_event structure */

displayInotifyEvent(struct inotify_event *i)
{

    //open socket_client
    printf("    wd =%2d; ", i->wd);
    if (i->cookie > 0)
        printf("cookie =%4d; ", i->cookie);

    printf("mask = ");
    if (i->mask & IN_ACCESS)        printf("IN_ACCESS ");
    if (i->mask & IN_ATTRIB)        printf("IN_ATTRIB ");
    if (i->mask & IN_CLOSE_NOWRITE) printf("IN_CLOSE_NOWRITE ");
    if (i->mask & IN_CLOSE_WRITE)   printf("IN_CLOSE_WRITE ");
    if (i->mask & IN_CREATE){ 
	printf("IN_CREATE ")
	if (i->mask & IN_ISDIR){
		printf("IN_ISDIR ");
		//write to socket_client via socket_server > Create Dir > i->Name
	} //In_Isdir
	else{
		//Open i->Name 
		//Read i->Name
		//Write to socket
	} //In_Create
    if (i->mask & IN_DELETE)        printf("IN_DELETE ");
    if (i->mask & IN_DELETE_SELF)   printf("IN_DELETE_SELF ");
    if (i->mask & IN_IGNORED)       printf("IN_IGNORED ");

    if (i->mask & IN_MODIFY)        printf("IN_MODIFY ");
    if (i->mask & IN_MOVE_SELF)     printf("IN_MOVE_SELF ");
    if (i->mask & IN_MOVED_FROM)    printf("IN_MOVED_FROM ");
    if (i->mask & IN_MOVED_TO)      printf("IN_MOVED_TO ");
    if (i->mask & IN_OPEN)          printf("IN_OPEN ");
    if (i->mask & IN_Q_OVERFLOW)    printf("IN_Q_OVERFLOW ");
    if (i->mask & IN_UNMOUNT)       printf("IN_UNMOUNT ");
    printf("\n");

    if (i->len > 0)
        printf("        name = %s\n", i->name);
    // close socket_client
}

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

int
main(int argc, char *argv[])
{
    int tuberIda[4]; int tuberVuelta[4];
    char msg[MAXIMO]; char address[MAXIMO];
    pidt_t procid;

    int inotifyFd, wd, j;
    char buf[BUF_LEN] __attribute__ ((aligned(8)));

    ssize_t numRead;
    char *p;

    struct inotify_event *event;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s ClienteServidorSO/Proyecto/\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    inotifyFd = inotify_init();         /* Create inotify instance */
    if (inotifyFd == -1) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    for (j = 1; j < argc; j++) {
        wd = inotify_add_watch(inotifyFd, argv[j], IN_ALL_EVENTS);
        if (wd == -1) {
            perror("inotify_add_watch");
            exit(EXIT_FAILURE);
        }

        printf("Watching %s using wd %d\n", argv[j], wd);
    }


    for (;;) {                          /* Read events forever */
        numRead = read(inotifyFd, buf, BUF_LEN);
        if (numRead <= 0) {
            fprintf(stderr, "read() from inotify fd returned <= 0!");
            exit(EXIT_FAILURE);
        }

        printf("Read %ld bytes from inotify fd\n", (long) numRead);

        /* Process all of the events in buffer returned by read() */

	/*pid = fork()
	if (pid == 0){
		for (p = buf; p < buf + numRead; ) {
		    event = (struct inotify_event *) p;
		    displayInotifyEvent(event); //pasar socket_client como parametro

		    p += sizeof(struct inotify_event) + event->len;
		}
		//socket_client
		break; 
	}
	else{

	}
	*/
    } // for(;;)
}


