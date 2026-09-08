#include "network_client.hpp"

#include <iostream>
#include <cstring>

NetworkClient::NetworkClient(const std::string& serverIp, int serverPort)
{
    if (!setupSocket(serverIp, serverPort))
    {
        std::cerr << "Failed to initialize network client\n";
        std::exit(1);
    }
}

NetworkClient::~NetworkClient()
{
    closesocket(sockfd);
}

bool NetworkClient::setupSocket(const std::string& serverIp, int serverPort)
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET)
    {
        std::cerr << "Failed to create socket\n";
        return false;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    inet_pton(AF_INET, serverIp.c_str(), &serverAddr.sin_addr);

    u_long mode = 1;
    ioctlsocket(sockfd, FIONBIO, &mode);

    return true;
}

bool NetworkClient::join()
{
    JoinRequest request{};

    for (int attempt = 0; attempt < 5; attempt++)
    {
        sendto(sockfd, (const char*)&request, sizeof(request), 0,
               (sockaddr*)&serverAddr, sizeof(serverAddr));

        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(sockfd, &readSet);

        timeval timeout{};
        timeout.tv_sec = 0;
        timeout.tv_usec = 500000;

        int result = select(0, &readSet, nullptr, nullptr, &timeout);

        if (result > 0)
        {
            char buffer[256]{};
            sockaddr_in fromAddr{};
            int fromLen = sizeof(fromAddr);

            int bytesReceived = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                                         (sockaddr*)&fromAddr, &fromLen);

            if (bytesReceived >= (int)sizeof(PacketType))
            {
                PacketType type;
                memcpy(&type, buffer, sizeof(PacketType));

                if (type == PacketType::Join && bytesReceived >= (int)sizeof(JoinAccept))
                {
                    JoinAccept accept{};
                    memcpy(&accept, buffer, sizeof(JoinAccept));
                    playerId = accept.playerId;
                    return true;
                }
            }
        }
    }

    return false;
}

void NetworkClient::sendInput(uint32_t tick, bool up, bool down)
{
    InputPacket packet{};
    packet.tick = tick;
    packet.up = up;
    packet.down = down;

    sendto(sockfd, (const char*)&packet, sizeof(packet), 0,
           (sockaddr*)&serverAddr, sizeof(serverAddr));
}

std::optional<StatePacket> NetworkClient::receiveState()
{
    char buffer[256]{};
    sockaddr_in fromAddr{};
    int fromLen = sizeof(fromAddr);

    int bytesReceived = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                                 (sockaddr*)&fromAddr, &fromLen);

    if (bytesReceived < (int)sizeof(PacketType))
    {
        return std::nullopt;
    }

    PacketType type;
    memcpy(&type, buffer, sizeof(PacketType));

    if (type == PacketType::State && bytesReceived >= (int)sizeof(StatePacket))
    {
        StatePacket packet{};
        memcpy(&packet, buffer, sizeof(StatePacket));
        return packet;
    }

    return std::nullopt;
}