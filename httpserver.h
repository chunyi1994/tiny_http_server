#ifndef HTTPSERVER_H
#define HTTPSERVER_H
#include "tcpserver.h"
namespace libevent{

class HttpServer : public TcpServer{
public:
    HttpServer(int port):TcpServer(port){}

protected:
    virtual void readCallback(const ConnectionPtr& conn);
    virtual void closeCallback(const ConnectionPtr& conn);
    virtual void newConnectionCallback(const ConnectionPtr& conn);
    virtual void startCallback();

private:
    void send404(const ConnectionPtr& conn);
    void send301(const ConnectionPtr& conn, const std::string &surl);
    void sendContentType(const ConnectionPtr& conn, const std::string &path);
    void sendContentLength(const ConnectionPtr& conn, const std::string &path);
    void sendHeader(const ConnectionPtr& conn);
    void sendFile(const ConnectionPtr& conn, int fd);
};
}
#endif // HTTPSERVER_H
