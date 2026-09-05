#include "server.hpp"

int main()
{
    PongServer server(SERVER_PORT);
    server.run();
    return 0;
}