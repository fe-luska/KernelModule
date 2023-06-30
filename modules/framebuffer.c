#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <stdio.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
   
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return -1;
    }
   
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        return -1;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
   
    // Binding the socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        return -1;
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        return -1;
    }

    // Open the framebuffer
    int fbfd = open("/dev/fb0", O_RDONLY);
    if (fbfd == -1) {
        perror("Error opening framebuffer device");
        return 1;
    }

    struct fb_var_screeninfo vinfo;
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        close(fbfd);
        return 1;
    }

    long screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    char *fbp

 = mmap(0, screensize, PROT_READ, MAP_SHARED, fbfd, 0);
    if (fbp == MAP_FAILED) {
        perror("Error mapping framebuffer device to memory");
        close(fbfd);
        return 1;
    }

    // Send the framebuffer data over the socket
    send(new_socket, fbp, screensize, 0);
   
    munmap(fbp, screensize);  // Unmap the memory
    close(fbfd);  // Close the framebuffer device
    close(new_socket); // Close the socket connection
    return 0;
}
