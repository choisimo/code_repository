#!/bin/bash

# Load environment variables from .env file
if [ -f .env ]; then
    export $(grep -v '^#' .env | xargs)
else
    echo "Error: .env file not found. Exiting."
    exit 1
fi

# 설정 변수
REMOTE_HOST="$REMOTE_HOST"
REMOTE_DIR="$REMOTE_DIR"
BACKUP_DIR="$BACKUP_DIR"
LOG_FILE="$LOG_FILE"
DATE=$(date +%Y-%m-%d_%H-%M-%S)
ERROR_EMAIL="$ERROR_EMAIL"  # 오류 발생 시 알림 받을 이메일

# 로그 함수
log() {
    echo "$(date +%Y-%m-%d %H:%M:%S) - $1" | tee -a "$LOG_FILE"
}

# USB 드라이브 마운트 확인
if ! mountpoint -q "$BACKUP_DIR"; then
    error_msg="백업 드라이브가 마운트되지 않았습니다."
    log "$error_msg"
    echo "$error_msg" | mail -s "백업 오류 알림" "$ERROR_EMAIL"
    exit 1
fi

# 디스크 공간 확인
AVAILABLE_SPACE=$(df -P "$BACKUP_DIR" | awk 'NR==2 {print $4}')
MIN_SPACE=1048576  # 1GB (1024*1024 KB)

if [ "$AVAILABLE_SPACE" -lt "$MIN_SPACE" ]; then
    error_msg="백업 디스크 공간이 부족합니다. 최소 1GB 필요."
    log "$error_msg"
    echo "$error_msg" | mail -s "백업 오류 알림" "$ERROR_EMAIL"
    exit 1
fi

# 백업 디렉토리 생성
if [ ! -d "$BACKUP_DIR" ]; then
    log "백업 디렉토리 생성: $BACKUP_DIR"
    mkdir -p "$BACKUP_DIR"
fi

# 백업 로그 시작
log "원격 서버 백업 시작: $REMOTE_HOST:$REMOTE_DIR -> $BACKUP_DIR"

# SSH 키 확인
if ! ssh -o BatchMode=yes -o ConnectTimeout=5 "$REMOTE_HOST" exit &>/dev/null; then
    error_msg="SSH 접속 실패: $REMOTE_HOST"
    log "$error_msg"
    echo "$error_msg" | mail -s "백업 오류 알림" "$ERROR_EMAIL"
    exit 1
fi

# rsync로 백업 실행
log "rsync 백업 실행 중..."
rsync -avz --delete --stats \
    --exclude="*.tmp" \
    --exclude=".Trash*" \
    --exclude="lost+found" \
    --timeout=1800 \
    -e "ssh -o ConnectTimeout=10" \
    "$REMOTE_HOST:$REMOTE_DIR" "$BACKUP_DIR/" >> "$LOG_FILE" 2>&1

# 백업 결과 확인
if [ $? -eq 0 ]; then
    log "백업 성공적으로 완료됨"

    # 백업 요약 정보 생성
    BACKUP_SIZE=$(du -sh "$BACKUP_DIR" | awk '{print $1}')
    log "백업 크기: $BACKUP_SIZE"

    # 백업 성공 기록 파일 생성
    echo "백업 시간: $(date)" > "$BACKUP_DIR/last_backup.txt"
    echo "백업 크기: $BACKUP_SIZE" >> "$BACKUP_DIR/last_backup.txt"
else
    error_msg="백업 실행 중 오류 발생"
    log "$error_msg"
    echo "$error_msg" | mail -s "백업 오류 알림" "$ERROR_EMAIL"
    exit 1
fi

log "백업 작업 완료"
exit 0





# # .env parameter settings #
# REMOTE_HOST=user@remote-server  # 원격 서버의 SSH 접속 정보
# REMOTE_DIR=/path/to/backup/source/  # 백업할 원격 서버의 디렉토리 경로
# BACKUP_DIR=/mnt/backup/server-backup  # USB 드라이브에 백업 파일이 저장될 경로
# LOG_FILE=/var/log/backup-remote.log  # 백업 로그 파일 경로
# ERROR_EMAIL=your-email@example.com  # 오류 발생 시 알림 받을 이메일

# # .env 파일 내부 flag 설명
# # REMOTE_HOST: 원격 서버의 SSH 접속 정보 (예: user@server-ip)
# # REMOTE_DIR: 백업할 원격 서버의 디렉토리 경로
# # BACKUP_DIR: USB 드라이브에 백업 파일이 저장될 경로
# # LOG_FILE: 백업 로그 파일 경로
# # ERROR_EMAIL: 오류 발생 시 알림 받을 이메일 주소
