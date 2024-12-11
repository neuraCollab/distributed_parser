#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "coordinator.h"

using grpc::Server;
using grpc::ServerBuilder;

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    CoordinatorServiceImpl service;

    // Вызов тестового метода
    service.testSubmitTask();

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main(int argc, char** argv) {
    RunServer();
    return 0;
}
