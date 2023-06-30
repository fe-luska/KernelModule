#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <netinet/in.h>

#define MAX_PAYLOAD_SIZE 1024
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    int sock_fd;
    struct sockaddr_nl src_addr, dest_addr;
    struct nlmsghdr *nlh;
    struct msghdr msg;
    struct iovec iov;
    char buffer[MAX_PAYLOAD_SIZE];

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

    sock_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_USERSOCK);
    if (sock_fd < 0) {
        perror("Socket creation failed");
        return EXIT_FAILURE;
    }

    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid();
    src_addr.nl_groups = 0;  // Receive unicast messages

    if (bind(sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr)) < 0) {
        perror("Socket bind failed");
        close(sock_fd);
        return EXIT_FAILURE;
    }

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0;  // Kernel
    dest_addr.nl_groups = 0;  // Unicast message

    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD_SIZE));
    if (!nlh) {
        perror("Failed to allocate nlmsghdr");
        close(sock_fd);
        return EXIT_FAILURE;
    }

    iov.iov_base = (void *)nlh;
    iov.iov_len = NLMSG_SPACE(MAX_PAYLOAD_SIZE);
    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        if (recvmsg(sock_fd, &msg, 0) < 0) {
            perror("Socket receive failed");
            break;
        }

        printf("Received message from kernel: %s\n", (char *)NLMSG_DATA(nlh));

	
    }

    free(nlh);
    close(sock_fd);

    return EXIT_SUCCESS;
}

