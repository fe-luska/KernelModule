#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <opencv2/opencv.hpp>

#define PORT 8080
#define WIDTH 800  // the width of the screen
#define HEIGHT 600 // the height of the screen

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // Buffer to store the raw framebuffer data
    char* buffer = new char[WIDTH * HEIGHT * 4];  // assuming 32 bit color depth

    // Read the framebuffer data from the socket
    read(sock, buffer, WIDTH * HEIGHT * 4);

    // Create a cv::Mat object from the raw data
    cv::Mat img = cv::Mat(HEIGHT, WIDTH, CV_8UC4, buffer);

    // Save the image
    cv::imwrite("framebuffer.png", img);

    delete[] buffer;

    return 0;
}

