#include "convert.h"

#include <string>
#include <vector>

#include <QtCore/QVariant>
#include <QtCore/QtCore>
#include <QString>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <xlsxdocument.h>


#include "xlsreader_module.h"

std::array<const char *, symbol_count> XlsReaderModule::exports = {
    "xls_getVersion",
    "xls_getError",
    "xls",
    "xls_parseWorkBook",
    "xls_parseWorkSheet",
    "xls_open_file",
    "xls_open_buffer",
    "xls_close_WB",
    "xls_open",
    "xls_getWorkSheet",
    "xls_close_WS",
    "xls_summaryInfo",
    "xls_close_summaryInfo",
    "xls_row",
    "xls_cell",
    "libiconv_open",
    "libiconv",
    "libiconv_close"};

static std::vector<std::vector<std::string>> key = {
    {"name", "emailAddr", "remark"},
    {"name", "emailAddr", "phone", "remark", "createTime"}};

std::string code_convert(const char *from_charset, const char *to_charset,
                         const std::string &in_str) {
  iconv_t cd;
  char *inbuf = const_cast<char *>(in_str.c_str());
  size_t inlen = in_str.size();
  std::unique_ptr<char[]> outbuf_ptr(new char[inlen * 4]{0});
  char *outbuf_temp = outbuf_ptr.get();
  char *outbuf = outbuf_temp;

  size_t outlen = inlen * 4;
  cd = XlsReaderModule::iconv_open(to_charset, from_charset);
  if (cd == (iconv_t)-1) {
    throw std::runtime_error("iconv_open failed");
  }
  if (XlsReaderModule::iconv(cd, &inbuf, &inlen, &outbuf_temp, &outlen) == -1) {
    throw std::runtime_error("iconv failed");
  }
  XlsReaderModule::iconv_close(cd);

  std::string out_str = outbuf;
  return out_str;
}

std::string utf8_to_gbk(const std::string &utf8_str) {
  return code_convert("utf-8", "gbk", utf8_str);
}

std::string gbk_to_utf8(const std::string &gbk_str) {
  return code_convert("gbk", "utf-8", gbk_str);
}

std::string gbk_to_utf16le(const std::string &gbk_str) {
  return code_convert("gbk", "UCS-2LE", gbk_str);
}

std::string utf16le_to_gbk(const std::string &utf16le_str) {
  return code_convert("UCS-2LE", "gbk", utf16le_str);
}

bool contact_xls2json(const std::string &xls_file, std::string &contacts_json) {
  try {

    nlohmann::json contacts;
    const char *xls_file_path = nullptr;
    std::string file_path;
#ifdef _WIN32
    file_path = utf8_to_gbk(xls_file);
    xls_file_path = file_path.c_str();
#else
    xls_file_path = xls_file.c_str();
#endif

    SPDLOG_INFO("test file : {}", file_path);

    xls::xls_error_t error = xls::LIBXLS_OK;

    std::unique_ptr<xls::xlsWorkBook, void (*)(xls::xlsWorkBook *)> work_book(
        XlsReaderModule::xls_open_file(xls_file_path, "UTF-8", &error),
        XlsReaderModule::xls_close_WB);

    if (work_book == nullptr) {
      SPDLOG_ERROR("Error reading file: {}\n",
                   XlsReaderModule::xls_getError(error));
      return false;
    }
    // sheets
    std::unique_ptr<xls::xlsWorkSheet, void (*)(xls::xlsWorkSheet *)>
        work_sheet(XlsReaderModule::xls_getWorkSheet(work_book.get(), 0),
                   XlsReaderModule::xls_close_WS);

    error = XlsReaderModule::xls_parseWorkSheet(work_sheet.get());
    if (error != xls::LIBXLS_OK) {
      SPDLOG_INFO("Error reading file: {}\n",
                  XlsReaderModule::xls_getError(error));
      return false;
    }

    int key_id = 0;
    if ((work_sheet->rows.lastcol + 1) == key[0].size()) {
      key_id = 0;
    } else {
      key_id = 1;
    }

    for (int j = 1; j <= work_sheet->rows.lastrow; j++) { // rows
      xls::xlsRow *row = XlsReaderModule::xls_row(work_sheet.get(), j);
      nlohmann::json contact;
      for (int k = 0; k <= work_sheet->rows.lastcol; k++) { // columns
        xls::xlsCell *cell = &row->cells.cell[k];
        if (cell->id == XLS_RECORD_NUMBER) {
          // use cell->d, a double-precision number
          contact[key[key_id][k]] = std::to_string((long long)(cell->d));
        } else if (cell->str != NULL) {
          // cell->str contains a string value
          contact[key[key_id][k]] = cell->str;
        } else {
          // wrong xls format
          SPDLOG_ERROR("wrong xls format");
          return false;
        }
      }
      contacts.push_back(contact);
    }

    contacts_json = contacts.dump(2);
    SPDLOG_INFO("contacts: {}", contacts_json);
    return true;

  } catch (const std::exception &e) {
    SPDLOG_ERROR("{}", e.what());
    return false;
  }
}

