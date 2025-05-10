import pandas as pd
import json
import csv
import io
from datetime import datetime
from typing import Dict, Any, List, Union, Optional, BinaryIO
"""
로그 분석에 필요한 유틸리티 함수들
"""
import re
import json
from datetime import datetime
from collections import Counter, defaultdict
import pandas as pd
import matplotlib.pyplot as plt
from io import BytesIO
import base64

def parse_log_entries(logs):
    """
    로그 텍스트를 개별 항목으로 분리
    
    Args:
        logs (str): 전체 로그 문자열
        
    Returns:
        list: 개별 로그 항목 목록
    """
    # 일반적인 로그 형식에 맞게 분리
    log_pattern = r"\[\d{4}-\d{2}-\d{2}\s\d{2}:\d{2}:\d{2}\]|\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}"
    log_entries = re.split(log_pattern, logs)
    timestamps = re.findall(log_pattern, logs)
    
    # 첫 번째 빈 항목 제거
    if log_entries and not log_entries[0].strip():
        log_entries.pop(0)
    
    # 타임스탬프와 로그 내용 결합
    parsed_entries = []
    for i, entry in enumerate(log_entries):
        if i < len(timestamps):
            parsed_entries.append({"timestamp": timestamps[i], "content": entry.strip()})
    
    return parsed_entries

def extract_error_types(log_entries):
    """
    로그 항목에서 오류 유형 추출
    
    Args:
        log_entries (list): 로그 항목 목록
        
    Returns:
        dict: 오류 유형별 빈도
    """
    error_types = Counter()
    error_patterns = [
        r"Error:|Exception:|Failed|Failure|Warning:|Critical:",
        r"ERR\b|WARN\b|CRITICAL\b|FATAL\b",
        r"status code [45]\d\d",
        r"timed? out",
        r"could not connect",
        r"refused",
        r"denied"
    ]
    
    compiled_patterns = [re.compile(pattern, re.IGNORECASE) for pattern in error_patterns]
    
    for entry in log_entries:
        content = entry.get("content", "")
        for pattern in compiled_patterns:
            if pattern.search(content):
                # 오류 유형 추출 시도
                error_match = re.search(r"([A-Za-z]+(?:Error|Exception|Warning|Failure))", content)
                if error_match:
                    error_types[error_match.group(1)] += 1
                else:
                    # 일반적인 오류 형태가 없으면 첫 10단어를 요약으로 사용
                    words = content.split()[:10]
                    summary = " ".join(words)
                    error_types[summary] += 1
                break
    
    return error_types

def analyze_time_patterns(log_entries):
    """
    시간대별 로그 패턴 분석
    
    Args:
        log_entries (list): 로그 항목 목록
        
    Returns:
        dict: 시간대별 로그 발생 빈도
    """
    time_patterns = defaultdict(int)
    
    for entry in log_entries:
        timestamp = entry.get("timestamp", "")
        try:
            # ISO 포맷(2023-01-01T12:34:56) 처리
            if "T" in timestamp:
                dt = datetime.strptime(timestamp, "[%Y-%m-%dT%H:%M:%S]" if timestamp.startswith("[") else "%Y-%m-%dT%H:%M:%S")
            # 일반 포맷([2023-01-01 12:34:56]) 처리
            else:
                dt = datetime.strptime(timestamp, "[%Y-%m-%d %H:%M:%S]" if timestamp.startswith("[") else "%Y-%m-%d %H:%M:%S")
            
            hour = dt.hour
            time_patterns[hour] += 1
        except:
            continue
    
    return dict(sorted(time_patterns.items()))

def assess_severity(log_entries):
    """
    로그의 심각도 평가
    
    Args:
        log_entries (list): 로그 항목 목록
        
    Returns:
        int: 심각도 점수 (1-5)
    """
    severity_score = 1
    critical_keywords = [
        "critical", "fatal", "emergency", "alert", "failed", "crash", "down", "outage"
    ]
    warning_keywords = [
        "warning", "attention", "notice", "timeout", "slow", "degraded"
    ]
    
    # 오류 발생 빈도 확인
    error_count = 0
    total_entries = len(log_entries)
    
    for entry in log_entries:
        content = entry.get("content", "").lower()
        
        # 심각한 오류 키워드 검색
        if any(keyword in content for keyword in critical_keywords):
            severity_score = max(severity_score, 4)
            error_count += 1
            
            # 추가 심각도 점수 증가 요인
            if "database" in content or "security" in content or "authentication" in content:
                severity_score = 5
        
        # 경고 수준 키워드 검색
        elif any(keyword in content for keyword in warning_keywords):
            severity_score = max(severity_score, 2)
            error_count += 1
            
            # 누적된 경고가 많으면 심각도 증가
            if error_count > total_entries * 0.2:
                severity_score = max(severity_score, 3)
    
    return severity_score

