#!/bin/bash
#==============================================================================
# 고급 파일 병합 스크립트 (Professional Edition)
# 기능: 파일 검색/병합, 디렉토리 크기 검사, 로깅, 자동 테스트
# 사용법: ./concat.sh [옵션] 또는 대화형 메뉴
#==============================================================================

#==============================================================================
# 초기화 
#==============================================================================
set -eo pipefail                                # 오류 발생 시 즉시 종료
VERSION="2.0.0"
SCRIPT_NAME=$(basename "$0")
CONFIG_DIR="${HOME}/config_concat"              # 설정 저장 디렉토리
LOG_DIR="${CONFIG_DIR}/logs"                    # 로그 저장 디렉토리
MAX_LOG_FILES=10                                # 최대 보관 로그 파일 수
recursive_mode=false                            # 재귀 모드 (기본값: 비활성화)
excluded_extensions=("*.o" "*.pyc" "*.class" "*.cache")  # 기본 제외 확장자

#==============================================================================
# 로깅 시스템
#==============================================================================
setup_logging() {
    mkdir -p "${LOG_DIR}"                       # 로그 디렉토리 생성
    LOG_FILE="${LOG_DIR}/${SCRIPT_NAME}_$(date +%Y%m%d_%H%M%S).log"
    echo "# Log started at $(date)" > "${LOG_FILE}"

    # 오래된 로그 파일 정리 (최대 10개 유지)
    local log_count=$(find "${LOG_DIR}" -type f -name "${SCRIPT_NAME}_*.log" | wc -l)
    if [ "$log_count" -gt "$MAX_LOG_FILES" ]; then
        find "${LOG_DIR}" -type f -name "${SCRIPT_NAME}_*.log" | sort | head -n $(( log_count - MAX_LOG_FILES )) | xargs rm -f
    fi
}

# 로그 레벨별 함수 (로그 파일과 콘솔에 동시 출력)
log_debug() { echo "[$(date +"%H:%M:%S")] [DEBUG] $1" | tee -a "${LOG_FILE}"; }
log_info() { echo "[$(date +"%H:%M:%S")] [INFO] $1" | tee -a "${LOG_FILE}"; }
log_warning() { echo "[$(date +"%H:%M:%S")] [WARNING] $1" | tee -a "${LOG_FILE}"; }
log_error() { echo "[$(date +"%H:%M:%S")] [ERROR] $1" | tee -a "${LOG_FILE}"; }

#==============================================================================
# 오류 처리 함수
#==============================================================================
error_exit() {
    log_error "$1"
    exit "${2:-1}"
}

#==============================================================================
# 기본값 설정
#==============================================================================
initialize_defaults() {
    start_dir="."                               # 기본 시작 디렉토리
    output_file="merged_$(date +%Y%m%d_%H%M%S).txt"  # 출력 파일명
    exclude_dirs=()                             # 제외 디렉토리 배열
    file_formats=("*.py")                       # 기본 검색 형식
    size_limit="10M"                            # 디렉토리 크기 제한
}

#==============================================================================
# 디렉토리 크기 체크
#==============================================================================
get_directory_size() {
    local dir="$1"
    # du 명령으로 디렉토리 크기 측정 (KB 단위)
    du -sk "$dir" 2>/dev/null | cut -f1
}

is_directory_too_large() {
    local dir="$1"
    local limit="$2"

    # 단위 해석 (M: 메가바이트, K: 킬로바이트, G: 기가바이트)
    local limit_kb
    case "${limit: -1}" in
        M|m) limit_kb=$((${limit%[Mm]} * 1024)) ;;   # MB -> KB 변환
        G|g) limit_kb=$((${limit%[Gg]} * 1024 * 1024)) ;; # GB -> KB 변환
        K|k) limit_kb=${limit%[Kk]} ;;          # KB 유지
        *) limit_kb=$limit ;;                   # 단위 없으면 KB로 간주
    esac

    local dir_size=$(get_directory_size "$dir")
    log_debug "디렉토리 $dir 크기: ${dir_size}KB, 제한: ${limit_kb}KB"

    # 크기 비교 (크기 초과 시 0(true) 반환)
    [ "$dir_size" -gt "$limit_kb" ]
}

