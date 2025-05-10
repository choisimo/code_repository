#!/bin/bash

# 기본값 설정
start_dir="."
output_file="merged_source_files.txt"

# 도움말 표시 함수
show_help() {
    echo "사용법: $0 [옵션]"
    echo "옵션:"
    echo "  -d, --directory DIR  검색 시작 디렉토리 지정 (기본값: 현재 디렉토리)"
    echo "  -o, --output FILE    출력 파일 지정 (기본값: merged_source_files.txt)"
    echo "  -h, --help           도움말 표시"
    exit 0
}

# 인자 파싱
while [[ $# -gt 0 ]]; do
    case $1 in
        -d|--directory)
            start_dir="$2"
            shift 2
            ;;
        -o|--output)
            output_file="$2"
            shift 2
            ;;
        -h|--help)
            show_help
            ;;
        *)
            echo "알 수 없는 옵션: $1"
            show_help
            ;;
    esac
done

# 시작 디렉토리가 존재하는지 확인
if [ ! -d "$start_dir" ]; then
    echo "오류: 디렉토리 '$start_dir'가 존재하지 않습니다."
    exit 1
fi

# 결과 파일 초기화
> "$output_file"

echo "디렉토리 '$start_dir'에서 .h 및 .cpp 파일을 검색하여 '$output_file'에 병합합니다..."

# 파일 카운터 초기화
file_count=0

# 재귀적으로 .h와 .cpp 파일 찾기
find "$start_dir" -type f \( -name "*.h" -o -name "*.cpp" \) -print0 | 
while IFS= read -r -d $'\0' file; do
    # 파일 카운터 증가
    ((file_count++))
    
    # 진행 상황 표시
    echo "처리 중: $file"
    
    # 파일 구분자 추가
    echo -e "\n\n===== $file =====\n" >> "$output_file"
    
    # 파일 내용을 결과 파일에 추가
    cat "$file" >> "$output_file"
done

# 병합 완료 메시지 (파이프라인이 서브셸에서 실행되어 file_count가 손실될 수 있음을 고려)
total_files=$(find "$start_dir" -type f \( -name "*.h" -o -name "*.cpp" \) | wc -l)
echo "완료: $total_files개의 파일이 '$output_file'에 병합되었습니다."
