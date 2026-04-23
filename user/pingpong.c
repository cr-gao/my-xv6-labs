#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {
    int p0[2], p1[2];
    pipe(p0);
    pipe(p1);

    char buf;

    int pid = fork();
    if(pid == 0) {
        // child
        close(p0[1]);
        close(p1[0]);
        int n1 = read(p0[0], &buf, 1);
        if(n1 < 0) {
            fprintf(2, "read error\n");
            exit(1);
        }
        fprintf(1, "%d: received ping\n", getpid());

        write(p1[1], "p", 1);
        close(p0[0]);
        close(p1[1]);
        exit(0);
    } else if((pid > 0)) {
        // parent
        close(p0[0]);
        close(p1[1]);
        write(p0[1], "p", 1);
        int n2 = read(p1[0], &buf, 1);
        if(n2 < 0) {
            fprintf(2, "read error\n");
            exit(1);
        }
        fprintf(1, "%d: received pong\n", getpid());
        wait(0);
        close(p0[1]);
        close(p1[0]);
        exit(0);
    } else {
        fprintf(2, "fork error\n");
        exit(1);
    }
}