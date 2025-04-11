#!/bin/bash

# Rsync 백업 스케줄링 스크립트
# 작성일: 2025년 4월 11일

# 색상 정의
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 기본 설정값 로드
load_defaults() {
    # 기본값
    BACKUP_SCHEDULE="daily"
    BACKUP_TIME="03:00"
    BACKUP_SOURCE_DIR="./"
    BACKUP_DEST_DIR="~/backup"
    BACKUP_LOG_DIR="./backup"
    REMOTE_SERVER="127.0.0.1"
    EXCLUDE_PATTERN=""
    RETENTION_DAYS=30
    SYNC_OPTIONS="-avz --delete"
}

# .env 파일에서 설정 로드
load_env_file() {
    if [ -f "$1" ]; then
        echo -e "${BLUE}$1 파일에서 설정을 로드합니다...${NC}"
        set -a  # 모든 변수를 export로 선언
        source "$1"
        set +a  # export 모드 해제
        return 0
    else
        echo -e "${YELLOW}경고: $1 파일을 찾을 수 없습니다.${NC}"
        return 1
    fi
}

# 사용자 입력으로 커스텀 설정
prompt_custom_settings() {
    echo -e "${BLUE}백업 커스텀 설정을 입력해주세요:${NC}"
    
    echo -e "${YELLOW}백업 일정 옵션:${NC}"
    echo "1. 매일 (Daily)"
    echo "2. 매주 (Weekly)"
    echo "3. 매월 (Monthly)"
    echo "4. 매월 마지막 토요일 (Last Saturday of Month)"
    echo "5. 커스텀 (Cron 문법 사용)"
    read -p "옵션을 선택하세요 (1-5): " schedule_choice
    
    case $schedule_choice in
        1) BACKUP_SCHEDULE="daily" ;;
        2) BACKUP_SCHEDULE="weekly" ;;
        3) BACKUP_SCHEDULE="monthly" ;;
        4) BACKUP_SCHEDULE="last_saturday" ;;
        5) 
            read -p "커스텀 Cron 일정을 입력하세요 (예: '0 3 * * 0'은 매주 일요일 오전 3시): " BACKUP_SCHEDULE
            ;;
        *) 
            echo -e "${YELLOW}유효하지 않은 선택입니다. 기본값(매일)을 사용합니다.${NC}"
            BACKUP_SCHEDULE="daily"
            ;;
    esac
    
    read -p "백업 시간 (HH:MM, 24시간 형식): " BACKUP_TIME
    read -p "백업할 소스 디렉토리/파일: " BACKUP_SOURCE_DIR
    
    echo -e "${YELLOW}백업 대상 유형:${NC}"
    echo "1. 로컬 (Local)"
    echo "2. 원격 (SSH를 통한 Remote)"
    read -p "옵션을 선택하세요 (1-2): " dest_choice
    
    if [ "$dest_choice" == "2" ]; then
        read -p "원격 서버 (user@hostname): " REMOTE_SERVER
        read -p "원격 서버의 대상 디렉토리: " BACKUP_DEST_DIR
    else
        read -p "대상 디렉토리: " BACKUP_DEST_DIR
    fi
    
    read -p "로그 디렉토리 (기본값: ./backup): " input_log_dir
    BACKUP_LOG_DIR=${input_log_dir:-"./backup"}
    
    read -p "제외 패턴 (쉼표로 구분된 목록, 예: '*.tmp,*.log'): " EXCLUDE_PATTERN
    read -p "보존 기간 (백업 유지 일수, 기본값: 30): " input_retention
    RETENTION_DAYS=${input_retention:-30}
    
    read -p "추가 rsync 옵션 (기본값: -avz --delete): " input_options
    SYNC_OPTIONS=${input_options:-"-avz --delete"}
    
    # 설정을 .env 파일에 저장
    read -p "이 설정을 .env 파일에 저장하시겠습니까? (y/n): " save_choice
    if [[ "$save_choice" =~ ^[Yy]$ ]]; then
        save_settings_to_env
    fi
}

