# Описание проекта

проект представляет собой распределённую систему для парсинга веб-страниц, включающую несколько компонентов: координатор, воркеры, и база данных Apache Cassandra. Каждый из компонентов контейнеризован с использованием Docker, а для оркестрации контейнеров применяется Kubernetes. Давайте рассмотрим работу вашего проекта поэтапно:

1. Запуск Проекта
Docker Compose или Kubernetes: Проект может быть развернут локально с помощью Docker Compose или в Kubernetes-кластере для продакшн-окружения.
С помощью Docker Compose создаются контейнеры для координатора, воркеров и базы данных Apache Cassandra.
С помощью Kubernetes развертываются и управляются эти контейнеры, обеспечивая отказоустойчивость и масштабируемость.
2. Развёртывание Координатора
Координатор — это центральный компонент, который управляет процессом парсинга и распределяет задачи между воркерами.
После запуска координатор ожидает получения задания (например, списка URL-адресов).
Координатор развертывается с помощью Deployment в Kubernetes и предоставляет API для получения списка URL-адресов, которые нужно обработать.
3. Развёртывание Воркеров
Воркеры — это компоненты, которые непосредственно занимаются загрузкой и парсингом страниц.
Координатор распределяет URL-адреса между воркерами, чтобы оптимизировать выполнение задач.
Воркеры загружаются в виде отдельных контейнеров и автоматически масштабируются в Kubernetes.
Каждый воркер получает URL-адрес от координатора и начинает обрабатывать страницу.
4. Загрузка Веб-Страницы (Воркеры)
После получения задания воркер использует библиотеку libcurl для загрузки веб-страницы.
Воркеры параллельно загружают веб-страницы, что позволяет оптимизировать использование ресурсов и повысить общую производительность.
5. Парсинг HTML-Контента (Воркеры)
После загрузки страницы воркер использует libxml2 для парсинга HTML-контента.
В частности, ищутся ссылки (<a>), и извлекаются их значения атрибута href.
Эта информация может быть полезна для дальнейшего анализа или для выполнения следующего уровня парсинга.
6. Сохранение Результатов в Базу Данных Apache Cassandra
После успешного парсинга ссылки сохраняются в базе данных Apache Cassandra.
Cassandra была выбрана из-за её способности обрабатывать большие объемы данных, обеспечивать отказоустойчивость и горизонтально масштабироваться.
Воркеры отправляют результаты (например, извлеченные ссылки или другие данные) в Cassandra для последующего хранения и анализа.
7. Координация и Управление Нагрузкой
Координатор управляет распределением заданий между воркерами.
Если нужно обработать большое количество URL-адресов, координатор распределяет их между несколькими воркерами для параллельного выполнения.
Координатор также может контролировать статус выполнения заданий и при необходимости повторно назначать задания, если воркер не справился.
8. Масштабирование и Отказоустойчивость
Kubernetes обеспечивает отказоустойчивость, автоматически перезапуская воркеры или координатора, если они завершились с ошибкой.
При увеличении нагрузки (например, необходимости обработки большого количества URL) Kubernetes может автоматически масштабировать количество воркеров.
StatefulSet используется для развертывания Cassandra, что позволяет каждому узлу базы данных сохранять своё состояние и обеспечивать надёжное хранение данных.
9. Мониторинг и Логи
Логи каждого компонента собираются и могут быть использованы для мониторинга работы системы.
Координатор логирует получение и распределение заданий, а также статус выполнения каждого из воркеров.
Воркеры логируют URL, которые они обрабатывают, и время, затраченное на загрузку и парсинг страницы.
Заключение
Ваш проект выполняет следующие этапы по порядку:

Развёртывание компонентов проекта в контейнерах с использованием Docker или Kubernetes.
Запуск координатора для управления задачами.
Развёртывание воркеров, которые получают задания от координатора.
Загрузка веб-страниц с помощью libcurl и парсинг HTML с использованием libxml2.
Сохранение извлеченных данных (например, ссылок) в базе данных Apache Cassandra.
Координация распределения нагрузки и управление задачами через координатор.
Масштабирование системы с помощью Kubernetes для обеспечения отказоустойчивости и повышения производительности.
Эта распределённая система помогает эффективно обрабатывать большие объёмы данных, позволяя использовать несколько воркеров для параллельного выполнения заданий, что значительно ускоряет процесс парсинга и анализа.

# Архитектура Проект

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