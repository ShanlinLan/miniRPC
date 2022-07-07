#include <iostream>
#include <string>
#include <cstring>

#include "../include/tcp/tcp_client.h"

void work(TcpClient* client){
    client->connect("162.14.64.62", 12345);
    std::string str;
    char buf[1024];
    while(true){
        LOG_INFO("enter>>");
        std::cin >> str;
        client->send(str.c_str(), str.size());
        LOG_INFO("send OK!");
        memset(buf, '\0', 1024);
        int num = client->recv(buf, 1024);
        LOG_INFO("the recv is:%s", buf);
    }
}

int main(){
    TcpClient* client = new TcpClient();
    minico::co_go([&client](){work(client);});
    minico::sche_join();
    return 0;
}

