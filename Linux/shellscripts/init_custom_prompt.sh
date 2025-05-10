#!/bin/bash

# /etc/profile.d/custom-prompt.sh
# 이 스크립트는 사용자에게 유용한 정보를 제공하는 커스텀 프롬프트를 설정합니다.

# 색상 설정
RED="\[\033[0;31m\]"
GREEN="\[\033[0;32m\]"
YELLOW="\[\033[0;33m\]"
BLUE="\[\033[0;34m\]"
RESET="\[\033[0m\]"

# echo 명령어용 색상
PRED="\033[0;31m"
PGREEN="\033[0;32m"
PYELLOW="\033[0;33m"
PBLUE="\033[0;34m"
PCYAN="\033[0;36m"
PRESET="\033[0m"

# 서비스가 존재하고 실행 중인지 확인하는 함수
check_service() {
    local service_name="$1"
    if systemctl list-units --full -all | grep -Fq "$service_name.service"; then
        if systemctl is-active --quiet "$service_name"; then
            echo -e "${PGREEN}[+] $service_name 서비스가 실행 중입니다.${PRESET}"
        else
            echo -e "${PRED}[-] $service_name 서비스가 중지되었습니다.${PRESET}"
        fi
    else
        echo -e "${PYELLOW}[?] $service_name 서비스가 존재하지 않습니다.${PRESET}"
    fi
}

# 주요 서비스 상태를 표시하는 함수
show_services_status() {
    echo -e "${PCYAN}===== 주요 서비스 상태 =====${PRESET}"
    # 확인할 주요 서비스 목록
    local services=("ssh" "apache2" "nginx" "mysql" "docker")
    
    for service in "${services[@]}"; do
        check_service "$service"
    done
    echo -e "${PCYAN}==========================${PRESET}"
}

# 현재 디렉토리에 대한 상세 정보를 표시하는 함수
show_directory_info() {
    local current_dir=$(pwd)
    echo -e "${PCYAN}===== 디렉토리 정보 =====${PRESET}"
    echo -e "${PGREEN}현재 디렉토리:${PRESET} $current_dir"
    echo -e "${PGREEN}소유자:${PRESET} $(ls -ld "$current_dir" | awk '{print $3}')"
    echo -e "${PGREEN}그룹:${PRESET} $(ls -ld "$current_dir" | awk '{print $4}')"
    echo -e "${PGREEN}권한:${PRESET} $(ls -ld "$current_dir" | awk '{print $1}')"
    
    # 파일 및 디렉토리 수 계산
    local file_count=$(find "$current_dir" -maxdepth 1 -type f | wc -l)
    local dir_count=$(find "$current_dir" -maxdepth 1 -type d | wc -l)
    
    # 현재 디렉토리 자체를 제외하기 위해 dir_count에서 1 빼기
    dir_count=$((dir_count - 1))
    
    echo -e "${PGREEN}파일 수:${PRESET} $file_count"
    echo -e "${PGREEN}디렉토리 수:${PRESET} $dir_count"
    
    # 디스크 사용량 표시
    echo -e "${PGREEN}디스크 사용량:${PRESET}"
    df -h . | tail -n 1 | awk '{print "  총 크기: " $2 ", 사용: " $3 " (" $5 "), 여유: " $4}'
    
    echo -e "${PCYAN}========================${PRESET}"
}

# 특정 디렉토리에 대한 설명을 제공하는 함수
explain_directory() {
    local dir_name="$1"
    
    echo -e "${PCYAN}===== 디렉토리 설명 =====${PRESET}"
    
    case "$dir_name" in
        "/")
            echo -e "${PGREEN}루트 디렉토리 (/)${PRESET}"
            echo -e "  - 시스템의 최상위 디렉토리입니다."
            echo -e "  - 모든 파일 시스템의 시작점이며, 다른 모든 디렉토리가 여기서부터 시작됩니다."
            ;;
        "/etc")
            echo -e "${PGREEN}/etc 디렉토리${PRESET}"
            echo -e "  - 시스템 구성 파일이 저장되는 곳입니다."
            echo -e "  - 대부분의 설정 파일이 이 디렉토리에 있습니다."
            echo -e "  - 주요 파일: /etc/passwd, /etc/fstab, /etc/hosts 등"
            ;;
        "/var")
            echo -e "${PGREEN}/var 디렉토리${PRESET}"
            echo -e "  - 가변 데이터가 저장되는 곳입니다."
            echo -e "  - 로그 파일, 메일 큐, 스풀 영역 등이 있습니다."
            echo -e "  - 시스템 실행 중에 크기가 변하는 파일들이 저장됩니다."
            ;;
        "/home")
            echo -e "${PGREEN}/home 디렉토리${PRESET}"
            echo -e "  - 일반 사용자의 홈 디렉토리가 있는 곳입니다."
            echo -e "  - 각 사용자는 /home/사용자명 형태의 디렉토리를 갖습니다."
            echo -e "  - 사용자의 개인 파일과 설정이 저장됩니다."
            ;;
        *)
            echo -e "${PYELLOW}이 디렉토리에 대한 설명이 없습니다.${PRESET}"
            ;;
    esac
    
    echo -e "${PCYAN}======================${PRESET}"
}

