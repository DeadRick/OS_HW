
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

    ftruncate(shm, mem_size);
	
    addr = mmap(0, mem_size, PROT_WRITE | PROT_READ, MAP_SHARED, shm, 0);

    int *data = (int *)addr;
    *data = 0;
    
    // Клиент генерирует числа.
    int number;
    srand(time(NULL));
    for (int i = 0; i < 50; ++i) {
        number = 1 + (rand() % 1000);
		
        *data = number;
        sleep(1);
    }

    *data = 0;
    
    munmap(addr, mem_size);
    shm_unlink(memn);
	
    return 0;
}