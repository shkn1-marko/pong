#include "server.hpp"

#include <iostream>
#include <cstring>

#include <unistd.h>
#include <sys/socket.h>
#include <poll.h>

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

std::optional<int> PongServer::identifyPlayer(const sockaddr_in& senderAddr)
{
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        if (playerAddrs[i].has_value() &&
            playerAddrs[i]->sin_addr.s_addr == senderAddr.sin_addr.s_addr &&
            playerAddrs[i]->sin_port == senderAddr.sin_port)
        {
            return i;
        }
    }

    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        if (!playerAddrs[i].has_value())
        {
            playerAddrs[i] = senderAddr;
            std::cout << "Player " << i << " connected\n";
            return i;
        }
    }

    return std::nullopt;
}

void PongServer::broadcast(const StatePacket& packet)
{
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        if (playerAddrs[i].has_value())
        {
            sendto(sockfd, &packet, sizeof(packet), 0,
                   (sockaddr*)&playerAddrs[i].value(), sizeof(sockaddr_in));
        }
    }
}

void PongServer::run()
{
    pollfd pfd{};
    pfd.fd = sockfd;
    pfd.events = POLLIN;

    while (true)
    {
        int result = poll(&pfd, 1, 10);

        if (result > 0 && (pfd.revents & POLLIN))
        {
            InputPacket incoming{};
            sockaddr_in senderAddr{};
            socklen_t senderLen = sizeof(senderAddr);

            ssize_t bytesReceived = recvfrom(sockfd, &incoming, sizeof(incoming),
                                             0, (sockaddr*)&senderAddr, &senderLen);

            if (bytesReceived > 0)
            {
                std::optional<int> playerId = identifyPlayer(senderAddr);
                if (playerId.has_value())
                {
                    tickManager.submitInput(playerId.value(), incoming);
                }
            }
        }

        std::optional<StatePacket> statePacket = tickManager.tryAdvance(Clock::now());
        if (statePacket.has_value())
        {
            broadcast(statePacket.value());
        }
    }
}