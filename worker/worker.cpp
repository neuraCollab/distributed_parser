#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <chrono>
#include <grpcpp/grpcpp.h>
#include "task.grpc.pb.h"
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <curl/curl.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using parser::Coordinator;
using parser::TaskRequest;
using parser::TaskResponse;
using parser::TaskResult;
using parser::ResultAck;

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = (char *)realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        std::cerr << "Not enough memory (realloc returned NULL)" << std::endl;
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

void fetchPage(const std::string &url, MemoryStruct &chunk) {
    CURL *curl_handle;
    CURLcode res;

    chunk.memory = (char *)malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();

    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl_handle);

    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }

    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();
}

void parseHtml(const std::string &htmlContent) {
    htmlDocPtr doc = htmlReadMemory(htmlContent.c_str(), htmlContent.size(), NULL, NULL, HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);
    if (doc == nullptr) {
        std::cerr << "Error: unable to parse HTML content" << std::endl;
        return;
    }

    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(doc);
    if (xpathCtx == nullptr) {
        std::cerr << "Error: unable to create new XPath context" << std::endl;
        xmlFreeDoc(doc);
        return;
    }

    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar *)"//a", xpathCtx);
    if (xpathObj == nullptr) {
        std::cerr << "Error: unable to evaluate XPath expression" << std::endl;
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return;
    }

    xmlNodeSetPtr nodes = xpathObj->nodesetval;
    for (int i = 0; i < nodes->nodeNr; i++) {
        xmlNodePtr node = nodes->nodeTab[i];
        xmlChar *href = xmlGetProp(node, (const xmlChar *)"href");
        if (href != nullptr) {
            std::cout << "Link: " << href << std::endl;
            xmlFree(href);
        }
    }

    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);
}

class WorkerClient {
public:
    WorkerClient(std::shared_ptr<Channel> channel)
        : stub_(Coordinator::NewStub(channel)) {}

    std::string GetTask() {
        TaskRequest request;
        request.set_worker_id("worker_1");

        TaskResponse response;
        ClientContext context;

        Status status = stub_->GetTask(&context, request, &response);
        if (status.ok() && response.has_task()) {
            return response.url();
        } else {
            return "";
        }
    }

    void ReportResult(const std::string& url, const std::string& result) {
        TaskResult taskResult;
        taskResult.set_worker_id("worker_1");
        taskResult.set_url(url);
        taskResult.set_result(result);

        ResultAck ack;
        ClientContext context;

        Status status = stub_->ReportResult(&context, taskResult, &ack);
        if (!status.ok()) {
            std::cerr << "Failed to report result: " << status.error_message() << std::endl;
        }
    }

private:
    std::unique_ptr<Coordinator::Stub> stub_;
};

int main(int argc, char** argv) {
    WorkerClient worker(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

    while (true) {
        std::string taskUrl = worker.GetTask();
        if (!taskUrl.empty()) {
           // Загружаем и парсим страницу
            std::cout << "Received task: " << taskUrl << std::endl;

            // Загрузка страницы
            MemoryStruct page;
            fetchPage(taskUrl, page);

            // Парсинг страницы
            if (page.size > 0) {
                parseHtml(page.memory);
                // Отправляем результат обратно координатору
                worker.ReportResult(taskUrl, "Completed");
                free(page.memory);
            }
        } else {
            // Если нет задания, ждем немного перед следующим запросом
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    return 0;
}
