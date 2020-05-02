/*
 * signal handling and sigsetjmp/siglongjmp example
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <setjmp.h>

sigjmp_buf env;
char buf[100];
bool is_loop_running = false;

void sigint_handler(int signo) {
    printf("\n");
    if (is_loop_running) {
        printf("SIGINT: break loop\n");
        is_loop_running = false;
    }
    siglongjmp(env, 1);
}

int main(void) {
    signal(SIGINT, sigint_handler);

    sigsetjmp(env, 1);

    while (true) {
        printf(">>> ");
        fgets(buf, 100, stdin);

        if (strncmp(buf, "loop", 4) == 0) {
            is_loop_running = true;
            while (1) {
                printf("HI\n");
                sleep(1);
            }
        }
        else if (strncmp(buf, "exit", 4) == 0) {
            printf("exit...\n");
            exit(0);
        }
    }

    return 0;
}
