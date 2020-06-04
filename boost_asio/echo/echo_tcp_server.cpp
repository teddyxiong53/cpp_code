#include <boost/asio.hpp>
#include <string>
#include <algorithm>
#include <functional>

using namespace boost::asio;
using std::placeholders::_1;
using std::placeholders::_2;
io_service service;

size_t read_complete(char *buff, const boost::system::error_code &err, size_t bytes)
{
    if(err) {
        return 0;
    }
    bool found = std::find(buff, buff+bytes, '\n') < buff+bytes;
    return found ? 0: 1;
}

void handle_connections()
{
    ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 8001));
    char buff[1024];
    while(true) {
        ip::tcp::socket sock(service);
        acceptor.accept(sock);
        int bytes = read(sock, buffer(buff),std::bind(read_complete, buff, _1, _2));
        std::string msg(buff, bytes);
        sock.write_some(buffer(msg));
        sock.close();
    }
}
int main(int argc, char const *argv[])
{
    handle_connections();
    return 0;
}
