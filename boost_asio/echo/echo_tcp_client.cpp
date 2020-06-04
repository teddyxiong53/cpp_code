#include <boost/asio.hpp>
#include <string>
#include <algorithm>
#include <functional>
#include <thread>
#include <chrono>
#include <stdio.h>
#include <boost/thread.hpp>

using namespace boost::asio;
using std::placeholders::_1;
using std::placeholders::_2;

io_service service;

size_t read_complete(char *buf, boost::system::error_code & err, size_t bytes)
{
    if(err) {
        return 0;
    }
    bool found = std::find(buf, buf+bytes, '\n') < buf+bytes;
    return found? 0: 1;
}

void sync_echo (std::string msg)
{
    msg += "\n";
    ip::tcp::socket sock(service);
    ip::tcp::endpoint ep(ip::tcp::v4(), 8001);
    sock.connect(ep);
    sock.write_some(buffer(msg));
    char buf[1024];
    int bytes  = read(sock, buffer(buf), std::bind(read_complete, buf, std::placeholders::_1, std::placeholders::_2));

    printf("echo from server:%s\n", buf);
}
int main(int argc, char const *argv[])
{
    char *messags[] = {"aaa", "bbb", "ccc", 0};
    boost::thread_group threads;
    for(char **message= messags; *message; ++message) {
        threads.create_thread(std::bind(sync_echo, *message));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    threads.join_all();
    return 0;
}
