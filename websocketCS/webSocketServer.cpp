#include <App.h>
#include <iostream>
#include <string>
#include <string_view>
#include "jsonUtil.h"
#include <windows.h>

#define DLL_EXPORTS

#if defined(DLL_EXPORTS)
#define MYFUNC_API __declspec(dllexport)
#else
#define MYFUNC_API __declspec(dllimport)
#endif


MYFUNC_API void initApp();

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved
                      )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
    {

        break;
    }
    default:
        break;
    }
    return TRUE;
}

/* ws->getUserData returns one of these */
struct PerSocketData {
    /* Fill with user data */
};

void initBehavior(uWS::App::WebSocketBehavior<PerSocketData> &behavior){
  behavior.compression = uWS::CompressOptions(uWS::DEDICATED_COMPRESSOR_4KB | uWS::DEDICATED_DECOMPRESSOR);
  behavior.maxPayloadLength = 100 * 1024 * 1024; // 16MB max payload
  behavior.idleTimeout = 16; // 16s idle timeout
  behavior.maxBackpressure = 100 * 1024 * 1024; // 1MB max backpressure
  behavior.closeOnBackpressureLimit = false;
  behavior.resetIdleTimeoutOnSend = false;
  behavior.sendPingsAutomatically = true; // true by default
  behavior.upgrade = nullptr;
  behavior.open = [](auto */*ws*/) {
    /* Open event here, you may access ws->getUserData() which points to a PerSocketData struct */
    std::cout << "open" << std::endl;
  };
  behavior.drain = [](auto *ws) {
    /* Check ws->getBufferedAmount() here */
  };
  behavior.ping = [](auto *ws, std::string_view message) {
    /* Not implemented yet */
		std::cout << "ping" << std::endl;
    ws->send(message, uWS::OpCode::PING, false);
  };
  behavior.pong = [](auto *ws, std::string_view message) {
    /* Not implemented yet */
		std::cout << "pong" << std::endl;
    ws->send(message, uWS::OpCode::PONG, false);
  };
  behavior.close = [](auto *ws, int code, std::string_view message) {
    /* You may access ws->getUserData() here */
		std::cout << "close::code=["<<code<<"] message=["<< message << "]"<<std::endl;
  };
}

// DLL EXPORT 
MYFUNC_API void initApp()
{
  // uwebsockets server demo c++17

  // An echo server that echos back every message it receives

  uWS::SocketContextOptions options = {
    "misc/cert.pem", "misc/key.pem", "1234",
  };

  // Create a server app
  // uWS::App app(options);
  uWS::App::WebSocketBehavior<PerSocketData> signBehavior, verifySignBehavior;

  initBehavior(signBehavior);
  initBehavior(verifySignBehavior);

  signBehavior.message = [](auto *ws, std::string_view message, uWS::OpCode opCode) {
    /* You may access ws->getUserData() here */
    std::cout << "Server: Message received, path is /sign, message is: " << message << std::endl;
    std::string messageStr(message);
    std::string msgResp = "server";
    ws->send(msgResp, opCode, msgResp.length() < 16 * 1024);
  };

  verifySignBehavior.message = [](auto *ws, std::string_view message, uWS::OpCode opCode) {
    /* You may access ws->getUserData() here */
    std::cout << "Server: Message received, path is /verifySign, message is: " << message << std::endl;
    std::string msgResp = "true";
    ws->send(msgResp, opCode, msgResp.length() < 16 * 1024);
  };

  uWS::App(options).ws<PerSocketData>("/sign", (uWS::App::WebSocketBehavior<PerSocketData>&&)signBehavior)
                   .ws<PerSocketData>("/verifySign", (uWS::App::WebSocketBehavior<PerSocketData>&&)verifySignBehavior)
    .listen(10000, [](auto *listen_socket) {
   // uWS::App(options).ws<PerSocketData>("/*", std::move(signBehavior)).listen(9001, [](auto *listen_socket) {
    if (listen_socket) {
      std::cout << "Listening on port " << 10000 << std::endl;
    }
  }).run();

  std::cout << "end initApp" << std::endl;
}

#ifndef DLL_COMPILE
// #if 1
int main(void) {
  initApp();


  return 0;
}
#endif


