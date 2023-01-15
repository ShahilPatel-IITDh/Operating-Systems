#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char *argv[]){

    if (argc == 1){
        exit(0);
    }

    else if (argc == 2){
        int value = atoi(argv[1]);
        int ans = value * value;
        int process_id = getpid();
        printf("Square: Current process id(cpid): %d, Current result: %d\n", process_id, ans);
        return 0;
    }

    if (argc > 2){
        int value = atoi(argv[argc - 1]);
        int ans = value * value;
        int process_id = getpid();
        printf("Square: Current process id (cpid): %d, Current result: %d\n", process_id, ans);
        sprintf(argv[argc - 1], "%d", ans);

        execvp(argv[1], argv + 1);
        return 0;
    }
}