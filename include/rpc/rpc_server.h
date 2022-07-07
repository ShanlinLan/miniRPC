#pragma once

// 2022/5/23
// 基于协程的rpc服务器

#include <unordered_map>

#include "service.h"
#include "rpc_server_stub.h"
#include "../minico/json.h"
#include "../tcp/tcp_server.h"

class RpcServer
{
public:
    DISALLOW_COPY_MOVE_AND_ASSIGN(RpcServer);

    RpcServer() : m_rpc_server_stub(new RpcServerStub())
    {
        LOG_INFO("rpcserver constructor the rpc-server-stub");
        
        // 添加心跳服务
        add_service(new Ping);
        LOG_INFO("add a service ping");
    }

    ~RpcServer()
    {
        LOG_INFO("rpcserver destructor the tcpserver");
        delete m_rpc_server_stub;
        m_rpc_server_stub = nullptr;
    };

    // 开启rpc服务器的运行
    void start(const char *ip, int port);
    void start_multi(const char *ip, int port);

    // 向服务器中添加一种服务，一种服务可以有多个接口方法，s必须是new创建的，方便统一管理
    void add_service(Service *s)
    {
        m_services[s->name()] = std::shared_ptr<Service>(s);
    }

    // 查找一种服务，一个rpc服务器可以包含多种service
    Service *find_service(const std::string &name)
    {
        auto it = m_services.find(name);
        if (it != m_services.end())
        {
            return it->second.get();
        }
        return nullptr;
    }

    // 业务处理逻辑函数
    void process(TinyJson &request, TinyJson &result);

private:
    // connection callback
    void on_connection(minico::Socket *conn);

    // tcp server存根
    RpcServerStub *m_rpc_server_stub;

    std::atomic<int> m_conn_number;

    // 保存的服务的容器
    std::unordered_map<std::string, std::shared_ptr<Service>> m_services;
};
