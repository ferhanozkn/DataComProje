#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};
    char choice[BUFFER_SIZE];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket oluşturulamadı");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bağlantı hatası");
        exit(EXIT_FAILURE);
    }

    recv(sock, buffer, BUFFER_SIZE, 0);
    printf("%s", buffer);

    scanf("%s", choice);
    send(sock, choice, strlen(choice), 0);

    memset(buffer, 0, BUFFER_SIZE);
    recv(sock, buffer, BUFFER_SIZE, 0);
    printf("Sonuç: %s\n", buffer);

    close(sock);

    return 0;
}