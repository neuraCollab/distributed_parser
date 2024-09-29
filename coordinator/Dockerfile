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
COPY ./coordinator /app
COPY ./generated /app/generated
COPY ./include /app/include

# Сборка координатора
RUN g++ coordinator.cpp main.cpp generated/task.pb.cc generated/task.grpc.pb.cc \
    -o coordinator -std=c++17 -lgrpc++ -lprotobuf -pthread

# Запуск координатора
CMD ["./coordinator"]
