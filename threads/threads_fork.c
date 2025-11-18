#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void *funcion(void *datos){
    for(int i = 0; i < 1000; i++){
        (*(int*)datos)++;
    }
    printf("[%d] thread: final value = %d\n", (int)getpid(), *(int*)datos);
    return NULL;
}

int main(){
    pthread_t hilo;
    int dato = 1234;
    int r;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        /* Child process */
        printf("[child %d] creando hilo\n", (int)getpid());
        r = pthread_create(&hilo, NULL, funcion, &dato);
        if (r != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(r));
            exit(EXIT_FAILURE);
        }
        r = pthread_join(hilo, NULL);
        if (r != 0) {
            fprintf(stderr, "pthread_join: %s\n", strerror(r));
            exit(EXIT_FAILURE);
        }
        printf("[child %d] despues join, dato=%d\n", (int)getpid(), dato);
        exit(EXIT_SUCCESS);
    } else {
        /* Parent process */
        printf("[parent %d] creando hilo\n", (int)getpid());
        int a;
        wait(&a);
        r = pthread_create(&hilo, NULL, funcion, &dato);
        if (r != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(r));
            exit(EXIT_FAILURE);
        }
        r = pthread_join(hilo, NULL);
        if (r != 0) {
            fprintf(stderr, "pthread_join: %s\n", strerror(r));
            exit(EXIT_FAILURE);
        }
        printf("[parent %d] despues join, dato=%d\n", (int)getpid(), dato);
        waitpid(pid, NULL, 0);
    }

    return 0;
}
