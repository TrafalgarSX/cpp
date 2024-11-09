#ifndef CONVERT_H
#define CONVERT_H

#include <string>

bool contact_xls2json(const std::string &xls_file, std::string &contacts_json);

bool contact_xlsx2json(const std::string &xlsx_file,
                       std::string &contacts_json);

bool contact_json2xlsx(const std::string &contacts_json,
                       const std::string &xlsx_file);

std::string utf8_to_gbk(const std::string& utf8_str);

std::string gbk_to_utf8(const std::string& gbk_str);

#endif // CONVERT_H