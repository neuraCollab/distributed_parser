# Используем официальный образ Ubuntu в качестве базового
FROM ubuntu:20.04

# Установка необходимых зависимостей
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    libprotobuf-dev \
    protobuf-compiler \
    libgrpc++-dev \
    libcurl4-openssl-dev \
    libxml2-dev \
    git

# Установка CMake
RUN apt-get install -y cmake

# Копирование исходного кода координатора в контейнер
WORKDIR /app
COPY . /app

# Сборка координатора
RUN cmake . && make

# Установка порта и запуск координатора
EXPOSE 50051
CMD ["./coordinator"]
