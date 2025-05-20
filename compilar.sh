#!/bin/bash

CONTAINER_NAME=tpe_contain

echo "Iniciando contenedor $CONTAINER_NAME..."
docker start $CONTAINER_NAME

echo "Limpiando compilaciones anteriores..."
docker exec -it $CONTAINER_NAME make clean -C /root/Toolchain
docker exec -it $CONTAINER_NAME make clean -C /root

echo "Compilando Toolchain..."
docker exec -it $CONTAINER_NAME make -C /root/Toolchain

echo "Compilando proyecto completo..."
docker exec -it $CONTAINER_NAME make -C /root

echo "Deteniendo contenedor..."
docker stop $CONTAINER_NAME