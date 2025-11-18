#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    pid_t pid = fork();

    if (pid == 0){
        int I, N;
        double hijo = 0.0;
        FILE *archivo;
        N = 1000000000;
        for (I = 0; I <= N; I++)
        {
            hijo -= 4.0/(4*I +3);

        }
        archivo = fopen("negativos.txt", "w");
        fprintf(archivo,"%.20lf", hijo);
        fclose(archivo);
        printf("%lf\n", hijo);

    }
    else
    {   
        int i, n, status;
        wait(&status);
        double padre = 0.0; 
        double negativos,pi;
        n = 1000000000;
        if(WIFEXITED(status)){
            FILE *archivo = fopen("negativos.txt", "r");
            fscanf(archivo,"%lf", &negativos);
            fclose(archivo);
            for (i = 0; i <= n; i++)
            {
                padre += 4.0/(4*i+1);

            }
            pi = (padre+negativos);
        }
        printf("%lf\n", padre);
        printf("%.20lf\n", pi);
    }
    
    return 0;
}