cmake_minimum_required(VERSION 3.15)

# Название проекта
project(Worker LANGUAGES CXX)

# Установка версии стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Параметры для сборки
set(CMAKE_BUILD_TYPE Release)

# Пути для поиска сторонних библиотек
include_directories(
    ${CMAKE_SOURCE_DIR}
    /usr/include
    /usr/local/include
)

# Найти зависимости: Protobuf, gRPC, Curl, XML
find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(CURL REQUIRED)
find_package(LibXml2 REQUIRED)

# Источники проекта
set(SOURCES
    main.cpp           # Основной файл
    worker.cpp         # Логика воркера
    ../generated/task.pb.cc   # Сгенерированный Protobuf
    ../generated/task.grpc.pb.cc  # Сгенерированный gRPC
)

# Сборка исполняемого файла
add_executable(worker ${SOURCES})

# Линковка библиотек
target_link_libraries(worker
    gRPC::grpc++
    protobuf::libprotobuf
    ${CURL_LIBRARIES}
    ${LIBXML2_LIBRARIES}
    pthread
)

# Указать include-директории для проекта
target_include_directories(worker PRIVATE
    ${CURL_INCLUDE_DIRS}
    ${LIBXML2_INCLUDE_DIRS}
    ${Protobuf_INCLUDE_DIRS}
)

# Указать дополнительные флаги компиляции для gRPC и Protobuf
add_definitions(-DGPRC_CPP11_ABI)
