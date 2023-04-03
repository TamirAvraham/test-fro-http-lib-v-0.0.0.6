

#include <tchar.h>
#include "TcpServer.h"
#include <string>
#include <sstream>

#define INIT_ERROR 505

namespace http {
    Logger logger;
}
using http::logger;

tcp::TcpServer::TcpServer(int port, std::string ip):_ip_address(ip),_port(port), _socketAddress_len(sizeof(_socketAddress)),_socketAddress()
{
    if (startServer()!=1)
    {
        std::ostringstream ss;
        ss << "Failed to start server with PORT: " << ntohs(_socketAddress.sin_port);
        logger.log(ss.str());
    }
    startListen();

   /* int iResult = WSAStartup(MAKEWORD(2, 2), &_wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        throw INIT_ERROR;
    }
    struct addrinfo* result = NULL, * ptr = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;


    iResult = getaddrinfo(NULL, port.c_str(), &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        throw INIT_ERROR;
    }

    _ListenSocket = INVALID_SOCKET;
    _ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (_ListenSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        throw INIT_ERROR;
    }*/
}

tcp::TcpServer::~TcpServer()
{
    closesocket(_ListenSocket);
    WSACleanup();
}

void tcp::TcpServer::HandleConection(std::function<void(SOCKET sock)> handleFunction)
{
    SOCKET newSocket;
    acceptConnection(newSocket);
    std::thread connectionThread(handleFunction,newSocket);
    connectionThread.detach();
}

int tcp::TcpServer::startServer()
{
    PVOID pAddrBuf;
    if (WSAStartup(MAKEWORD(2, 0), &_wsaData) != 0)
    {

        logger.exitWithError("WSAStartup failed");
        return INIT_ERROR;
    }

    _ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_ListenSocket < 0)
    {
        logger.exitWithError("Cannot create socket");
        throw INIT_ERROR;
    }

    _socketAddress.sin_family = AF_INET;
    _socketAddress.sin_port = htons(_port);

    std::wstring widestr = std::wstring(_ip_address.begin(), _ip_address.end());
    const wchar_t* widecstr = widestr.c_str();

    InetPton(_socketAddress.sin_family, widecstr, &_socketAddress.sin_addr.s_addr);

    if (bind(_ListenSocket, (sockaddr*)&_socketAddress, _socketAddress_len) < 0)
    {
        logger.exitWithError("Cannot connect socket to address");
        throw INIT_ERROR;
    }
    return 1;
}

void tcp::TcpServer::startListen()
{
    if (listen(_ListenSocket, 20) < 0)
    {
       http::logger. exitWithError("Socket listen failed");
    }
    std::ostringstream ss;
    PWSTR str=nullptr;
    InetNtop(_socketAddress.sin_family, &_socketAddress.sin_addr.s_addr, str, 50);
    ss << "\n*** Listening on ADDRESS: "
        << str
        << " PORT: " << ntohs(_socketAddress.sin_port)
        << " ***\n\n";
    http::logger.log(ss.str());
}

void tcp::TcpServer::acceptConnection(SOCKET& new_socket)
{
    new_socket = accept(_ListenSocket, (sockaddr*)&_socketAddress,&_socketAddress_len);
    if (new_socket < 0)
    {
        std::ostringstream ss;
        PWSTR str = nullptr;
        InetNtop(_socketAddress.sin_family, &_socketAddress.sin_addr.s_addr, str, 50);
        ss <<
            "Server failed to accept incoming connection from ADDRESS: "
            << str << "; PORT: "
            << ntohs(_socketAddress.sin_port);
        http::logger.exitWithError(ss.str());
    }
}

template<class I>
void tcp::TcpServer::HandleConection(std::function<void(I, SOCKET sock)> handleFunction, I inputs)
{
    SOCKET newSocket;
    acceptConnection(newSocket);
    std::thread connectionThread(handleFunction, I,newSocket);
    connectionThread.detach();
}







tcp::simpleSocket::simpleSocket(SOCKET socket):_socket(socket)
{
}

tcp::simpleSocket::~simpleSocket()
{
    //closesocket(_socket);
}

std::string tcp::simpleSocket::read(int bufferSize)
{
    char *buffer = new char[bufferSize];
    int bytesRead = recv(_socket, buffer, bufferSize, 0);
    if (bytesRead < 0)
    {
        http::logger.log("Failed to receive bytes from client socket connection");
    }
    std::string ret = buffer;
    return ret;
}

void tcp::simpleSocket::write(std::string msg)
{
    int totalBytesSent = 0;
    int bytesSent = 0;
    while (totalBytesSent<msg.size())
    {
        bytesSent = send(_socket, msg.c_str(), msg.size(), 0);
        if (bytesSent < 0)
        {
            break;
        }
        totalBytesSent += bytesSent;
    }
    if (totalBytesSent == msg.size())
    {
        http::logger.log("\n\n------ Server Response sent to client ------\n\n");
        http::logger.log(msg);
    }
    else
    {
        http::logger.log("Error sending response to client.");
    }
}

