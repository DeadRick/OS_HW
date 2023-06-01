#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(int argc, char **argv) {
    char memn[] = "shared-memory";
    char buf[] = "default";
    char *addr;
    int shm;
    int mem_size = 100;

    srand(time(NULL));
    unsigned int selected_time = time(0) + atoi(argv[1]);

    while (time(0) < selected_time) {
        int random_number = rand() % 10;
        sprintf(buf, "%d", random_number);

        if ((shm = shm_open(memn, O_RDWR, 0666)) == -1) {
            return 1;
        }

        addr = mmap(0, mem_size, PROT_WRITE | PROT_READ, MAP_SHARED, shm, 0);
        if (addr == (char *) -1) {
            return 1;
        }

        memcpy(addr, buf, sizeof(buf));
        printf("Recorded:\t%s\n", addr);

        close(shm);
    }

    return 0;
}
