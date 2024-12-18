#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <ctype.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_PLAYERS 2

int client_sockets[MAX_PLAYERS];
char choices[MAX_PLAYERS][BUFFER_SIZE];

const char* determine_winner(const char *choice1, const char *choice2) {
    if (strcmp(choice1, choice2) == 0) {
        return "Berabere!";
    } else if ((strcmp(choice1, "tas") == 0 && strcmp(choice2, "makas") == 0) ||
               (strcmp(choice1, "makas") == 0 && strcmp(choice2, "kagit") == 0) ||
               (strcmp(choice1, "kagit") == 0 && strcmp(choice2, "tas") == 0)) {
        return "Oyuncu 1 kazandı!";
    } else {
        return "Oyuncu 2 kazandı!";
    }
}

int main() {
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket oluşturulamadı");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind hatası");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_PLAYERS) == -1) {
        perror("Listen hatası");
        exit(EXIT_FAILURE);
    }

    printf("Sunucu başlatıldı. Oyuncuların bağlanması bekleniyor...\n");

    for (int i = 0; i < MAX_PLAYERS; i++) {
        client_sockets[i] = accept(server_fd, (struct sockaddr*)&client_addr, &addrlen);
        if (client_sockets[i] == -1) {
            perror("Accept hatası");
            exit(EXIT_FAILURE);
        }
        printf("Oyuncu %d bağlandı.\n", i + 1);
        send(client_sockets[i], "Taş, Kağıt veya Makas seçin (tas/kagit/makas): ", 47, 0);
    }

    for (int i = 0; i < MAX_PLAYERS; i++) {
        memset(buffer, 0, BUFFER_SIZE);
        recv(client_sockets[i], buffer, BUFFER_SIZE, 0);
        strcpy(choices[i], buffer);
        printf("Oyuncu %d'nin seçimi: %s\n", i + 1, choices[i]);
    }

    const char *result = determine_winner(choices[0], choices[1]);
    printf("Sonuç: %s\n", result);

    for (int i = 0; i < MAX_PLAYERS; i++) {
        send(client_sockets[i], result, strlen(result), 0);
    }

    for (int i = 0; i < MAX_PLAYERS; i++) {
        close(client_sockets[i]);
    }
    close(server_fd);

    return 0;
}