#!/bin/bash

# default start : ./deploy.sh
# custom start : ./deploy.sh -n appname -d /path/to/app -p 3000

# 1. default values
APP_NAME="node"
APP_DIR="/server/backend"
APP_PORT=3000


# 2.2 get values from command line
while getopts "n:d:p:" opt; do
  case $opt in
    n) APP_NAME="$OPTARG" ;;
    d) APP_DIR="$OPTARG" ;;
    p) APP_PORT="$OPTARG" ;;
    \?) echo "Invalid option -$opt" >&2; exit 1 ;;
  esac
done


# 3. Confirm values if not provided through arguments
if [ -z "$APP_NAME" ]; then
  read -p "Enter app name (default: $APP_NAME): " input_name
  APP_NAME=${input_name:-$APP_NAME}
fi

if [ -z "$APP_DIR" ]; then
  read -p "Enter app directory (default: $APP_DIR): " input_dir
  APP_DIR=${input_dir:-$APP_DIR}
fi

if [ -z "$APP_PORT" ]; then
  read -p "Enter app port (default: $APP_PORT): " input_port
  APP_PORT=${input_port:-$APP_PORT}
fi


# 4. Navigate to app directory
echo "Navigating to app directory: $APP_DIR"
if ! cd "$APP_DIR"; then
  echo "Error: Directory $APP_DIR does not exist. Exiting."
  sudo mkdir -p $APP_DIR
    sudo chown -R $USER:$USER $APP_DIR
    if ! cd "$APP_DIR"; then
      echo "Error: Cannot create directory $APP_DIR. Exiting."
      echo "Please create the directory and try again."
      echo "check if the directory exists and you have permission to create directory"
      exit 1
    fi
    exit 1
fi

# 4. stop app
echo "Stop app: $APP_NAME (if running)"
pm2 stop $APP_NAME || echo "App is not running or not found $APP_NAME" 

# 5. start app
echo "Start app: $APP_NAME"
PORT="$APP_PORT" pm2 start app.js --name "$APP_NAME" --watch --ignore-watch="node_modules"

# 6. show app status
echo "Show app status: $APP_NAME"
pm2 show $APP_NAME
