#include "helloworld.pb.h"
#include <grpcpp/completion_queue.h>
#include <iostream>
#include <memory>
#include <string>

#include <absl/log/check.h>

#include <grpc/support/log.h>
#include <grpcpp/grpcpp.h>

#include <helloworld.grpc.pb.h>

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using HelloWorld::Greeter;
using HelloWorld::HelloReply;
using HelloWorld::HelloRequest;

class GreeterClient {
public:
  explicit GreeterClient(std::shared_ptr<Channel> channel)
      : stub_(Greeter::NewStub(channel)) {}

  std::string SayHello(const std::string &user) {
    HelloRequest request;
    request.set_name(user);
    HelloRequest::TransferBytesReq *bytesReq = request.mutable_transferbytesreq();
    bytesReq->set_data("client", strlen("client") + 1);

    HelloReply reply;

    ClientContext context;

    CompletionQueue cq;

    Status status;

    std::unique_ptr<ClientAsyncResponseReader<HelloReply>> rpc(
        stub_->AsyncSayHello(&context, request, &cq));

    rpc->Finish(&reply, &status, (void *)1);

    void *got_tag;
    bool ok = false;

    CHECK(cq.Next(&got_tag, &ok));

    CHECK_EQ(got_tag, (void *)1);

    CHECK(ok);

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

int main(int argc, char **argv) {
  std::string target_str = "localhost:50051";
  GreeterClient greeter(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

  std::string user("World");
  std::string reply = greeter.SayHello(user);
  std::cout << "Greeter received: " << reply << std::endl;
}
