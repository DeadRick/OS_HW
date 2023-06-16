#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>

#define SEM_KEY 12345

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main() {
    int pipefd[2];
    pid_t child_pid;
    int semid;
    union semun sem_args;
    struct sembuf sem_op;

    // Создаем неименованный канал
    if (pipe(pipefd) == -1) {
        perror("Ошибка создания канала");
        exit(EXIT_FAILURE);
    }

    // Создаем семафор
    semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("Ошибка создания семафора");
        exit(EXIT_FAILURE);
    }

    // Инициализируем семафор значением 1
    sem_args.val = 1;
    if (semctl(semid, 0, SETVAL, sem_args) == -1) {
        perror("Ошибка инициализации семафора");
        exit(EXIT_FAILURE);
    }

    // Создаем дочерний процесс
    child_pid = fork();
    if (child_pid == -1) {
        perror("Ошибка создания дочернего процесса");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Код дочернего процесса
        close(pipefd[1]);  // Закрываем записывающий конец канала в дочернем процессе

        int received_message = 0;
        while (received_message < 10) {
            // Ждем сигнала от родительского процесса для получения сообщения
            sem_op.sem_num = 0;
            sem_op.sem_op = -1;
            sem_op.sem_flg = 0;
            semop(semid, &sem_op, 1);

            int message;
            if (read(pipefd[0], &message, sizeof(int)) == -1) {
                perror("Ошибка чтения из канала");
                exit(EXIT_FAILURE);
            }
            printf("Дочерний процесс получил сообщение: %d\n", message);
            received_message++;

            // Отправляем сигнал родительскому процессу о готовности к отправке сообщения
            sem_op.sem_op = 1;
            semop(semid, &sem_op, 1);
        }

        close(pipefd[0]);  // Закрываем чтящий конец канала в дочернем процессе
        exit(EXIT_SUCCESS);
    } else {
        // Код родительского процесса
        close(pipefd[0]);  // Закрываем чтящий конец канала в родительском процессе

        int sent_message = 0;
        while (sent_message < 10) {
            // Ждем сигнала от дочернего процесса о готовности к получению сообщения
            sem_op.sem_num = 0;
            sem_op.sem_op = -1;
            sem_op.sem_flg = 0;
            semop(semid, &sem_op, 1);

            int message = sent_message + 1;
            if (write(pipefd[1], &message, sizeof(int)) == -1) {
                perror("Ошибка записи в канал");
                exit(EXIT_FAILURE);
            }
            printf("Родительский процесс отправил сообщение: %d\n", message);
            sent_message++;

            // Отправляем сигнал дочернему процессу о готовности к получению сообщения
            sem_op.sem_op = 1;
            semop(semid, &sem_op, 1);
        }

        close(pipefd[1]);  // Закрываем записывающий конец канала в родительском процессе

        // Ждем завершения дочернего процесса
        wait(NULL);

        // Удаляем семафор
        if (semctl(semid, 0, IPC_RMID, sem_args) == -1) {
            perror("Ошибка удаления семафора");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
    }

    return 0;
}