# 설정을 .env 파일에 저장
save_settings_to_env() {
    ENV_FILE=".env"
    
    echo "# 백업 설정 - 생성일: $(date)" > $ENV_FILE
    echo "BACKUP_SCHEDULE=\"$BACKUP_SCHEDULE\"" >> $ENV_FILE
    echo "BACKUP_TIME=\"$BACKUP_TIME\"" >> $ENV_FILE
    echo "BACKUP_SOURCE_DIR=\"$BACKUP_SOURCE_DIR\"" >> $ENV_FILE
    echo "BACKUP_DEST_DIR=\"$BACKUP_DEST_DIR\"" >> $ENV_FILE
    echo "BACKUP_LOG_DIR=\"$BACKUP_LOG_DIR\"" >> $ENV_FILE
    echo "REMOTE_SERVER=\"$REMOTE_SERVER\"" >> $ENV_FILE
    echo "EXCLUDE_PATTERN=\"$EXCLUDE_PATTERN\"" >> $ENV_FILE
    echo "RETENTION_DAYS=$RETENTION_DAYS" >> $ENV_FILE
    echo "SYNC_OPTIONS=\"$SYNC_OPTIONS\"" >> $ENV_FILE
    
    echo -e "${GREEN}설정이 $ENV_FILE에 저장되었습니다${NC}"
}

