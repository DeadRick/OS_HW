#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    int fd_in, fd_out;                  // Файловые дескрипторы для входного и выходного файлов
    char *input_file, *output_file;     // Имена входного и выходного файлов
    char buffer[BUFFER_SIZE];           // Буфер для чтения и записи данных
    ssize_t nread;                      // Количество прочитанных байт

    // Проверка правильного количества аргументов командной строки
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    input_file = argv[1];   // Получение имени входного файла из аргумента командной строки
    output_file = argv[2];  // Получение имени выходного файла из аргумента командной строки

    // Открытие входного файла для чтения
    fd_in = open(input_file, O_RDONLY);
    if (fd_in == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Открытие выходного файла для записи (создание, если не существует)
    fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Чтение данных из входного файла и запись их в выходной файл
    while ((nread = read(fd_in, buffer, BUFFER_SIZE)) > 0) {
        if (write(fd_out, buffer, nread) != nread) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    // Проверка наличия ошибок при чтении из входного файла
    if (nread == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    // Закрытие файловых дескрипторов
    close(fd_in);
    close(fd_out);

    return 0;
}
