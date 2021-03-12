#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>

/*
 T1 Scrieti un program C care primeste oricate argumente, numere intregi.
 Pentru fiecare argument se pornesc 2 threaduri.
         - primul thread calculeaza factorialul acestui numar si il afiseaza pe ecran
         - al doilea aduna valoarea argumentului in variabila sum(global) daca acesta este prim
 
 Cand se lucreaza pe variabilele gloabale se foloseste mutex pentru sincronizare.
 Se scad puncte pentru indentare gresita.
 */

int sum;
pthread_mutex_t m;

void *f1 (void* a){
    
    int nr = *(int *)a;
    int factorial=1;
    
    for(int i = 2; i<=nr; i++)
        factorial *= i;
    
    pthread_mutex_lock(&m);
    printf("Factorialul lui %d este %d\n", nr, factorial);
    pthread_mutex_unlock(&m);
    return NULL;
}

void *f2 (void* a){
    
    int nr = *(int *) a;
    
    if(nr < 2 || (nr > 2 && nr%2 == 0) )
        return NULL;
    
    for(int i=3; i*i < nr; i=i+2)
        if(nr%i == 0)
            return NULL;
    
    pthread_mutex_lock(&m);
    sum += nr;
    pthread_mutex_unlock(&m);
    return NULL;
}

int main(int argc, char** argv){
    
    int nr_arg;
    nr_arg = argc-1;
    pthread_t t[nr_arg *2];
    pthread_mutex_init (&m, NULL);
    
    int argumente[nr_arg];
    
    int i,j=0,pozitie;
    
    for(i=1, pozitie=0; i<=nr_arg ; i++, pozitie++){
        argumente[pozitie] = atoi(argv[i]);
        
        pthread_create(&t[j], NULL, f1, &argumente[pozitie]);
        pthread_create(&t[j+1], NULL, f2, &argumente[pozitie]);
        j += 2;
    }
    
    for(int i=0;i<2*nr_arg;i++)
        pthread_join(t[i], NULL);
    
    pthread_mutex_destroy(&m);
    
    printf("Suma globala este: %d\n", sum);
    return 0;
}







































/*
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int sum = 0;
pthread_mutex_t m;

void* f1(void* a) {
  int factorial = 1;
  int nr = *(int*)a;

  for (int i = 1; i <= nr; i++) {
    factorial *= i;
  }

  printf("%d! = %d\n", nr, factorial);
  return NULL;
}

void* f2(void* a) {
  int nr = *(int*)a;
  
  if (nr < 2 || (nr > 2 && nr % 2 == 0)) {
    return NULL;
  }
  for (int d = 3; d * d <= nr; d += 2) {
    if (nr % d == 0) {
      return NULL;
    }
  }

  pthread_mutex_lock(&m);
  sum += nr;
  pthread_mutex_unlock(&m);
  return NULL;
}

int main(int argc, char** argv) {
  const int nr_arg = argc - 1;
  int argumente[nr_arg];

  pthread_t t[nr_arg * 2];
  pthread_mutex_init(&m, NULL);
  
  int j = 0;
  for (int i = 1, poz = 0; i <= nr_arg; i++, poz++) {
    argumente[poz] = atoi(argv[i]);

    pthread_create(&t[j], NULL, f1, &argumente[poz]);
    pthread_create(&t[j + 1], NULL, f2, &argumente[poz]);

    j += 2;
  }

  for (int i = 0; i < j; i++) {
    pthread_join(t[i], NULL);
  }

  pthread_mutex_destroy(&m);
  printf("SUM PRIM: %d\n", sum);
  
  return 0;
}
*/
