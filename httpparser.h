/*************************************************************************
    > File Name: httpparser.h
    > Author: Sunfish
    > Mail: 704613309@qq.com 
    > Created Time: 2016年05月27日 星期五 14时33分07秒
 ************************************************************************/
#ifndef __HTTPMSGPARSER_H__
#define __HTTPMSGPARSER_H__

#include <map>

namespace http{
	enum MsgType{TYPE_REQUEST, TYPE_RESPONSE};
	class HttpMsgParser{
		public:
			typedef std::map<std::string, std::string> HeadersMap;
			HttpMsgParser(MsgType msgType);
//			HeadersMap& getHeadersMap() const;
			const std::string getHeader(const std::string &);
			void setBody(const std::string &body);
			int getContentLength();
			const std::string& getBody() const;
			const std::string& getMethod() const;
			const std::string& getPath() const;
			const std::string& getVersion() const;
			int getStatusCode() const;
			void parse(const std::string &content);
		private:

			MsgType msgType_;
			std::string body_;
			std::string method_;
			int statusCode_;
			std::string path_;
			std::string version_;
			HeadersMap headersMap_;
	};

	
}

#endif
