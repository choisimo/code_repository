#!/bin/bash
read -p "NAVER_MAPS_CLIENT_ID: " NAVER_MAPS_CLIENT_ID
read -p "API_BASE_URL: " API_BASE_URL
read -p "WEATHER_API_KEY: " WEATHER_API_KEY

# check whether input or default value
read -p "DB_HOST: " DB_HOST || DB_HOST="db"
read -p "DB_PORT: " DB_PORT || DB_PORT="5432"
read -p "DB_NAME: " DB_NAME || DB_NAME="weatherdb"
read -p "DB_USERNAME: " DB_USERNAME || DB_USERNAME="postgres"
read -p "DB_PASSWORD: " DB_PASSWORD || DB_PASSWORD="yourpassword"

read -p "MONGO_HOST: " MONGO_HOST || MONGO_HOST="mongo"
read -p "MONGO_PORT: " MONGO_PORT || MONGO_PORT="27017"
read -p "MONGO_DB: " MONGO_DB || MONGO_DB="weatherdb"
