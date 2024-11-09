#ifndef ETCRYPTCOSIGNVERIFY_H
#define ETCRYPTCOSIGNVERIFY_H

#include <array>
#include <replex.h>
#include <iostream>


struct BlkEtCert{
  uint32_t m_u32Version;
  uint8_t* m_pu8SerialNum;
  uint32_t m_u32SerialNumLen;
  char* m_pszIssuerDn;
  char* m_pszBefore;
  char* m_pszAfter;
  char* m_pszSubjectDn;
  uint8_t* m_pu8PubKey;
  uint32_t m_u32PubKeyLen;
  uint8_t* m_pu8Signature;
  uint32_t m_u32SignatureLen;
  uint32_t m_u32AsymAlgId;
  uint32_t m_u32HashAlgId;
};

enum class CoSignState{
    TerminalInit = 0,
    TerminalNotInit = 1,
    UserInit = 2,
    UserNotInit = 3,
};

enum class UnBlockState{
    Authorized = 0,     // 已授权
    UnAuthorized = 1,        // 未授权
};

typedef long	ETBOOL;
const int symbol_count = 22;

class EtCryptMediaModule : public ReplexModule<EtCryptMediaModule, symbol_count> {
  friend class ReplexModule<EtCryptMediaModule, symbol_count>;

public:
  virtual ~EtCryptMediaModule() {
    std::cout << "EtCryptMediaModule::~EtCryptMediaModule()" << std::endl;
  }


  static uint32_t EtCoSignLibInit(const char* pszUniqueCode, const char *pszTerminalPath, const char *pszAppCode, const char *pszServerSystemName, const char* pszCustomerCode) {
    return GetInstance().Execute<uint32_t>("EtCoSignLibInit", pszUniqueCode, pszTerminalPath, pszAppCode, pszServerSystemName, pszCustomerCode);
  }

  static uint32_t EtCoSignLibRelease() {
    return GetInstance().Execute<uint32_t>("EtCoSignLibRelease");
  }

  static uint32_t EtCosignSetTicket(const char* pszAppticket) {
    return GetInstance().Execute<uint32_t>("EtCosignSetTicket", pszAppticket);
  }

  static uint32_t EtCoSignSetUrl(const char* pszIp, uint32_t u32Port) {
    return GetInstance().Execute<uint32_t>("EtCoSignSetUrl", pszIp, u32Port);
  }

  static uint32_t EtGetCosignState(const char *pszTerminalPath, const char* pszUserCode, uint32_t *coSignState) {
    return GetInstance().Execute<uint32_t>("EtGetCosignState", pszTerminalPath, pszUserCode, coSignState);
  }

  static uint32_t EtCoSignTerminalInit() {
    return GetInstance().Execute<uint32_t>("EtCoSignTerminalInit");
  }

  static uint32_t EtCoSignUserInit(const char *pszUserCode, const char *pszPin) {
    return GetInstance().Execute<uint32_t>("EtCoSignUserInit", pszUserCode, pszPin);
  }

  static uint32_t EtCoSignLogin(const char* pszUserCode, const char *pszPin, char **ppszAuthTicket) {
    return GetInstance().Execute<uint32_t>("EtCoSignLogin", pszUserCode, pszPin, ppszAuthTicket);
  }

  static uint32_t EtCoSignLoginVerify(const char* pszUserCode, const char *pszPin) {
    return GetInstance().Execute<uint32_t>("EtCoSignLoginVerify", pszUserCode, pszPin);
  }

  static uint32_t EtCoSignGetUserCert(const char *pszUserCode, BlkEtCert *blkSignCert, BlkEtCert *blkEncCert) {
    return GetInstance().Execute<uint32_t>("EtCoSignGetUserCert", pszUserCode, blkSignCert, blkEncCert);
  }

  static void EtCertRelease(BlkEtCert* pblkEtCert) {
    GetInstance().Execute<void>("EtCertRelease", pblkEtCert);
  }

  static uint32_t EtCoSignVerifyPin(const char *pszUserCode, const char *pszPin) {
    return GetInstance().Execute<uint32_t>("EtCoSignVerifyPin", pszUserCode, pszPin);
  }

  static uint32_t EtCoSignChangeUserPin(const char *pszUserCode, const char *pszOldPin, const char *pszNewPin) {
    return GetInstance().Execute<uint32_t>("EtCoSignChangeUserPin", pszUserCode, pszOldPin, pszNewPin);
  }

  static uint32_t EtCoSignGetLocalPinInfo(const char *pszUserCode, uint32_t *pu32MaxRetryCount, uint32_t *pu32RemainRetryCount, ETBOOL* pbDefaultPin) {
    return GetInstance().Execute<uint32_t>("EtCoSignGetLocalPinInfo", pszUserCode, pu32MaxRetryCount, pu32RemainRetryCount, pbDefaultPin);
  }

  static uint32_t EtCoSignRequestUnblockPin(const char *pszUserCode) {
    return GetInstance().Execute<uint32_t>("EtCoSignRequestUnblockPin", pszUserCode);
  }

  static uint32_t EtCoSignGetWebPinStats(const char *pszUserCode, uint32_t *unblockState) {
    return GetInstance().Execute<uint32_t>("EtCoSignGetWebPinStats", pszUserCode, unblockState);
  }

  static uint32_t EtCoSignDoUnblockPin(const char *pszUserCode, const char *pszNewPin, ETBOOL *pbIsSuccess) {
    return GetInstance().Execute<uint32_t>("EtCoSignDoUnblockPin", pszUserCode, pszNewPin, pbIsSuccess);
  }

  static uint32_t EtCoSignSetAccount(const char *pszUserCode) {
    return GetInstance().Execute<uint32_t>("EtCoSignSetAccount", pszUserCode);
  }

  static uint32_t EtCosignGetInfo(const char *pszUserCode, char** ppszCosignInfo) {
    return GetInstance().Execute<uint32_t>("EtCosignGetInfo", pszUserCode, ppszCosignInfo);
  }

  static uint32_t EtCosignMemFree(void** ppvInfo) {
    return GetInstance().Execute<uint32_t>("EtCosignMemFree", ppvInfo);
  }

  static uint32_t EtCosignGetErrorMsg(uint32_t u32ErrorCode, char* pszErrorMsg) {
    return GetInstance().Execute<uint32_t>("EtCosignGetErrorMsg", u32ErrorCode, pszErrorMsg);
  }

  static uint32_t EtCosignGetErrorMsgEx(uint32_t u32ErrorCode, char** ppszErrorMsg) {
    return GetInstance().Execute<uint32_t>("EtCosignGetErrorMsgEx", u32ErrorCode, ppszErrorMsg);
  }

  //   static int GetBar() { return *GetInstance().GetVar<int>("bar"); }
protected:
  const char *GetPathImpl() const {
#ifndef NDEBUG // Debug mode
    return R"(D:\workspace\cpp\zhcosign\EtCosignVerify.dll)";
#else
    return R"(D:\workspace\cpp\zhcosign\EtCosignVerify.dll)";
#endif
  }

  std::array<const char *, symbol_count> &GetSymbolNamesImpl() const {
    return exports;
  }

private:
  static std::array<const char *, symbol_count> exports;
};

#endif // ETCRYPTCOSIGNVERIFY_H
