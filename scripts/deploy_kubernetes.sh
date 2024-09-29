#!/bin/bash

# Загрузка переменных из .env файла
export $(grep -v '^#' ../.env | xargs)

# Создание namespace для проекта (опционально)
kubectl create namespace $KUBERNETES_NAMESPACE

# Создание ConfigMap с переменными окружения
kubectl apply -f ../kubernetes/env-configmap.yaml -n $KUBERNETES_NAMESPACE

# Применение конфигураций для координатора
kubectl apply -f ../kubernetes/coordinator-deployment.yaml -n $KUBERNETES_NAMESPACE
kubectl apply -f ../kubernetes/coordinator-service.yaml -n $KUBERNETES_NAMESPACE

# Применение конфигураций для воркеров
kubectl apply -f ../kubernetes/worker-deployment.yaml -n $KUBERNETES_NAMESPACE
kubectl apply -f ../kubernetes/worker-service.yaml -n $KUBERNETES_NAMESPACE

# Применение конфигураций для Cassandra
kubectl apply -f ../kubernetes/cassandra-statefulset.yaml -n $KUBERNETES_NAMESPACE
kubectl apply -f ../kubernetes/cassandra-service.yaml -n $KUBERNETES_NAMESPACE

echo "All components have been deployed to Kubernetes namespace $KUBERNETES_NAMESPACE!"