#==============================================================================
# 파일 검색 엔진
#==============================================================================
find_files() {
    local find_args=("$start_dir")
    local exclude_pattern=()

    # 제외 디렉토리 처리
    for dir in "${exclude_dirs[@]}"; do
        find_args+=(-not -path "$dir/*")        # 제외 디렉토리 하위 모두 제외
    done

    # 크기 제한 초과 디렉토리 찾기
    log_info "디렉토리 크기 검사 중..."
    local large_dirs=()
    while IFS= read -r -d $'\0' dir; do
        if is_directory_too_large "$dir" "$size_limit"; then
            large_dirs+=("$dir")
            log_warning "크기 제한 초과 디렉토리: $dir"
        fi
    done < <(find "$start_dir" -type d -print0)

    # 크기 제한 초과 디렉토리 제외
    for dir in "${large_dirs[@]}"; do
        find_args+=(-not -path "$dir/*")
    done

    # 파일 형식 필터링
    find_args+=(-type f)

    # 다중 파일 형식 지원
    if [ ${#file_formats[@]} -gt 1 ]; then
        find_args+=("(")
        for format in "${file_formats[@]}"; do
            find_args+=(-name "$format" -o)
        done
        # 마지막 -o 제거
        unset 'find_args[${#find_args[@]}-1]'
        find_args+=(")")
    else
        find_args+=(-name "${file_formats[0]}")
    fi

    # 제외 확장자 처리
    if [ ${#excluded_extensions[@]} -gt 0 ]; then
        find_args+=("!")
        find_args+=("(")
        for ext in "${excluded_extensions[@]}"; do
            find_args+=(-name "$ext" -o)
        done
        # 마지막 -o 제거
        unset 'find_args[${#find_args[@]}-1]'
        find_args+=(")")
    fi

    # 디버그용 명령어 출력
    log_debug "실행 명령: find ${find_args[*]} -print0"

    # 실행 및 결과 반환 (null 종료 문자로 구분)
    find "${find_args[@]}" -print0
}

#==============================================================================
# 설정 파일 관리
#==============================================================================
manage_config() {
    local action=$1
    local config_name=$2
    local config_path="${CONFIG_DIR}/${config_name}.cfg"

    case $action in
        save)
            mkdir -p "$CONFIG_DIR"              # 디렉토리 생성 (없는 경우)
            cat > "$config_path" << EOF
# Concat Script Config - $(date)
start_dir="$start_dir"
output_file="$output_file"
exclude_dirs=(${exclude_dirs[*]})
file_formats=(${file_formats[*]})
size_limit="$size_limit"
EOF
            log_info "설정 저장됨: $config_path"
            ;;
        load)
            if [[ -f "$config_path" ]]; then
                log_info "설정 불러오는 중: $config_path"
                source "$config_path"            # 설정 파일 소스로 불러오기
            else
                error_exit "설정 파일 없음: ${config_path}" 3
            fi
            ;;
        list)
            find "${CONFIG_DIR}" -name "*.cfg" -printf "%f\n" | sed 's/\.cfg$//'
            ;;
    esac
}

#==============================================================================
# 강화된 출력 포맷팅
#==============================================================================
format_file_header() {
    local file="$1"
    local file_size=$(du -h "$file" 2>/dev/null | cut -f1)
    local last_modified=$(stat -c "%y" "$file" 2>/dev/null || date -r "$file" 2>/dev/null || echo "Unknown")

    # 미적으로 디자인된 헤더 포맷 (파일 메타데이터 포함)
    cat << EOF

#==============================================================================
# 파일: $(basename "$file")
# 경로: $(dirname "$file")
# 크기: ${file_size}
# 수정: ${last_modified}
#==============================================================================

EOF
}

#==============================================================================
# 테스트 기능 구현
#==============================================================================
enhanced_test() {
    clear
    log_info "고급 테스트 및 진단 시작"
    echo "=========================================="
    echo "   고급 테스트 및 진단"
    echo "=========================================="

    # 1. 필수 명령어 검사
    echo "필수 명령어 검사 중..."
    local required_commands=("find" "cat" "du" "stat" "tee")
    local missing_commands=()

    for cmd in "${required_commands[@]}"; do
        if ! command -v "$cmd" &> /dev/null; then
            missing_commands+=("$cmd")
        fi
    done

    if [ ${#missing_commands[@]} -gt 0 ]; then
        log_error "필수 명령어 누락: ${missing_commands[*]}"
        echo "❌ 필수 명령어 누락: ${missing_commands[*]}"
    else
        echo "✓ 필수 명령어 검사 통과"
    fi

    # 2. 디렉토리 접근 권한 검사
    echo "디렉토리 접근 권한 검사 중..."
    if [ ! -r "$start_dir" ]; then
        log_error "시작 디렉토리 읽기 권한 없음: $start_dir"
        echo "❌ 시작 디렉토리 읽기 권한 없음: $start_dir"
    else
        echo "✓ 디렉토리 접근 권한 검사 통과"
    fi

    # 3. 출력 파일 쓰기 권한 검사
    echo "출력 파일 쓰기 권한 검사 중..."
    local output_dir=$(dirname "$output_file")
    if [ ! -w "$output_dir" ]; then
        log_error "출력 디렉토리 쓰기 권한 없음: $output_dir"
        echo "❌ 출력 디렉토리 쓰기 권한 없음: $output_dir"
    else
        echo "✓ 출력 파일 쓰기 권한 검사 통과"
    fi

    # 4. 파일 찾기 테스트
    echo "파일 검색 테스트 중..."
    local test_count=0
    local test_files=()

    while IFS= read -r -d $'\0' file; do
        test_files+=("$file")
        ((test_count++))
        if [ "$test_count" -le 5 ]; then
            echo "  발견: $file (크기: $(du -h "$file" 2>/dev/null | cut -f1))"
        fi
    done < <(find_files)

    if [ "$test_count" -eq 0 ]; then
        log_warning "검색 조건에 맞는 파일 없음"
        echo "⚠️ 검색 조건에 맞는 파일이 없습니다"
    else
        local display_count="$test_count"
        if [ "$test_count" -gt 5 ]; then
            echo "  ... 외 $((test_count - 5))개 파일"
        fi
        echo "✓ 파일 검색 테스트 완료 ($test_count 파일 발견)"
    fi

    # 5. 크기 제한 초과 디렉토리 검사
    echo "디렉토리 크기 검사 중..."
    local large_dirs=0

    while IFS= read -r -d $'\0' dir; do
        if is_directory_too_large "$dir" "$size_limit"; then
            ((large_dirs++))
            if [ "$large_dirs" -le 3 ]; then
                local dir_size=$(du -sh "$dir" 2>/dev/null | cut -f1)
                echo "  대용량 디렉토리: $dir ($dir_size, 제한: $size_limit)"
            fi
        fi
    done < <(find "$start_dir" -type d -print0)

    if [ "$large_dirs" -gt 0 ]; then
        if [ "$large_dirs" -gt 3 ]; then
            echo "⚠️ 크기 제한 초과 디렉토리: 총 $large_dirs개 (상위 3개만 표시)"
        else
            echo "⚠️ 크기 제한 초과 디렉토리: $large_dirs개"
        fi
    else
        echo "✓ 모든 디렉토리가 크기 제한 이내입니다"
    fi

    # 6. 파일 읽기 테스트 (첫 5개 파일만)
    echo "파일 읽기 테스트 중..."
    local read_success=0
    local read_failed=0

    for ((i=0; i<5 && i<${#test_files[@]}; i++)); do
        if [ -r "${test_files[$i]}" ]; then
            ((read_success++))
        else
            ((read_failed++))
            echo "  ❌ 읽기 실패: ${test_files[$i]}"
        fi
    done

    if [ "$read_failed" -gt 0 ]; then
        echo "⚠️ 일부 파일 읽기 실패: $read_failed개"
    else
        echo "✓ 샘플 파일 읽기 테스트 통과 ($read_success개)"
    fi

    # 예상 결과 요약
    echo
    echo "========================================="
    echo "예상 병합 결과:"
    echo "- 출력 파일: $output_file"
    echo "- 대상 파일 형식: ${file_formats[*]}"
    echo "- 예상 파일 수: $test_count"
    local estimated_size=$(find_files | xargs -0 du -ch 2>/dev/null | tail -n 1 | cut -f1 || echo "계산 불가")
    echo "- 예상 출력 크기: $estimated_size"
    echo "========================================="

    log_info "테스트 완료 (예상 파일 수: $test_count)"
    read -p "아무 키나 눌러 계속..." -n1
}

#==============================================================================
# 대화형 메뉴 시스템
#==============================================================================
show_menu() {
    clear
    cat << EOF
========================================
   고급 파일 병합 도구 v${VERSION}
========================================
1. 실행 - 현재 설정으로 병합 실행
2. 파일 형식 설정 - 검색할 파일 유형 선택
3. 제외 디렉토리 관리 - 병합에서 제외할 디렉토리
4. 고급 테스트 - 병합 가능성 및 시스템 검사
5. 설정 관리 - 저장/불러오기
6. 종료
========================================
현재 설정:
- 디렉토리: $start_dir
- 출력 파일: $output_file
- 파일 형식: ${file_formats[*]}
- 디렉토리 크기 제한: $size_limit
========================================
EOF

    read -p "메뉴 선택 (1-6): " choice
    case $choice in
        1) validate_and_execute ;;
        2) configure_file_formats ;;
        3) manage_exclusions ;;
        4) enhanced_test ;;
        5) config_management_menu ;;
        6) log_info "사용자 종료 요청"; exit 0 ;;
        *) log_warning "잘못된 선택: $choice"; sleep 1; show_menu ;;
    esac
}

