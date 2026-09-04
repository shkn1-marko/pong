#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <optional>

#include "player_input_queue.hpp"
#include "protocol.hpp"
#include "clock.hpp"

const int NUM_PLAYERS = 2;

class TickManager
{
public:
    void submitInput(int playerId, const InputPacket& packet);

    std::optional<StatePacket> tryAdvance(TimePoint now);

private:
    std::array<PlayerInputQueue, NUM_PLAYERS> queues;

    uint32_t currentTick = 0;
    TimePoint tickStartTime = Clock::now();
    Milliseconds timeout{150};

    bool allReady();
    StatePacket buildStatePacket();
};