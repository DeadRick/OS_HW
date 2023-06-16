#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_MSG_LEN 1024
#define SERVER_PORT 8888

int main() {
    int serverSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;
    char buffer[MAX_MSG_LEN];
    int addrLen, bytesReceived;

    // Создание сокета
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0) {
        perror("Ошибка при создании сокета");
        exit(1);
    }

    // Настройка адреса сервера
    memset((char *)&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Привязка сокета к адресу сервера
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Ошибка при привязке сокета к адресу сервера");
        exit(1);
    }

    printf("Сервер запущен и прослушивает порт %d...\n", SERVER_PORT);

    while (1) {
        // Ожидание получения сообщения от клиента
        addrLen = sizeof(clientAddr);
        bytesReceived = recvfrom(serverSocket, buffer, MAX_MSG_LEN, 0, (struct sockaddr *)&clientAddr, &addrLen);
        if (bytesReceived < 0) {
            perror("Ошибка при получении сообщения от клиента");
            exit(1);
        }

        buffer[bytesReceived] = '\0';

        // Вывод полученного сообщения
        printf("Получено сообщение от клиента %s: %s\n", inet_ntoa(clientAddr.sin_addr), buffer);

        // Отправка сообщения обратно клиенту
        if (sendto(serverSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr)) < 0) {
            perror("Ошибка при отправке сообщения клиенту");
            exit(1);
        }
    }

    return 0;
}
