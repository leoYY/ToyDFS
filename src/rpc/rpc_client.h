// Copyright (c) 2014, Baidu.com, Inc. All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Author: yanshiguang02@baidu.com

#ifndef  BFS_RPC_CLIENT_H_
#define  BFS_RPC_CLIENT_H_

#include <sofa/pbrpc/pbrpc.h>

namespace bfs {
    
class RpcClient {
public:
    template <class T>
    bool GetStub(const std::string server, T** stub) {
        sofa::pbrpc::RpcChannel* channel = NULL;
        HostMap::iterator it = _host_map.find(server);
        if (it != _host_map.end()) {
            channel = it->second;
        } else {
            // 定义 channel，代表通讯通道，每个服务器地址对应一个 channel
            // 可以通过 channel_options 指定一些配置参数
            sofa::pbrpc::RpcChannelOptions channel_options;
            channel = new sofa::pbrpc::RpcChannel(&rpc_client, server, channel_options);
            _host_map[server] = channel;
        }
        *stub = new T(channel);
        return true;
    }
    template <class Stub, class Request, class Response, class Callback>
    bool SendRequest(Stub* stub, void(Stub::*func)(
                    google::protobuf::RpcController*,
                    const Request*, Response*, Callback*),
                    const Request* request, Response* response,
                    int32_t rpc_timeout, int retry_times) {
        // 定义 controller 用于控制本次调用，并设定超时时间（也可以不设置，缺省为10s）
        sofa::pbrpc::RpcController controller;
        controller.SetTimeout(rpc_timeout);
        for (int32_t retry = 0; retry < retry_times; ++retry) {
            (stub->*func)(&controller, request, response, NULL);
            if (controller.Failed()) {
                if (retry < retry_times - 1) {
                    printf("Send failed, retry ...\n");
                    usleep(1000000);
                }
            } else {
                return true;
            }
            controller.Reset();
        }
        return false;
    }
private:
    // 定义 client 对象，一个 client 程序只需要一个 client 对象
    // 可以通过 client_options 指定一些配置参数，譬如线程数、流控等
    sofa::pbrpc::RpcClientOptions client_options;
    sofa::pbrpc::RpcClient rpc_client;
    typedef std::map<std::string, sofa::pbrpc::RpcChannel*> HostMap;
    HostMap _host_map;
};

} // namespace bfs

#endif  // BFS_RPC_CLIENT_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
