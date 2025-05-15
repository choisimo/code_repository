# docker 디렉토리에 start.sh 파일 생성
mkdir -p docker
cat > docker/start.sh << 'EOF'
#!/bin/sh

# 데이터베이스 연결 대기
echo "Waiting for database connection..."
while ! nc -z db 5432; do
  sleep 1
done
echo "Database is up and running!"

# 환경 변수 확인
echo "Checking environment variables..."
if [ -z "$SPRING_DATASOURCE_URL" ]; then
  echo "SPRING_DATASOURCE_URL is not set, using default"
  export SPRING_DATASOURCE_URL=jdbc:postgresql://db:5432/weatherdb
fi

# 애플리케이션 실행
echo "Starting application..."
java -jar /app/app.jar
EOF

# 실행 권한 부여
chmod +x docker/start.sh