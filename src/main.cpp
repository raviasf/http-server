#include "server.hpp"
#include <iostream>
#include <csignal>

Server* globalServer = nullptr;

int main(){
    try{
        Server server(8080, 4);
        globalServer = &server;

        // Handle SIGINT (Ctrl+C) to gracefully shut down the server
        std::signal(SIGINT, [](int) {
            std::cout<<"\nShutting down server...\n";
            globalServer->stop();
            exit(0);
        });

        server.start();
    }catch(const std::exception& e){
        std::cerr<<"Error: "<<e.what()<<"\n";
        return 1;
    }
    return 0;
}