#include "JsonParser.h"

std::map<std::string, http::json::JsonValue> http::json::JsonParser::parse(std::string jsonInput)
{
    size_t firstBracket, lastBracket;
    std::map<std::string, http::json::JsonValue> jsonKeyValuesMap;
    JsonKeyValuePair pair;

    firstBracket = jsonInput.find('{');
    lastBracket = jsonInput.find_last_of('}');
    
    if (firstBracket==std::string::npos||lastBracket== std::string::npos)
    {
        throw PARSE_ERROR;
    }
   
    jsonInput = jsonInput.substr(firstBracket + 1, lastBracket);
    pair = parseValue(jsonInput);
    while (pair.first!="cpplibinternalerror")
    {
        jsonKeyValuesMap.insert(pair);
        pair = parseValue(jsonInput);

    }
    

    return jsonKeyValuesMap;
}

JsonLine http::json::JsonParser::getJsonLine(std::string& jsonObject)
{
    
    std::string nameOfLine = GetLineName(jsonObject);
    auto [valueOfLine, endOfLine] = GetLineValue(jsonObject);
    if (endOfLine == -1)
        return { "","" };
    size_t cut = endOfLine + nameOfLine.length()+2;
    jsonObject = jsonObject.substr(cut+1, jsonObject.length());
    
    return {nameOfLine,valueOfLine};
}

std::string http::json::JsonParser::GetLineName(std::string jsonObject)
{
    size_t startOfName = jsonObject.find('\"');
    if (startOfName == std::string::npos)
    {
        return "";
    }
    size_t endOfName = jsonObject.find('\"', startOfName+1);
    if (endOfName == std::string::npos)
    {
        return "";
    }

    return jsonObject.substr(startOfName+1,(endOfName-startOfName)-1);
}

std::pair<std::string, int> http::json::JsonParser::GetLineValue(std::string jsonObject)
{
    //finding the split of the keyy value pair
    size_t startOfvalue = jsonObject.find(':');
    //return vars init
    int returnNumber = -1;
    std::string returnString = "";
    //cheack if there is a key value pair if no quiit with error return
    if (startOfvalue == std::string::npos)
    {
        return { returnString,returnNumber };
    }
    //return string = the value part of the key value pair and the rest of the pairs
    jsonObject = jsonObject.substr(startOfvalue + 1, jsonObject.size());
    //start of value becomes 0 since we are now working with a new string
    startOfvalue = 0;
    //first char fo value
    char firstCharOfValue = jsonObject[startOfvalue];
    //inc 
    while (firstCharOfValue==' ')
    {
        firstCharOfValue = jsonObject[++startOfvalue];
    }
    //plus 1 for later
    startOfvalue++;
    size_t endOfValue;
    switch (firstCharOfValue)
    {
    case '{':
    case '[':
        endOfValue = findMatchingClosingBracket(jsonObject,startOfvalue-1);
        if (endOfValue == -1)
        {
            return { returnString, returnNumber };
        }
        returnString = jsonObject.substr(startOfvalue-1, endOfValue + 1);
        
        break;
    case '\"':
        endOfValue = jsonObject.find('\"',startOfvalue);
        if (endOfValue == std::string::npos)
        {
            return { returnString,returnNumber };
        }
        returnString = jsonObject.substr(startOfvalue, endOfValue-1);
        break;
    default:

        endOfValue = jsonObject.find(',', startOfvalue);
        
        if (endOfValue == std::string::npos)
        {
            //checking if there is a nother value fo value name in the object (if not this is the last of the value in the object)
            if (jsonObject.find("\"")==std::string::npos)
            {
                std::remove_if(jsonObject.begin(), jsonObject.end(), [](char c) {return c == '\n'; });
                endOfValue = jsonObject.find('}');
                if (endOfValue==std::string::npos)
                {
                    return { returnString,returnNumber };
                }
            }
            else
                return { returnString,returnNumber };
        }
        returnString = jsonObject.substr(startOfvalue - 1 , endOfValue);
        break;
    }
    auto next_value = jsonObject.find('\"', endOfValue + 1);
    returnNumber += (next_value==std::string::npos?returnString.size():next_value) + startOfvalue;
    return { returnString,returnNumber };
}
size_t http::json::JsonParser::findMatchingClosingBracket(std::string jsonString, size_t startIndex)
{
    int bracketCount = 1;
    char openBracket = jsonString[startIndex];
    char closeBracket;
    if (openBracket == '{')
        closeBracket = '}';
    else if (openBracket == '[')
        closeBracket = ']';
    else
        return -1;
    for (size_t i = startIndex + 1; i < jsonString.size(); i++)
    {
        if (jsonString[i] == openBracket)
            bracketCount++;
        else if (jsonString[i] == closeBracket)
            bracketCount--;
        if (bracketCount == 0)
            return i;
    }
    return -1;
}
JsonKeyValuePair http::json::JsonParser::parseValue(std::string& json)
{
    
    JsonLine line = getJsonLine(json);
   
    if (line.second == "" && line.first == "")
        return { "cpplibinternalerror",http::json::JsonValue()};
    http::json::JsonValue jsonValue(getTypeFromString(line.second), line.second);
    return { line.first,jsonValue };
}


