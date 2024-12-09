# Используем официальный образ Cassandra
FROM cassandra:latest

# Устанавливаем переменные окружения для конфигурации Cassandra
ENV CASSANDRA_CLUSTER_NAME="MyCluster"
ENV CASSANDRA_DC="datacenter1"
ENV CASSANDRA_RACK="rack1"
ENV CASSANDRA_SEEDS="127.0.0.1"
ENV CASSANDRA_LISTEN_ADDRESS="0.0.0.0"
ENV CASSANDRA_BROADCAST_ADDRESS="127.0.0.1"
ENV CASSANDRA_BROADCAST_RPC_ADDRESS="127.0.0.1"
ENV CASSANDRA_ENDPOINT_SNITCH="GossipingPropertyFileSnitch"

# Открываем стандартный порт Cassandra
EXPOSE 7000 7001 7199 9042 9160

# Добавляем пользовательский файл конфигурации (при необходимости)
# COPY cassandra.yaml /etc/cassandra/cassandra.yaml

# Старт сервера Cassandra
CMD ["cassandra", "-f"]
