// All of the above with redirection
/**
 * example of the benefit of separating the fork() and exec().
 * When the child is createed, before calling exec(), the shell closes standard output and opens the file newfile.txt.
 * By doing so, any output from the soon-to-be running program "wc" is sent to the file insted of the scereen
 * open file descriptors are kept open across the exec() call.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
    int rc = fork();
    if (rc < 0) {
        //fork dfailed
        fprintf(stderr, "Fork Failed\n");
        exit(1);
    } else if (rc == 0) {
        // child: rediect standard output to a file
        close(STDOUT_FILENO);
        open("./p4.output", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
        // now exec "wc"...
        char *myargs[3];
        myargs[0] = strdup("wc");
        myargs[1] = strdup("figure5.4.c");
        myargs[2] = NULL;
        execvp(myargs[0], myargs);
    } else {
        int rc_wait = wait(NULL);
    }
    
    return 0;
}