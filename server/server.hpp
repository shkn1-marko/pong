#pragma once

#include <array>
#include <optional>

#include <netinet/in.h>

#include "protocol.hpp"
#include "tick_manager.hpp"
#include "clock.hpp"

const int SERVER_PORT = 7777;

class PongServer
{
public:
    PongServer(int port);
    ~PongServer();

    void run();

private:
    int sockfd;
    int port;

    std::array<std::optional<sockaddr_in>, NUM_PLAYERS> playerAddrs;

    TickManager TickManager;

    bool setupSocket();
    std::optional<int> identifyPlayer(const sockaddr_in& senderAddr);
    void broadcast(const StatePacket& packet);
};