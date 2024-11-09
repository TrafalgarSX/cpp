#include <grpcpp/grpcpp.h>
#include <helloworld.grpc.pb.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <string>


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using HelloWorld::Greeter;
using HelloWorld::HelloReply;
using HelloWorld::HelloRequest;

class GreeterClient {
 public:
  GreeterClient(std::shared_ptr<Channel> channel)
      : stub_(Greeter::NewStub(channel)) {}

  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::string SayHello(const std::string& user) {
    // Data we are sending to the server.
    HelloRequest request;
    request.set_name(user);
    HelloRequest::TransferBytesReq* bytesReq =
        request.mutable_transferbytesreq();
    bytesReq->set_data("client", strlen("client") + 1);

    // Container for the data we expect from the server.
    HelloReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->SayHello(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      std::cout << reply.transferbytesresp().data() << std::endl;
      return reply.message();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

 private:
  std::unique_ptr<Greeter::Stub> stub_;
};

int main(int argc, char** argv) {
  std::string target_str = "localhost:50051";
  // We indicate that the channel isn't authenticated (use of
  // InsecureChannelCredentials()).
  GreeterClient greeter(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

  // use chrono to calc run time

  auto start = std::chrono::high_resolution_clock::now();

  for (size_t i = 0; i < 100; i++) {
    std::string user("world");
    std::string reply = greeter.SayHello(user);
    std::cout << "Greeter received: " << reply << std::endl;
  }

  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> elapsed = end - start;

  std::cout << "Elapsed time: " << elapsed.count() << " s\n";

  return 0;
}