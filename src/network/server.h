#ifndef SERVER_H
#define SERVER_H

#include "../core/kv_store.h"

class Server {
public:
    Server(int port);
    void start();

private:
    int port;
    int server_fd;
    KVStore kv;

    void handleClient(int client_socket);
};

#endif