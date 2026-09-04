#include "player_input_queue.hpp"

void PlayerInputQueue::push(const InputPacket& packet)
{
    pending.push(packet);
}

PlayerInput PlayerInputQueue::consume(uint32_t tick)
{
    // Discard anything older than the tick we're consuming for
    while (!pending.empty() && pending.front().tick < tick)
    {
        pending.pop();
    }

    // If the front is exactly this tick, use it and remember it
    if (!pending.empty() && pending.front().tick == tick)
    {
        InputPacket packet = pending.front();
        pending.pop();
        lastKnown = PlayerInput{ packet.up, packet.down };
    }

    // Otherwise (nothing arrived for this tick yet),
    // lastKnown is reused as-is
    return lastKnown;
}