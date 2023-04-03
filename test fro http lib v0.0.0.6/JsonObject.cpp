#include "JsonObject.h"

http::json::JsonObject::JsonObject():_jsonMap(),_changed(true)
{
}

http::json::JsonObject::JsonObject(const std::string jsonString):_asString("/\\"),_changed(false)
{
	try
	{
		_jsonMap = http::json::JsonParser::parse(jsonString);
	}
	catch (int errorCode) {
		if (errorCode==PARSE_ERROR)
		{
			throw std::invalid_argument("invalid argument");
		}
	}
	catch (const std::exception&)
	{
		throw std::invalid_argument("invalid argument");
	}
	_asString = jsonString;
}

http::json::JsonObject::JsonObject(const std::map<std::string, http::json::JsonValue> jsonMap):_jsonMap(jsonMap),_changed(false)
{
	_asString = ToString();
}

http::json::JsonObject& http::json::JsonObject::operator=(std::string jsonString)
{
	*this = JsonObject(jsonString);
	return *this;
}

http::json::JsonObject& http::json::JsonObject::operator=(const std::map<std::string, http::json::JsonValue> jsonMap)
{
	*this = JsonObject(jsonMap);
	return *this;
}

http::json::JsonValue http::json::JsonObject::operator[](const std::string& name)const
{
	JsonValue ret;
	try {
		ret = _jsonMap.at(name);
	}
	catch (std::exception& error) {
		throw std::invalid_argument("name was not in json");
	}
	return ret;
}

void http::json::JsonObject::insert(JsonKeyValuePair keyValuePair) 
{
	_jsonMap.insert(keyValuePair);
	_changed = true;
}

void http::json::JsonObject::set(const std::string& keyName, http::json::JsonValue val) noexcept
{
	_jsonMap[keyName] = val;
}

std::string http::json::JsonObject::ToString()
{
	if (_asString!="/\\"&&!_changed)
	{
		std::string cahedRet = _asString;
		int cP = cahedRet.length();
		char c = cahedRet[cP-1];
		if (c!='}')
		{
			cahedRet += "\n}";
		}
		return cahedRet;
	}
	//TODO:
	//add a way to add only the new values to _as string to save on time
	std::string ret;
	ret += "{\n ";
	for (JsonKeyValuePair jsonKeyValue : _jsonMap) {
		JsonValue jsonValue = jsonKeyValue.second;
		ret += '\"';
		ret += jsonKeyValue.first;
		ret += "\" : ";
		const std::string valAsString=jsonValue.string_value();
		if (valAsString.empty())
		{
			ret += "null";
		}
		else if (jsonValue.type()==JsonType::String)
		{
			ret += '\"';
			ret += valAsString;
			ret += '\"';
		}
		else
		{
			ret += valAsString;
		}
		ret += ',';
		ret += "\n ";
	}
	ret = ret.substr(0, ret.length() - 3);
	_asString = ret;
	ret += "\n}";
	return ret;
}
