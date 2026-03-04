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
        Server(int port, int threadCount);       //declarative costructor
        void start();
        void stop();
        
        private:
        int threadCount;
        int port;
        int server_fd;
        std::vector<std::thread>workers;
        std::atomic<bool> running{true};

        std::queue<int> taskQueue; //holds accepted clients
        std::mutex queueMutex; //mutex to protect access to the task queue/race conditions
        std::condition_variable cv; //notify worker threads of new tasks
        bool shutDown{false}; //shutdown signal

        void setupSocket();
        void handleClient(int client_fd);
        void workerLoop();
};