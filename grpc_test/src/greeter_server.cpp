#include <iostream>
#include <memory>
#include <string>
#include <random>
#include <fmt/core.h>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>


#include <helloworld.grpc.pb.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using HelloWorld::Greeter;
using HelloWorld::HelloReply;
using HelloWorld::HelloRequest;

// generate random number between 0 and 10
int generate_random_number() {
  std::random_device rd;

  std::mt19937 gen(rd());

  std::uniform_int_distribution<> dis(0, 10);

  return dis(gen);
}

// Logic and data behind the server's behavior.
class GreeterServiceImpl final : public Greeter::Service {
  Status SayHello(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply) override {
    std::string prefix("Hello ");
    reply->set_message(prefix + request->name());
    HelloReply::TransferBytesResp *bytesResp = reply->mutable_transferbytesresp();
    bytesResp->set_data("server", strlen("server") + 1);

    std::cout << request->transferbytesreq().data() << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(generate_random_number()));
    
    return Status::OK;
  }
};

void RunServer(uint16_t port) {
  std::string server_address = fmt::format("0.0.0.0:{}", port);
  GreeterServiceImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer(50051); // "50051
  return 0;
}