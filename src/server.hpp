#pragma once
#include <string>
#include <vector>
#include <thread>       //make std::thread possible
#include <atomic>

class Server{
    public:
        Server(int port);       //declarative costructor
        void start();
        void stop();
        
        private:
        int port;
        int server_fd;
        std::vector<std::thread>workers;
        std::atomic<bool> running{true};

        void setupSocket();
        void handleClient(int client_fd);
};