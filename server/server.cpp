#include "server.hpp"

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>

PongServer::PongServer(int port) : port(port)
{
    if (!setupSocket())
    {
        std::cerr << "Failed to initilize server\n";
        std::exit(1);
    }
}

PongServer::~PongServer()
{
    close(sockfd);
}

bool PongServer::setupSocket()
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Failed to create socket\n";
        return false;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Failed to bind socket\n";
        return false;
    }

    std::cout << "Server listening on port " << port << "\n";
    return true;
}