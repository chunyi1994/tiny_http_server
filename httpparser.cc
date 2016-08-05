/*************************************************************************
    > File Name: httpparser.cc
    > Author: Sunfish
    > Mail: 704613309@qq.com 
    > Created Time: 2016年05月27日 星期五 16时04分40秒
 ************************************************************************/
#include "httpparser.h"
#include <assert.h>
#include <sstream>
#include <stdlib.h>  //for atoi()
namespace http{
	HttpMsgParser::HttpMsgParser(MsgType msgType):
		msgType_(msgType){
		
	}
	
	void HttpMsgParser::setBody(const std::string& body){
		body_ = body;
	}
	
	int HttpMsgParser::getContentLength(){
		HeadersMap::iterator it = headersMap_.find("Content-Length");
		if(it == headersMap_.end()){
			return 0;
		}else{
			return atoi(it->second.c_str());
		}
	}
	
	void HttpMsgParser::parse(const std::string& content){
			
		assert(content.length() > 0);
		std::string line;
		std::stringstream ss(content);	
		std::getline(ss, line, '\r');
		size_t pos;
		size_t left;
		if(msgType_ == TYPE_REQUEST){
			//method
			pos = line.find(" ");
			assert(pos != std::string::npos);
			method_ = line.substr(0, pos);
			//path
			left = pos + 1;
			pos = line.find(" ", left);
			assert(pos != std::string::npos);
			path_ = line.substr(left, pos - left);
			//version
			left = pos + 1;
			pos = line.length();
			version_ = line.substr(left, pos - left);
		}else{
			//version
			pos = line.find(" ");
			assert(pos != std::string::npos);
			version_ = line.substr(0, pos);
			//status code
			left = pos + 1;
			pos = line.find(" ", left);
			assert(pos != std::string::npos);
			std::stringstream codess;
			codess << line.substr(left, pos);
			codess >> statusCode_;
		}
		//让getline用\r来当换行符,则下一行的开头会是'\n'.因为设置以后他不具备换行作用了.
		headersMap_.clear();
		while(getline(ss, line, '\r')){
			//name
			pos = line.find(":");
			if(pos == std::string::npos){
				break;
			}
			std::string name = line.substr(1, pos - 1);
			//value
			left = pos + 2;
			pos = line.length() - 1;
			std::string value = line.substr(left, pos);
			
			headersMap_[name] = value;
		}
	}

//	HttpMsgParser::HeadersMap& HttpMsgParser::getHeadersMap() const{
//		return headersMap_;
//	}
	
	const std::string HttpMsgParser::getHeader(const std::string &key){
		HeadersMap::iterator it = headersMap_.find(key);
		if(it != headersMap_.end()){
			return it->second; 
		}
		
		return std::string("novalue");
	}

	const std::string& HttpMsgParser::getBody() const{
		return body_;
	}

	const std::string& HttpMsgParser::getMethod() const{
		return method_;
	}

	const std::string& HttpMsgParser::getPath() const{
		return path_;
	}
	
	const std::string& HttpMsgParser::getVersion() const{
		return version_;
	}
	int HttpMsgParser::getStatusCode() const{
		return statusCode_;
	}


}
/*
#include<iostream>

using namespace http;
using namespace std;

int main(){
	//HTTP/1.1 200 OK
	string cont("GET /mvc/Blog/UnderPostNews.aspx HTTP/1.1\r\nConnection: keep-alive\r\nHost: www.cnblogs.com\r\nConnection: keep-alive\r\nX-Requested-With: XMLHttpRequest\r\nConnection: keep-alive\r\n\r\nkkkkkkk");
	HttpMsgParser parser(cont, TYPE_REQUEST);
	auto hmap = parser.getHeadersMap();
	for(const auto &w : hmap){
		cout<<w.first<<",second:"<<w.second<<"#"<<endl;
	}
	
	cout<<"#"<<parser.getBody()<<"#"<<endl;
	cout<<"#"<<parser.getMethod()<<"#"<<endl;
	//cout<<"#"<<parser.getStatusCode()<<"#"<<endl;
	cout<<"#"<<parser.getPath()<<"#"<<endl;
	cout<<"#"<<parser.getVersion()<<"#"<<endl;
	
	return 0;
}*/