def identify_affected_systems(log_entries):
    """
    로그에서 영향 받는 시스템 식별
    
    Args:
        log_entries (list): 로그 항목 목록
        
    Returns:
        list: 영향 받는 시스템 목록
    """
    systems = set()
    system_patterns = [
        r"database|db|sql|postgres|mysql|mongodb|redis",
        r"api|rest|http|endpoint|service",
        r"auth|authentication|login|user",
        r"network|connection|socket|dns",
        r"file|storage|disk|memory|cache",
        r"app|application|server|client",
        r"queue|broker|kafka|rabbitmq",
        r"frontend|backend|ui|interface"
    ]
    
    compiled_patterns = [re.compile(pattern, re.IGNORECASE) for pattern in system_patterns]
    
    for entry in log_entries:
        content = entry.get("content", "")
        for i, pattern in enumerate(compiled_patterns):
            if pattern.search(content):
                # 시스템 타입에 따라 구체적인 이름 정의
                if i == 0:
                    systems.add("데이터베이스 시스템")
                elif i == 1:
                    systems.add("API 서비스")
                elif i == 2:
                    systems.add("인증 시스템")
                elif i == 3:
                    systems.add("네트워크 인프라")
                elif i == 4:
                    systems.add("스토리지 시스템")
                elif i == 5:
                    systems.add("애플리케이션 서버")
                elif i == 6:
                    systems.add("메시지 큐 시스템")
                elif i == 7:
                    systems.add("사용자 인터페이스")
    
    return list(systems) if systems else ["알 수 없음"]

def generate_time_histogram(log_entries):
    """
    시간대별 로그 발생 히스토그램 생성
    
    Args:
        log_entries (list): 로그 항목 목록
        
    Returns:
        str: Base64로 인코딩된 히스토그램 이미지
    """
    time_data = analyze_time_patterns(log_entries)
    
    if not time_data:
        return None
    
    # 데이터프레임 생성
    df = pd.DataFrame({
        '시간': list(time_data.keys()),
        '발생 수': list(time_data.values())
    })
    
    # 그래프 생성
    plt.figure(figsize=(10, 6))
    plt.bar(df['시간'], df['발생 수'], color='skyblue')
    plt.title('시간대별 로그 발생 빈도')
    plt.xlabel('시간 (24시간)')
    plt.ylabel('로그 발생 수')
    plt.xticks(range(0, 24))
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    
    # 이미지를 바이트 스트림으로 저장
    buf = BytesIO()
    plt.savefig(buf, format='png')
    buf.seek(0)
    plt.close()
    
    # Base64로 인코딩
    img_str = base64.b64encode(buf.read()).decode('utf-8')
    return img_str

