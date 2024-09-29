# Используем официальный образ для Ubuntu
FROM ubuntu:20.04

# Установка необходимых зависимостей
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    libprotobuf-dev \
    protobuf-compiler \
    libgrpc++-dev \
    curl \
    libcurl4-openssl-dev \
    libxml2-dev

# Копируем файлы проекта
WORKDIR /app
COPY ./worker /app
COPY ./generated /app/generated
COPY ./include /app/include

# Сборка воркера
RUN g++ worker.cpp html_parser.cpp page_fetcher.cpp main.cpp \
    generated/task.pb.cc generated/task.grpc.pb.cc \
    -o worker -std=c++17 -lgrpc++ -lprotobuf -lxml2 -lcurl -pthread

# Запуск воркера
CMD ["./worker"]
