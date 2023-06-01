# Домашнее задание #7
## Демьяненко Виктор БПИ217

## Задание
Разработать программы клиента и сервера, взаимодействующих через разделяемую память с использованием функций POSIX.
Клиент генерирует случайные числа в том же диапазоне, что и ранее рассмотренный на семинаре пример. Сервер осуществляет их вывод.

### Комментарии
Клиент генерирует случайные числа в диапазоне от 1 до 1000, а сервер выводит их на экран.
В клиенте для генерации прописана пауза в 1 секунду, а вывод сервера с паузой в 2 секунды.


## Код

### client.c
```c
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

```

### server.c
```c
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

```

