#include "TcpServer.h"
#include "JsonObject.h"
#include "ThreadPool.h"
#include "HttpSocket.h"
#include "HttpServer.h"
#include "HtmlFileReader.h"
#include "Dog.h"
#include "CreateDogHandlerFunc.h"
#include <iostream>

/*
TODO:
db:
create dog model DONE
api:
create api route for adding a dog 
create api route to get all dogs as json
create api route to get a spesific dog deatails by id

front:
create the website

lib:

bugs:


*/
int main() {
	http::HttpRoute createDogRoute({ "/api/create" }, { CreateDogHandlerFunc });
	http::HttpRoute getDogRoute({ "/api/get_dog/:id" }, { getDogHandlerFunc });
	http::HttpRoute getAllDogsRoute({ "/api/get_all_dogs" }, { getAllDogsHandlerFunc });
	http::HttpServer server(8080,"127.0.0.1");
	server.HandleRoute(http::HttpPOST, createDogRoute);
	server.HandleRoute(http::HttpGET, getDogRoute);
	server.HandleRoute(http::HttpGET, getAllDogsRoute);
	server.serve();


	return 0;
}
