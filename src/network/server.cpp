#include "server.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

Server::Server(int port) : port(port) {}

void Server::start() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
    kv.loadFromDisk();
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        cout << "Bind failed, error: " << WSAGetLastError() << endl;
        return;
    }

    listen(server_fd, 5);

    cout << "\n===== ShardCore Node =====\n";
    cout << "Port: " << port << endl;
    cout << "Waiting for clients...\n\n";

    while (true) {
        int addrlen = sizeof(address);
        SOCKET client_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);

        cout << "Client connected\n";

        handleClient(client_socket);

        closesocket(client_socket);
    }
}

void Server::handleClient(int client_socket) {
    char buffer[1024] = {0};

    int valread = recv(client_socket, buffer, 1024, 0);
    if (valread <= 0) return;

    string input(buffer);
    cout << "Request: " << input;

    stringstream ss(input);

    string command, key, value;
    ss >> command;

    string response;

    if (command == "PUT") {
        ss >> key >> value;
        kv.put(key, value);
        response = "OK\n";
    }
    else if (command == "GET") {
        ss >> key;
        response = kv.get(key) + "\n";
    }
    else if (command == "DELETE") {
        ss >> key;
        kv.remove(key);
        response = "DELETED\n";
    }
    else {
        response = "INVALID COMMAND\n";
    }

    send(client_socket, response.c_str(), response.size(), 0);
}