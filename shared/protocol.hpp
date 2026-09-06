#pragma once

#include <cstdint>

enum class PacketType : uint32_t
{
    Join = 0,
    JoinAccept = 1,
    Input = 2,
    State = 3
};

struct JoinRequest
{
    PacketType type = PacketType::Join;
};

struct JoinAccept
{
    PacketType type = PacketType::JoinAccept;
    int32_t playerId;
};

struct InputPacket
{
    PacketType type = PacketType::Input;
    uint32_t tick;
    bool up;
    bool down;
};

struct StatePacket
{
    PacketType type = PacketType::State;
    uint32_t tick;
    bool player1Up;
    bool player1Down;
    bool player2Up;
    bool player2Down;
};