#==============================================================================
# 파일 형식 설정 메뉴
#==============================================================================
configure_file_formats() {
    clear
    cat << EOF
========================================
   파일 형식 설정
========================================
현재 설정: ${file_formats[*]}

1. Python 파일만 (*.py)
2. 스크립트 파일 (*.py, *.sh, *.rb)
3. 텍스트 파일 (*.txt, *.md, *.log)
4. 사용자 지정 형식
5. 이전 메뉴로
========================================
EOF

    read -p "선택 (1-5): " choice
    case $choice in
        1) file_formats=("*.py"); log_info "파일 형식 설정: Python 파일" ;;
        2) file_formats=("*.py" "*.sh" "*.rb"); log_info "파일 형식 설정: 스크립트 파일" ;;
        3) file_formats=("*.txt" "*.md" "*.log"); log_info "파일 형식 설정: 텍스트 파일" ;;
        4)
            read -p "공백으로 구분된 파일 패턴 입력 (예: *.java *.xml): " custom_formats
            IFS=' ' read -r -a file_formats <<< "$custom_formats"
            log_info "파일 형식 설정: 사용자 지정 ($custom_formats)"
            ;;
        5) show_menu; return ;;
        *) log_warning "잘못된 선택"; configure_file_formats ;;
    esac

    show_menu
}

