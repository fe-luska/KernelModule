#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char *response = "Hello from server!";

    // create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // associate the socket with the address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Failed to bind");
        exit(EXIT_FAILURE);
    }

    // wait for conections
    if (listen(server_fd, 3) < 0) {
        perror("Failed to listen");
        exit(EXIT_FAILURE);
    }

    // acepted conection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Failed to accept");
        exit(EXIT_FAILURE);
    }

    // read the message from the client
    //valread = read(new_socket, buffer, BUFFER_SIZE);
    printf("Client: %s\n", buffer);

    // send the response to the client
    //send(new_socket, response, strlen(response), 0);
    printf("Response sent\n");

    return 0;
}