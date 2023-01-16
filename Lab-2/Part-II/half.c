#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>


int main(int argc, char* argv[]){

    if (argc == 1){
        exit(0);
    }

    else if (argc == 2){
        // atoi: converts a character string to an integer value
        int value = atoi(argv[1]);
        int ans = value / 2;

        // getpid(): returns the process ID of the parent of the calling process
        int process_id = getpid();
        printf("Half: Current process id(cpid): %d, Current result: %d\n", process_id, ans);
        return 0;
    }

    if (argc > 2){
        // atoi: converts a character string to an integer value
        int value = atoi(argv[argc - 1]);
        int ans = value / 2;

        // getpid(): returns the process ID of the parent of the calling process
        int process_id = getpid();
        printf("Half: Current process id (cpid): %d, Current result: %d\n", process_id, ans);

        // assign argv[argc-1] the value of ans, as it is a string we use sprintf;
        sprintf(argv[argc - 1], "%d", ans);
        
        // use execvp to pass the next .out file and rest of the arguments in the next file.
        execvp(argv[1], argv + 1);
        return 0;
    }   
}