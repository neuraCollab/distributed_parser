# Используем официальный образ Ubuntu в качестве базового
FROM ubuntu:20.04

# Установка переменной окружения для предотвращения интерактивных запросов
ENV DEBIAN_FRONTEND=noninteractive 

# Установка необходимых инструментов и библиотек
RUN apt-get update && apt-get install -y \
    g++ \
    cmake \
    libcurl4-openssl-dev \
    libxml2-dev \
    git \
    libgrpc++-dev \
    protobuf-compiler-grpc \
    libprotobuf-dev \
    protobuf-compiler \
    wget \
    unzip \
    && apt-get clean

# Установка CMake (если требуется более свежая версия)
RUN apt-get install -y software-properties-common && \
    add-apt-repository ppa:ubuntu-toolchain-r/test && \
    apt-get update && apt-get install -y cmake

# Рабочая директория
WORKDIR /app

# Копирование исходного кода проекта
COPY . /app

# Кэширование Abseil и других git репозиториев
# Использование кеша для скачивания репозиториев
RUN if [ ! -d "/app/abseil-cpp" ]; then \
      git clone https://github.com/abseil/abseil-cpp.git; \
    fi

# Установка gRPC и protobuf
RUN if [ ! -d "/tmp/grpc" ]; then \
      git clone -b v1.44.0 https://github.com/grpc/grpc /tmp/grpc && \
      cd /tmp/grpc && \
      git submodule update --init && \
      mkdir -p cmake/build && \
      cd cmake/build && \
      cmake -DCMAKE_INSTALL_PREFIX=/usr/local ../.. && \
      make && \
      make install && \
      make plugins; \
    fi

# Сборка Abseil из исходников
RUN cd /app/abseil-cpp && \
    rm -rf build && mkdir build && cd build && \
    cmake -DABSL_BUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build . --target install && \
    cd ../..

# Сборка проекта
RUN rm -rf build && mkdir -p build && cd build && \
    cmake .. && \
    make

# Открытие порта
EXPOSE 50051

# Команда запуска
CMD ["./build/coordinator"]
