#pragma once
#include <string>
namespace http {
	enum class HttpStatus:short {
		Continue = 100,
		SwitchingProtocols = 101,
		OK = 200,
		Created = 201,
		Accepted = 202,
		NonAuthoritativeInformation = 203,
		NoContent = 204,
		ResetContent = 205,
		PartialContent = 206,
		MultipleChoices = 300,
		MovedPermanently = 301,
		Found = 302,
		SeeOther = 303,
		NotModified = 304,
		UseProxy = 305,
		TemporaryRedirect = 307,
		BadRequest = 400,
		Unauthorized = 401,
		PaymentRequired = 402,
		Forbidden = 403,
		NotFound = 404,
		MethodNotAllowed = 405,
		NotAcceptable = 406,
		ProxyAuthenticationRequired = 407,
		RequestTimeout = 408,
		Conflict = 409,
		Gone = 410,
		LengthRequired = 411,
		PreconditionFailed = 412,
		RequestEntityTooLarge = 413,
		RequestURITooLong = 414,
		UnsupportedMediaType = 415,
		RequestedRangeNotSatisfiable = 416,
		ExpectationFailed = 417,
		InternalServerError = 500,
		NotImplemented = 501,
		BadGateway = 502,
		ServiceUnavailable = 503,
		GatewayTimeout = 504,
		HTTPVersionNotSupported = 505
        
	};
    inline std::string GetStatusLine(HttpStatus status) noexcept {
        switch (status) {
        case HttpStatus::Continue:
            return "100 Continue";
        case HttpStatus::SwitchingProtocols:
            return "101 Switching Protocols";
        case HttpStatus::OK:
            return "200 OK";
        case HttpStatus::Created:
            return "201 Created";
        case HttpStatus::Accepted:
            return "202 Accepted";
        case HttpStatus::NonAuthoritativeInformation:
            return "203 Non-Authoritative Information";
        case HttpStatus::NoContent:
            return "204 No Content";
        case HttpStatus::ResetContent:
            return "205 Reset Content";
        case HttpStatus::PartialContent:
            return "206 Partial Content";
        case HttpStatus::MultipleChoices:
            return "300 Multiple Choices";
        case HttpStatus::MovedPermanently:
            return "301 Moved Permanently";
        case HttpStatus::Found:
            return "302 Found";
        case HttpStatus::SeeOther:
            return "303 See Other";
        case HttpStatus::NotModified:
            return "304 Not Modified";
        case HttpStatus::UseProxy:
            return "305 Use Proxy";
        case HttpStatus::TemporaryRedirect:
            return "307 Temporary Redirect";
        case HttpStatus::BadRequest:
            return "400 Bad Request";
        case HttpStatus::Unauthorized:
            return "401 Unauthorized";
        case HttpStatus::PaymentRequired:
            return "402 Payment Required";
        case HttpStatus::Forbidden:
            return "403 Forbidden";
        case HttpStatus::NotFound:
            return "404 Not Found";
        case HttpStatus::MethodNotAllowed:
            return "405 Method Not Allowed";
        case HttpStatus::NotAcceptable:
            return "406 Not Acceptable";
        case HttpStatus::ProxyAuthenticationRequired:
            return "407 Proxy Authentication Required";
        case HttpStatus::RequestTimeout:
            return "408 Request Timeout";
        case HttpStatus::Conflict:
            return "409 Conflict";
        case HttpStatus::Gone:
            return "410 Gone";
        case HttpStatus::LengthRequired:
            return "411 Length Required";
        case HttpStatus::PreconditionFailed:
            return "412 Precondition Failed";
        case HttpStatus::RequestEntityTooLarge:
            return "413 Request Entity Too Large";
        case HttpStatus::RequestURITooLong:
            return "414 Request-URI Too Long";
        case HttpStatus::UnsupportedMediaType:
            return "415 Unsupported Media Type";
        case HttpStatus::RequestedRangeNotSatisfiable:
            return "416 Requested Range Not Satisfiable";
        case HttpStatus::ExpectationFailed:
            return "417 Expectation Failed";
        case HttpStatus::InternalServerError:
            return "500 Internal Server Error";
        case HttpStatus::NotImplemented:
            return "501 Not Implemented";
        case HttpStatus::BadGateway:
            return "502 Bad Gateway";
        case HttpStatus::ServiceUnavailable:
            return "503 Service Unavaliable";
        case HttpStatus::HTTPVersionNotSupported:
            return "505 HTTP Version Not Supported";
        case HttpStatus::GatewayTimeout:
            return "504 Gateway Timeout";
        default:
            return "";
        }
    }
}