#include "server.hpp"

const int SERVER_PORT = 7777;

int main()
{
    PongServer server(SERVER_PORT);
    server.run();
    return 0;
}