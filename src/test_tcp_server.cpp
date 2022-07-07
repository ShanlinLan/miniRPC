#include "../include/tcp/tcp_server.h"

int main(){
    TcpServer *server = new TcpServer();
    server->start_multi("10.0.0.4", 12345);
    minico::sche_join();
    return 0;
}

