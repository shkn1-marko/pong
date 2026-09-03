#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "protocol.hpp"

const int SERVER_PORT = 7777;

int main()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Failed to create socket\n";
        return -1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT);

    if (bind(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Failed to bind socket\n";
        close(sockfd);
        return -1;
    }

    std::cout << "Server listening on port " << SERVER_PORT << "\n";

    close(sockfd);
    return 0;
}