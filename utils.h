#ifndef __UTILS_H__
#define __UTILS_H__
#include <string>
#include <netdb.h>
#include <sys/socket.h>
#include <assert.h>
#include <arpa/inet.h>
#include<ctime>
#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>
namespace libevent{

static ssize_t getFileSize(const char *path){
    ssize_t fileSize = -1;
    struct stat statBuff;
    if(stat(path, &statBuff) >= 0){
        fileSize = statBuff.st_size;
    }
    return fileSize;
}

static std::string getTime(){
    std::time_t t;
    std::time ( &t );
    std::string tStr = std::ctime(&t);
    //去掉回车
    tStr.erase(tStr.size() - 1, 1);
    return tStr;
}

static void log(const std::string & msg){
    std::cout<<getTime()<<" : "<<msg<<std::endl;
}


static bool beginWith(const std::string& content, const std::string& begin){
    if(begin.length() > content.length()){
        return false;
    }
    if(begin == content.substr(0, begin.length())){
        return true;
    }
    return false;
}

static bool isIpAddress(std::string host){
    size_t len = host.length();
    for(size_t i = 0; i < len; i++){
        if(host[i] != '.' && (host[i] < '0' || host[i] > '9')){
            return false;
        }
    }
    return true;
}

static size_t getPathStartPosByUrl(std::string url){
    size_t left = 0;
    size_t right;
    if(beginWith(url, "http://")){
        left = 7;
    }

    right = url.find("/", left);

    return right;
}

static std::string getHostByUrl(std::string url){
    size_t left = 0;
    size_t right;
    if(beginWith(url, "http://")){
        left = 7;
    }

    if((right = url.find("/", left)) == std::string::npos){
        right = url.length();
    }

    return url.substr(left, right - left);
}


static std::string getIpByUrl(std::string url){
    std::string host = getHostByUrl(url);
    if(isIpAddress(host)){
        return host;
    }
    std::string ip;
    struct hostent *ht = NULL;
    ht = gethostbyname(host.c_str());
    assert(ht);
    ip = inet_ntoa(*(in_addr*)ht->h_addr_list[0]);

    return ip;
}

static std::string getPathByUrl(std::string url){
    size_t left = 0;
    size_t right;
    if(beginWith(url, "http://")){
        left = 7;
    }

    if((right = url.find("/", left)) == std::string::npos){
        return std::string("/");
    }else{
        return url.substr(right, url.length() - right);
    }


}

static int string2int(const std::string& s){
    if(s.length() == 0){
        return 0;
    }
    std::istringstream is(s);
    int ret = 0;
    is>>ret;
    return ret;
}

static std::string int2string(int i){
    std::stringstream is;
    is<<i;
    return is.str();
}

}




#endif // UTILS

