#include "HttpParser.h"
#include <regex>

http::HttpTokenizer::HttpTokenizer(std::string request)
{
	_error = HttpStatus::OK;
	parse(request);
}

std::string http::HttpTokenizer::GetBody()
{
	return _body;
}

std::string http::HttpTokenizer::GetRoute()
{
	return _route;
}

http::HttpRequestType http::HttpTokenizer::GetType()
{
	return _requestType;
}

http::HttpStatus http::HttpTokenizer::GetError()
{
	return _error;
}
std::pair<bool, std::string> http::HttpTokenizer::isCss()
{
	return _isCssAndFileName;
}
std::pair<bool, std::string> http::HttpTokenizer::isJs()
{
	return _isJsAndFileName;
}
std::string getFileNameFromRoute(std::string route) {

	size_t lastSlash = route.find_last_of('/');
	if (lastSlash==std::string::npos)
	{
		throw http::HttpStatus::BadRequest;
	}
	std::string fileName=route.substr(lastSlash);
	fileName = fileName.substr(1);
	return fileName;
}
void http::HttpTokenizer::parse(std::string req)
{
	std::stringstream ss(req);
	std::getline(ss, _header);

	std::stringstream headerAsString(_header);
	std::string type,route;

	headerAsString >> type;
	headerAsString >> route;
	

	_route = route;
	std::regex cssFileRegex("^(.*\.(?:css))$");
	std::regex jsFileRegex("^(.*\.(?:js))$");
	bool isCss = std::regex_match(route, cssFileRegex);
	if (isCss)
	{
		_isCssAndFileName = { isCss,getFileNameFromRoute(route) };
	}
	bool isJs = std::regex_match(route, jsFileRegex);
	if (isJs)
	{
		_isJsAndFileName = { isJs,getFileNameFromRoute(route) };
	}
	try
	{
		_requestType = StringToHttpRequestType(type);
	}
	catch (HttpStatus error)
	{
		_error = error;
	}
	getBody(req);
}

void http::HttpTokenizer::getBody(std::string req)
{
	auto HasContent = req.find("Content-Type");

	if (HasContent!=std::string::npos)
	{
		auto isHtml = req.find("html");
		auto isJson = req.find("json");
		//data is json
		if (isHtml == std::string::npos && isJson != std::string::npos)
		{
			std::cout << "is json" << '\n';
			size_t startOfData = req.find('{');
			size_t endOfData = req.find('}');
			if (startOfData == std::string::npos || endOfData == std::string::npos) {
				std::cout << "no data" << '\n';
				_error = HttpStatus::BadRequest;
				return;
			}
			_body = req.substr(startOfData, endOfData);
			return;
		}
		//data is html
		if (isHtml != std::string::npos && isJson == std::string::npos) {
			std::cout << "is html" << '\n';
			size_t startOfData = req.find("<");
			size_t endOfData = req.find_last_of(">");
			if (startOfData == std::string::npos || endOfData == std::string::npos) {
				std::cout << "no data" << '\n';
				_error = HttpStatus::BadRequest;
				return;
			}
			_body = req.substr(startOfData, endOfData);
			return;
		}
		_error = HttpStatus::BadRequest;
	}
}

http::HttpRequestType http::HttpTokenizer::StringToHttpRequestType(std::string requestType)
{
	if (requestType=="GET")
	{
		return HttpGET;
	}
	if (requestType=="PUT")
	{
		return HttpPUT;
	}
	if (requestType=="POST")
	{
		return HttpPOST;
	}
	if (requestType=="DELETE")
	{
		return HttpDELETE;
	}
	if (requestType=="OPTIONS")
	{
		return HttpOPTIONS;
	}
	throw HttpStatus::BadRequest;
}

