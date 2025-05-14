#!/bin/bash

# 프론트엔드와 백엔드를 동시에 실행하는 스크립트

# 터미널 색상 정의
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${GREEN}==== Nodove 날씨 프로젝트 실행 ====${NC}"

# 백엔드 빌드 및 실행
echo -e "${BLUE}백엔드 실행 중...${NC}"
cd backend
./gradlew bootRun &
BACKEND_PID=$!

# 프론트엔드 실행
echo -e "${BLUE}프론트엔드 실행 중...${NC}"
cd ../frontend
npm run dev &
FRONTEND_PID=$!

# 종료 시 모든 프로세스 정리
function cleanup {
  echo -e "${GREEN}애플리케이션 종료 중...${NC}"
  kill $BACKEND_PID
  kill $FRONTEND_PID
  exit 0
}

# Ctrl+C로 종료 시 cleanup 함수 실행
trap cleanup INT

# 무한 대기
echo -e "${GREEN}애플리케이션이 실행 중입니다. 종료하려면 Ctrl+C를 누르세요.${NC}"
wait
