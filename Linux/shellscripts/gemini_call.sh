#!/bin/bash

# 더 나은 가독성을 위한 색상 정의
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
BOLD='\033[1m'
NC='\033[0m' # No Color

help()
{
  echo -e "${BOLD}고급 Gemini API 클라이언트${NC}"
  echo "함수 호출 및 스트리밍을 포함한 Google의 Gemini API와 상호 작용하는 스크립트"
  echo
  echo -e "${BOLD}사용법:${NC} ./script.sh [옵션]"
  echo
  echo -e "${BOLD}기본 옵션:${NC}"
  echo "  -p '프롬프트'     Gemini API에 전송할 텍스트 프롬프트 (필수)"
  echo "  -t '온도'         무작위성에 대한 온도 설정 (0-1) (기본값: 0.7)"
  echo "  -m '최대 토큰'    최대 출력 토큰 수 (기본값: 800)"
  echo "  -s                실시간 응답을 위한 스트리밍 모드 활성화"
  echo "  -h                이 도움말 메시지 표시"
  echo
  echo -e "${BOLD}함수 호출 옵션:${NC}"
  echo "  -f '함수 파일'    함수 선언이 포함된 JSON 파일"
  echo
  echo -e "${BOLD}출력 옵션:${NC}"
  echo "  -o '파일명'       응답을 지정된 파일에 저장"
  echo "  -r                Raw 모드 - JSON 없이 텍스트 응답만 출력"
  echo
  echo -e "${BOLD}예제:${NC}"
  echo "  ./script.sh -p '재미있는 농담 하나 해줘' -t 0.8 -m 1000"
  echo "  ./script.sh -p '뉴욕의 날씨는 어때?' -f weather_functions.json"
  echo "  ./script.sh -p '실시간 결과 보여줘' -s"
  exit 0
}

# jq 설치 확인
check_jq() {
  if ! command -v jq &> /dev/null; then
    echo -e "${YELLOW}경고: jq가 설치되어 있지 않습니다. 출력이 예쁘게 포맷되지 않을 수 있습니다.${NC}"
    echo "jq 설치 방법:"
    echo "  Debian/Ubuntu: sudo apt install jq"
    echo "  CentOS/RHEL: sudo yum install jq"
    echo "  macOS: brew install jq"
    return 1
  fi
  return 0
}

# API 키 확인
check_api_key() {
  if [ -z "$GEMINI_API_KEY" ]; then
    echo -e "${RED}오류: GEMINI_API_KEY 환경 변수가 설정되지 않았습니다.${NC}"
    echo "다음 명령으로 설정하세요: export GEMINI_API_KEY='your-api-key-here'"
    exit 1
  fi
}