#==============================================================================
# 제외 디렉토리 관리
#==============================================================================
manage_exclusions() {
    clear
    local current_dir=$(pwd)
    cat << EOF
========================================
   제외 디렉토리 관리
========================================
현재 작업 디렉토리: $current_dir
========================================
현재 제외 디렉토리:
EOF

    if [ ${#exclude_dirs[@]} -eq 0 ]; then
        echo "없음"
    else
        for i in "${!exclude_dirs[@]}"; do
            echo "$((i+1)). ${exclude_dirs[$i]}"
        done
    fi

    cat << EOF
----------------------------------------
재귀 모드: $([ "$recursive_mode" == true ] && echo "활성화" || echo "비활성화")
----------------------------------------
1. 디렉토리 추가
2. 디렉토리 제거
3. 디렉토리 크기 제한 설정 (현재: ${size_limit})
4. 재귀 모드 토글 ($([ "$recursive_mode" == true ] && echo "활성화" || echo "비활성화"))
5. 제외 파일 확장자 관리
6. 이전 메뉴로
========================================
EOF

    read -p "선택 (1-6): " choice
    case $choice in
        1)
            read -p "제외할 디렉토리 경로: " new_dir
            if [ -d "$new_dir" ]; then
                exclude_dirs+=("$new_dir")
                log_info "제외 디렉토리 추가: $new_dir"
            else
                log_warning "디렉토리가 존재하지 않습니다: $new_dir"
            fi
            manage_exclusions
            ;;
        2)
            if [ ${#exclude_dirs[@]} -eq 0 ]; then
                log_warning "제거할 디렉토리가 없습니다"
                sleep 1
            else
                read -p "제거할 디렉토리 번호 선택 (1-${#exclude_dirs[@]}): " remove_idx
                if [[ "$remove_idx" =~ ^[0-9]+$ ]] && [ "$remove_idx" -ge 1 ] && [ "$remove_idx" -le "${#exclude_dirs[@]}" ]; then
                    log_info "제외 디렉토리 제거: ${exclude_dirs[$((remove_idx-1))]}"
                    exclude_dirs=("${exclude_dirs[@]:0:$((remove_idx-1))}" "${exclude_dirs[@]:$remove_idx}")
                else
                    log_warning "잘못된 선택"
                fi
            fi
            manage_exclusions
            ;;
        3)
            read -p "디렉토리 크기 제한 입력 (예: 10M, 1G, 500K): " new_limit
            if [[ "$new_limit" =~ ^[0-9]+[KMGkmg]?$ ]]; then
                size_limit="$new_limit"
                log_info "디렉토리 크기 제한 설정: $size_limit"
            else
                log_warning "잘못된 형식. 숫자 + 단위(K,M,G)로 입력하세요"
            fi
            manage_exclusions
            ;;
        4)
            if [ "$recursive_mode" == true ]; then
                recursive_mode=false
                log_info "재귀 모드 비활성화"
            else
                recursive_mode=true
                log_info "재귀 모드 활성화"
            fi
            manage_exclusions
            ;;
        5)
            manage_excluded_extensions
            ;;
        6) show_menu; return ;;
        *) log_warning "잘못된 선택"; manage_exclusions ;;
    esac
}

