#pragma once
#include <string>
#include "JsonObject.h"
class Dog
{
private:
	std::string _name;
	std::string _pictureLocation;
	std::string _bread;
	int _age;
	int _id;
	char _gender;
public:

	http::json::JsonObject toJson()const noexcept;
	

public:
	static Dog New(int age, char gender, const std::string& name, const std::string& bread, const std::string& pathToPhoto);
	static Dog GetById(int id);
	static std::vector<Dog> getAllDogs();
	static bool DeleteDogById(int id);
};

