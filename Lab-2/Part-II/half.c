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
        int value = atoi(argv[1]);
        int ans = value / 2;
        int process_id = getpid();
        printf("Half: Current process id(cpid): %d, Current result: %d\n", process_id, ans);
        return 0;
    }

    if (argc > 2){
        int value = atoi(argv[argc - 1]);
        int ans = value / 2;
        int process_id = getpid();
        printf("Half: Current process id (cpid): %d, Current result: %d\n", process_id, ans);
        sprintf(argv[argc - 1], "%d", ans);
        execvp(argv[1], argv + 1);
        return 0;
    }   
}