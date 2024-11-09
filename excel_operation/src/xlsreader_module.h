#ifndef XLSREADER_H
#define XLSREADER_H

#include <array>
#include <replex.h>
#include <xls.h>


#undef iconv_t
#define iconv_t libiconv_t
typedef void *iconv_t;

const int symbol_count = 18;

class XlsReaderModule : public ReplexModule<XlsReaderModule, symbol_count> {
  friend class ReplexModule<XlsReaderModule, symbol_count>;

public:
  virtual ~XlsReaderModule() {
    std::cout << "XlsReaderModule::~XlsReaderModule()" << std::endl;
  }
  static const char *xls_getVersion() {
    return GetInstance().Execute<const char *>("xls_getVersion");
  }
  static const char *xls_getError(xls::xls_error_t code) {
    return GetInstance().Execute<const char *>("xls_getError", code);
  }

  static int xls(int debug) { return GetInstance().Execute<int>("xls", debug); }
  static void xls_set_formula_hander(xls::xls_formula_handler handler) {
    return GetInstance().Execute<void>("xls_set_formula_hander", handler);
  }
  static xls::xls_error_t xls_parseWorkBook(xls::xlsWorkBook *pWB) {
    return GetInstance().Execute<xls::xls_error_t>("xls_parseWorkBook", pWB);
  }
  static xls::xls_error_t xls_parseWorkSheet(xls::xlsWorkSheet *pWS) {
    return GetInstance().Execute<xls::xls_error_t>("xls_parseWorkSheet", pWS);
  }

  // outError 外部分配内存
  static xls::xlsWorkBook *xls_open_file(const char *file, const char *charset,
                                         xls::xls_error_t *outError) {
    return GetInstance().Execute<xls::xlsWorkBook *>("xls_open_file", file,
                                                     charset, outError);
  }
  static xls::xlsWorkBook *xls_open_buffer(const unsigned char *data,
                                           size_t data_len, const char *charset,
                                           xls::xls_error_t *outError) {
    return GetInstance().Execute<xls::xlsWorkBook *>(
        "xls_open_buffer", data, data_len, charset, outError);
  }
  static void xls_close_WB(xls::xlsWorkBook *pWB) {
    return GetInstance().Execute<void>("xls_close_WB", pWB);
  }

  static xls::xlsWorkBook *xls_open(const char *file, const char *charset) {
    return GetInstance().Execute<xls::xlsWorkBook *>("xls_open", file, charset);
  }
  static void xls_close(xls::xlsWorkBook *pWB) {
    return GetInstance().Execute<void>("xls_close_WB", pWB);
  }

  static xls::xlsWorkSheet *xls_getWorkSheet(xls::xlsWorkBook *pWB, int num) {
    return GetInstance().Execute<xls::xlsWorkSheet *>("xls_getWorkSheet", pWB,
                                                      num);
  }
  static void xls_close_WS(xls::xlsWorkSheet *pWS) {
    return GetInstance().Execute<void>("xls_close_WS", pWS);
  }

  static xls::xlsSummaryInfo *xls_summaryInfo(xls::xlsWorkBook *pWB) {
    return GetInstance().Execute<xls::xlsSummaryInfo *>("xls_summaryInfo", pWB);
  }
  static void xls_close_summaryInfo(xls::xlsSummaryInfo *pSI) {
    return GetInstance().Execute<void>("xls_close_summaryInfo", pSI);
  }

  static xls::xlsRow *xls_row(xls::xlsWorkSheet *pWS, WORD cellRow) {
    return GetInstance().Execute<xls::xlsRow *>("xls_row", pWS, cellRow);
  }
  static xls::xlsCell *xls_cell(xls::xlsWorkSheet *pWS, WORD cellRow,
                                WORD cellCol) {
    return GetInstance().Execute<xls::xlsCell *>("xls_cell", pWS, cellRow,
                                                 cellCol);
  }

  // extra iconv functions
  static iconv_t iconv_open(const char *tocode, const char *fromcode) {
    return GetInstance().Execute<iconv_t>("libiconv_open", tocode, fromcode);
  }
  static int iconv_close(iconv_t cd) {
    return GetInstance().Execute<int>("libiconv_close", cd);
  }
  static size_t iconv(iconv_t cd, char **inbuf, size_t *inbytesleft,
                      char **outbuf, size_t *outbytesleft) {
    return GetInstance().Execute<size_t>("libiconv", cd, inbuf, inbytesleft,
                                         outbuf, outbytesleft);
  }

  //   static int GetBar() { return *GetInstance().GetVar<int>("bar"); }
protected:
  const char *GetPathImpl() const {
#ifndef NDEBUG // Debug mode
    return R"(E:/work/libxls/VS2022-x86/Debug/xlsreader.dll)";
#else
    return R"(D:\workspace\cpp\excel_operation\lib\xlsreader.dll)";
#endif
  }

  std::array<const char *, symbol_count> &GetSymbolNamesImpl() const {
    return exports;
  }

private:
  static std::array<const char *, symbol_count> exports;
};

#endif // XLSREADER_H