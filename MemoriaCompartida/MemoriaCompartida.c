#include <sys/shm.h>
#include <sys/wait.h>
#include <stdio.h>    // printf, perror
#include <stdlib.h>   // exit
#include <unistd.h>   // fork


int main(){
    key_t key = 12345;
    double *apuntador;
    int status;
    int r;
    int shmId;
    // shmget()     key un int| syze bytes, pero agarra una pagina (8kb)| =devuelve descriptor de mem
    shmId = shmget(key, sizeof(double), 0666 | IPC_CREAT); // 6 = 0110 0rwx
    if(shmId<0){
        perror("error al crear shmId");
        exit(-1);
    }
    // shmat() void *shmat(int shmid, const void *shmaddr, int shmflg); = devuelve un  apuntador
    apuntador = (double *)shmat(shmId, 0, 0); //devuelve void
    if(apuntador == (void *)-1){
        perror("error al crear apuntador");
        exit(-1);
    }
    pid_t pid = fork();
    if(pid<0){
        perror("error");
        exit(-1);
    }
    if(pid == 0){
        *apuntador = 3.1415926535;
    }else{
        wait(&status);
        printf("%.10f\n", *apuntador);
    }
    // shmdt()
    r = shmdt(apuntador);
    if(r == -1){
        perror("error al desvincular");
        exit(-1);
    }
    return 0;

}
