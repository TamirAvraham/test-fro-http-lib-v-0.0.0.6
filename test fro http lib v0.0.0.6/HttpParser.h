#pragma once
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "TcpServer.h"
#include "HttpStatus.h"


namespace http {

	enum HttpRequestType
	{
		HttpGET = 0,
		HttpPUT = 1,
		HttpPOST = 2,
		HttpDELETE = 3,
		HttpOPTIONS = 4
	};

	class HttpTokenizer
	{
	public:
		HttpTokenizer(std::string request);
		std::string GetBody();
		std::string GetRoute();
		HttpRequestType GetType();
		HttpStatus GetError();
		std::pair<bool, std::string> isCss();
		std::pair<bool, std::string> isJs();
	private:
		std::string _header;
		std::string _body;
		std::string _route;
		std::pair<bool, std::string> _isCssAndFileName;
		std::pair<bool, std::string> _isJsAndFileName;
		HttpRequestType _requestType;
		HttpStatus _error;

		void parse(std::string req);
		void getBody(std::string req);
		HttpRequestType StringToHttpRequestType(std::string requestType);
	};
};