# 백업 실행
perform_backup() {
    # 이 백업을 위한 타임스탬프 생성
    TIMESTAMP=$(date +%Y%m%d_%H%M%S)
    LOG_FILE="$BACKUP_LOG_DIR/backup_$TIMESTAMP.log"
    
    # 로그 디렉토리가 없으면 생성
    mkdir -p "$BACKUP_LOG_DIR"
    
    # 시작 시간 기록
    START_TIME=$(date +%s)
    START_TIME_HUMAN=$(date)
    echo "백업 시작 시간: $START_TIME_HUMAN" | tee -a "$LOG_FILE"
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" | tee -a "$LOG_FILE"
    
    # 제외 옵션 구성
    EXCLUDE_OPTS=""
    if [ ! -z "$EXCLUDE_PATTERN" ]; then
        IFS=',' read -ra EXCLUDE_ITEMS <<< "$EXCLUDE_PATTERN"
        for item in "${EXCLUDE_ITEMS[@]}"; do
            EXCLUDE_OPTS="$EXCLUDE_OPTS --exclude='$item'"
        done
    fi
    
    # 대상 경로 구성
    if [ ! -z "$REMOTE_SERVER" ]; then
        FULL_DEST="$REMOTE_SERVER:$BACKUP_DEST_DIR"
    else
        FULL_DEST="$BACKUP_DEST_DIR"
    fi
    
    # 백업 명령 생성
    BACKUP_CMD="rsync $SYNC_OPTIONS $EXCLUDE_OPTS \"$BACKUP_SOURCE_DIR\" \"$FULL_DEST\""
    
    echo "실행 명령: $BACKUP_CMD" | tee -a "$LOG_FILE"
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" | tee -a "$LOG_FILE"
    
    # 백업 실행 및 상세 로깅
    SUCCESS_COUNT=0
    FAIL_COUNT=0
    ERROR_LIST=""
    
    # 소스가 여러 항목이 있는 디렉토리인 경우
    if [ -d "$BACKUP_SOURCE_DIR" ]; then
        for item in "$BACKUP_SOURCE_DIR"/*; do
            item_name=$(basename "$item")
            item_size=$(du -sh "$item" 2>/dev/null | cut -f1)
            
            echo "백업 중: $item_name (크기: $item_size)" | tee -a "$LOG_FILE"
            item_start=$(date +%s)
            
            if [ ! -z "$REMOTE_SERVER" ]; then
                if rsync $SYNC_OPTIONS $EXCLUDE_OPTS "$item" "$REMOTE_SERVER:$BACKUP_DEST_DIR" 2>> "$LOG_FILE"; then
                    SUCCESS_COUNT=$((SUCCESS_COUNT + 1))
                    echo -e "${GREEN}성공${NC}: $item_name 백업 완료" | tee -a "$LOG_FILE"
                else
                    FAIL_COUNT=$((FAIL_COUNT + 1))
                    echo -e "${RED}실패${NC}: $item_name 백업 실패" | tee -a "$LOG_FILE"
                    ERROR_LIST="$ERROR_LIST\n- $item_name"
                fi
            else
                if rsync $SYNC_OPTIONS $EXCLUDE_OPTS "$item" "$BACKUP_DEST_DIR" 2>> "$LOG_FILE"; then
                    SUCCESS_COUNT=$((SUCCESS_COUNT + 1))
                    echo -e "${GREEN}성공${NC}: $item_name 백업 완료" | tee -a "$LOG_FILE"
                else
                    FAIL_COUNT=$((FAIL_COUNT + 1))
                    echo -e "${RED}실패${NC}: $item_name 백업 실패" | tee -a "$LOG_FILE"
                    ERROR_LIST="$ERROR_LIST\n- $item_name"
                fi
            fi
            
            item_end=$(date +%s)
            item_duration=$((item_end - item_start))
            echo "소요 시간: ${item_duration}초" | tee -a "$LOG_FILE"
            echo "------------------------" | tee -a "$LOG_FILE"
        done
    else
        # 단일 파일 백업
        item_name=$(basename "$BACKUP_SOURCE_DIR")
        item_size=$(du -sh "$BACKUP_SOURCE_DIR" 2>/dev/null | cut -f1)
        
        echo "백업 중: $item_name (크기: $item_size)" | tee -a "$LOG_FILE"
        item_start=$(date +%s)
        
        if [ ! -z "$REMOTE_SERVER" ]; then
            if rsync $SYNC_OPTIONS $EXCLUDE_OPTS "$BACKUP_SOURCE_DIR" "$REMOTE_SERVER:$BACKUP_DEST_DIR" 2>> "$LOG_FILE"; then
                SUCCESS_COUNT=$((SUCCESS_COUNT + 1))
                echo -e "${GREEN}성공${NC}: $item_name 백업 완료" | tee -a "$LOG_FILE"
            else
                FAIL_COUNT=$((FAIL_COUNT + 1))
                echo -e "${RED}실패${NC}: $item_name 백업 실패" | tee -a "$LOG_FILE"
                ERROR_LIST="$ERROR_LIST\n- $item_name"
            fi
        else
            if rsync $SYNC_OPTIONS $EXCLUDE_OPTS "$BACKUP_SOURCE_DIR" "$BACKUP_DEST_DIR" 2>> "$LOG_FILE"; then
                SUCCESS_COUNT=$((SUCCESS_COUNT + 1))
                echo -e "${GREEN}성공${NC}: $item_name 백업 완료" | tee -a "$LOG_FILE"
            else
                FAIL_COUNT=$((FAIL_COUNT + 1))
                echo -e "${RED}실패${NC}: $item_name 백업 실패" | tee -a "$LOG_FILE"
                ERROR_LIST="$ERROR_LIST\n- $item_name"
            fi
        fi
        
        item_end=$(date +%s)
        item_duration=$((item_end - item_start))
        echo "소요 시간: ${item_duration}초" | tee -a "$LOG_FILE"
    fi
    
    # 오래된 백업 정리
    echo "$RETENTION_DAYS일보다 오래된 백업 정리 중..." | tee -a "$LOG_FILE"
    
    if [ ! -z "$REMOTE_SERVER" ]; then
        ssh $REMOTE_SERVER "find \"$BACKUP_DEST_DIR\" -type f -mtime +$RETENTION_DAYS -delete" 2>> "$LOG_FILE"
    else
        find "$BACKUP_DEST_DIR" -type f -mtime +$RETENTION_DAYS -delete 2>> "$LOG_FILE"
    fi
    
    # 종료 시간 기록
    END_TIME=$(date +%s)
    END_TIME_HUMAN=$(date)
    DURATION=$((END_TIME - START_TIME))
    
    # 요약 표시
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" | tee -a "$LOG_FILE"
    echo "백업 요약:" | tee -a "$LOG_FILE"
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" | tee -a "$LOG_FILE"
    echo "1. 원본 디렉토리: $BACKUP_SOURCE_DIR" | tee -a "$LOG_FILE"
    echo "2. 작업 수행 내용: rsync를 사용한 파일/디렉토리 백업" | tee -a "$LOG_FILE"
    echo "3. 성공/실패 여부: $SUCCESS_COUNT 성공, $FAIL_COUNT 실패" | tee -a "$LOG_FILE"
    echo "4. 수행된 파일 저장 위치: $FULL_DEST" | tee -a "$LOG_FILE"
    echo "5. 백업 로그 저장 위치: $(realpath "$LOG_FILE")" | tee -a "$LOG_FILE"
    echo "6. 사용자 설정 파일 위치: $(realpath .env 2>/dev/null || echo '저장되지 않음')" | tee -a "$LOG_FILE"
    echo "7. 백업 시작 시간: $START_TIME_HUMAN" | tee -a "$LOG_FILE"
    echo "8. 백업 종료 시간: $END_TIME_HUMAN" | tee -a "$LOG_FILE"
    echo "9. 총 실행 시간: ${DURATION}초" | tee -a "$LOG_FILE"
    
    if [ ! -z "$ERROR_LIST" ]; then
        echo -e "${RED}백업 중 오류가 발생한 항목:${NC}" | tee -a "$LOG_FILE"
        echo -e "$ERROR_LIST" | tee -a "$LOG_FILE"
    fi
    
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" | tee -a "$LOG_FILE"
    
    # 적어도 하나의 백업이 성공하면 성공 반환
    if [ $SUCCESS_COUNT -gt 0 ]; then
        return 0
    else
        return 1
    fi
}

# 백업 스케줄 설정
setup_backup_schedule() {
    case $BACKUP_SCHEDULE in
        daily)
            CRON_SCHEDULE="0 ${BACKUP_TIME%%:*} * * *"
            ;;
        weekly)
            CRON_SCHEDULE="0 ${BACKUP_TIME%%:*} * * 0"
            ;;
        monthly)
            CRON_SCHEDULE="0 ${BACKUP_TIME%%:*} 1 * *"
            ;;
        last_saturday)
            # 매월 마지막 토요일을 위한 복잡한 표현식
            CRON_SCHEDULE="0 ${BACKUP_TIME%%:*} * * 6 [ \$(date +\\%d -d 'next saturday') -le 7 ]"
            ;;
        *)
            # 커스텀 cron 형식으로 가정
            CRON_SCHEDULE="$BACKUP_SCHEDULE"
            ;;
    esac
    
    SCRIPT_PATH=$(realpath "$0")
    
    echo -e "${BLUE}이 백업을 스케줄링하려면 crontab에 다음 줄을 추가하세요:${NC}"
    echo "$CRON_SCHEDULE $SCRIPT_PATH --run"
    echo ""
    echo -e "${YELLOW}'crontab -e' 명령으로 crontab을 편집할 수 있습니다.${NC}"
}

# 메인 실행
main() {
    # 기본 설정을 먼저 로드
    load_defaults
    
    # --run 플래그로 실행되었는지 확인 (cron 실행용)
    if [ "$1" == "--run" ]; then
        # .env에서 로드하고 프롬프트 없이 실행
        load_env_file ".env"
        perform_backup
        exit $?
    fi
    
    # 대화형 모드
    echo "=================================================="
    echo "         RSYNC 백업 스케줄링 스크립트           "
    echo "=================================================="
    
    read -p ".env 파일에서 설정을 로드하시겠습니까? (y/n): " load_choice
    
    if [[ "$load_choice" =~ ^[Yy]$ ]]; then
        read -p ".env 파일 경로를 입력하세요 (기본값: ./.env): " env_path
        env_path=${env_path:-"./.env"}
        
        if ! load_env_file "$env_path"; then
            echo -e "${YELLOW}커스텀 설정을 지정하시겠습니까? (y/n): ${NC}"
            read setup_choice
            
            if [[ "$setup_choice" =~ ^[Yy]$ ]]; then
                prompt_custom_settings
            else
                echo -e "${GREEN}기본 설정을 사용합니다.${NC}"
            fi
        fi
    else
        prompt_custom_settings
    fi
    
    # 현재 설정 표시
    echo "=================================================="
    echo "현재 백업 설정:"
    echo "=================================================="
    echo "일정: $BACKUP_SCHEDULE"
    echo "시간: $BACKUP_TIME"
    echo "소스: $BACKUP_SOURCE_DIR"
    echo "대상: $BACKUP_DEST_DIR"
    if [ ! -z "$REMOTE_SERVER" ]; then
        echo "원격 서버: $REMOTE_SERVER"
    fi
    echo "로그 디렉토리: $BACKUP_LOG_DIR"
    echo "보존 기간: $RETENTION_DAYS 일"
    echo "=================================================="
    
    # 작업 선택 프롬프트
    echo -e "${YELLOW}무엇을 하시겠습니까?${NC}"
    echo "1. 지금 백업 실행"
    echo "2. 백업 일정 설정 (crontab)"
    echo "3. 설정 편집"
    echo "4. 종료"
    read -p "옵션을 선택하세요 (1-4): " action_choice
    
    case $action_choice in
        1)
            perform_backup
            ;;
        2)
            setup_backup_schedule
            ;;
        3)
            prompt_custom_settings
            main
            ;;
        4)
            echo "종료합니다."
            exit 0
            ;;
        *)
            echo -e "${RED}유효하지 않은 선택입니다. 종료합니다.${NC}"
            exit 1
            ;;
    esac
}

# 스크립트 시작
main "$@"
