# Домашнее задание #3
## Демьяненко Виктор БПИ217

## Запуск программы
```
gcc main.c -o res && ./res 9
```

## Вывод программы
```
deadrick@VICTOR-DESKTOP:~/sem_hw$ gcc main.c -o res && ./res 9
Parent process. Fibo: 21
Child process. Factorial: 362880
```


## Код программы

```c
#include <unistd.h>  // Заголовочный файл для использования функции fork()
#include <stdio.h>   // Заголовочный файл для использования функции printf()

// Рекурсивная функция для вычисления чисел Фибоначчи
int fibo(int num) {
    if (num == 1) {
        return 0;
    } else if (num == 2) {
        return 1;
    } else {
        return fibo(num - 1) + fibo(num - 2);
    }
}

// Рекурсивная функция для вычисления факториала числа
int factorial(int num) {
    if (num == 1) {
        return 1;
    } else {
        return factorial(num - 1) * num;
    }
}

int main(int argc, char *argv[]) {
    pid_t chpid = 0; // Переменная для хранения идентификатора дочернего процесса
    int num;
    
    sscanf(*(argv + 1), "%d", &num);  // Получаем число для вычислений из аргумента командной строки

    chpid = fork(); // Создаем новый процесс

    if (chpid == 0) {
        // В дочернем процессе
        printf("Child process. Factorial: %d\n", factorial(num));
    } else {
        // В родительском процессе
        printf("Parent process. Fibo: %d\n", fibo(num));
    }
    
    return 0;
}
```