# 표준 요청 처리
process_request() {
  local prompt="$1"
  local temp="$2"
  local max_tokens="$3"
  local output_file="$4"
  local raw_mode="$5"
  local func_file="$6"
  local streaming="$7"

  # 기본 JSON 템플릿
  local json_template='{
    "contents": [{
      "parts":[{"text": ""}]
    }],
    "generationConfig": {
      "temperature": 0.7,
      "maxOutputTokens": 800
    }
  }'

  # JSON 데이터 생성
  local json_data
  if command -v jq &> /dev/null; then
    # jq를 사용하여 데이터 생성
    json_data=$(echo "$json_template" | jq --arg prompt "$prompt" '.contents[0].parts[0].text = $prompt')

    # 온도 값 업데이트
    if [ -n "$temp" ]; then
      json_data=$(echo "$json_data" | jq --arg temp "$temp" '.generationConfig.temperature = ($temp | tonumber)')
    fi

    # 최대 토큰 업데이트
    if [ -n "$max_tokens" ]; then
      json_data=$(echo "$json_data" | jq --arg max "$max_tokens" '.generationConfig.maxOutputTokens = ($max | tonumber)')
    fi

    # 함수 선언 추가
    if [ -n "$func_file" ] && [ -f "$func_file" ]; then
      echo -e "${BLUE}$func_file에서 함수 선언 사용 중${NC}"
      # 파일에서 함수 읽기
      local functions=$(cat "$func_file")
      # JSON 데이터에 함수 추가
      json_data=$(echo "$json_data" | jq --argjson funcs "$functions" '. + {tools: [{functionDeclarations: $funcs}]}')
    fi

    # 스트리밍 활성화
    if [ "$streaming" = true ]; then
      json_data=$(echo "$json_data" | jq '. + {streamEnabled: true}')
    fi
  else
    # 대체 방법으로 간단한 문자열 대체
    # sed를 위한 프롬프트의 특수 문자 이스케이프
    local escaped_prompt=$(printf '%s\n' "$prompt" | sed -e 's/[\/&]/\\&/g')
    json_data=$(echo "$json_template" | sed "s/\"text\": \"\"/\"text\": \"$escaped_prompt\"/")

    # 온도 및 최대 토큰 업데이트
    if [ -n "$temp" ]; then
      json_data=$(echo "$json_data" | sed "s/\"temperature\": 0.7/\"temperature\": $temp/")
    fi

    if [ -n "$max_tokens" ]; then
      json_data=$(echo "$json_data" | sed "s/\"maxOutputTokens\": 800/\"maxOutputTokens\": $max_tokens/")
    fi

    # 함수 선언 및 스트리밍은 jq 필요
    if [ -n "$func_file" ] || [ "$streaming" = true ]; then
      echo -e "${RED}오류: 함수 선언 및 스트리밍은 jq 설치가 필요합니다.${NC}"
      exit 1
    fi
  fi

  echo -e "${BLUE}Gemini API 요청 전송 중...${NC}"

  # API 엔드포인트
  local endpoint="https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent"

  # 스트리밍이 활성화된 경우 스트리밍 엔드포인트 사용
  if [ "$streaming" = true ]; then
    endpoint="${endpoint}Stream"

    echo -e "${GREEN}스트리밍 응답:${NC}"
    # 스트리밍을 위해 버퍼링을 비활성화한 curl 사용
    curl -N -s "$endpoint?key=${GEMINI_API_KEY}" \
      -H 'Content-Type: application/json' \
      -X POST \
      -d "$json_data" | while read -r line; do
        # 빈 줄 건너뛰기
        [ -z "$line" ] && continue

        # 스트림의 각 줄 처리
        if command -v jq &> /dev/null; then
          # 각 청크에서 텍스트만 추출
          text=$(echo "$line" | jq -r '.candidates[0].content.parts[0].text // empty')
          if [ -n "$text" ]; then
            echo -n "$text"
            # 요청된 경우 파일에 추가
            if [ -n "$output_file" ]; then
              echo -n "$text" >> "$output_file"
            fi
          fi
        else
          echo "$line"
        fi
    done
    echo # 스트리밍 끝에 개행 추가
    return 0
  fi

  # 표준 API 호출 및 응답 캡처
  local response
  response=$(curl -s "$endpoint?key=${GEMINI_API_KEY}" \
    -H 'Content-Type: application/json' \
    -X POST \
    -d "$json_data")

  # 응답에 오류가 있는지 확인
  if echo "$response" | grep -q "\"error\""; then
    echo -e "${RED}API 오류:${NC}"
    if command -v jq &> /dev/null; then
      echo "$response" | jq '.error'
    else
      echo "$response"
    fi
    return 1
  fi

  # 함수 호출 확인
  local has_function_call=false
  if command -v jq &> /dev/null; then
    has_function_call=$(echo "$response" | jq 'has("candidates") and (.candidates[0].content.parts[0] | has("functionCall"))')
    if [ "$has_function_call" = "true" ]; then
      echo -e "${YELLOW}응답에서 함수 호출이 감지되었습니다${NC}"
      echo -e "${BLUE}함수 세부 정보:${NC}"
      echo "$response" | jq '.candidates[0].content.parts[0].functionCall'

      # 실제 구현에서는 여기서 함수 실행을 처리
      echo -e "${YELLOW}참고: 자동 함수 실행은 이 스크립트에 구현되어 있지 않습니다.${NC}"
      echo -e "${YELLOW}      함수 호출 로직을 직접 구현해야 합니다.${NC}"
    fi
  fi

  # 텍스트 내용 추출 (함수 호출이 아닌 경우)
  local text_content
  if [ "$has_function_call" = "false" ] && command -v jq &> /dev/null; then
    text_content=$(echo "$response" | jq -r '.candidates[0].content.parts[0].text // "응답에 텍스트가 없습니다"')
  elif command -v jq &> /dev/null; then
    text_content="(응답에 직접 텍스트가 아닌 함수 호출이 포함되어 있습니다)"
  else
    # jq가 없는 경우 간단한 텍스트 추출
    text_content=$(echo "$response" | grep -o '"text":"[^"]*"' | sed 's/"text":"//;s/"$//')
    if [ -z "$text_content" ]; then
      text_content="응답에서 텍스트를 추출할 수 없습니다 (더 나은 파싱을 위해 jq 설치)"
    fi
  fi

  # 요청된 경우 파일에 저장
  if [ -n "$output_file" ]; then
    echo "$text_content" > "$output_file"
    echo -e "${GREEN}응답이 ${BOLD}$output_file${NC}${GREEN}에 저장되었습니다${NC}"
  fi

  # 출력 표시
  if [ "$raw_mode" = true ]; then
    # 텍스트 내용만 표시
    echo "$text_content"
  else
    echo -e "\n${BOLD}===== Gemini API 응답 =====${NC}\n"
    echo "$text_content"

    if command -v jq &> /dev/null; then
      echo -e "\n${BOLD}===== 전체 JSON 응답 =====${NC}\n"
      echo "$response" | jq '.'
    fi
  fi

  return 0
}

