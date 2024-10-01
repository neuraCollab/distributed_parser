#include "worker.h"
#include "page_fetcher.h"
#include "html_parser.h"
#include <cassandra.h>
#include <iostream>
#include <chrono>

// Конструктор для WorkerClient
WorkerClient::WorkerClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(parser::Coordinator::NewStub(channel)) {}

// Получение задания от координатора
std::string WorkerClient::GetTask() {
    parser::TaskRequest request;
    request.set_worker_id("worker_1");

    parser::TaskResponse response;
    grpc::ClientContext context;

    grpc::Status status = stub_->GetTask(&context, request, &response);
    if (status.ok() && response.has_task()) {
        return response.url();
    } else {
        return "";
    }
}

// Отправка результата координатору
void WorkerClient::ReportResult(const std::string& url, const std::string& result) {
    parser::TaskResult taskResult;
    taskResult.set_worker_id("worker_1");
    taskResult.set_url(url);
    taskResult.set_result(result);

    parser::ResultAck ack;
    grpc::ClientContext context;

    grpc::Status status = stub_->ReportResult(&context, taskResult, &ack);
    if (!status.ok()) {
        std::cerr << "Failed to report result: " << status.error_message() << std::endl;
    }
}

// Сохранение результата в базу данных Apache Cassandra
void saveToCassandra(const std::string& url, const std::vector<std::string>& links, double time_taken) {
    // Инициализация подключения к Cassandra
    CassCluster* cluster = cass_cluster_new();
    CassSession* session = cass_session_new();

    // Настройка контактов (узлов кластера)
    cass_cluster_set_contact_points(cluster, "127.0.0.1");

    // Подключение к ключевому пространству "parser"
    CassFuture* connect_future = cass_session_connect_keyspace(session, cluster, "parser");

    // Ожидание подключения
    if (cass_future_error_code(connect_future) == CASS_OK) {
        // Подготовка запроса на вставку данных
        const char* query = "INSERT INTO parsed_results (url, links, time_taken) VALUES (?, ?, ?)";
        CassStatement* statement = cass_statement_new(query, 3);
        
        // Установка значений в запрос
        cass_statement_bind_string(statement, 0, url.c_str());

        // Конвертация ссылок в формат Cassandra (set<text>)
        CassCollection* link_collection = cass_collection_new(CASS_COLLECTION_TYPE_SET, links.size());
        for (const auto& link : links) {
            cass_collection_append_string(link_collection, link.c_str());
        }
        cass_statement_bind_collection(statement, 1, link_collection);
        cass_collection_free(link_collection);

        // Установка времени выполнения
        cass_statement_bind_double(statement, 2, time_taken);

        // Выполнение запроса
        CassFuture* result_future = cass_session_execute(session, statement);

        if (cass_future_error_code(result_future) != CASS_OK) {
            const char* message;
            size_t message_length;
            cass_future_error_message(result_future, &message, &message_length);
            std::cerr << "Unable to save result to Cassandra: " << std::string(message, message_length) << std::endl;
        }

        // Освобождение ресурсов
        cass_statement_free(statement);
        cass_future_free(result_future);
    } else {
        std::cerr << "Unable to connect to Cassandra" << std::endl;
    }

    // Освобождение ресурсов
    cass_future_free(connect_future);
    cass_session_free(session);
    cass_cluster_free(cluster);
}

// Функция для обработки задания: загрузка, парсинг и сохранение результата
void processTask(WorkerClient& worker, const std::string& taskUrl) {
    std::cout << "Received task: " << taskUrl << std::endl;

    // Начало измерения времени выполнения задания
    auto start_time = std::chrono::steady_clock::now();

    // Загрузка страницы
    MemoryStruct page;
    fetchPage(taskUrl, page);

    // Парсинг страницы и сохранение результата
    if (page.size > 0) {
        auto links = parseHtml(page.memory);

        // Конец измерения времени выполнения
        auto end_time = std::chrono::steady_clock::now();
        double time_taken = std::chrono::duration<double>(end_time - start_time).count();

        // Сохранение результатов в Cassandra
        saveToCassandra(taskUrl, links, time_taken);

        // Отправка результата обратно координатору
        worker.ReportResult(taskUrl, "Completed");
        
        // Освобождение памяти
        free(page.memory);
    } else {
        std::cerr << "Failed to fetch the page: " << taskUrl << std::endl;
    }
}
