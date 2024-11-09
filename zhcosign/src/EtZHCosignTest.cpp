#include <iostream>
#include <string>
#include <memory>
#include "etcryptmedia_module.h"

#ifndef IF_FAIL_GOTO_END
// #	define	IF_FAIL_GOTO_END(c)						if ( c ){goto END;}
#	define	IF_FAIL_GOTO_END(c)						if ( c ){goto END;}
#endif

using u8 = uint8_t;
using u32 = uint32_t;

static std::string ip = "10.4.91.179";
static u32  port = 8999;

// 定义释放函数
void EtCosignMemFreeDeleter(char* ptr) {
    void* p = static_cast<void*>(ptr);
    EtCryptMediaModule::EtCosignMemFree(&p);
}

// 使用 using 关键字定义 std::unique_ptr 类型
using UniqueCharPtr = std::unique_ptr<char, decltype(&EtCosignMemFreeDeleter)>;

// 示例用法
void example() {
    // 假设 ppszCosignInfo 是从 EtCosignGetInfo 获得的
    char* ppszCosignInfo = nullptr;
    EtCryptMediaModule::EtCosignGetInfo("user_code", &ppszCosignInfo);

    // 使用 UniqueCharPtr 管理 ppszCosignInfo
    UniqueCharPtr cosignInfo(ppszCosignInfo, EtCosignMemFreeDeleter);

    // 现在 cosignInfo 将自动管理 ppszCosignInfo 的内存释放
}

u32 TestGetCosignState()
{
    u32   u32Result;
    u32   u32CosignState;

    u32Result = EtCryptMediaModule::EtGetCosignState("tmp/EtCosign", nullptr, &u32CosignState);
    IF_FAIL_GOTO_END(0x00!=u32Result);

    if(u32CosignState != (u32)CoSignState::TerminalNotInit) {
        u32Result = -1;
        std::cout << "EtGetCosignState check terminalinit failed!" << std::endl;
        goto END;
    }

    u32Result = EtCryptMediaModule::EtGetCosignState("/home/trafalgar/AZHcosigntest", nullptr, &u32CosignState);
    IF_FAIL_GOTO_END(0x00!=u32Result);

    if(u32CosignState != (u32)CoSignState::TerminalInit) {
        u32Result = -1;
        std::cout << "EtGetCosignState check terminalinit failed!" << std::endl;
        goto END;
    }

    u32Result = EtCryptMediaModule::EtGetCosignState("/home/trafalgar/AZHcosigntest", "guoyawen", &u32CosignState);
    IF_FAIL_GOTO_END(0x00!=u32Result);

    if(u32CosignState != (u32)CoSignState::UserInit) {
        u32Result = -1;
        std::cout << "EtGetCosignState check userinit failed!" << std::endl;
        goto END;
    }

    u32Result = EtCryptMediaModule::EtGetCosignState("/home/trafalgar/AZHcosigntest", "xisiyu", &u32CosignState);
    IF_FAIL_GOTO_END(0x00!=u32Result);

    if(u32CosignState != (u32)CoSignState::UserNotInit) {
        u32Result = -1;
        std::cout << "EtGetCosignState check userinit failed!" << std::endl;
        goto END;
    }

END:
    return u32Result;
}

u32 TestPin(const std::string usercode, const std::string &pin, const std::string &newpin)
{
    u32  u32Result;

    u32  maxRetryCount = 0;
    u32 remainRetryCount = 0;
    ETBOOL defaultPin = FALSE;
    // start test pin interface

    u32Result = EtCryptMediaModule::EtCoSignVerifyPin(usercode.c_str(), pin.c_str());
    IF_FAIL_GOTO_END(0x00!=u32Result);

    u32Result = EtCryptMediaModule::EtCoSignGetLocalPinInfo(usercode.c_str(), &maxRetryCount, &remainRetryCount, &defaultPin);
    IF_FAIL_GOTO_END(0x00!=u32Result);

    u32Result = EtCryptMediaModule::EtCoSignChangeUserPin(usercode.c_str(), pin.c_str(), newpin.c_str());
    IF_FAIL_GOTO_END(0x00!=u32Result);

    u32Result = EtCryptMediaModule::EtCoSignGetLocalPinInfo(usercode.c_str(), &maxRetryCount, &remainRetryCount, &defaultPin);
    IF_FAIL_GOTO_END(0x00!=u32Result);

    u32Result = EtCryptMediaModule::EtCoSignVerifyPin(usercode.c_str(), newpin.c_str());
    IF_FAIL_GOTO_END(0x00!=u32Result);

    u32Result = EtCryptMediaModule::EtCoSignChangeUserPin(usercode.c_str(), newpin.c_str(), pin.c_str());
    IF_FAIL_GOTO_END(0x00!=u32Result);

    u32Result = EtCryptMediaModule::EtCoSignVerifyPin(usercode.c_str(), pin.c_str());
    IF_FAIL_GOTO_END(0x00!=u32Result);
    // end
END:
    return u32Result;
}

