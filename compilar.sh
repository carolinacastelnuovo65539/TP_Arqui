#!/bin/bash

CONTAINER_NAME=tpe_contain

# Verificar si el contenedor ya existe
if [ "$(docker ps -a -q -f name=$CONTAINER_NAME)" ]; then
    echo "El contenedor $CONTAINER_NAME ya existe. Reutilizándolo..."
    docker start $CONTAINER_NAME
else
    echo "Creando un nuevo contenedor $CONTAINER_NAME..."
    docker run -d -v ${PWD}:/root --security-opt seccomp:unconfined -it --name $CONTAINER_NAME agodio/itba-so:2.0
fi


echo "Limpiando compilaciones anteriores..."
docker exec -it $CONTAINER_NAME make clean -C /root/Toolchain
docker exec -it $CONTAINER_NAME make clean -C /root

echo "Compilando Toolchain..."
docker exec -it $CONTAINER_NAME make -C /root/Toolchain

echo "Compilando proyecto completo..."
docker exec -it $CONTAINER_NAME make -C /root

echo "Deteniendo contenedor..."
docker stop $CONTAINER_NAME