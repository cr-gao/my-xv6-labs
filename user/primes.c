#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int p0[2]) {
    close(p0[1]);
    int prime;
    int n = read(p0[0], &prime, 4);
    if(n <= 0) {
        close(p0[0]);
        exit(0);
    }
    fprintf(1, "prime %d\n", prime);
    int p1[2];
    pipe(p1);
    int pid = fork();
    if(pid == 0) {
        // child
        close(p0[0]);
        primes(p1);
        exit(0);
    } else {
        // parent
        close(p1[0]);
        int m;
        while((n = read(p0[0], &m, 4)) > 0) {
            if(m % prime != 0) {
                write(p1[1], &m, 4);
            }
        }
        close(p0[0]);
        close(p1[1]);
        wait(0);
        exit(0);
    }
}

int
main(int argc, char *argv[]) {
    int p[2];
    pipe(p);
    int pid = fork();
    if(pid == 0) {
        close(p[1]);
        primes(p);
    } else {
        close(p[0]);
        for(int i = 2; i <= 35; i++) {
            write(p[1], &i, 4);
        }
        close(p[1]);
        wait(0);
    }
    exit(0);
}