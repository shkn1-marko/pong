#include "network_client.hpp"

#include <cstring>

bool NetworkClient::join()
{
    JoinRequest request{};

    for (int attempt = 0; attempt < 5; attempt++)
    {
        sendto(sockfd, (const char*)&request, sizeof(request), 0,
               (sockaddr*)&serverAddr, sizeof(serverAddr));

        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(sockfd, &readSet);

        timeval timeout{};
        timeout.tv_sec = 0;
        timeout.tv_usec = 500000;

        int result = select(0, &readSet, nullptr, nullptr, &timeout);

        if (result > 0)
        {
            char buffer[256]{};
            sockaddr_in fromAddr{};
            int fromLen = sizeof(fromAddr);

            int bytesReceived = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                                         (sockaddr*)&fromAddr, &fromLen);

            if (bytesReceived >= (int)sizeof(PacketType))
            {
                PacketType type;
                memcpy(&type, buffer, sizeof(PacketType));

                if (type == PacketType::Join && bytesReceived >= (int)sizeof(JoinAccept))
                {
                    JoinAccept accept{};
                    memcpy(&accept, buffer, sizeof(JoinAccept));
                    playerId = accept.playerId;
                    return true;
                }
            }
        }
    }
}