#==============================================================================
# 제외 확장자 관리 함수
#==============================================================================
manage_excluded_extensions() {
    clear
    cat << EOF
========================================
   제외 파일 확장자 관리
========================================
현재 제외 확장자:
EOF

    if [ ${#excluded_extensions[@]} -eq 0 ]; then
        echo "없음"
    else
        for i in "${!excluded_extensions[@]}"; do
            echo "$((i+1)). ${excluded_extensions[$i]}"
        done
    fi

    cat << EOF
----------------------------------------
1. 확장자 추가
2. 확장자 제거
3. 기본 확장자로 초기화
4. 이전 메뉴로
========================================
EOF

    read -p "선택 (1-4): " choice
    case $choice in
        1)
            read -p "제외할 확장자 패턴 입력 (예: *.tmp): " new_ext
            if [[ "$new_ext" == *"*."* ]]; then
                excluded_extensions+=("$new_ext")
                log_info "제외 확장자 추가: $new_ext"
            else
                log_warning "잘못된 형식. '*.확장자' 형식으로 입력하세요"
            fi
            manage_excluded_extensions
            ;;
        2)
            if [ ${#excluded_extensions[@]} -eq 0 ]; then
                log_warning "제거할 확장자가 없습니다"
                sleep 1
            else
                read -p "제거할 확장자 번호 선택 (1-${#excluded_extensions[@]}): " remove_idx
                if [[ "$remove_idx" =~ ^[0-9]+$ ]] && [ "$remove_idx" -ge 1 ] && [ "$remove_idx" -le "${#excluded_extensions[@]}" ]; then
                    log_info "제외 확장자 제거: ${excluded_extensions[$((remove_idx-1))]}"
                    excluded_extensions=("${excluded_extensions[@]:0:$((remove_idx-1))}" "${excluded_extensions[@]:$remove_idx}")
                else
                    log_warning "잘못된 선택"
                fi
            fi
            manage_excluded_extensions
            ;;
        3)
            excluded_extensions=("*.o" "*.pyc" "*.class" "*.cache")
            log_info "제외 확장자 초기화"
            manage_excluded_extensions
            ;;
        4) manage_exclusions; return ;;
        *) log_warning "잘못된 선택"; manage_excluded_extensions ;;
    esac
}

