#include "JsonValue.h"
#include "JsonParser.h"

http::json::JsonValue::JsonValue(const std::vector<JsonValue>& json_values)
{
    _type = JsonType::Array;
    _string_value = "[";
    for (auto json_val : json_values) {
        _string_value += '\n';
        _string_value += json_val.string_value();
        _string_value += ',';
    }
    _string_value = _string_value.substr(0, _string_value.length() - 1);
    _string_value += ']';
        
}

std::vector<http::json::JsonValue> http::json::JsonValue::array_value() const
{
    size_t startOfArray = _string_value.find('[');
    size_t endOfArray = _string_value.find_last_of(']');
    
    if (startOfArray==std::string::npos||endOfArray==std::string::npos)
    {
        throw std::invalid_argument("value is not an a json array");
    }

    std::string arrayInside = _string_value.substr(startOfArray+1, endOfArray-1);
    std::cout << arrayInside << '\n';
    std::stringstream insideAsStream(arrayInside);
    std::string arrayIteam;
    std::vector<JsonValue> arrayValues;
    while (!insideAsStream.eof())
    {
        std::getline(insideAsStream, arrayIteam, ',');
        std::cout << arrayIteam << '\n';
        arrayValues.push_back(JsonValue(json::getTypeFromString(arrayIteam), arrayIteam));
    }

    return arrayValues;
}

std::map<std::string, http::json::JsonValue> http::json::JsonValue::object_value() const
{

    return http::json::JsonParser::parse(_string_value);
}



