#pragma once
#include <string>

class Server{
    public:
        Server(int port);       //declarative costructor
        void start();

    private:
        int port;
        int server_fd;

        void setupSocket();
        void handleClient(int client_fd);
};