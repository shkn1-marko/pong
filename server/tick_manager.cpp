#include "tick_manager.hpp"

void TickManager::submitInput(int playerId, const InputPacket& packet)
{
    queues[playerId].push(packet);
}

bool TickManager::allReady()
{
    for (auto& queue : queues)
    {
        if (!queue.peek(currentTick))
        {
            return false;
        }
    }
}

StatePacket TickManager::buildStatePacket()
{
    StatePacket packet{};
    packet.tick = currentTick;

    PlayerInput p1 = queues[0].consume(currentTick);
    PlayerInput p2 = queues[1].consume(currentTick);

    packet.player1Up = p1.up;
    packet.player1Down = p1.down;
    packet.player2Up = p2.up;
    packet.player2Down = p2.down;

    return packet;
}

std::optional<StatePacket> TickManager::tryAdvance(TimePoint now)
{
    bool timedOut = (now - tickStartTime) >= timeout;

    if (!allReady() && !timedOut)
    {
        return std::nullopt;
    }

    StatePacket packet = buildStatePacket();

    currentTick++;
    tickStartTime = now;

    return packet;
}