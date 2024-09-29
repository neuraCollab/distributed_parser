# Используем официальный образ Cassandra в качестве базового
FROM cassandra:latest

# Копируем кастомные конфигурационные файлы в контейнер
COPY cassandra-config.yaml /etc/cassandra/cassandra.yaml

# Установка переменных среды для конфигурации Cassandra
ENV CASSANDRA_CLUSTER_NAME=$CASSANDRA_CLUSTER_NAME
ENV CASSANDRA_LISTEN_ADDRESS=$CASSANDRA_LISTEN_ADDRESS
ENV CASSANDRA_RPC_ADDRESS=$CASSANDRA_RPC_ADDRESS
ENV CASSANDRA_NUM_TOKENS=$CASSANDRA_NUM_TOKENS

# Выставляем порты, необходимые для работы Cassandra
EXPOSE 7000 7001 7199 9042 9160

# Настройка команды запуска Cassandra
CMD ["cassandra", "-f"]
