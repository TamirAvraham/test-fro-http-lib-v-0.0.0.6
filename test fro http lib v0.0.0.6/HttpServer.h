#pragma once


#include<string>
#include<set>
#include<vector>
#include "HtmlFileReader.h"
#include "HttpParser.h"
#include "HttpStatus.h"
#include "JsonObject.h"
#include "TcpServer.h"
#include "ThreadPool.h"
#include "HttpSocket.h"

namespace http {
	struct HttpRouteParam
	{
		std::string _paramName;
		std::string _paramValue;
		inline HttpRouteParam(std::string name, std::string value):_paramName(name),_paramValue(value) {}
	};


	

	struct HttpRoute;

	class HttpServer:protected tcp::TcpServer
	{
	public:
		void serve();
		HttpServer(int port, std::string ip);
		void HandleRoute(http::HttpRequestType,HttpRoute);
		void ServeHtmlPage(const std::string& routeName, HtmlFileReader& htmlFileReader);
		void ServeHtmlPage(const std::string&& routeName, HtmlFileReader& htmlFileReader);


		class HttpContext {

		public:
			HttpContext(std::string body, std::vector<HttpRouteParam> params, SOCKET sock);
			std::string GetParam(std::string paramName)const;

			std::string GetBody()const noexcept;
			json::JsonObject GetBodyAsJson()const noexcept;
			
			void sendJson(http::HttpStatus status, http::json::JsonObject& jsonObject, http::HttpHeaders headers=http::HttpHeaders()) noexcept;
			void sendHtml(http::HttpStatus status, http::FileReader& htmlfile, http::HttpHeaders headers=http::HttpHeaders()) noexcept;
			// alloc aysnc task

			/// <summary>
			/// alloc task to thread pool
			/// </summary>
			/// <typeparam name="F"> function type</typeparam>
			/// <typeparam name="...Args">function args type</typeparam>
			/// <param name="f">function to alloc to a worker</param>
			/// <param name="...args">function args</param>
			/// <returns>future for function sent for alloc</returns>
			template<class F, class... Args> auto allocTask(F&& f, Args&&... args) const ->std::future<decltype(f(args...))>;
		private:
			std::string _body;
			std::vector<HttpRouteParam> _params;
			http::HttpSocket _sock;
		};

	private:
		void ConnHandler(SOCKET sock);
		std::pair<http::HttpServer::HttpContext, std::function<void(http::HttpServer::HttpContext&)>> getContextFromReq(std::string req, SOCKET sock);
		std::pair<bool,std::vector<HttpRouteParam>> getParamsFromRoute(std::string route, std::string templateRoute)const;
		std::pair<std::vector<HttpRouteParam>,std::function<void(HttpServer::HttpContext&)>> matchRoute(std::string gotRoute, http::HttpRequestType reqType);
		
		/*std::vector<http::HttpRouteParam> getRouteParams(std::string route,std::string parttern)const;*/
		std::map<HttpRequestType,std::vector<HttpRoute>> _routes;
		ThreadPool _threadPool;
	};
	
	template<class F, class ...Args>
	inline auto HttpServer::HttpContext::allocTask(F&& f, Args && ...args) const -> std::future<decltype(f(args ...))>
	{
		return _threadPool.async(f,args);
	}
	struct HttpRoute
	{
		/// <summary>
		/// ctor for HttpRoute
		/// </summary>
		/// <param name="reqType"> type of requast</param>
		/// <param name="routeTemplate"> the route as a string route params need to have a : and a name to get them from http context example:
		/// "api/:userId/get" in this route the :userId will be a route param and be receved using HttpContext::getParam("userId")</param>
		/// <param name="handler">user function to be called when using the route</param>
		HttpRoute(std::string routeTemplate, std::function<void(HttpServer::HttpContext&)> handler);
		std::string _route;
		std::function<void(HttpServer::HttpContext&)> _handler;
	};
	using HttpContext = HttpServer::HttpContext;
}