def suggest_solutions(error_types):
    """
    오류 유형에 따른 해결책 제안
    
    Args:
        error_types (dict): 오류 유형별 빈도
        
    Returns:
        str: 권장 해결책
    """
    if not error_types:
        return "분석할 오류가 발견되지 않았습니다."
    
    # 가장 빈번한 오류 추출
    most_common_error = max(error_types.items(), key=lambda x: x[1])[0]
    
    # 오류 유형별 해결책
    solutions = {
        "Connection": "네트워크 연결 상태를 확인하고, 방화벽 설정을 검토하세요. 또한 서비스 엔드포인트의 가용성을 확인하세요.",
        "Timeout": "시스템 타임아웃 값을 증가시키고, 네트워크 지연 문제를 조사하세요. 서버 부하 상태도 점검하는 것이 좋습니다.",
        "Authentication": "사용자 인증 정보가 올바른지 확인하고, 인증 서비스의 로그를 검토하세요. 토큰 만료 여부도 확인하세요.",
        "Permission": "관련 리소스에 대한 접근 권한을 확인하고, IAM 정책이나 역할 설정을 검토하세요.",
        "Database": "데이터베이스 연결 설정을 확인하고, 쿼리 최적화를 검토하세요. 데이터베이스 서버의 리소스 사용량도 모니터링하세요.",
        "Memory": "메모리 누수 가능성을 확인하고, 애플리케이션의 메모리 할당을 최적화하세요. 필요시 리소스 한도를 증가시키는 것을 고려하세요.",
        "Disk": "디스크 공간을 확보하고, 불필요한 파일을 정리하세요. 스토리지 확장 계획을 수립하는 것도 좋습니다.",
        "Config": "설정 파일의 구문과 경로를 확인하고, 환경 변수가 올바르게 설정되었는지 검토하세요.",
        "Syntax": "코드에서 문법 오류를 수정하고, 적절한 예외 처리를 구현하세요.",
        "Version": "라이브러리나 서비스의 버전 호환성을 확인하고, 필요시 업데이트 또는 다운그레이드를 수행하세요."
    }
    
    # 오류 유형에 가장 근접한 키워드 찾기
    matching_solution = None
    for keyword, solution in solutions.items():
        if keyword.lower() in most_common_error.lower():
            matching_solution = solution
            break
    
    if not matching_solution:
        return f"가장 빈번한 오류 '{most_common_error}'에 대해 시스템 로그를 자세히 분석하고, 관련 컴포넌트의 동작을 검토하세요. 필요시 개발 팀과 문제를 공유하고 도움을 요청하세요."
    
    return matching_solution
def format_timestamp(timestamp: Union[str, datetime], output_format: str = "%Y-%m-%d %H:%M:%S") -> str:
    """
    타임스탬프를 지정된 형식으로 변환합니다.
    
    Args:
        timestamp: 변환할 타임스탬프
        output_format: 출력 형식
        
    Returns:
        형식이 지정된 타임스탬프 문자열
    """
    if isinstance(timestamp, str):
        try:
            dt = pd.to_datetime(timestamp)
            return dt.strftime(output_format)
        except:
            return timestamp
    elif isinstance(timestamp, datetime):
        return timestamp.strftime(output_format)
    else:
        return str(timestamp)

def export_results(df: pd.DataFrame, analysis_results: Dict[str, Any], export_format: str = "csv") -> Optional[BinaryIO]:
    """
    분석 결과를 지정된 형식으로 내보냅니다.
    
    Args:
        df: 로그 데이터프레임
        analysis_results: GPT 분석 결과
        export_format: 내보내기 형식 (csv, json, excel)
        
    Returns:
        내보내기 파일 스트림
    """
    now = datetime.now().strftime("%Y%m%d_%H%M%S")
    
    if export_format == "csv":
        # CSV 형식으로 내보내기
        output = io.BytesIO()
        
        # 로그 데이터 저장
        df.to_csv(output, index=False, encoding='utf-8')
        
        # 분석 결과 추가
        writer = csv.writer(output)
        writer.writerow([])
        writer.writerow(["# 분석 결과"])
        
        for key, value in analysis_results.get("structured_analysis", {}).items():
            writer.writerow([])
            writer.writerow([f"## {key}"])
            for line in value.strip().split('\n'):
                writer.writerow([line])
        
        output.seek(0)
        return output
    
    elif export_format == "json":
        # JSON 형식으로 내보내기
        output = io.BytesIO()
        
        # 결합된 데이터 준비
        export_data = {
            "log_data": json.loads(df.to_json(orient="records")),
            "analysis": analysis_results
        }
        
        output.write(json.dumps(export_data, ensure_ascii=False, indent=2).encode('utf-8'))
        output.seek(0)
        return output
    
    elif export_format == "excel":
        # Excel 형식으로 내보내기
        output = io.BytesIO()
        
        # 엑셀 작성자 생성
        with pd.ExcelWriter(output, engine='openpyxl') as writer:
            # 로그 데이터 시트 작성
            df.to_excel(writer, sheet_name='Log Data', index=False)
            
            # 분석 결과 시트 작성
            analysis_data = []
            for key, value in analysis_results.get("structured_analysis", {}).items():
                analysis_data.append([f"# {key}"])
                for line in value.strip().split('\n'):
                    analysis_data.append([line])
                analysis_data.append([""])  # 빈 줄 추가
            
            analysis_df = pd.DataFrame(analysis_data, columns=["분석 결과"])
            analysis_df.to_excel(writer, sheet_name='Analysis', index=False)
        
        output.seek(0)
        return output
    
    else:
        return None
