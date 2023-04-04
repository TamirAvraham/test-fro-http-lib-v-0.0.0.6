#include "HttpServer.h"

//#include <regex>
#include <sstream>

//std::vector<std::string> regex_split(const std::string& input, const std::regex& pattern)
//{
//    std::vector<std::string> result;
//    std::copy(std::sregex_token_iterator(input.begin(), input.end(), pattern, -1),
//        std::sregex_token_iterator(),
//        std::back_inserter(result));
//    return result;
//}
//
//std::vector<http::HttpRouteParam> GetParams(const std::string& route, const std::string& routeTemplate)
//{
//    std::vector<http::HttpRouteParam> params;
//
//    // Split the route string into individual components
//    auto routeComponents = regex_split(route, std::regex("/"));
//
//    // Split the route template string into individual components
//    auto routeTemplateComponents = regex_split(routeTemplate, std::regex("/"));
//
//    // Iterate over both route components and template components
//    for (size_t i = 0; i < routeComponents.size(); i++)
//    {
//        // If the current component in the route template starts with a colon, it's a parameter
//        if (!routeTemplateComponents[i].empty() && routeTemplateComponents[i][0] == ':')
//        {
//            // Extract the parameter name
//            auto paramName = routeTemplateComponents[i].substr(1);
//
//            // Add the parameter name and value to the params vector
//            params.emplace_back(http::HttpRouteParam(paramName, routeComponents[i]));
//        }
//    }
//
//    return params;
//}

std::pair<bool, std::vector<http::HttpRouteParam>> http::HttpServer::getParamsFromRoute(std::string route, std::string templateRoute) const
{
    std::stringstream routeAsStream(route),templateRouteAsStream(templateRoute);
    std::vector<std::string> routeVector, templateRouteVector;

    auto getRouteComponents = [](std::vector<std::string>& vec,std::stringstream& routeToSplit) {
        std::string part;
        while (std::getline(routeToSplit, part, '/'))
        {
            vec.push_back(part);
        }
    };

    getRouteComponents(routeVector, routeAsStream);
    getRouteComponents(templateRouteVector, templateRouteAsStream);

    if (routeVector.size()!=templateRouteVector.size())
    {
        return{ false,{} };
    }

    std::vector<http::HttpRouteParam> params;

    for (size_t i = 0; i < templateRouteVector.size(); i++)
    {
        if (templateRouteVector[i][0] == ':')
        {
            std::string cache1 = templateRouteVector[i].substr(1), cache2 = routeVector[i];
            params.push_back({ cache1,cache2 });
            continue;
        }
        else if (templateRouteVector[i] != routeVector[i])
            return { false,{} };
    }

    return {true,params};
}
std::pair<std::vector<http::HttpRouteParam>, std::function<void(http::HttpServer::HttpContext&)>> http::HttpServer::matchRoute(std::string gotRoute,http::HttpRequestType reqType)
{
    for (auto route:_routes.at(reqType))
    {
        auto [is_route, params] = getParamsFromRoute(gotRoute, route._route);
        if (is_route)
        {
            return { params,route._handler };
        }
    }
    throw http::HttpStatus::NotFound;
}
void http::HttpServer::serve()
{

    while (true)
    {
        SOCKET newsock;
        acceptConnection(newsock);
        ConnHandler(newsock);
    }
}
void http::HttpServer::ConnHandler(SOCKET sock)
{
    tcp::simpleSocket simpleSock(sock);
    std::string req = simpleSock.read(1000);
    std::cout << req << '\n';
    try
    {
        _threadPool.async([req, sock, this]() {

            try
            {
                auto [context, handler] = getContextFromReq(req, sock);
                handler(context);
                closesocket(sock);
            }
            catch (const http::HttpStatus& stat) {
                HttpSocket _sock(sock);
                switch (stat)
                {
                case http::HttpStatus::BadRequest:
                    _sock.bindMsg(stat, HtmlFileReader("Error400.html"));
                    break;
                case http::HttpStatus::NotFound:
                    _sock.bindMsg(stat, HtmlFileReader("Error404.html"));
                    break;
                default:
                    break;
                }
                closesocket(sock);
            }
            catch (const std::exception& e)
            {
                std::cout << e.what() << '\n';
                closesocket(sock);
            }
            catch (...) {
                std::cout << "error" << '\n';
                closesocket(sock);
            }


            });
    }
    catch (const http::HttpStatus& stat) {
        HttpSocket _sock(sock);
        switch (stat)
        {
        case http::HttpStatus::BadRequest:
            _sock.bindMsg(stat, HtmlFileReader("Error400.html"));
            break;
        case http::HttpStatus::NotFound:
            _sock.bindMsg(stat, HtmlFileReader("Error404.html"));
            break;
        default:
            break;
        }
        closesocket(sock);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << '\n';
        closesocket(sock);
    }
    catch (...) {
        std::cout << "error" << '\n';
        closesocket(sock);
    }

}
http::HttpServer::HttpServer(int port, std::string ip):tcp::TcpServer(port,ip),_threadPool(50)
{
}
void http::HttpServer::HandleRoute(http::HttpRequestType type, HttpRoute route)
{
    auto typeRouteVec = _routes.find(type);
    if (typeRouteVec==_routes.end())
    {
        _routes.insert({ type,{} });
    }
    _routes.at(type).push_back(route);
}
void http::HttpServer::ServeHtmlPage(const std::string& routeName, HtmlFileReader& htmlFileReader)
{
    _routes.at(http::HttpGET).push_back({ routeName,[&htmlFileReader](HttpContext context) {
        context.sendHtml(HttpStatus::OK,htmlFileReader);
} });
}
void http::HttpServer::ServeHtmlPage(const std::string&& routeName, HtmlFileReader& htmlFileReader)
{
    _routes.at(http::HttpGET).push_back({ routeName,[&htmlFileReader](HttpContext context) {
        context.sendHtml(HttpStatus::OK,htmlFileReader);
        }
    });
}

