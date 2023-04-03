#pragma once
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "JsonType.h"





namespace http {
	namespace json {
		class JsonParser;
		class JsonValue
		{
		public: 
			
			JsonValue() : _type(JsonType::String), _string_value("") {}
			JsonValue(JsonType type, const std::string& value) : _type(type), _string_value(value) {}
			JsonValue(const std::string& value):_type(http::json::getTypeFromString(value)),_string_value(value){}
			JsonValue(const std::vector<JsonValue>& json_values);
			JsonType type() const { return _type; }
			std::string string_value() const { return _string_value; }
			int integer_value() const { return std::stoi(_string_value); }
			double real_value() const { return std::stod(_string_value); }
			bool boolean_value() const { return _string_value == "true"; }
			std::vector<JsonValue> array_value() const;
			std::map<std::string, http::json::JsonValue> object_value() const;
		private:
			JsonType _type;
			std::string _string_value;
			

			// array and object type
			
		};
	};
}

