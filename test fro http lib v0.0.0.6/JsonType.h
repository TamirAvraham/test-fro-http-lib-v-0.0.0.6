#pragma once
#include <string>
#include <iostream>

/*
* TODO:
* improve string detection by looking for " and '
*/
namespace http {
	namespace json {
		enum class JsonType:char {
			String,
			Integer,
			Real,
			Boolean,
			Array,
			Object,
			Null
		};
		JsonType getTypeFromString(std::string stringAsValue);
	}
}