# 시스템 자원 정보를 표시하는 함수
show_system_resources() {
    echo -e "${PCYAN}===== 시스템 자원 정보 =====${PRESET}"
    
    # CPU 정보
    echo -e "${PGREEN}CPU 정보:${PRESET}"
    echo -e "  $(grep "model name" /proc/cpuinfo | head -n1 | cut -d ':' -f2 | sed 's/^[ \t]*//')"
    echo -e "  코어 수: $(grep -c processor /proc/cpuinfo)"
    
    # 메모리 정보
    echo -e "${PGREEN}메모리 정보:${PRESET}"
    free -h | grep "Mem" | awk '{print "  총 크기: " $2 ", 사용: " $3 ", 여유: " $4}'
    
    # 디스크 정보
    echo -e "${PGREEN}디스크 정보:${PRESET}"
    df -h / | tail -n 1 | awk '{print "  총 크기: " $2 ", 사용: " $3 " (" $5 "), 여유: " $4}'
    
    # 시스템 부하
    echo -e "${PGREEN}시스템 부하:${PRESET} $(cat /proc/loadavg | awk '{print $1 ", " $2 ", " $3}')"
    
    echo -e "${PCYAN}==========================${PRESET}"
}

# 네트워크 정보를 표시하는 함수
show_network_info() {
    echo -e "${PCYAN}===== 네트워크 정보 =====${PRESET}"
    
    # IP 주소
    echo -e "${PGREEN}IP 주소:${PRESET}"
    ip -4 addr show | grep inet | grep -v "127.0.0.1" | awk '{print "  " $NF ": " $2}'
    
    # 기본 게이트웨이
    echo -e "${PGREEN}기본 게이트웨이:${PRESET}"
    ip route | grep default | awk '{print "  " $3 " (" $5 ")"}'
    
    # DNS 서버
    echo -e "${PGREEN}DNS 서버:${PRESET}"
    cat /etc/resolv.conf | grep nameserver | awk '{print "  " $2}'
    
    echo -e "${PCYAN}========================${PRESET}"
}

# 도움말 정보를 제공하는 함수
show_help() {
    echo -e "${PCYAN}===== 도움말 =====${PRESET}"
    echo -e "${PGREEN}사용 가능한 명령어:${PRESET}"
    echo -e "  ${PYELLOW}services${PRESET} - 주요 서비스 상태 표시"
    echo -e "  ${PYELLOW}dirinfo${PRESET} - 현재 디렉토리 정보 표시"
    echo -e "  ${PYELLOW}sysinfo${PRESET} - 시스템 자원 정보 표시"
    echo -e "  ${PYELLOW}netinfo${PRESET} - 네트워크 정보 표시"
    echo -e "  ${PYELLOW}dirhelp${PRESET} [디렉토리] - 특정 디렉토리에 대한 설명 (예: dirhelp /etc)"
    echo -e "  ${PYELLOW}help${PRESET} - 이 도움말 표시"
    echo -e "${PCYAN}=====================${PRESET}"
}

# 환영 메시지를 표시하는 함수
show_welcome() {
    local username=$(whoami)
    local hostname=$(hostname)
    local date_time=$(date +"%Y-%m-%d %H:%M:%S")
    local os_info=$(cat /etc/os-release | grep PRETTY_NAME | cut -d '=' -f 2 | tr -d '"')
    local kernel=$(uname -r)
    
    echo -e "${PCYAN}========================================${PRESET}"
    echo -e "${PGREEN}환영합니다, ${PYELLOW}${username}${PGREEN}님!${PRESET}"
    echo -e "${PGREEN}현재 시간:${PRESET} ${date_time}"
    echo -e "${PGREEN}OS:${PRESET} ${os_info}"
    echo -e "${PGREEN}커널:${PRESET} ${kernel}"
    echo ""
    echo -e "${PYELLOW}주요 명령어 도움말을 보려면 'help'를 입력하세요.${PRESET}"
    echo -e "${PCYAN}========================================${PRESET}"
}

# 사용자 정의 함수에 대한 별칭 생성
alias services='show_services_status'
alias dirinfo='show_directory_info'
alias sysinfo='show_system_resources'
alias netinfo='show_network_info'
alias help='show_help'
alias dirhelp='explain_directory'

# git 저장소에 있는 경우 프롬프트에 git 브랜치를 표시하는 함수
parse_git_branch() {
    git branch 2> /dev/null | sed -e '/^[^*]/d' -e 's/* \(.*\)/(\1)/'
}

# 현재 작업 디렉토리의 마지막 부분만 표시하는 함수
prompt_dir() {
    local pwd=$(pwd)
    local home=$HOME
    local result=${pwd/#$home/\~}
    
    # 경로가 3단계 이상인 경우 축약
    if [[ "$result" == */* && "$result" != \~/* ]]; then
        local dirs=(${result//\// })
        if [ ${#dirs[@]} -gt 2 ]; then
            result="…/${dirs[-2]}/${dirs[-1]}"
        fi
    fi
    
    echo "$result"
}

# 질문에서 요청한 대로 간단한 프롬프트 설정
PS1="$ "

# 환영 메시지 표시
show_welcome

# 로그인 시 주요 서비스 상태 표시
show_services_status
