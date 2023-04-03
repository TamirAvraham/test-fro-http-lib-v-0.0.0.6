#include "JsonType.h"

http::json::JsonType http::json::getTypeFromString(std::string valueAsString)
{
	if (valueAsString.empty())
	{
		return JsonType::Null;
	}
    char firstChar = valueAsString[0], lastChar = valueAsString[valueAsString.length() - 1];
	std::cout << "first char: " << firstChar << " last char: " << lastChar << " \n";
	switch (firstChar)
	{
	case '[':
		if (valueAsString.find_last_of(']') != std::string::npos)
			return JsonType::Array;
		break;
	case '{':
		if (valueAsString.find_last_of('}')!=std::string::npos)
		{
			return JsonType::Object;
		}
		break;
	default:
		break;
	}
	if (valueAsString=="true"||valueAsString=="false")
	{
		return JsonType::Boolean;
	}
	
	if (isdigit(firstChar))
	{
		size_t dotInNumber=valueAsString.find('.');
		if (dotInNumber!=std::string::npos)
		{
			return JsonType::Real;
		}
		return JsonType::Integer;
	}
	if (valueAsString=="null"||valueAsString=="undefined")
	{
		return JsonType::Null;
	}
    return JsonType::String;
}

