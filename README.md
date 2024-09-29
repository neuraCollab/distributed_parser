
## Архитектура Проект

/distributed_parser_project
│
├── /proto_files
│   └── task.proto                      # Файл .proto с определением gRPC-сервисов и сообщений
│
├── /generated
│   ├── task.pb.cc                      # Сгенерированный protobuf файл для C++
│   ├── task.pb.h
│   ├── task.grpc.pb.cc                 # Сгенерированный файл gRPC для C++
│   └── task.grpc.pb.h
│
├── /coordinator
│   ├── coordinator.cpp                 # Реализация gRPC координатора
│   ├── coordinator.h                   # Заголовочный файл для координатора
│   ├── main.cpp                        # Точка входа в приложение координатора
│   ├── Dockerfile                      # Dockerfile для контейнеризации координатора
│   └── Makefile                        # Makefile для сборки координатора
│
├── /worker
│   ├── worker.cpp                      # Реализация gRPC воркера
│   ├── worker.h                        # Заголовочный файл для воркера
│   ├── html_parser.cpp                 # Функции для парсинга HTML с использованием libxml2
│   ├── html_parser.h                   # Заголовочный файл для функций парсинга HTML
│   ├── page_fetcher.cpp                # Функции для загрузки страниц с использованием libcurl
│   ├── page_fetcher.h                  # Заголовочный файл для загрузчика страниц
│   ├── main.cpp                        # Точка входа в приложение воркера
│   ├── Dockerfile                      # Dockerfile для контейнеризации воркера
│   └── Makefile                        # Makefile для сборки воркера
│
├── /cassandra
│   ├── cassandra-config.yaml           # Конфигурационный файл для настройки Cassandra
│   ├── Dockerfile                      # Dockerfile для создания контейнера Cassandra (при необходимости кастомизации)
│
├── /kubernetes
│   ├── coordinator-deployment.yaml     # Kubernetes Deployment для координатора
│   ├── worker-deployment.yaml          # Kubernetes Deployment для воркеров
│   ├── cassandra-statefulset.yaml      # Kubernetes StatefulSet для Cassandra
│   ├── coordinator-service.yaml        # Сервис для координатора (LoadBalancer/ClusterIP)
│   ├── worker-service.yaml             # Сервис для воркеров
│   └── cassandra-service.yaml          # Сервис для Cassandra (Headless для кластера)
│
├── /include
│   ├── task.pb.h                       # Сгенерированный protobuf заголовочный файл (копия из /generated для удобства)
│   ├── task.grpc.pb.h                  # Сгенерированный заголовочный файл для gRPC (копия из /generated для удобства)
│   └── common.h                        # Общие объявления и утилиты, используемые в координаторе и воркере
│
├── /scripts
│   ├── build_all.sh                    # Скрипт для сборки всех компонентов проекта
│   ├── run_coordinator.sh              # Скрипт для запуска координатора
│   ├── run_worker.sh                   # Скрипт для запуска воркера
│   ├── generate_protos.sh              # Скрипт для генерации protobuf и gRPC файлов
│   └── deploy_kubernetes.sh            # Скрипт для развертывания всех компонентов в Kubernetes
│
├── /libs
│   ├── libgrpc++                       # Внешние зависимости: библиотеки gRPC, если не установлены глобально
│   ├── libprotobuf                     # Внешние зависимости: библиотеки protobuf
│   ├── libxml2                         # Внешние зависимости: библиотеки libxml2
│   └── libcurl                         # Внешние зависимости: библиотеки libcurl
│
├── /build
│   └── ...                             # Директория для скомпилированных бинарных файлов
│
├── .gitignore                          # Файл для исключения временных и скомпилированных файлов из репозитория
├── README.md                           # Документация проекта
└── LICENSE                             # Лицензия проекта


Описание Папок и Их Назначение
/proto_files:

Содержит файл task.proto с определением gRPC-сервисов и сообщений. Все изменения в интерфейсах должны быть внесены в этом файле.
/generated:

Содержит автоматически сгенерированные файлы protobuf и gRPC (task.pb.cc, task.pb.h, task.grpc.pb.cc, task.grpc.pb.h). Эти файлы генерируются с помощью protoc и нужны для реализации как координатора, так и воркеров.
/coordinator:

Содержит реализацию координатора. Здесь находится coordinator.cpp с основной логикой работы gRPC-сервера, а также main.cpp, который запускает сервер координатора. Makefile используется для сборки проекта.
/worker:

Содержит реализацию воркера. Файлы worker.cpp, html_parser.cpp, и page_fetcher.cpp отвечают за реализацию gRPC-клиента, парсинг HTML с использованием libxml2, и загрузку страниц с использованием libcurl. Makefile для сборки проекта воркера также присутствует здесь.
/include:

Содержит заголовочные файлы, как общие, так и сгенерированные. Это упрощает организацию проекта, так как позволяет легко подключать заголовочные файлы и использовать их в различных модулях.
/scripts:

Содержит скрипты для автоматизации задач, таких как генерация protobuf файлов (generate_protos.sh), сборка всех компонентов (build_all.sh), и запуск координатора и воркеров.
/libs:

Содержит внешние зависимости проекта. Если библиотеки gRPC, protobuf, libxml2, и libcurl не установлены глобально, они могут быть добавлены сюда.
/build:

Директория для скомпилированных бинарных файлов координатора и воркеров. Это помогает поддерживать корень проекта чистым.