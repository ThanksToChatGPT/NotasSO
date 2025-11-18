#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    int pipefd[2]; //pipefd[0] - lectura, pipefd[1] - escritura
    int r;
    
    r = pipe(pipefd);
    if(r == -1){
        perror("Error en pipe");
        exit(-1);
    }

    pid_t pid = fork();
    if(pid == -1){
        perror("Error en fork");
        exit(-1);
    }

    if (pid == 0){
        //Hijo
        close(pipefd[0]); //cerrar extremo de lectura pq va a escribir 
        r = write(pipefd[1], "Hola padreeeee", 8);
        printf("Mensaje enviado al padre.\n");
        if(r == -1){
            perror("Error en write");
            exit(-1);
        }
        close(pipefd[1]); //cerrar extremo de escritura
        exit(0);
    }else   //padre
    {   
        close(pipefd[1]); //cerrar extremo de escritura pq va a leer 
        char buffer[11];
        r = read(pipefd[0], buffer, 11);
        if(r == -1){
            perror("Error en read");
            exit(-1);
        }
        printf("Se han enviado %d bytes al padre.\n", r);
        buffer[r] = '\0'; //Asegurar fin de cadena
        printf("Mensaje recibido del hijo: %s\n", buffer);
        close(pipefd[0]); //cerrar extremo de lectura
    }
    
    return 0;
}