void printHex(const char*  hint, u8* pu8Data, u32 u32DataLen)
{
    u32 i = 0;
    printf("%s ", hint);
    for (i = 0; i < u32DataLen; i++)
    {
        printf("%02X", pu8Data[i]);
    }
    printf("\n");
}
#if 0
u32 PrintPubKey(eetrust_cryptmedia::EnCertType enCertType)
{
    u32   u32Result = 0;
    BlkAsymKey blkAsymKey = {0};

    u32Result = eetrust_cryptmedia::EtCryptMediaExportPubKey(enCertType, &blkAsymKey);
    IF_FAIL_GOTO_END(0x00!=u32Result);

    printHex("pubkeyx:", blkAsymKey.m_unionAsymKey.m_blkSm2Key.m_pu1PubKeyX, 32);
    printHex("pubkeyy:", blkAsymKey.m_unionAsymKey.m_blkSm2Key.m_pu1PubKeyY, 32);

END:
    return u32Result;
}
#endif

u32 EtCosignFullTest()
{
    u32   u32Result;
    std::string ticket = "5564257944";
    std::string usercode = "test2";
    std::string pin = "123456";
    std::string newpin = "654321";
    BlkEtCert blkSignCert = {0};
    BlkEtCert blkEncCert = {0};
    u32 u32UnblockState = 1; 
    ETBOOL bIsSuccess = FALSE;
    char *pszCosignInfo = nullptr;



    // start test cosign terminalinit  userinit(appreg, certapply)  login  readcert
    u32Result = EtCryptMediaModule::EtCoSignSetUrl(ip.c_str(), port);
    IF_FAIL_GOTO_END(0x00!=u32Result);

    u32Result = EtCryptMediaModule::EtCoSignTerminalInit();
    IF_FAIL_GOTO_END(0x00!=u32Result);

    u32Result = EtCryptMediaModule::EtCosignSetTicket(ticket.c_str());
    IF_FAIL_GOTO_END(0x00!=u32Result);

#if 0
    u32Result = TestPin(usercode, pin, newpin);
    IF_FAIL_GOTO_END(0x00!=u32Result);
#endif

    // 证书申请还没有成功, 证书请求之前的都完成了。
    u32Result = EtCryptMediaModule::EtCoSignUserInit(usercode.c_str(), pin.c_str());
    IF_FAIL_GOTO_END(0x00!=u32Result);

    u32Result = EtCryptMediaModule::EtCosignGetInfo(usercode.c_str(), &pszCosignInfo);
    IF_FAIL_GOTO_END(0x00!=u32Result);
    // PrintPubKey(eetrust_cryptmedia::EnCertType_UserSign);

    u32Result = EtCryptMediaModule::EtCoSignLoginVerify(usercode.c_str(), pin.c_str());
    IF_FAIL_GOTO_END(0x00!=u32Result);

    u32Result = EtCryptMediaModule::EtCoSignGetWebPinStats(usercode.c_str(), &u32UnblockState);
    IF_FAIL_GOTO_END(0x00!=u32Result);

    if(u32UnblockState == (u32)UnBlockState::Authorized) {
        std::cout << "EtCoSignGetWebPinStats unblocked" << std::endl;
        u32Result = EtCryptMediaModule::EtCoSignDoUnblockPin(usercode.c_str(), newpin.c_str(), &bIsSuccess);
        IF_FAIL_GOTO_END(0x00!=u32Result);
    } 
    // end
END:
    EtCryptMediaModule::EtCosignMemFree((void**)&pszCosignInfo);
    EtCryptMediaModule::EtCertRelease(&blkSignCert);
    EtCryptMediaModule::EtCertRelease(&blkEncCert);
    return u32Result;
}


int main(int argc, const char** argv) {
    u32   u32Result;

    std::string terminalPath = R"(D:\AZHcosigntest)";
    std::string appCode = "Bizsecuredoc";
    std::string serverSystemName = "boc";
    std::string customerCode = "boc";

    EtCryptMediaModule::LoadLib();
    u32Result = EtCryptMediaModule::EtCoSignLibInit("15527283877stss", terminalPath.c_str(), appCode.c_str(),serverSystemName.c_str(), customerCode.c_str());
    IF_FAIL_GOTO_END(0x00 != u32Result);
#if 1
    u32Result = EtCosignFullTest();
    IF_FAIL_GOTO_END(0x00 != u32Result);
#else
    // u32Result = TestGetCosignState();
    // IF_FAIL_GOTO_END(0x00 != u32Result);
#endif

END:
    EtCryptMediaModule::EtCoSignLibRelease();
    EtCryptMediaModule::UnloadLib();

    return u32Result;
}