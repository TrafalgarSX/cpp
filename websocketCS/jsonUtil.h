#ifndef _JSONUTIL_H_
#define _JSONUTIL_H_

#include <string>
#include <vector>

namespace jsonUtil {
    std::string getJsonString(const char* json, const char* key);
    int getJsonInt(const char* json, const char* key);
    bool getJsonBool(const char* json, const char* key);
    double getJsonDouble(const char* json, const char* key);

    void setJsonString(std::string& json, const char* key, const char* value);
    void setJsonInt(std::string& json, const char* key, int value);
    void setJsonBool(std::string& json, const char* key, bool value);
    void setJsonDouble(std::string& json, const char* key, double value);

    void setJsonArray(std::string& json, const char* key, std::vector<std::string> value);
    void setJsonArray(std::string& json, const char* key, std::vector<bool> value);
    void setJsonArray(std::string& json, const char* key, std::vector<int> value);
    void setJsonArray(std::string& json, const char* key, std::vector<float> value);
    void setJsonArray(std::string& json, const char* key, std::vector<double> value);
}


#endif
