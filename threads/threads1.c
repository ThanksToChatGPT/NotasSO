
#include <pthread.h>
#include <stdio.h>    // printf, perror
#include <stdlib.h>   // exit

void *funcion(void *datos){
    for(int i=0; i != 10000000; i++){
        (*(int*)datos)++;
        (*(int*)datos)--;
        (*(int*)datos)--;
        (*(int*)datos)++;
    }
    printf("%i\n", *(int*)datos);
    return NULL;
}


int main(){
    pthread_t hilo;
    int dato;
    int *retval;
    int r;
    dato = 1234;
    r = pthread_create(&hilo, NULL, (void*)funcion, (void*)&dato);
    if (r != 0) {
        perror("Error al crear hilo");
        exit(-1);
    }

    int join = pthread_join(hilo, (void**)&retval);
    if (join != 0) {
        perror("Error en join");
        exit(-1);
    }
    return(0);
}