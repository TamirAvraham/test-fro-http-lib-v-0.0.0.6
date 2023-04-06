#include "CreateDogHandlerFunc.h"
#include "Dog.h"
void CreateDogHandlerFunc(http::HttpContext& context)
{
	try
	{
		auto bodyJson = context.GetBodyAsJson();
		const std::string name = bodyJson["name"].string_value();
		const std::string bread = bodyJson["breed"].string_value();
		const char gender = bodyJson["gender"].string_value()[0];
		const int age = bodyJson["age"].integer_value();
		auto ret = Dog::New(age, gender, name, bread, "").toJson();
		context.sendJson(http::HttpStatus::OK,ret);
	}
	catch (const std::invalid_argument& e) {
		http::json::JsonObject errorJson;
		errorJson.insert({ {"error"},JsonValue("body is not proper json") });
		context.sendJson(http::HttpStatus::BadRequest, errorJson);
	}
	catch (const std::exception& e)
	{
		http::json::JsonObject errorJson;
		errorJson.insert({ {"error"},JsonValue(e.what()) });
		context.sendJson(http::HttpStatus::BadRequest, errorJson);
	}
	
}

void getDogHandlerFunc(http::HttpContext& context)
{
	try
	{
		int id = std::stoi(context.GetParam("id"));
		auto ret = Dog::GetById(id).toJson();
		context.sendJson(http::HttpStatus::OK, ret);
	}
	catch (const std::exception& e)
	{
		http::json::JsonObject errorJson;
		errorJson.insert({ {"error"},JsonValue(e.what()) });
		context.sendJson(http::HttpStatus::BadRequest, errorJson);
	}
}

void getAllDogsHandlerFunc(http::HttpContext& context)
{
	try
	{
		auto dogsVec = Dog::getAllDogs();
		http::json::JsonObject ret;
		std::vector<http::json::JsonValue> dogsAsJsonValues;
		for (auto dog:dogsVec)
		{
			dogsAsJsonValues.push_back({ dog.toJson().ToString() });
		}
		ret.insert({ "dogs",dogsAsJsonValues });
		context.sendJson(http::HttpStatus::OK, ret);
	}
	catch (const std::exception& e)
	{
		http::json::JsonObject ret;
		ret.insert({ "error",{e.what()} });
		context.sendJson(http::HttpStatus::InternalServerError, ret);
	}
}

void adoptDog(http::HttpContext& context)
{
	http::json::JsonObject ret;
	try
	{
		int id = std::stoi(context.GetParam("id"));
		bool worked = Dog::DeleteDogById(id);
		if (worked)
		{
			ret.insert({ "success",{"dog adopted successfly"} });
		}
		else
		{
			throw std::exception("error: cant adopt dog");

		}
		context.sendJson(http::HttpStatus::OK, ret);
	}
	catch (const std::exception& e)
	{
		
		ret.insert({ "error",{e.what()} });
		context.sendJson(http::HttpStatus::InternalServerError, ret);
	}
}
