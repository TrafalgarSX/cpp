#include "convert.h"
#include "xlsreader_module.h"
#include <iostream>
#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
  // if(argc < 2) {
  // 	printf("Usage: test_cpp <file.xls>\n");
  // 	exit(1);
  // }

  // 初始化控制台日志记录器
#if 1
  spdlog::set_level(spdlog::level::info);             // 设置日志级别
  spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v"); // 设置日志格式
  spdlog::info("Logger initialized");
#endif

  XlsReaderModule::LoadLib();

  // std::string strGbk = std::string(argv[1]);

  // std::string strUtf8 = gbk_to_utf8(strGbk);
  // std::cout << "gbk to utf8: " << strUtf8 << std::endl;

  // std::string s = std::string(argv[1]);
  std::string s = R"(D:\迅雷下载\联系人.xls)";
  const char *ss = "D:\\迅雷下载\\联系人.xls";
  std::string contacts_json;
  std::cout << s << std::endl;
  //   contact_xls2json(ss, contacts_json);
  contact_xlsx2json("D:\\迅雷下载\\联系人.xlsx", contacts_json);
#if 1
  contacts_json = R"(
        [
  {
    "createTime": "2024-08-26 15:25:36",
    "emailAddr": "cc@eetrust.com",
    "name": "cc",
    "phone": "",
    "remark": "cc"
  }
        ]
    )";

  contact_json2xlsx(contacts_json, "D:\\迅雷下载\\联系人_out.xlsx");
#endif

  XlsReaderModule::UnloadLib();
  std::cout << "exit" << std::endl;

  return 0;
}