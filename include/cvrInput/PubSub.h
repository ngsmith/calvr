#pragma once

#include <string>
#include <zmq.hpp>
#include <google/protobuf/message_lite.h>

class PubSocket
{
    zmq::socket_t socket;

  public:

    PubSocket(zmq::context_t& context, std::string connection_string)
        :   socket(context, ZMQ_PUB)
    {
        uint64_t hwm = 1;
        socket.setsockopt(ZMQ_HWM, &hwm, sizeof(hwm));
        socket.bind(connection_string.c_str());
    }

    void send(const google::protobuf::MessageLite& msg)
    {
        int length = msg.ByteSize();
        zmq::message_t packet(length);
        msg.SerializeToArray(packet.data(), length);
        socket.send(packet);
    }
};


template<typename MsgType>
class SubSocket
{
    zmq::socket_t socket;

  public:
    SubSocket(zmq::context_t& context, std::string connection_string)
        :   socket(context, ZMQ_SUB)
    {
        uint64_t hwm = 1;
        int timeout  = 100; // in milliseconds
        socket.setsockopt(ZMQ_HWM, &hwm, sizeof(hwm));
        socket.setsockopt(ZMQ_RCVTIMEO, &timeout, sizeof(timeout));
        socket.setsockopt(ZMQ_SUBSCRIBE, "", 0);
        socket.connect(connection_string.c_str());
    }

    bool recv(MsgType& write_to)
    {
        zmq::message_t packet;
        if(socket.recv(&packet, ZMQ_NOBLOCK))
        {
            write_to.ParseFromArray(packet.data(), packet.size());
            return true;
        }

        return false;
    }
};

