## C++ HTTP Server  
A low-level HTTP server written in C++ with a focus on understanding how network servers actually work. The project is built around Linux system primitives and aims to stay close to the operating system instead of abstracting everything away.
  
```This is an ongoing project. The current implementation already handles concurrent connections using epoll, and the codebase will continue to evolve as more parts of the HTTP stack are implemented.  ```  

---
## Motivation  
  
Most web frameworks hide the mechanics of networking behind layers of abstraction. This project takes the opposite approach.  
The goal here is to work directly with:  
  sockets,  
  non-blocking I/O,  
  event driven architecture,  
  Linux system calls.  
  
Building these pieces manually provides a deeper understanding of how production servers operate internally.  

---
## Current Features  
  
TCP server implementation in C++  
Non-blocking sockets  
Event driven architecture using epoll  
Concurrent client connection handling  
Basic HTTP request parsing  
Modular server structure  


## Architecture  
  
The server follows a simple event loop model.  
The server initializes a listening socket.  
The socket is registered with epoll.  
epoll_wait() monitors incoming events.  
  
When connections arrive:  
new client sockets are accepted  
sockets are set to non-blocking mode  
they are registered with the epoll instance  
Client events are processed as they arrive.  
  
This allows the server to handle many connections within a single thread without blocking.


## Project Structure  
├── server.cpp  
├── server.hpp  
├── Makefile  
└── README.md  
  
The structure will expand as additional HTTP functionality is implemented.  


## Build  
  
The project currently builds with make.  

```make```

This produces the server binary.  

## Run  
```
./server
```
The server will start listening for incoming connections on the configured port.

---

## Current Status  
  
Implemented:  

socket setup  
non-blocking mode  
epoll event loop  
connection acceptance  
basic request handling  
  
## Planned work includes:  
  
HTTP response generation  
request routing  
better request parsing  
connection lifecycle management  
improved error handling  
