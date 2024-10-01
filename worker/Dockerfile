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
    git \
    make

# Установка CMake
RUN apt-get install -y cmake

# Копирование исходного кода воркера в контейнер
WORKDIR /app
COPY . /app

# Сборка воркера
WORKDIR /app/worker
RUN make

# Установка порта и запуск воркера
CMD ["./worker"]
