#include "coordinator.h"
#include <iostream>
#include <mutex>

// Конструктор класса CoordinatorServiceImpl
CoordinatorServiceImpl::CoordinatorServiceImpl() {
    // Изначально список заданий пуст, задания будут добавляться через SubmitTask
}

// Метод для получения задания воркером
grpc::Status CoordinatorServiceImpl::GetTask(grpc::ServerContext* context, const parser::TaskRequest* request, parser::TaskResponse* response) {
    std::lock_guard<std::mutex> lock(mu_);
    if (tasks.empty()) {
        response->set_has_task(false);
    } else {
        response->set_url(tasks.back());
        response->set_has_task(true);
        tasks.pop_back();
    }
    return grpc::Status::OK;
}

// Метод для получения результатов от воркеров
grpc::Status CoordinatorServiceImpl::ReportResult(grpc::ServerContext* context, const parser::TaskResult* result, parser::ResultAck* ack) {
    std::lock_guard<std::mutex> lock(mu_);
    std::cout << "Worker " << result->worker_id() << " completed task for URL " << result->url() 
              << " with result: " << result->result() << std::endl;
    ack->set_success(true);
    return grpc::Status::OK;
}

// Метод для добавления нового задания в очередь (получение URL-адреса от клиента)
grpc::Status CoordinatorServiceImpl::SubmitTask(grpc::ServerContext* context, const parser::SubmitTaskRequest* request, parser::SubmitTaskResponse* response) {
    std::lock_guard<std::mutex> lock(mu_);
    tasks.push_back(request->url());
    std::cout << "Received new task: " << request->url() << std::endl;
    response->set_success(true);
    return grpc::Status::OK;
}
