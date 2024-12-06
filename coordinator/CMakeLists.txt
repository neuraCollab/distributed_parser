cmake_minimum_required(VERSION 3.15)

# Название проекта
project(Coordinator LANGUAGES CXX)

# Установка версии стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -D_GLIBCXX_USE_CXX11_ABI=0")

# Параметры для сборки
set(CMAKE_BUILD_TYPE Release)

# Пути для поиска сторонних библиотек
include_directories(
    .
    ../generated
    /usr/local/include
    /usr/include
    /app/abseil-cpp
)

# Найти зависимости: Curl, XML, Protobuf, gRPC
find_package(CURL REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

# Источники проекта
set(SOURCES
    main.cpp
    coordinator.cpp
    ../generated/task.pb.cc
    ../generated/task.grpc.pb.cc
)

# Добавить gRPC и protobuf в проект
set(GRPC_SRCS
    ../generated/task.pb.cc
    ../generated/task.grpc.pb.cc
)

# Указать путь для сборки abseil-cpp
add_subdirectory(/app/abseil-cpp)

# Сборка исполняемого файла
add_executable(coordinator ${SOURCES} ${GRPC_SRCS})

# Линковка библиотек
target_link_libraries(coordinator
    absl::strings
    absl::synchronization
    absl::time
    absl::memory
    gRPC::grpc++
    protobuf::libprotobuf
    ${CURL_LIBRARIES}
    ${LIBXML2_LIBRARIES}
    pthread
)

# Указать include-директории для проекта
target_include_directories(coordinator PRIVATE
    ${CURL_INCLUDE_DIRS}
    ${LIBXML2_INCLUDE_DIRS}
    ${Protobuf_INCLUDE_DIRS}
    ${gRPC_INCLUDE_DIRS}
)
