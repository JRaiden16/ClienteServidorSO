//registros.c

#include <stdio.h>
#include <time.h>
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>





int main(){
	 unsigned long int a;
	 register unsigned long int b;
	a = 4294967296/1024;
	printf("El valor maximo de unsigned a = %ld \n", a);

time_t t0 = time(0);
	FILE* fd = fopen("archivo.txt", "w");
	for(b=0; b<a; b++){
		fprintf(fd, "b= %ld\n", b);	
	}
	fclose(fd);
time_t t1 = time(0);
double datetime_diff_ms = difftime(t1, t0);

	printf("elapsed time = %f \n", datetime_diff_ms);
}
