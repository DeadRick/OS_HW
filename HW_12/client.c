#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_MSG_LEN 1024
#define SERVER_PORT 8888

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[MAX_MSG_LEN];
    int bytesReceived;

    // Создание сокета
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0) {
        perror("Ошибка при создании сокета");
        exit(1);
    }

    // Настройка адреса сервера
    memset((char *)&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Замените на IP-адрес сервера

    printf("Введите сообщение для отправки серверу:\n");
    fgets(buffer, MAX_MSG_LEN, stdin);

    // Отправка сообщения серверу
    if (sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Ошибка при отправке сообщения серверу");
        exit(1);
    }

    // Ожидание получения ответа от сервера
    bytesReceived = recvfrom(clientSocket, buffer, MAX_MSG_LEN, 0, NULL, NULL);
    if (bytesReceived < 0) {
        perror("Ошибка при получении ответа от сервера");
        exit(1);
    }

    buffer[bytesReceived] = '\0';

    // Вывод полученного ответа
    printf("Получен ответ от сервера: %s\n", buffer);

    return 0;
}
