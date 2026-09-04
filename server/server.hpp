#pragma once

#include <cstdint>
#include <netinet/in.h>

#include "protocol.hpp"

class PongServer
{
public:
    PongServer(int port);
    ~PongServer();

    void run();

private:
    int sockfd;
    int port;

    sockaddr_in player1Addr{};
    sockaddr_in player2Addr{};
    bool hasPlayer1 = false;
    bool hasPlayer2 = false;

    bool player1Up = false, player1Down = false;
    bool player2Up = false, player2Down = false;

    uint32_t currentTick = 0;

    bool setupSocket();
    bool isMatchingAddr(const sockaddr_in& a, const sockaddr_in& b);
    void handlePacket(const InputPacket& incoming, const sockaddr_in& senderAddr);
    void broadcastState();
};