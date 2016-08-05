#include "httpserver.h"
#include "utils.h"
#include "httpparser.h"
using namespace http;
namespace libevent{
void HttpServer::readCallback(const ConnectionPtr &conn){
    std::string msg = conn->readAll();
    HttpMsgParser parser(TYPE_REQUEST);
    parser.parse(msg);
    if(parser.getMethod() == "GET"){
            std::string path = parser.getPath();
            path.erase(0,1);
            if(path == ""){
                    path = "index.html";
            }
            int fd = open(path.c_str(), O_RDONLY);
            if(fd == -1){
                    send301(conn, "http://www.qq.com");
            }else{
                    sendHeader(conn);
                    sendContentLength(conn, path);
                    sendContentType(conn,path);
                    conn->send("\r\n");
                    sendFile(conn, fd);
            }
    }
}

void HttpServer::closeCallback(const ConnectionPtr &conn){
    log("Connection close.");
}

void HttpServer::newConnectionCallback(const ConnectionPtr &conn){
    log("New connection.");
}

void HttpServer::startCallback(){
     log("Http Server is starting");
}

void HttpServer::send404(const ConnectionPtr &conn){
    conn->send(  "HTTP/1.0 404 Not Found\r\n");
    conn->send( "Content-Type: text/html; charset=utf-8\r\n");
    conn->send(  "Server: SunfishServer 0.1\r\n");
    conn->send(  "Cache-Control: no-cache\r\n");
    conn->send( "Transfer-Encoding: chunked\r\n");
    conn->send( "Vary: Accept-Encoding\r\n");
    conn->send(  "P3P: CP=\"NON UNI COM NAV STA LOC CURa DEVa PSAa PSDa OUR IND\"\r\n");
    conn->send(  "X-Error-Page: 404-custom\r\n");
    conn->send(  "Date: Sat, 28 Nov 2015 01:48:30 GMT\r\n");
    //	sendContentLength(bev,  "404.html");
    conn->send(  "Connection: close\r\n");
    conn->send( "\r\n");
}

void HttpServer::send301(const ConnectionPtr &conn, const std::string &url){
    conn->send("HTTP/1.0 301 Moved Permanently\r\n");
    std::string str("Location: ");
    str = str + url + "\r\n";
    conn->send(str);
    conn->send("Server: SunfishServer 0.1\r\n");
    conn->send("Date: Thu, 26 Nov 2015 11:54:09 GMT\r\n");
    conn->send("Content-Length: 0\r\n");
    conn->send("Connection: close\r\n");
    conn->send("\r\n");
}

void HttpServer::sendContentType(const ConnectionPtr &conn, const std::string &path){
    size_t dotPos = path.find(".");
    if(dotPos == std::string::npos){
        return;
    }

    std::string ext = path.substr(dotPos, 4);
    if(ext == ".png"){
        conn->send("Content-Type: image/png\r\n");
    }else if(ext == ".css"){
        conn->send("Content-Type: text/css\r\n");
    }else if(ext == ".jpg"){
        conn->send("Content-Type: image/jpeg\r\n");
    }else if(ext == ".gif"){
        conn->send("Content-Type: image/gif\r\n");
    }else if(ext == ".htm"){
        conn->send("Content-Type: text/html\r\n\r\n");
    }
}

void HttpServer::sendContentLength(const ConnectionPtr &conn, const std::string &path){
    //可以确保存在这个文件，否则不会调用这个函数
    int fSize = getFileSize(path.c_str());
    std::string str = "Content-Length: ";
    str = str + int2string(fSize) + "\r\n";
    conn->send(str);
}

void HttpServer::sendHeader(const ConnectionPtr &conn){
    conn->send( "HTTP/1.1 200 OK\r\n");
    conn->send("Server: SunfishServer 0.1 \r\n");
    conn->send("Accept-Ranges: bytes\r\n");
    conn->send("Connection: keep-alive\r\n");
    conn->send("Content-type: text/html; charset=gbk\r\n");
    conn->send("Cache-Control: no-cache\r\n");
}

void HttpServer::sendFile(const ConnectionPtr &conn, int fd){
    char msgg[1024];
    int n = 0;
    while(1){
        n = read(fd, msgg, sizeof(msgg));
        if(n <= 0)break;
        conn->send(msgg, n);
    }
}

}
