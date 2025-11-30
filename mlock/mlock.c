/*
 * Ejemplo mínimo de uso de mlock() en Linux/POSIX.
 * Reserva memoria, la bloquea en RAM, escribe un texto y lo lee.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>   // mlock, munlock

#define SIZE 4096   // 4 KB

int main(void) {
    char *buffer;

    /* 1. Reservar memoria dinámica */
    buffer = malloc(SIZE);
    if (!buffer) {
        perror("malloc");
        exit(1);
    }

    /* 2. Bloquear la memoria en RAM */
    if (mlock(buffer, SIZE) == -1) {
        perror("mlock");
        free(buffer);
        exit(1);
    }

    /* 3. Escribir en la región bloqueada */
    const char *msg = "Memoria bloqueada con mlock()\n";
    memcpy(buffer, msg, strlen(msg) + 1);

    /* 4. Leer desde la región */
    printf("%s", buffer);

    /* 5. Desbloquear la memoria */
    if (munlock(buffer, SIZE) == -1) {
        perror("munlock");
    }

    /* 6. Liberar memoria */
    free(buffer);

    return 0;
}
