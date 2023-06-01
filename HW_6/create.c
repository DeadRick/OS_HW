#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
    char memn[] = "shared-memory";
    int shm;
    int mem_size = 100;

    if ((shm = shm_open(memn, O_CREAT | O_RDWR, 0666)) == -1) {
        printf("Object is already open\n");
        perror("shm_open");
        return 1;
    }

    if (ftruncate(shm, mem_size) == -1) {
        printf("Memory sizing error\n");
        perror("ftruncate");
        return 1;
    }

    close(shm);

    return 0;
}
