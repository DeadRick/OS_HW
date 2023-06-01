#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include "stdlib.h"

int main(int argc, char **argv) {
    char memn[] = "shared-memory";
    char *addr;
    int shm;
    int mem_size = 100;

    unsigned int retTime = time(0) + atoi(argv[1]);

    while (time(0) < retTime) {
        if ((shm = shm_open(memn, O_RDWR, 0666)) == -1) {
            return 1;
        }

        addr = mmap(0, mem_size, PROT_WRITE | PROT_READ, MAP_SHARED, shm, 0);
        if (addr == (char *) -1) {
            return 1;
        }

        printf("Obtained:\t%s\n", addr);

        close(shm);
    }

    return 0;
}

