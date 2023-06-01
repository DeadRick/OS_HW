#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
    char *memn = "shared-memory";

    if (shm_unlink(memn) == -1) {
        printf("Shared memory is absent\n");
        perror("shm_unlink");
        return 1;
    }

    return 0;
}
