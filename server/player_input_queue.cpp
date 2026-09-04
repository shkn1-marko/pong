#include "player_input_queue.hpp"

void PlayerInputQueue::push(const InputPacket& packet)
{
    pending.push(packet);
}

void PlayerInputQueue::prune(uint32_t tick)
{
    while (!pending.empty() && pending.front().tick < tick)
    {
        pending.pop();
    }
}

bool PlayerInputQueue::peek(uint32_t tick)
{
    prune(tick);
    return !pending.empty() && pending.front().tick == tick;
}

PlayerInput PlayerInputQueue::consume(uint32_t tick)
{
    prune(tick);

    if (!pending.empty() && pending.front().tick == tick)
    {
        InputPacket packet = pending.front();
        pending.pop();
        lastKnown = PlayerInput{ packet.up, packet.down };
    }

    return lastKnown;
}