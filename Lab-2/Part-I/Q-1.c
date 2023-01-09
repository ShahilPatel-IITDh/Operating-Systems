#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>  

int main(){
    char str[] = "Hello World";
    int pid = 0;

    for(int i=0; i<11; i++){

        // Print for the child process only.
        if(pid == 0){
            printf("%c (pid = %d)\n", str[i],(int) getpid());
            // Select an random integer between 1 to 4
            int n = (rand()%4) + 1;
            sleep(n);

            if(i != 10){
            	pid = fork();
            }
        }
    }
    wait(NULL);
    return 0;
}
// Minimum lines of C code: 20 