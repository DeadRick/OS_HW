#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int main () {
    char memn[] = "shared_m";
    int *addr;
    int shm;
    int mem_size = 100;

    shm = shm_open(memn, O_CREAT | O_RDWR, 0666);

    addr = mmap(0, mem_size, PROT_WRITE | PROT_READ, MAP_SHARED, shm, 0);

    int *data = (int *)addr;
    
    // Сервер осуществляет вывод.
    while (1) {
        printf("Server: %d\n", *data);
        
        sleep(2);
        if (*data == 0) {
            break;
        }
    }
    
    munmap(addr, mem_size);
    return 0;
}