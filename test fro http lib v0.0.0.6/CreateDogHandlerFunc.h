#pragma once
#include "HttpServer.h"
void CreateDogHandlerFunc(http::HttpContext& context);
void getDogHandlerFunc(http::HttpContext& context);
void getAllDogsHandlerFunc(http::HttpContext& context);
void adoptDog(http::HttpContext& context);

