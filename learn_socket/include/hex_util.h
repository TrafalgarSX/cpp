#ifndef _HEXUTIL_H_
#define _HEXUTIL_H_

#include <string>
#include <vector>

std::string bytesToHexString(const std::vector<uint8_t> buf, char sep);
std::string bytesToHexString(const uint8_t* buf, int len, char sep);
uint8_t* hexStringToBytes(const std::string& hexString, int* outLen, char sep);
std::vector<uint8_t> hexStringToBytes(const std::string& hexString, char sep);

#endif
