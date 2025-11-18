/*
(evitar intercalado de impresión).
 Compilar: Makefile
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>   // sleep
#include <string.h>   // strerror
#include <errno.h>    // errno

#define MAX_PROCESOS 1
#define NUM_HILOS 8

/* representa la informacion que le pasamos a cada hilo al crearlo */
struct datos_tipo {
    int dato; /* un número que el hilo imprime */
    int p;    /* cuantas veces se imprime */
};

/* mutex */
pthread_mutex_t mutex_seccion_critica;

/*
 * Función para ejecutar cada hilo. Recibe un datos_tipo pero como void
 */
void *proceso(void *datos) {
    /* Void a datos_tipo */
    struct datos_tipo *datos_proceso;
    datos_proceso = (struct datos_tipo *) datos;

    /* Separar las variables */
    int i;
    int dato = datos_proceso->dato;
    int cantidad = datos_proceso->p;   




    /* -----SECCIÓN CRÍTICA ----- */
    /* se ejecuta un hilo a la vez, bloqueandolos si ya hay otro*/
    
    /* devuelve 0 si todo salio bien */
    if (pthread_mutex_lock(&mutex_seccion_critica) != 0) {
        printf("Hilo %d: error al bloquear mutex\n", dato);
    }

    /* Dentro de mutex */
    for (i = 0; i <= cantidad; i++) {
        printf("%i ", dato);
    }

    fflush(stdout);
    sleep(1);

    for(i=0; i<=cantidad; i++){
        printf("- ");
    }
    printf("\n");
    fflush(stdout);


    /* devuelve 0 si todo sale bien*/
    if (pthread_mutex_unlock(&mutex_seccion_critica) != 0) {
        printf("Hilo %d: error al desbloquear mutex: %s\n", dato, strerror(errno));
    }
    /*----- Fin de la SECCIÓN CRÍTICA ----- */
    
}

int main() {
    int i, error;
    char *valor_devuelto;

    /* Arreglos para datos y identificadores de hilos */
    struct datos_tipo hilo_datos[NUM_HILOS];
    pthread_t idhilo[NUM_HILOS];

    /* Crear los datos */
    for (i = 0; i < NUM_HILOS; i++) {
        hilo_datos[i].dato = i;      /* hilo i tiene dato i*/
        hilo_datos[i].p = i + 1;     /* hilo i se imprime i+1 veces */
    }

    /* Inicialización del mutex */
    if (pthread_mutex_init(&mutex_seccion_critica, NULL) != 0) {
        printf("Error inicializando mutex\n");
        return -1;
    }

    /* Crear los hilos */
    for (i = 0; i < NUM_HILOS; i++) {
        /* pthread_create recibe: id, atributos, función, argumento */
        error = pthread_create(&idhilo[i], NULL, (void *) proceso, (void *) &hilo_datos[i]);
        if (error != 0) {
            /* Si falla la creación, lo informamos y salimos */
            perror("No puedo crear hilo\n");
            /* Intentamos destruir mutex antes de terminar */
            pthread_mutex_destroy(&mutex_seccion_critica);
            exit(-1);
            
        }
    }

    /* Esperar a que terminen todos los hilos (join) */
    for (i = 0; i < NUM_HILOS; i++) {
        error = pthread_join(idhilo[i], NULL);
        if (error != 0) {
            printf("Error en pthread_join para hilo %d: \n", i);
        }
    }

    /* Destruir mutex */
    if (pthread_mutex_destroy(&mutex_seccion_critica) != 0) {
        fprintf(stderr, "Error destruyendo mutex: %s\n", strerror(errno));
        return -1;
    }

    printf("Todos los hilos terminaron correctamente.\n");
    return 0;
}
