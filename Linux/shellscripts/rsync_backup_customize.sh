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
    REMOTE_SERVER=""
    REMOTE_HOST=""
    REMOTE_PORT="22"
    REMOTE_USER=""
    IDENTITY_FILE=""
    USE_SSHD_CONFIG="yes"
    EXCLUDE_PATTERN=""
    RETENTION_DAYS=30
    SYNC_OPTIONS="-avz --delete"
}

# ~/.ssh/config 파일에서 호스트 정보 추출
read_ssh_config() {
    local config_file="$HOME/.ssh/config"
    local hosts=()
    
    if [ ! -f "$config_file" ]; then
        echo -e "${YELLOW}경고: ~/.ssh/config 파일이 존재하지 않습니다.${NC}"
        return 1
    fi
    
    # Host 항목 추출
    while read -r line; do
        # 주석과 빈 줄 제외
        if [[ "$line" =~ ^[[:space:]]*# ]] || [[ -z "${line// }" ]]; then
            continue
        fi
        
        if [[ "$line" =~ ^[[:space:]]*Host[[:space:]]+([^*][^[:space:]]+) ]]; then
            # 와일드카드(*) 포함하지 않는 호스트만 추가
            local host_name="${BASH_REMATCH[1]}"
            if [[ ! "$host_name" =~ \* ]]; then
                hosts+=("$host_name")
            fi
        fi
    done < "$config_file"
    
    if [ ${#hosts[@]} -eq 0 ]; then
        echo -e "${YELLOW}경고: ~/.ssh/config 파일에 유효한 Host 항목이 없습니다.${NC}"
        return 1
    fi
    
    echo -e "${BLUE}~/.ssh/config 파일에서 가져온 호스트 목록:${NC}"
    for i in "${!hosts[@]}"; do
        echo "$(($i+1))) ${hosts[$i]}"
    done
    
    read -p "호스트를 선택하세요 (1-${#hosts[@]}): " host_choice
    
    if ! [[ "$host_choice" =~ ^[0-9]+$ ]] || [ "$host_choice" -lt 1 ] || [ "$host_choice" -gt ${#hosts[@]} ]; then
        echo -e "${RED}유효하지 않은 선택입니다.${NC}"
        return 1
    fi
    
    selected_host="${hosts[$((host_choice-1))]}"
    echo -e "${GREEN}선택된 호스트: $selected_host${NC}"
    
    # 호스트 정보 추출
    get_host_info "$selected_host" "$config_file"
    
    return 0
}

# 선택한 호스트의 세부 정보 추출
get_host_info() {
    local host="$1"
    local config_file="$2"
    local in_host_section=false
    local hostname=""
    local port=""
    local user=""
    local identity_file=""
    
    while read -r line; do
        # 모든 선행/후행 공백 제거
        line=$(echo "$line" | xargs)
        
        # 주석과 빈 줄 제외
        if [[ "$line" =~ ^# ]] || [[ -z "$line" ]]; then
            continue
        fi
        
        # 호스트 섹션 시작 확인
        if [[ "$line" =~ ^Host[[:space:]]+(.*) ]]; then
            # 정확한 호스트 매칭을 위해 호스트 이름을 단어 단위로 분리하여 확인
            local hosts_in_line=(${BASH_REMATCH[1]})
            in_host_section=false
            
            for h in "${hosts_in_line[@]}"; do
                if [[ "$h" == "$host" ]]; then
                    in_host_section=true
                    break
                fi
            done
            continue
        fi
        
        # 현재 호스트 섹션 내에서 정보 추출
        if [ "$in_host_section" = true ]; then
            if [[ "$line" =~ ^HostName[[:space:]]+(.+)$ ]]; then
                hostname="${BASH_REMATCH[1]}"
            elif [[ "$line" =~ ^Port[[:space:]]+(.+)$ ]]; then
                port="${BASH_REMATCH[1]}"
            elif [[ "$line" =~ ^User[[:space:]]+(.+)$ ]]; then
                user="${BASH_REMATCH[1]}"
            elif [[ "$line" =~ ^IdentityFile[[:space:]]+(.+)$ ]]; then
                identity_file="${BASH_REMATCH[1]}"
            fi
        fi
    done < "$config_file"
    
    # 추출한 정보를 설정 변수에 적용
    if [ ! -z "$hostname" ]; then
        REMOTE_HOST="$hostname"
        echo "호스트명: $REMOTE_HOST"
    else
        REMOTE_HOST="$host"
        echo "호스트명: $REMOTE_HOST (Host 항목에서 가져옴)"
    fi
    
    if [ ! -z "$port" ]; then
        REMOTE_PORT="$port"
        echo "포트: $REMOTE_PORT"
    else
        REMOTE_PORT="22"
        echo "포트: $REMOTE_PORT (기본값)"
    fi
    
    if [ ! -z "$user" ]; then
        REMOTE_USER="$user"
        echo "사용자: $REMOTE_USER"
    fi
    
    if [ ! -z "$identity_file" ]; then
        # ~/ 경로를 $HOME으로 확장
        IDENTITY_FILE="${identity_file/#\~\//$HOME/}"
        echo "인증 파일: $IDENTITY_FILE"
    fi
    
    # REMOTE_SERVER 구성
    if [ ! -z "$REMOTE_USER" ] && [ ! -z "$REMOTE_HOST" ]; then
        REMOTE_SERVER="${REMOTE_USER}@${REMOTE_HOST}"
    elif [ ! -z "$REMOTE_HOST" ]; then
        REMOTE_SERVER="$REMOTE_HOST"
    fi
    
    echo "최종 원격 서버 설정: $REMOTE_SERVER"
    
    return 0
}

prompt_ssh_settings() {
    echo -e "${BLUE}SSH 연결 설정:${NC}"
    echo "1. ~/.ssh/config 파일에서 정보 가져오기"
    echo "2. 직접 설정하기"
    read -p "옵션을 선택하세요 (1-2): " ssh_choice
    
    if [ "$ssh_choice" == "1" ]; then
        # ~/.ssh/config 파일에서 호스트 선택
        if read_ssh_config; then
            echo -e "${GREEN}SSH config에서 서버 설정을 가져왔습니다.${NC}"
        else
            echo -e "${YELLOW}SSH config에서 서버 선택에 실패했습니다. 직접 입력해주세요.${NC}"
            ssh_choice="2"  # 직접 입력으로 전환
        fi
    fi
    
    if [ "$ssh_choice" == "2" ]; then
        # 직접 입력 방식
        read -p "원격 서버 (user@hostname): " REMOTE_SERVER
        read -p "SSH 포트 (기본값: 22): " input_port
        REMOTE_PORT=${input_port:-"22"}
        
        echo -e "${YELLOW}인증 방식:${NC}"
        echo "1. 비밀번호 인증"
        echo "2. 공개키(PublicKey) 인증"
        read -p "인증 방식을 선택하세요 (1-2): " auth_choice
        
        if [ "$auth_choice" == "2" ]; then
            read -p "Identity 파일 경로 (예: ~/.ssh/id_rsa): " IDENTITY_FILE
            if [ -z "$IDENTITY_FILE" ]; then
                echo -e "${YELLOW}Identity 파일이 지정되지 않았습니다. 기본 SSH 키를 사용합니다.${NC}"
            elif [ ! -f "$IDENTITY_FILE" ]; then
                echo -e "${RED}지정한 Identity 파일을 찾을 수 없습니다: $IDENTITY_FILE${NC}"
                return 1
            fi
        fi
    fi
    
    return 0
}

# 원격 디렉토리 접근성 검증
check_remote_directory() {
    local host="$1"
    local dir="$2"
    local port="$3"
    local identity=""
    
    if [ ! -z "$IDENTITY_FILE" ]; then
        identity="-i $IDENTITY_FILE"
    fi
    
    echo -e "${BLUE}원격 디렉토리 $dir 접근성을 확인 중...${NC}"
    
    # 디렉토리 존재 여부 확인
    if ! ssh $identity -p "$port" "$host" "test -d $dir"; then
        echo -e "${YELLOW}원격 디렉토리 $dir가 존재하지 않습니다. 생성을 시도합니다...${NC}"
        
        # 디렉토리 생성 시도
        if ! ssh $identity -p "$port" "$host" "mkdir -p $dir"; then
            echo -e "${RED}오류: 원격 디렉토리를 생성할 수 없습니다. 권한을 확인하세요.${NC}"
            return 1
        fi
        echo -e "${GREEN}원격 디렉토리를 성공적으로 생성했습니다.${NC}"
    fi
    
    # 디렉토리 쓰기 권한 확인
    if ! ssh $identity -p "$port" "$host" "touch $dir/.test_write && rm $dir/.test_write"; then
        echo -e "${RED}오류: 원격 디렉토리에 쓰기 권한이 없습니다.${NC}"
        return 1
    fi
    
    echo -e "${GREEN}원격 디렉토리 접근 및 쓰기 권한 확인 완료.${NC}"
    return 0
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

# SSH 설정 커스텀 프롬프트
prompt_ssh_settings() {
    echo -e "${BLUE}SSH 연결 설정:${NC}"
    echo "1. 원격 서버 sshd_config 파일에서 정보 가져오기"
    echo "2. 직접 설정하기"
    read -p "옵션을 선택하세요 (1-2): " ssh_choice
    
    if [ "$ssh_choice" == "1" ]; then
        USE_SSHD_CONFIG="yes"
    else
        USE_SSHD_CONFIG="no"
        read -p "SSH 포트 (기본값: 22): " input_port
        REMOTE_PORT=${input_port:-"22"}
        
        echo -e "${YELLOW}인증 방식:${NC}"
        echo "1. 비밀번호 인증"
        echo "2. 공개키(PublicKey) 인증"
        read -p "인증 방식을 선택하세요 (1-2): " auth_choice
        
        if [ "$auth_choice" == "2" ]; then
            read -p "Identity 파일 경로 (예: ~/.ssh/id_rsa): " IDENTITY_FILE
            if [ -z "$IDENTITY_FILE" ]; then
                echo -e "${YELLOW}Identity 파일이 지정되지 않았습니다. 기본 SSH 키를 사용합니다.${NC}"
            elif [ ! -f "$IDENTITY_FILE" ]; then
                echo -e "${RED}지정한 Identity 파일을 찾을 수 없습니다: $IDENTITY_FILE${NC}"
                return 1
            fi
        fi
    fi
    
    return 0
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
        echo -e "${YELLOW}원격 서버 선택 방법:${NC}"
        echo "1. ~/.ssh/config 파일에서 선택"
        echo "2. 직접 입력"
        read -p "옵션을 선택하세요 (1-2): " server_choice
        
        if [ "$server_choice" == "1" ]; then
            if read_ssh_config; then
                echo -e "${GREEN}SSH config에서 서버 설정을 가져왔습니다.${NC}"
                read -p "원격 서버의 대상 디렉토리: " BACKUP_DEST_DIR
            else
                echo -e "${YELLOW}SSH config에서 서버 선택에 실패했습니다. 직접 입력해주세요.${NC}"
                read -p "원격 서버 (user@hostname): " REMOTE_SERVER
                read -p "원격 서버의 대상 디렉토리: " BACKUP_DEST_DIR
                # SSH 설정 프롬프트
                prompt_ssh_settings
            fi
        else
            read -p "원격 서버 (user@hostname): " REMOTE_SERVER
            read -p "원격 서버의 대상 디렉토리: " BACKUP_DEST_DIR
            # SSH 설정 프롬프트
            prompt_ssh_settings
        fi
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
    echo "REMOTE_HOST=\"$REMOTE_HOST\"" >> $ENV_FILE
    echo "REMOTE_PORT=\"$REMOTE_PORT\"" >> $ENV_FILE
    echo "REMOTE_USER=\"$REMOTE_USER\"" >> $ENV_FILE
    echo "USE_SSHD_CONFIG=\"$USE_SSHD_CONFIG\"" >> $ENV_FILE
    echo "IDENTITY_FILE=\"$IDENTITY_FILE\"" >> $ENV_FILE
    echo "EXCLUDE_PATTERN=\"$EXCLUDE_PATTERN\"" >> $ENV_FILE
    echo "RETENTION_DAYS=$RETENTION_DAYS" >> $ENV_FILE
    echo "SYNC_OPTIONS=\"$SYNC_OPTIONS\"" >> $ENV_FILE
    
    echo -e "${GREEN}설정이 $ENV_FILE에 저장되었습니다${NC}"
}

# 백업 실행
perform_backup() {
    # 기존 백업 코드 (변경 없음)
    # ...

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
    
    # 원격 서버 설정 확인 및 검증
    if [ ! -z "$REMOTE_SERVER" ]; then
        # SSH 포트 및 인증 설정
        local ssh_opts=""
        local rsync_opts=""
        
        # sshd_config에서 설정 가져오기
        if [ "$USE_SSHD_CONFIG" == "yes" ]; then
            prompt_ssh_settings "$REMOTE_SERVER" || echo -e "${YELLOW}SSH 설정을 가져오는 데 실패했습니다. 기본값을 사용합니다.${NC}" | tee -a "$LOG_FILE"
        fi
        
        # SSH 포트 설정
        ssh_opts="-p $REMOTE_PORT"
        rsync_opts="-e \"ssh -p $REMOTE_PORT"
        
        # Identity 파일 설정
        if [ ! -z "$IDENTITY_FILE" ]; then
            ssh_opts="$ssh_opts -i $IDENTITY_FILE"
            rsync_opts="$rsync_opts -i $IDENTITY_FILE"
        fi
        
        rsync_opts="$rsync_opts\""
        
        # 원격 디렉토리 접근성 확인
        echo "원격 서버 디렉토리 접근성 검증 중..." | tee -a "$LOG_FILE"
        if ! check_remote_directory "$REMOTE_SERVER" "$BACKUP_DEST_DIR" "$REMOTE_PORT"; then
            echo -e "${RED}오류: 원격 디렉토리에 접근할 수 없습니다. 백업을 중단합니다.${NC}" | tee -a "$LOG_FILE"
            END_TIME=$(date +%s)
            DURATION=$((END_TIME - START_TIME))
            echo "백업 실행 시간: ${DURATION}초" | tee -a "$LOG_FILE"
            echo "백업 상태: 실패 (원격 디렉토리 접근 불가)" | tee -a "$LOG_FILE"
            return 1
        fi
    fi
    
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
    if [ ! -z "$REMOTE_SERVER" ]; then
        BACKUP_CMD="rsync $SYNC_OPTIONS $rsync_opts $EXCLUDE_OPTS \"$BACKUP_SOURCE_DIR\" \"$FULL_DEST\""
    else
        BACKUP_CMD="rsync $SYNC_OPTIONS $EXCLUDE_OPTS \"$BACKUP_SOURCE_DIR\" \"$FULL_DEST\""
    fi
    
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
                if eval rsync $SYNC_OPTIONS $rsync_opts $EXCLUDE_OPTS \"$item\" \"$FULL_DEST\" 2>> "$LOG_FILE"; then
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
            if eval rsync $SYNC_OPTIONS $rsync_opts $EXCLUDE_OPTS \"$BACKUP_SOURCE_DIR\" \"$FULL_DEST\" 2>> "$LOG_FILE"; then
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
        ssh $ssh_opts $REMOTE_SERVER "find \"$BACKUP_DEST_DIR\" -type f -mtime +$RETENTION_DAYS -delete" 2>> "$LOG_FILE"
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
        echo "원격 포트: $REMOTE_PORT"
        if [ ! -z "$IDENTITY_FILE" ]; then
            echo "인증 파일: $IDENTITY_FILE"
        fi
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