#==============================================================================
# 설정 관리 메뉴
#==============================================================================
config_management_menu() {
    clear
    cat << EOF
========================================
   설정 관리
========================================
1. 현재 설정 저장
2. 설정 불러오기
3. 설정 목록 보기
4. 이전 메뉴로
========================================
EOF

    read -p "선택 (1-4): " choice
    case $choice in
        1)
            read -p "설정 이름 입력: " config_name
            manage_config "save" "$config_name"
            ;;
        2)
            echo "사용 가능한 설정:"
            manage_config "list"
            read -p "불러올 설정 이름: " config_name
            manage_config "load" "$config_name"
            ;;
        3)
            echo "저장된 설정 목록:"
            manage_config "list"
            read -p "아무 키나 눌러 계속..." -n1
            ;;
        4) show_menu; return ;;
        *) log_warning "잘못된 선택"; config_management_menu ;;
    esac

    config_management_menu
}

#==============================================================================
# 주요 기능 실행
#==============================================================================
validate_and_execute() {
    log_info "병합 작업 시작: 시작 디렉토리=$start_dir, 출력 파일=$output_file"

    # 권한 및 환경 검사
    if [ ! -d "$start_dir" ]; then
        error_exit "디렉토리가 존재하지 않음: $start_dir" 2
    fi

    # 출력 파일 준비
    local output_dir=$(dirname "$output_file")
    if [ ! -d "$output_dir" ] && [ "$output_dir" != "." ]; then
        log_info "출력 디렉토리 생성: $output_dir"
        mkdir -p "$output_dir" || error_exit "출력 디렉토리 생성 실패: $output_dir" 3
    fi

    # 출력 파일 초기화
    > "$output_file" || error_exit "출력 파일 초기화 실패: $output_file" 4

    # 파일 헤더 추가 (파일 정보 포함)
    {
        echo "# ==============================================================="
        echo "# 파일 병합 결과: $(date)"
        echo "# 시작 디렉토리: $start_dir"
        echo "# 파일 형식: ${file_formats[*]}"
        echo "# 제외 디렉토리: ${exclude_dirs[*]:-없음}"
        echo "# ==============================================================="
        echo ""
    } >> "$output_file"

    # 파일 처리 시작
    process_files
}

