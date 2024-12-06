# Используем официальный образ Ubuntu 20.04 в качестве базового
FROM base-vcpkg

# Устанавливаем переменную окружения для предотвращения интерактивных запросов
ENV DEBIAN_FRONTEND=noninteractive

# Установка необходимых инструментов и библиотек
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    unzip \
    libssl-dev \
    pkg-config \
    autoconf \
    automake \
    libtool \
    curl \
    zlib1g-dev

# Клонирование репозитория gRPC версии v1.44.0 и инициализация подмодулей
RUN git clone -b v1.44.0 https://github.com/grpc/grpc /tmp/grpc && \
    cd /tmp/grpc && \
    git submodule update --init --recursive

RUN vcpkg install  abseil c-ares
# Сборка и установка gRPC и его зависимостей
RUN mkdir -p /tmp/grpc/cmake/build && \
    cd /tmp/grpc/cmake/build && \
    cmake -DCMAKE_INSTALL_PREFIX=/tmp/grpc/bin/grpc -DgRPC_ABSL_PROVIDER=package \ 
          ../.. && \
    make -j$(nproc) && \
    make install

# Установка переменных окружения
ENV PATH="/usr/local/bin:${PATH}"
ENV PKG_CONFIG_PATH="/usr/local/lib/pkgconfig"

# Установка protoc версии 21.12
RUN PROTOC_ZIP=protoc-21.12-linux-x86_64.zip && \
    wget https://github.com/protocolbuffers/protobuf/releases/download/v21.12/${PROTOC_ZIP} && \
    unzip -o ${PROTOC_ZIP} -d /usr/local bin/protoc 'include/*' && \
    rm -f ${PROTOC_ZIP}

# Проверка установки protoc
RUN protoc --version

# Установка рабочей директории
WORKDIR /app

# Копирование исходного кода проекта
COPY . .

# Сборка проекта
RUN mkdir -p build && \
    cd build && \
    cmake -DCMAKE_TOOLCHAIN_FILE=/vcpkg/scripts/buildsystems/vcpkg.cmake .. && \
    make -j$(nproc)

# Открытие порта
EXPOSE 50051

# Команда запуска
CMD ["./build/coordinator"]
