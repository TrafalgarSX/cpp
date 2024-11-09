#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "jsonUtil.h"
#include <iostream>
#include <string>
#include <vector>

using namespace rapidjson;

std::string jsonUtil::getJsonString(const char* json, const char* key)
{
    Document d;
    std::cout << "json: " << json << std::endl;
    d.Parse(json);
    if (d.HasParseError())
    {
        std::cout << "GetParseError " << d.GetParseError() << std::endl;
        return "";
    }
    if (d.HasMember(key))
    {
        return d[key].GetString();
    }
    return "";
}

int jsonUtil::getJsonInt(const char* json, const char* key)
{
    Document d;
    d.Parse(json);
    if (d.HasParseError())
    {
        std::cout << "GetParseError " << d.GetParseError() << std::endl;
        return 0;
    }
    if (d.HasMember(key))
    {
        return d[key].GetInt();
    }
    return 0;
}

bool jsonUtil::getJsonBool(const char* json, const char* key)
{
    Document d;
    d.Parse(json);
    if (d.HasParseError())
    {
        std::cout << "GetParseError " << d.GetParseError() << std::endl;
        return false;
    }
    if (d.HasMember(key))
    {
        return d[key].GetBool();
    }
    return false;
}

double jsonUtil::getJsonDouble(const char* json, const char* key)
{
    Document d;
    d.Parse(json);
    if (d.HasParseError())
    {
        std::cout << "GetParseError " << d.GetParseError() << std::endl;
        return 0.0;
    }
    if (d.HasMember(key))
    {
        return d[key].GetDouble();
    }
    return 0.0;
}

void jsonUtil::setJsonString(std::string& json, const char* key, const char* value)
{
    Document d;
    d.Parse(json.c_str());
    if (d.HasParseError())
    {
        std::cout << "GetParseError " << d.GetParseError() << std::endl;
        return;
    }
    if (d.HasMember(key))
    {
        d[key].SetString(value, d.GetAllocator());
    }
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);
    json = buffer.GetString();
}

void jsonUtil::setJsonInt(std::string& json, const char* key, int value)
{
    Document d;
    d.Parse(json.c_str());
    if (d.HasParseError())
    {
        std::cout << "GetParseError " << d.GetParseError() << std::endl;
        return;
    }
    if (d.HasMember(key))
    {
        d[key].SetInt(value);
    }
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);
    json = buffer.GetString();
}

void jsonUtil::setJsonBool(std::string& json, const char* key, bool value)
{
    Document d;
    d.Parse(json.c_str());
    if (d.HasParseError())
    {
        std::cout << "GetParseError " << d.GetParseError() << std::endl;
        return;
    }
    if (d.HasMember(key))
    {
        d[key].SetBool(value);
    }
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);
    json = buffer.GetString();
}

void jsonUtil::setJsonDouble(std::string& json, const char* key, double value)
{
    Document d;
    d.Parse(json.c_str());
    if (d.HasParseError())
    {
        std::cout << "GetParseError " << d.GetParseError() << std::endl;
        return;
    }
    if (d.HasMember(key))
    {
        d[key].SetDouble(value);
    }
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);
    json = buffer.GetString();
}

void jsonUtil::setJsonArray(std::string& json, const char* key, std::vector<std::string> value)
{
    Document d;
    d.Parse(json.c_str());
    if (d.HasParseError())
    {
        std::cout << "GetParseError " << d.GetParseError() << std::endl;
        return;
    }
    if (d.HasMember(key))
    {
        Value array(kArrayType);
        for (int i = 0; i < value.size(); i++)
        {
            Value v;
            v.SetString(value[i].c_str(), value[i].size(), d.GetAllocator());
            array.PushBack(v, d.GetAllocator());
        }
        d[key] = array;
    }
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);
    json = buffer.GetString();
}

void jsonUtil::setJsonArray(std::string& json, const char* key, std::vector<bool> value)
{
    Document d;
    d.Parse(json.c_str());
    if (d.HasParseError())
    {
        std::cout << "GetParseError " << d.GetParseError() << std::endl;
        return;
    }
    if (d.HasMember(key))
    {
        Value array(kArrayType);
        for (int i = 0; i < value.size(); i++)
        {
            Value v;
            v.SetBool(value[i]);
            array.PushBack(v, d.GetAllocator());
        }
        d[key] = array;
    }
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);
    json = buffer.GetString();
}

void jsonUtil::setJsonArray(std::string& json, const char* key, std::vector<int> value)
{
    Document d;
    d.Parse(json.c_str());
    if (d.HasParseError())
    {
        std::cout << "GetParseError " << d.GetParseError() << std::endl;
        return;
    }
    if (d.HasMember(key))
    {
        Value array(kArrayType);
        for (int i = 0; i < value.size(); i++)
        {
            Value v;
            v.SetInt(value[i]);
            array.PushBack(v, d.GetAllocator());
        }
        d[key] = array;
    }
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);
    json = buffer.GetString();
}

void jsonUtil::setJsonArray(std::string& json, const char* key, std::vector<float> value)
{
    Document d;
    d.Parse(json.c_str());
    if (d.HasParseError())
    {
        std::cout << "GetParseError " << d.GetParseError() << std::endl;
        return;
    }
    if (d.HasMember(key))
    {
        Value array(kArrayType);
        for (int i = 0; i < value.size(); i++)
        {
            Value v;
            v.SetFloat(value[i]);
            array.PushBack(v, d.GetAllocator());
        }
        d[key] = array;
    }
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);
    json = buffer.GetString();
}

void jsonUtil::setJsonArray(std::string& json, const char* key, std::vector<double> value)
{
    Document d;
    d.Parse(json.c_str());
    if (d.HasParseError())
    {
        std::cout << "GetParseError " << d.GetParseError() << std::endl;
        return;
    }
    if (d.HasMember(key))
    {
        Value array(kArrayType);
        for (int i = 0; i < value.size(); i++)
        {
            Value v;
            v.SetDouble(value[i]);
            array.PushBack(v, d.GetAllocator());
        }
        d[key] = array;
    }
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    d.Accept(writer);
    json = buffer.GetString();
}