#==============================================================================
# 파일 처리 함수
#==============================================================================
process_files() {
    local count=0
    local success=0
    local total_size=0
    local human_size="0B"

    log_info "파일 검색 및 병합 시작"

    while IFS= read -r -d $'\0' file; do
        ((count++))
        log_debug "처리: $file"

        # 파일 유효성 검증
        if [ ! -f "$file" ] || [ ! -r "$file" ]; then
            log_warning "파일 접근 불가: $file"
            continue
        fi

        # 헤더 및 내용 추가
        if format_file_header "$file" >> "$output_file" && cat "$file" >> "$output_file"; then
            ((success++))
            local file_size=$(stat -c "%s" "$file" 2>/dev/null || wc -c < "$file" 2>/dev/null || echo "0")
            total_size=$((total_size + file_size))
            echo "처리 중: $file (${success}/${count})"
        else
            log_error "파일 추가 실패: $file"
        fi
    done < <(find_files)

    # 결과 보고
    if [ "$count" -eq 0 ]; then
        log_warning "검색 조건에 맞는 파일이 없습니다."
        echo "주의: 검색 조건에 맞는 파일을 찾지 못했습니다."
    else
        # 사람이 읽기 쉬운 파일 크기 변환
        if [ "$total_size" -ge $((1024*1024*1024)) ]; then
            human_size="$(echo "scale=2; $total_size/1024/1024/1024" | bc)GB"
        elif [ "$total_size" -ge $((1024*1024)) ]; then
            human_size="$(echo "scale=2; $total_size/1024/1024" | bc)MB"
        elif [ "$total_size" -ge 1024 ]; then
            human_size="$(echo "scale=2; $total_size/1024" | bc)KB"
        else
            human_size="${total_size}B"
        fi

        log_info "병합 완료: 총 $count개 중 $success개 성공 (총 크기: $human_size)"
        echo "완료: $count개 파일 중 $success개 성공적으로 병합 (총 크기: $human_size)"
    fi

    # 결과 요약 추가
    {
        echo
        echo "# ==============================================================="
        echo "# 병합 작업 완료: $(date)"
        echo "# 결과: $success/$count 파일 성공"
        echo "# 출력 파일: $output_file (${human_size})"
        echo "# ==============================================================="
    } >> "$output_file"
}

#==============================================================================
# 도움말 표시 함수
#==============================================================================
show_help() {
    cat << EOF
사용법: $SCRIPT_NAME [옵션]

옵션:
  -d, --directory DIR   검색 시작 디렉토리 지정 (기본값: 현재 디렉토리)
  -o, --output FILE     출력 파일 지정 (기본값: merged_날짜시간.txt)
  -e, --exclude DIR     검색에서 제외할 디렉토리 (여러 번 사용 가능)
  -s, --size-limit SIZE 디렉토리 크기 제한 (예: 10M, 1G)
  -f, --format PATTERN  파일 형식 지정 (예: *.py)
  -t, --test            시스템 및 파일 검색 테스트 실행
  -h, --help            도움말 표시

예시:
  $SCRIPT_NAME -d ./src -o merged.txt -e node_modules -e .git
  $SCRIPT_NAME -d . -f "*.java" -s 50M
EOF
}

#==============================================================================
# 메인 실행 블록
#==============================================================================
main() {
    # 로깅 시스템 초기화
    setup_logging
    log_info "스크립트 시작 - 버전 ${VERSION}"

    # 기본값 초기화
    initialize_defaults

    # 명령줄 인자 처리
    if [[ $# -gt 0 ]]; then
        while [[ $# -gt 0 ]]; do
            case "$1" in
                -d|--dir|--directory)
                    start_dir="${2:-$start_dir}"
                    shift 2 || shift
                    ;;
                -o|--output)
                    output_file="${2:-$output_file}"
                    shift 2 || shift
                    ;;
                -e|--exclude)
                    [[ -n "$2" ]] && exclude_dirs+=("$2")
                    shift 2 || shift
                    ;;
                -s|--size-limit)
                    size_limit="${2:-$size_limit}"
                    shift 2 || shift
                    ;;
                -f|--format)
                    [[ -n "$2" ]] && file_formats=("$2")
                    shift 2 || shift
                    ;;
                -t|--test)
                    enhanced_test
                    exit 0
                    ;;
                -h|--help)
                    show_help
                    exit 0
                    ;;
                *)
                    log_warning "알 수 없는 옵션: $1"
                    show_help
                    exit 1
                    ;;
            esac
        done
        # 인자가 있으면 검증 후 바로 실행
        validate_and_execute
    else
        # 인자가 없으면 대화형 메뉴 표시
        show_menu
    fi
}

#==============================================================================
# 스크립트 실행 시작
#==============================================================================
# Ctrl+C 등 인터럽트 처리
trap 'log_warning "스크립트 중단됨"; exit 1' INT TERM

# 명령줄 인자로 스크립트 실행
main "$@"
