#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void print_handler(char *buffer, int recv_bytes){
	int i = 0, j = 0;
	char aux[1024];
	// Splits the received message back into individual words
	while(i < recv_bytes){
		if(buffer[i] != ' ' && buffer[i] != '\0'){
			aux[j] = buffer[i];
			i++;
			j++;
		} else if(buffer[i] != '\0'){
			aux[j] = '\0';
			printf("Remote send: %s\n", aux);
			j = 0;
			i++;
		} else {
			return;
		}
	}
}


int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set server address configuration
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 address from string to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server\n");

    // Receive data from the server
    ssize_t recv_bytes;
    while(1){
 	recv_bytes = recv(sock, buffer, BUFFER_SIZE - 1, 0);
    	if (recv_bytes < 0) {
        	perror("Failed to receive data from the server");
        	exit(EXIT_FAILURE);
    	}
        buffer[recv_bytes] = '\0';

	// Prints the words separetly
	print_handler(buffer, recv_bytes);
    }


    // Close the socket
    close(sock);

    return 0;
}

