#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include <winsock.h>
#include <WS2tcpip.h>

#include "protocol.hpp"

class NetworkClient
{
public:
    NetworkClient(const std::string& serverIp, int serverPort);
    ~NetworkClient();

    bool join();
    void sendInput(uint32_t tick, bool up, bool down);
    std::optional<StatePacket> receiveState();

    int getPlayerId() const { return playerId; }

private:
    SOCKET sockfd;
    sockaddr_in serverAddr{};
    int playerId = -1;

    bool setupSocket(const std::string& serverIp, int serverPort);
};