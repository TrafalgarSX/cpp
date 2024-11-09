#include "etcryptmedia_module.h"

std::array<const char*, symbol_count> EtCryptMediaModule::exports = {"EtCoSignLibInit",
                                                                        "EtCoSignLibRelease",
                                                                        "EtCoSignSetUrl",
                                                                        "EtGetCosignState",
                                                                        "EtCoSignTerminalInit",
                                                                        "EtCoSignUserInit",
                                                                        "EtCoSignLogin",
                                                                        "EtCoSignGetUserCert",
                                                                        "EtCertRelease",
                                                                        "EtCoSignVerifyPin",
                                                                        "EtCoSignChangeUserPin",
                                                                        "EtCoSignGetLocalPinInfo",
                                                                        "EtCoSignRequestUnblockPin",
                                                                        "EtCoSignGetWebPinStats",
                                                                        "EtCoSignDoUnblockPin",
                                                                        "EtCoSignSetAccount",
                                                                        "EtCosignSetTicket",
                                                                        "EtCoSignLoginVerify",
                                                                        "EtCosignGetInfo",
                                                                        "EtCosignMemFree",
                                                                        "EtCosignGetErrorMsg",
                                                                        "EtCosignGetErrorMsgEx"
                                                                        };