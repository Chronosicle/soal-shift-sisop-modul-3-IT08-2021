#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/wait.h>
#include<unistd.h>


int main(){
    int p1[2];
    int p2[2];
    pid_t pp, child_id;
    if(pipe(p1) == -1)
    {
        printf("");
        fprintf(stderr, "pipe failed");
        printf("\n");
        return 1;
    }
    if(pipe(p2) == -1)
    {
        printf("");
        fprintf(stderr, "pipe failed");
        printf("\n");
        return 1;
    }
    pp = fork();
    if(pp < 0){
        printf("");
        fprintf(stderr, "fork failed");
        printf("\n");
        exit(EXIT_FAILURE);
    }
    if(pp == 0){
        close(p1[0]);
        dup2(p1[1], STDOUT_FILENO);
        char *argv[] = {"ps", "aux", NULL};
        execv("/bin/ps", argv);
        exit(EXIT_SUCCESS);
    }else{
        wait(NULL);
        child_id = fork();
        if(child_id < 0){
            printf("gagal");
            printf("\n");
            exit(EXIT_FAILURE);
        }
        if(child_id == 0){
            close(p1[1]);
            dup2(p1[0], STDIN_FILENO);
            close(p2[0]);
            dup2(p2[1], STDOUT_FILENO);
            char *argv[] = {"sort", "-nrk 3,3", NULL};
            execv("/usr/bin/sort", argv);
            exit(EXIT_SUCCESS);
        }else{
            close(p2[1]);
            dup2(p2[0], STDIN_FILENO);
            close(p1[1]);
            close(p1[0]);
            char *argv[] = {"head", "-5", NULL};
            execv("/usr/bin/head", argv);
            exit(EXIT_SUCCESS);
        }
    }
}