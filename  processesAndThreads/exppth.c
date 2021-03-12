#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>

pthread_mutex_t par=PTHREAD_MUTEX_INITIALIZER, imp=PTHREAD_MUTEX_INITIALIZER;
int sumpar, sumimpar;

void *f1(void* a){
	int nr = *(int*) a, fact=1, i;
	printf("nr f1 este %d\n", nr);
	for(i=2;i<=nr;i++)
		fact *= nr;
	pthread_mutex_lock(&par);
	sumpar += fact;
	pthread_mutex_unlock(&par);
	printf("suma din f1 este %d\n", sumpar);
	return NULL;
}


void *f2(void* a){
        int nr = *(int*) a, sqr;
	printf("nr f2 este %d", nr);
        sqr = nr*nr;
        pthread_mutex_lock(&imp);
        sumimpar += sqr; 
        pthread_mutex_unlock(&imp);
	 printf("suma din f2 este %d\n", sumimpar);
	return NULL;
}


int main(int argc, char** argv){
	int nr_arg = argc-1;
	pthread_t tp[nr_arg], ti[nr_arg];

	int a2b[2], b2p[2];
	pipe(a2b);pipe(b2p);
	
	if(fork() == 0){ //procesul A
		int i;
		int nr_thread=-1, nr[argc];
		close(a2b[0]);
		close(b2p[0]);
		close(b2p[1]);

		for( i=1; i<= nr_arg ; i++){
			nr[i] = atoi(argv[i]);
			if(nr[i]%2 == 0){
				nr_thread++;
				pthread_create(&tp[nr_thread], NULL, f1, &nr[i]);
			}
		}
		for( i=0; i<= nr_thread ; i++){
			pthread_join(tp[i], NULL);
		}
		write(a2b[1], &sumpar, sizeof(int));
		
		close(a2b[1]);
		exit(0); 
	}

	if(fork() == 0){ //procesul B
		int i,  nr_thread=-1, sumTotal, sp, nr[argc];
		
		close(a2b[1]);
        close(b2p[0]);

                for( i=1; i<= nr_arg ; i++){
                        nr[i] = atoi(argv[i]);
                        if(nr[i]%2 != 0){
                                nr_thread++;
                                pthread_create(&ti[nr_thread], NULL, f2, &nr[i]);
                        }
                }
		for( i=0; i<= nr_thread ; i++){
                        pthread_join(ti[i], NULL);
                }
		read(a2b[0], &sp, sizeof(int));
		sumTotal = sp + sumimpar;
		write(b2p[1], &sumTotal, sizeof(int));
		close(a2b[0]);close(b2p[1]);
		exit(0);
	}	

	close(a2b[0]);
	close(a2b[1]);
	close(b2p[1]);
	int suma;
	read(b2p[0], &suma, sizeof(int));
	printf("Suma totala este: %d", suma);
	wait(0);
	wait(0);

	return 0;
}

