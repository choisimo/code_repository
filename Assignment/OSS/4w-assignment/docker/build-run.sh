#!/bin/bash

echo "Building and running Docker container..."

cd /workspace/code_repository/Assignment/OSS/4w-assignment/
sudo docker build -t weather-app -f docker/Dockerfile .
sudo docker run -d -p 8085:8080 --name weather-app weather-app

echo "Container started successfully."
