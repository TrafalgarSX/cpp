#include <iostream>
#include <string>
#include <ClientApp.h>
#include <string_view>
#include <thread>

void clientThread(){
  // uwebsockets client demo c++17

  std::cout << "start clientThread" << std::endl;
  // An echo client that sends a message every second
  uWS::WebSocketClientBehavior behavior;

  behavior.open = []() {
    std::cout << "Connected to server" << std::endl;
    // ws->send("Hello from uWebSockets client!");
  };
  // behavior.message = [](auto *ws, std::string_view message, uWS::OpCode opCode) {
  behavior.message = [](){
    // std::cout << "Message from server: " << message << std::endl;
    std::cout << "Message from server: " << std::endl;
  };
  behavior.close = []() {
    std::cout << "Disconnected from server with code " << std::endl;
  };

  uWS::ClientApp app(std::move(behavior));

  app.connect("ws://127.0.0.1:10000/verifySign", nullptr);
  app.run();

}

void testThread(){
  std::cout << "start testThread" << std::endl;
  for (int i = 0; i < 10; ++i) {
    std::cout << "testThread i=["<<i<<"]" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  std::cout << "end testThread" << std::endl;
}

extern void initApp();


int main(void){

#if 1
  std::thread server(initApp);
  // sleep
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << "start client" << std::endl;
  std::thread client(testThread);
  server.join();
  client.join();
#endif

  return 0;
}
