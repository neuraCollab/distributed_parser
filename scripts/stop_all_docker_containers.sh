#!/bin/bash

# Получаем список всех запущенных контейнеров
CONTAINERS=$(docker ps -q)

if [ -z "$CONTAINERS" ]; then
    echo "Нет запущенных контейнеров для остановки."
    exit 0
fi

# Останавливаем все запущенные контейнеры
echo "Останавливаем все контейнеры..."
docker stop $CONTAINERS

echo "Все контейнеры остановлены."