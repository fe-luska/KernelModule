#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *message = "Hello from client!";
    char buffer[BUFFER_SIZE] = {0};

    // create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // converting the IP address from string to binary
    // use the server machine s IP address
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // establishing the connection with the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Failed to connect");
        exit(EXIT_FAILURE);
    }

    // send the message to the server
    //send(sock, message, strlen(message), 0);
    printf("Message sent\n");

    // read the response from the server
    //valread = read(sock, buffer, BUFFER_SIZE);
    printf("Server response: %s\n", buffer);

    return 0;
}
