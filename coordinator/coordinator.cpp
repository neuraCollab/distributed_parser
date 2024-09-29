#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include <grpcpp/grpcpp.h>
#include "task.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using parser::Coordinator;
using parser::TaskRequest;
using parser::TaskResponse;
using parser::TaskResult;
using parser::ResultAck;

class CoordinatorServiceImpl final : public Coordinator::Service {
public:
    CoordinatorServiceImpl() {
        // Добавление заданий в очередь
        tasks = {"https://example.com", "https://example2.com", "https://example3.com"};
    }

    Status GetTask(ServerContext* context, const TaskRequest* request, TaskResponse* response) override {
        std::lock_guard<std::mutex> lock(mu_);
        if (tasks.empty()) {
            response->set_has_task(false);
        } else {
            response->set_url(tasks.back());
            response->set_has_task(true);
            tasks.pop_back();
        }
        return Status::OK;
    }

    Status ReportResult(ServerContext* context, const TaskResult* result, ResultAck* ack) override {
        std::lock_guard<std::mutex> lock(mu_);
        std::cout << "Worker " << result->worker_id() << " completed task for URL " << result->url() 
                  << " with result: " << result->result() << std::endl;
        ack->set_success(true);
        return Status::OK;
    }

private:
    std::vector<std::string> tasks;
    std::mutex mu_;
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    CoordinatorServiceImpl service;

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
