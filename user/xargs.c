#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int
main(int argc, char *argv[]) {
    if(argc < 2) {
        fprintf(2, "usage: xargs <command> [args...]\n");
        exit(1);
    }
    char *new_argv[MAXARG];
    for(int i = 1; i < argc && i < MAXARG - 1; i++) {
        new_argv[i - 1] = argv[i];
    }

    char buf[512];
    int m = 0;
    while((read(0, buf + m, sizeof(char))) > 0) {
        m++;
        if(buf[m - 1] == '\n') {
            buf[m - 1] = 0;
            new_argv[argc - 1] = buf;
            new_argv[argc] = 0;
            if(fork() == 0) {
                exec(new_argv[0], new_argv);
            }
            wait(0);
            m = 0;
            buf[0] = 0;
        }
    }
    exit(0);
}