std::pair<http::HttpServer::HttpContext,std::function<void(http::HttpServer::HttpContext&)>> http::HttpServer::getContextFromReq(std::string req, SOCKET sock)
{
    http::HttpTokenizer reqAsHttpToken(req);
    if (reqAsHttpToken.GetError() != HttpStatus::OK)
    {
        throw reqAsHttpToken.GetError();
    }
    else if (reqAsHttpToken.isJs().first)
    {
        HttpSocket(sock).bindJs(HttpStatus::OK,reqAsHttpToken.isJs().second);
        throw HttpStatus::OK;
    }
    else if (reqAsHttpToken.isCss().first)
    {
        HttpSocket(sock).bindCss(HttpStatus::OK, reqAsHttpToken.isCss().second);
        throw HttpStatus::OK;
    }
    auto [routeParams, handler] = matchRoute(reqAsHttpToken.GetRoute(), reqAsHttpToken.GetType());
    http::HttpServer::HttpContext con(reqAsHttpToken.GetBody(), routeParams, sock);
    return {con, handler};
}
//
//std::vector<http::HttpRouteParam> http::HttpServer::getRouteParams(std::string route, std::string parttern) const
//{
//    return GetParams(route,parttern);
//}

http::HttpServer::HttpContext::HttpContext(std::string body, std::vector<HttpRouteParam> params, SOCKET sock):_body(body),_params(params),_sock(sock)
{
}

std::string http::HttpServer::HttpContext::GetParam(std::string paramName) const
{
    for (HttpRouteParam param : _params) {
        if (param._paramName==paramName)
        {
            return param._paramValue;
        }
    }
    throw std::invalid_argument("param name was not found");
}

std::string http::HttpServer::HttpContext::GetBody() const noexcept
{
    return _body;
}

http::json::JsonObject http::HttpServer::HttpContext::GetBodyAsJson() const noexcept
{
    return json::JsonObject(_body);
}

void http::HttpServer::HttpContext::sendJson(http::HttpStatus status, http::json::JsonObject& jsonObject, http::HttpHeaders headers) noexcept
{
    _sock.bindMsg(status, jsonObject, headers);
}



void http::HttpServer::HttpContext::sendHtml(http::HttpStatus status, http::FileReader& htmlfile, http::HttpHeaders headers) noexcept
{
    _sock.bindMsg(status, htmlfile, headers);
}

http::HttpRoute::HttpRoute(std::string routeTemplate, std::function<void(HttpServer::HttpContext&)> handler):_route(routeTemplate),_handler(handler)
{
}
