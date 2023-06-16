#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

// Функция для создания и настройки серверного сокета
int createServerSocket(int port) {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Ошибка при создании сокета сервера");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Ошибка при привязке сокета сервера к адресу");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 1) == -1) {
        perror("Ошибка при прослушивании сокета сервера");
        exit(EXIT_FAILURE);
    }

    return serverSocket;
}

// Функция для создания клиентского сокета и подключения к серверу
int createClientSocket(const char* serverIP, int serverPort) {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Ошибка при создании сокета клиента");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(serverIP);
    serverAddress.sin_port = htons(serverPort);

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Ошибка при подключении клиента к серверу");
        exit(EXIT_FAILURE);
    }

    return clientSocket;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Использование: %s <IP сервера> <порт сервера>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* serverIP = argv[1];
    int serverPort = atoi(argv[2]);

    int serverSocket = createServerSocket(serverPort);

    printf("Ожидание подключения клиента №1...\n");
    int client1Socket = accept(serverSocket, NULL, NULL);
    printf("Клиент №1 подключен.\n");

    printf("Ожидание подключения клиента №2...\n");
    int client2Socket = createClientSocket(serverIP, serverPort);
    printf("Клиент №2 подключен.\n");

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    while (1) {
        bytesRead = read(client1Socket, buffer, sizeof(buffer));
        if (bytesRead <= 0) {
            perror("Ошибка при чтении данных от клиента №1");
            break;
        }

        buffer[bytesRead] = '\0';
        printf("Клиент №1: %s\n", buffer);

        if (strcmp(buffer, "The End") == 0) {
            printf("Завершение работы клиентов и сервера.\n");
            break;
        }

        if (write(client2Socket, buffer, bytesRead) == -1) {
            perror("Ошибка при записи данных клиенту №2");
            break;
        }
    }

    close(client1Socket);
    close(client2Socket);
    close(serverSocket);

    return EXIT_SUCCESS;
}
