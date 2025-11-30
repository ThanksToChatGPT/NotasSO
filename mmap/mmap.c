/*
 * Ejemplo mínimo de uso de mmap() en Linux/POSIX.
 * Crea un archivo, lo mapea en memoria, escribe un texto y lo lee.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

#define FILE_NAME "mmap_demo.bin"
#define FILE_SIZE 4096   // 1 página típica

int main(void) {
    int fd;
    void *map;

    /* 1. Crear o abrir archivo */
    fd = open(FILE_NAME, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    /* 2. Ajustar su tamaño */
    if (ftruncate(fd, FILE_SIZE) == -1) {
        perror("ftruncate");
        close(fd);
        exit(1);
    }

    /* 3. Mapear archivo a memoria */
    map = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        perror("mmap");
        close(fd);
        exit(1);
    }

    /* 4. Escribir usando memoria mapeada */
    const char *mensaje = "Hola desde mmap!\n";
    memcpy(map, mensaje, strlen(mensaje));

    /* 5. Leer lo que hay en el mapa */
    printf("Contenido leído del mmap: %s", (char *)map);

    /* 6. Desmapear */
    if (munmap(map, FILE_SIZE) == -1)
        perror("munmap");

    close(fd);
    return 0;
}
