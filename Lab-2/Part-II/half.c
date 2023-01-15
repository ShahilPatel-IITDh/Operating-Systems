#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>

char* itoa(int val, int base);

int main(int argc, char* argv[]) {
    if (argc == 1){
        exit(0);
    }   
    int argument = argc-1;
    int n = atoi(argv[argument]);
    int ans = n/2;
    int process_id = getpid();

    printf("Half: Current process id: %u, Current result: %d\n", process_id, ans);

    char* v[argc];
    for (int i = 1; i < (argc-1) ; i++) {
        v[i-1] = argv[i];
    }
        
    v[argc-2] = itoa(ans, 10);
    v[argument] = NULL;
    
    execvp(v[0], v);
}

char* itoa(int val, int base){
    
    static char buf[32] = {0};    
    int i = 30;    
    for(; val && i ; --i, val /= base)    
        buf[i] = "0123456789abcdef"[val % base];   
    return &buf[i+1];
    
}
    
