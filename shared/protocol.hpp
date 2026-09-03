#pragma once

#include <cstdint>

struct InputPacket
{
    uint32_t tick;
    bool up;
    bool down;
};

struct StatePacket
{
    uint32_t tick;
    bool player1Up;
    bool player1Down;
    bool player2Up;
    bool player2Down;
};