# 변수 초기화
PROMPT=""
TEMPERATURE=""
MAX_TOKENS=""
OUTPUT_FILE=""
RAW_MODE=false
FUNC_FILE=""
STREAMING=false

# 인자가 없으면 도움말 표시
if [ $# -eq 0 ]; then
  help
fi

# 매개변수 파싱
while getopts "p:t:m:o:rf:sh" opt; do
  case $opt in
    p) PROMPT="$OPTARG" ;;
    t) TEMPERATURE="$OPTARG" ;;
    m) MAX_TOKENS="$OPTARG" ;;
    o) OUTPUT_FILE="$OPTARG" ;;
    r) RAW_MODE=true ;;
    f) FUNC_FILE="$OPTARG" ;;
    s) STREAMING=true ;;
    h) help ;;
    *) echo -e "${RED}잘못된 옵션: -$OPTARG${NC}" >&2; help ;;
  esac
done

# API 키 확인
check_api_key

# jq 설치 확인
check_jq

# 프롬프트 확인
if [ -z "$PROMPT" ]; then
  echo -e "${RED}오류: 프롬프트(-p)는 필수입니다.${NC}"
  help
fi

# 온도 값 검증
if [ -n "$TEMPERATURE" ]; then
  if ! [[ "$TEMPERATURE" =~ ^[0-9]*\.?[0-9]+$ ]] || (( $(echo "$TEMPERATURE  1" | bc -l) )); then
    echo -e "${RED}오류: 온도 값은 0과 1 사이의 숫자여야 합니다.${NC}"
    exit 1
  fi
fi

# 최대 토큰 검증
if [ -n "$MAX_TOKENS" ]; then
  if ! [[ "$MAX_TOKENS" =~ ^[0-9]+$ ]]; then
    echo -e "${RED}오류: 최대 토큰 값은 양의 정수여야 합니다.${NC}"
    exit 1
  fi
fi

# 함수 파일 존재 확인
if [ -n "$FUNC_FILE" ] && [ ! -f "$FUNC_FILE" ]; then
  echo -e "${RED}오류: 함수 파일 '$FUNC_FILE'이 존재하지 않습니다.${NC}"
  exit 1
fi

# 요청 처리
process_request "$PROMPT" "$TEMPERATURE" "$MAX_TOKENS" "$OUTPUT_FILE" "$RAW_MODE" "$FUNC_FILE" "$STREAMING"