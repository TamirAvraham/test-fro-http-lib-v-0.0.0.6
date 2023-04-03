#pragma once
#include "JsonType.h"
#include "JsonValue.h"
#include <vector>
#include <map>

#define JsonLine	std::pair<std::string, std::string>
#define JsonKeyValuePair std::pair<std::string,http::json::JsonValue>
#define PARSE_ERROR 400
namespace http {
	namespace json {
		class JsonParser
		{
		public:
			static std::map<std::string,http::json::JsonValue> parse(std::string);
		private:
			static JsonLine getJsonLine(std::string&);
			static std::string GetLineName(std::string);
			static std::pair<std::string,int> GetLineValue(std::string);
			static size_t findMatchingClosingBracket(std::string jsonString, size_t startIndex=0);
			static std::string removeBraces(std::string);
			static JsonKeyValuePair parseValue(std::string&);
		};
	}
}


