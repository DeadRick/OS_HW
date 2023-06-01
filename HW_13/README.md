# Домашняя работа #13
## Демьяненко Виктор БПИ217

Данная программа на языке C позволяет определить глубину рекурсии символьных связей при открытии файлов на вашей системе.

```
deadrick@VICTOR-DESKTOP:~/seminar_hw/wtf$ gcc main.c -o main
deadrick@VICTOR-DESKTOP:~/seminar_hw/wtf$ ./main
41
```

Не советую Вам запускать эту программу, так как она создаст файлы, которые нельзя потом удалить.

## Код программы
```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main() {
    const char* path = "dir/info.txt";
    char link[] = "a.txt";
    char prev_link[] = "a.txt";
    int fd;
    
    if ((fd = open(path, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR)) < 0) {
        perror("Open or create error.");
        return 1;
    }
    
    close(fd);
    
    int pointer = 0;
    int amount = 1;
    int res = symlink(path, link);
    
    if (res != 0) {
        printf("Error: %d\n", res);
        return 1;
    }
    
    link[pointer]++;
    amount++;
    
    for (int i = 0; i < 100; ++i) {
        res = symlink(prev_link, link);
        
        if (res != 0) {
            printf("error! %d\n", res);
            return 1;
        }
        
        if (access(link, F_OK) == -1) {
            break;
        }
        
        if (link[pointer] == 'z') {
            pointer++;
            prev_link[pointer - 1]++;
            prev_link[pointer]--;
        }
        
        prev_link[pointer]++;
        link[pointer]++;
        amount++;
    }
    
    printf("%d\n", amount);
    
    return 0;
}

```