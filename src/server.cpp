#include <iostream>
#include <sys/socket.h>         //socket(),bind(),listen(),accept()
#include <netinet/in.h>         //struct sockaddr_in for ip/port numbers
#include <unistd.h>             //linux sys call utilities - close()

int main(){
    int server_fd = socket(AF_INET, SOCK_STREAM,0);
    if(server_fd==-1){
        std::cout<<"Socket creation failed\n";
        return 1;
    }
    std::cout<<"server starting...\n";
    return 0;
}