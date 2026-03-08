#pragma once
#include <string>
#include <vector>
#include <thread>       //make std::thread possible
#include <atomic>
#include <queue>        
#include <mutex>        
#include <condition_variable> 

class Server{
    public:
        Server(int port);       //declarative costructor
        void start();
        void stop();
        
        private:
        int port;
        int server_fd;
        
        int epoll_fd;
        static const int MAX_EVENTS = 64;

        std::atomic<bool> running{true};

        void setupSocket();
        void handleClient(int client_fd);
};