bool contact_xlsx2json(const std::string &xlsx_file,
                       std::string &contacts_json) {

  QXlsx::Document xlsx(xlsx_file.c_str());

  if (!xlsx.load()) {
    SPDLOG_ERROR("load xlsx file failed");
    return false;
  }

  nlohmann::json contacts;
  int key_id = 0;
  if (xlsx.dimension().columnCount() == key[0].size()) {
    key_id = 0;
  } else {
    key_id = 1;
  }

  for (int row = 2; row <= xlsx.dimension().rowCount(); row++) { // rows
    nlohmann::json contact;
    for (int col = 1; col <= xlsx.dimension().columnCount(); col++) { // columns
      QXlsx::Cell *cell = xlsx.cellAt(row, col);
      if (cell->cellType() == QXlsx::Cell::NumberType) {
        contact[key[key_id][col - 1]] =
            std::to_string(cell->value().toLongLong());
      } else if (cell->cellType() == QXlsx::Cell::SharedStringType) {
        contact[key[key_id][col - 1]] = cell->value().toString().toStdString();
      } else {
        // wrong xls format
        SPDLOG_ERROR("row {}, col {}, type: {}", row, col,
                     static_cast<int>(cell->cellType()));
        SPDLOG_ERROR("wrong xls format");
        return false;
      }
    }
    contacts.push_back(contact);
  }

  contacts_json = contacts.dump(2);
  SPDLOG_INFO("contacts: {}", contacts_json);

  return true;
}

bool contact_json2xlsx(const std::string &contacts_json,
                       const std::string &xlsx_file) {

  QXlsx::Document xlsx;

  QString xlsx_file_path;
//   xlsx_file_path = QString.fromLocal8Bit(xlsx_file.c_str());
  xlsx_file_path = QString::fromStdString(xlsx_file);
  SPDLOG_INFO("save file : {}", xlsx_file);

  nlohmann::json contacts = nlohmann::json::parse(contacts_json);
  // 导出5列
  xlsx.write("A1", "联系人名称");
  xlsx.write("B1", "邮箱地址");
  xlsx.write("C1", "联系电话");
  xlsx.write("D1", "备注");
  xlsx.write("E1", "创建时间");

  for (int i = 0; i < contacts.size(); i++) {
    xlsx.write(i + 2, 1, QVariant(contacts[i]["name"].get<std::string>().c_str()));
    xlsx.write(i + 2, 2, QVariant(contacts[i]["emailAddr"].get<std::string>().c_str()));
    xlsx.write(i + 2, 3, QVariant(contacts[i]["phone"].get<std::string>().c_str()));
    xlsx.write(i + 2, 4, QVariant(contacts[i]["remark"].get<std::string>().c_str()));
    xlsx.write(i + 2, 5, QVariant(contacts[i]["createTime"].get<std::string>().c_str()));
  }

  if(xlsx.saveAs(xlsx_file_path)) {
    SPDLOG_INFO("save xlsx file success");
    return true;
  } else {
    SPDLOG_ERROR("save xlsx file failed");
    return false;
  }
}