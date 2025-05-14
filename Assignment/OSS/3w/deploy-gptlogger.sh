#!/bin/bash

# GPTLogger Docker 배포 자동화 스크립트
# 사용법: ./deploy-gptlogger.sh [버전] [호스트포트][컨테이너 이름]
echo "Usage: ./deploy-gptlogger.sh [version] [host_port] [container_name]"
echo "Example: ./deploy-gptlogger.sh latest 8502 gptlogger-test"

# 기본값 설정
VERSION=${1:-"latest"}
HOST_PORT=${2:-8502}
CONTAINER_PORT=8501
CONTAINER_NAME="gptlogger-test"
IMAGE_NAME="gptlogger"

# 색상 정의
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 로그 함수
log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# 스크립트 시작
log_info "GPTLogger 배포 시작 (버전: ${VERSION}, 포트: ${HOST_PORT})"

# 1. 실행 중인 컨테이너 중지
if sudo docker ps -a | grep -q ${CONTAINER_NAME}; then
    log_info "기존 컨테이너 중지 중: ${CONTAINER_NAME}"
    sudo docker stop ${CONTAINER_NAME} || log_warning "컨테이너가 이미 중지되었거나 존재하지 않습니다."
    
    log_info "기존 컨테이너 삭제 중: ${CONTAINER_NAME}"
    sudo docker rm ${CONTAINER_NAME} || log_warning "컨테이너를 삭제할 수 없습니다."
else
    log_info "기존 컨테이너가 없습니다. 새로 생성합니다."
fi

# 1.1 이미지 삭제 및 해당 포트 프로세스 찾아서 존재 시 종료
if sudo docker ps -a | grep -q ${IMAGE_NAME}:${VERSION}; then
    log_info "기존 이미지 삭제 중: ${IMAGE_NAME}:${VERSION}"
    sudo docker rmi ${IMAGE_NAME}:${VERSION} || log_warning "이미지를 삭제할 수 없습니다."
fi

if sudo lsof -ti :${HOST_PORT} | xargs kill -9; then
    log_info "기존 포트 프로세스 종료 중: ${HOST_PORT}"
else
    log_warning "포트 프로세스를 종료할 수 없습니다."
fi

# 2. 이미지 빌드
log_info "Docker 이미지 빌드 중: ${IMAGE_NAME}:${VERSION}"
if sudo docker build -t ${IMAGE_NAME}:${VERSION} .; then
    log_success "이미지 빌드 완료: ${IMAGE_NAME}:${VERSION}"
else
    log_error "이미지 빌드 실패!"
    exit 1
fi

# 3. 컨테이너 실행
log_info "새 컨테이너 실행 중: ${CONTAINER_NAME} (${HOST_PORT}:${CONTAINER_PORT})"
if sudo docker run -d -p ${HOST_PORT}:${CONTAINER_PORT} --name ${CONTAINER_NAME} ${IMAGE_NAME}:${VERSION}; then
    log_success "컨테이너 실행 완료: ${CONTAINER_NAME}"
else
    log_error "컨테이너 실행 실패!"
    exit 1
fi

# 4. 상태 확인
log_info "컨테이너 상태 확인 중..."
sudo docker ps -a | grep ${CONTAINER_NAME}

# 5. 로그 표시
log_info "컨테이너 로그 확인 중... (10줄)"
sudo docker logs ${CONTAINER_NAME} --tail 10

# 기본 접속 localhost 
log_success "배포 완료! 접속 URL: http://localhost:${HOST_PORT}"
# ifconfig 으로 내부 아이피 추출
log_success "배포 완료! 접속 URL: http://$(ifconfig | grep "inet " | grep -v "127.0.0.1" | awk '{print $2}' | cut -d/ -f1):${HOST_PORT}"
# docker ps 로 컨테이너 아이피 추출
log_success "배포 완료! 접속 URL: http://$(sudo docker ps -a | grep ${CONTAINER_NAME} | awk '{print $4}'):${HOST_PORT}"

