/*****************************************************
 * Productor – Consumidor con mutex y variables
 * condicionales POSIX (pthread_cond_t).
 *
 * Compilar:
 *      gcc -pthread productor_consumidor.c -o prod_cons
 *****************************************************/

#include <stdio.h>      // printf, perror
#include <stdlib.h>     // exit, EXIT_FAILURE
#include <pthread.h>    // pthread_*
#include <unistd.h>     // sleep

/* Tamaño del buffer circular */
#define MAX_BUFFER 16

/* Número total de datos a producir/consumir */
#define MAX_DATOS 1000

/*----------------- Variables globales -----------------*/

/* Mutex para proteger el acceso al buffer y a num_datos */
pthread_mutex_t mutex;

/* Variables condicionales:
 *   - vacio:  se usa para despertar al consumidor cuando hay datos.
 *   - lleno:  se usa para despertar al productor cuando hay espacio.
 */
pthread_cond_t vacio, lleno;

/* Buffer circular y contador de elementos actuales */
int buffer[MAX_BUFFER];
int num_datos = 0;   // número de datos almacenados en el buffer sin leer

/*------------------------------------------------------*/
/* Prototipos de las funciones de hilo */
void *productor(void *arg);
void *consumidor(void *arg);
/*------------------------------------------------------*/

/*-------------------- Hilo consumidor -----------------*/
void *consumidor(void *arg)
{
    /* 'posicion' indica de qué índice del buffer se va a leer */
    int posicion = 0;
    int dato;
    int i;

    for (i = 0; i < MAX_DATOS; i++)
    {
        /* Sección crítica: acceso a buffer y num_datos */
        pthread_mutex_lock(&mutex);

        /* Si no hay datos en el buffer, el consumidor
         * se bloquea esperando a que el productor
         * produzca al menos uno.
         *
         * Aquí se bloquea cuando el buffer está vacío.
         */
        while (num_datos == 0)
        {
            /* pthread_cond_wait desbloquea el mutex internamente
             * mientras espera la señal, y lo vuelve a bloquear
             * antes de retornar.
             */
            pthread_cond_wait(&vacio, &mutex);
        }

        /* Ahora hay al menos un dato, lo consumimos */
        dato = buffer[posicion];

        /* Actualizamos posición en el buffer circular */
        if (posicion == (MAX_BUFFER - 1))
            posicion = 0;
        else
            posicion++;

        /* Decrementamos el número de datos (hemos consumido uno) */
        num_datos--;

        /* Si el buffer estaba lleno (num_datos == MAX_BUFFER)
         * y ahora hay un espacio más (num_datos == MAX_BUFFER-1),
         * despertamos al productor.
         *
         * Aquí desbloqueamos al productor cuando había buffer lleno.
         */
        if (num_datos == (MAX_BUFFER - 1))
        {
            pthread_cond_signal(&lleno);
        }

        /* Salimos de la sección crítica */
        pthread_mutex_unlock(&mutex);

        /* Mensaje de depuración */
        printf("\n[CONSUMIDOR] Se ha consumido el dato: %d", dato);
        fflush(stdout);

        /* Pequeña pausa para observar el comportamiento */
        sleep(1);
    }

    pthread_exit(NULL);
}

/*-------------------- Hilo productor ------------------*/
void *productor(void *arg)
{
    /* 'posicion' indica en qué índice del buffer se va a escribir */
    int posicion = 0;
    int dato;
    int i;

    for (i = 0; i < MAX_DATOS; i++)
    {
        /* Sección crítica: acceso a buffer y num_datos */
        pthread_mutex_lock(&mutex);

        /* Si el buffer está lleno, el productor se bloquea
         * esperando a que el consumidor libere espacio.
         *
         * Aquí se bloquea cuando el buffer está lleno.
         */
        while (num_datos == MAX_BUFFER)
        {
            pthread_cond_wait(&lleno, &mutex);
        }

        /* Hay al menos un espacio libre en el buffer,
         * producimos un valor (en este caso i).
         */
        buffer[posicion] = i;
        dato = i;

        /* Actualizamos posición en el buffer circular */
        if (posicion == (MAX_BUFFER - 1))
            posicion = 0;
        else
            posicion++;

        /* Incrementamos el número de datos (hemos producido uno) */
        num_datos++;

        /* Si antes el buffer estaba vacío (num_datos == 1),
         * despertamos al consumidor para que empiece a leer.
         */
        if (num_datos == 1)
        {
            pthread_cond_signal(&vacio);
        }

        /* Salimos de la sección crítica */
        pthread_mutex_unlock(&mutex);

        /* Mensaje de depuración */
        printf("\n[PRODUCTOR] Se ha producido el dato: %d", dato);
        fflush(stdout);

        /* (Opcional) Podríamos poner un sleep aquí también */
        /* sleep(1); */
    }

    pthread_exit(NULL);
}

/*-------------------------- main ----------------------*/
int main(void)
{
    int error;
    char *valor_devuelto;  // No lo usamos realmente, pero se ajusta al ejemplo

    /* Identificadores de hilos para productor y consumidor */
    pthread_t idhilo_productor;
    pthread_t idhilo_consumidor;

    /* Inicializamos el mutex y las variables condicionales */
    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        perror("Error al inicializar el mutex");
        exit(EXIT_FAILURE);
    }

    if (pthread_cond_init(&lleno, NULL) != 0)
    {
        perror("Error al inicializar la variable condicional 'lleno'");
        exit(EXIT_FAILURE);
    }

    if (pthread_cond_init(&vacio, NULL) != 0)
    {
        perror("Error al inicializar la variable condicional 'vacio'");
        exit(EXIT_FAILURE);
    }

    /* Creamos hilo productor */
    error = pthread_create(&idhilo_productor, NULL, productor, NULL);
    if (error != 0)
    {
        perror("No puedo crear hilo productor");
        exit(EXIT_FAILURE);
    }

    /* Creamos hilo consumidor */
    error = pthread_create(&idhilo_consumidor, NULL, consumidor, NULL);
    if (error != 0)
    {
        perror("No puedo crear hilo consumidor");
        exit(EXIT_FAILURE);
    }

    /* Esperamos a que terminen ambos hilos */
    pthread_join(idhilo_consumidor, (void **)&valor_devuelto);
    pthread_join(idhilo_productor, (void **)&valor_devuelto);

    /* Destruimos el mutex y las variables condicionales */
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&lleno);
    pthread_cond_destroy(&vacio);

    return 0;
}
