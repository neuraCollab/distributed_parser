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
    libabsl-dev \
    libgrpc++-dev \
    libcurl4-openssl-dev \
    git

# Установка CMake
RUN apt-get install -y cmake git

# Копирование исходного кода координатора в контейнер
WORKDIR /app
COPY . /app

RUN git clone https://github.com/abseil/abseil-cpp.git && cd abseil-cpp && mkdir build && cd build \
    && cmake -DgRPC_INSTALL=ON -DCMAKE_BUILD_TYPE=Release -DgRPC_ABSL_PROVIDER=module -DgRPC_CARES_PROVIDER=module -DgRPC_PROTOBUF_PROVIDER=module -DgRPC_RE2_PROVIDER=module -DgRPC_SSL_PROVIDER=module -DgRPC_ZLIB_PROVIDER=module  -DCMAKE_CXX_FLAGS=-std=c++17 -DCMAKE_CXX_FLAGS='-D_GLIBCXX_USE_CXX11_ABI=0'  ../ \
    && make -j$(nproc) && make install && cd ../..

# Сборка координатора
RUN cmake . && make

# Установка порта и запуск координатора
EXPOSE 50051
CMD ["./coordinator"]
