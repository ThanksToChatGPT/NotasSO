#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 3535
#define BACKLOG 5

int main() {
    int fd, r;
    struct sockaddr_in server;

    // Para TCP se debe usar (SOCK_STREAM).
    // Crear socket UDP
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd ==-1) {
        perror("Error al crear socket");
        exit(-1);
    }

    // Configurar estructura servidor
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;
    bzero(&server.sin_zero, 8);

    // Hacer bind
    r = bind(fd, (struct sockaddr*)&server, sizeof(struct sockaddr));
    if (r == -1) {
        perror("Error en bind");
        close(fd);
        exit(-1);
    }





        // Escuchar conexiones entrantes
    r = listen(fd, BACKLOG);
    if (r == -1) {
        perror("Error en listen");
        close(fd);
        exit(-1);
    }
    
    // Aceptar conexión
    socklen_t size = sizeof(struct sockaddr_in);
    struct sockaddr_in client;
    int fd2 = accept(fd, (struct sockaddr*)&client, &size);
    if (fd2 == -1) {
        perror("Error en accept");
        close(fd);
        exit(-1);
    }

    // Enviar mensaje al cliente
    r = send(fd2, "hola cliente", 12, 0);
    if (r == -1) {
        perror("Error en send");
        close(fd2);
        close(fd);
        exit(-1);
    }

    // Recibir mensaje del cliente
    char buffer[20];
    r = recv(fd2, buffer, 20, 0);
    if (r == -1) {
        perror("Error en recv");
        close(fd2);
        close(fd);
        exit(-1);
    }
    buffer[r] = '\0'; // aseguramos terminación de string

    printf("%s\n", buffer);

    // Cerrar sockets
    close(fd2);
    close(fd);

   
}
