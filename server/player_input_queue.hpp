#pragma once

#include <cstdint>
#include <queue>

#include "protocol.hpp"

struct PlayerInput
{
    bool up;
    bool down;
};

class PlayerInputQueue
{
public:
    void push(const InputPacket& packet);
    PlayerInput consume(uint32_t tick);
    bool peek(uint32_t tick);

private:
    std::queue<InputPacket> pending;
    PlayerInput lastKnown{ false, false };

    void prune(uint32_t tick);
};