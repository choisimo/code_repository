# 1. 프론트엔드 빌드 스테이지
FROM node:21-alpine AS frontend-build
WORKDIR /app/frontend

# 프론트엔드 의존성 설치
COPY frontend/package*.json ./
RUN npm install

# 프론트엔드 소스 복사 및 빌드
COPY frontend/ ./
ARG NAVER_MAPS_CLIENT_ID
ENV VUE_APP_NAVER_MAPS_CLIENT_ID=${NAVER_MAPS_CLIENT_ID}
ARG API_BASE_URL
ENV VUE_APP_API_BASE_URL=${API_BASE_URL:-http://localhost:8080}
RUN npm run build

# 2. 백엔드 빌드 스테이지
FROM gradle:8-jdk21 AS backend-build
WORKDIR /app/backend

# 그래들 캐시 최적화를 위한 의존성 먼저 복사
COPY backend/build.gradle backend/settings.gradle ./
COPY backend/gradle ./gradle

# 의존성 추가 확인 (build.gradle에 PostgreSQL 드라이버가 있는지 확인)
RUN echo "dependencies { implementation 'org.postgresql:postgresql:42.6.0' }" >> build.gradle
RUN gradle dependencies --no-daemon

# 백엔드 소스 복사 및 빌드
COPY backend/src ./src
RUN gradle build --no-daemon -x test

# 3. 최종 실행 이미지
FROM eclipse-temurin:21-jre-alpine
WORKDIR /app

# 백엔드 JAR 파일 복사
COPY --from=backend-build /app/backend/build/libs/*.jar app.jar

# 프론트엔드 정적 파일 복사 (Spring Boot의 static 디렉토리로)
COPY --from=frontend-build /app/frontend/dist /app/static

# 환경 변수 설정
ENV SPRING_RESOURCES_STATIC_LOCATIONS=classpath:/static/,file:/app/static/
ENV SPRING_PROFILES_ACTIVE=prod

# 포트 노출
EXPOSE 8080

# 애플리케이션 실행
ENTRYPOINT ["java", "-jar", "/app/app.jar"]