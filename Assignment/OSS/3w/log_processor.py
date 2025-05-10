import pandas as pd
import re
from datetime import datetime
import json
from io import StringIO
import time
from typing import Dict, List, Any, Union

# 로그 형식 정규 표현식 패턴
LOG_PATTERNS = {
    'timestamp': r'(\d{4}-\d{2}-\d{2}[ T]\d{2}:\d{2}:\d{2}(?:\.\d+)?(?:Z|[+-]\d{2}:?\d{2})?)',
    'error_code': r'(?:ERR|ERROR|FATAL|WARN|WARNING)[_\-]?(\d{4,6})',
    'ip_address': r'(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})',
    'request_id': r'(?:req|request|id)[_\-:]([a-f0-9]{8,32})',
    'log_level': r'\b(DEBUG|INFO|WARN(?:ING)?|ERROR|CRITICAL|FATAL)\b'
}
import pandas as pd
import re
from datetime import datetime
import json
from io import StringIO
import time
from typing import Dict, List, Any, Union

# 로그 형식 정규 표현식 패턴
LOG_PATTERNS = {
    "standard": r"(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2},\d{3})\s+(\w+)\s+\[([^\]]+)\]\s+(.*)",
    "apache": r'(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}) - - \[(.*?)\] "(.*?)" (\d+) (\d+|-)',
    "nginx": r'(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}) - (\S+) \[(.*?)\] "(.*?)" (\d+) (\d+) "(.*?)" "(.*?)"',
    "custom": r"(\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}\.\d{3}Z)\s+\[(\w+)\]\s+(\w+):\s+(.*)"
}

def process_log(content: str, log_format: str = "standard") -> pd.DataFrame:
    """
    로그 파일을 처리하고 구조화된 데이터프레임으로 변환합니다.
    
    Args:
        content: 로그 파일 내용
        log_format: 로그 형식 (standard, apache, nginx, custom)
        
    Returns:
        구조화된 로그 데이터를 담은 DataFrame
    """
    pattern = LOG_PATTERNS.get(log_format)
    if not pattern:
        raise ValueError(f"지원하지 않는 로그 형식: {log_format}")
    
    logs = []
    for line in content.splitlines():
        match = re.match(pattern, line)
        if match:
            if log_format == "standard":
                timestamp, level, component, message = match.groups()
                logs.append({
                    "timestamp": timestamp,
                    "level": level,
                    "component": component,
                    "message": message
                })
            elif log_format == "apache":
                ip, timestamp, request, status, size = match.groups()
                logs.append({
                    "ip": ip,
                    "timestamp": timestamp,
                    "request": request,
                    "status": status,
                    "size": size if size != "-" else "0"
                })
            elif log_format == "nginx":
                ip, user, timestamp, request, status, size, referer, user_agent = match.groups()
                logs.append({
                    "ip": ip,
                    "user": user,
                    "timestamp": timestamp,
                    "request": request,
                    "status": status,
                    "size": size,
                    "referer": referer,
                    "user_agent": user_agent
                })
            elif log_format == "custom":
                timestamp, level, component, message = match.groups()
                logs.append({
                    "timestamp": timestamp,
                    "level": level,
                    "component": component,
                    "message": message
                })
    
    return pd.DataFrame(logs)

def normalize_logs(df: pd.DataFrame, log_format: str = "standard") -> pd.DataFrame:
    """
    로그 데이터프레임을 정규화하고 추가 분석을 위한 필드를 추가합니다.
    
    Args:
        df: 로그 데이터프레임
        log_format: 로그 형식
        
    Returns:
        정규화된 로그 데이터프레임
    """
    # 로그 형식에 따른 기본 정규화
    if log_format == "standard" or log_format == "custom":
        # 타임스탬프를 datetime 객체로 변환
        if log_format == "standard":
            df["timestamp"] = pd.to_datetime(df["timestamp"], format="%Y-%m-%d %H:%M:%S,%f", errors="coerce")
        else:
            df["timestamp"] = pd.to_datetime(df["timestamp"], format="%Y-%m-%dT%H:%M:%S.%fZ", errors="coerce")
        
        # 오류 메시지 추출
        df["is_error"] = df["level"].str.upper().isin(["ERROR", "CRITICAL", "FATAL"])
        df["error_code"] = None
        
        # 오류 코드 추출 시도
        error_pattern = r"error\s+code[:\s]+(\d+)"
        df.loc[df["is_error"], "error_code"] = df.loc[df["is_error"], "message"].str.extract(error_pattern, expand=False)
        
    elif log_format == "apache" or log_format == "nginx":
        # Apache/Nginx 타임스탬프 변환
        df["timestamp"] = pd.to_datetime(df["timestamp"], format="%d/%b/%Y:%H:%M:%S %z", errors="coerce")
        
        # HTTP 상태 코드로 오류 판단
        df["status"] = df["status"].astype(int)
        df["is_error"] = df["status"] >= 400
        df["error_code"] = df.loc[df["is_error"], "status"]
    
    # 시간 관련 추가 필드
    df["hour"] = df["timestamp"].dt.hour
    df["minute"] = df["timestamp"].dt.minute
    df["day"] = df["timestamp"].dt.day
    df["weekday"] = df["timestamp"].dt.day_name()
    
    return df
# 로그 처리 함수
def process_log(log_content: str) -> pd.DataFrame:
    """
    로그 텍스트를 처리하여 구조화된 DataFrame으로 변환합니다.
    
    Args:
        log_content: 원시 로그 텍스트
        
    Returns:
        pandas DataFrame: 구조화된 로그 데이터
    """
    # 로그 형식 감지
    log_format = detect_log_format(log_content[:1000])  # 처음 1000자를 검사하여 형식 감지
    
    # 형식에 따른 처리
    if log_format == 'json':
        return process_json_logs(log_content)
    elif log_format == 'csv':
        return process_csv_logs(log_content)
    else:  # 텍스트 기반 로그
        return process_text_logs(log_content)

def detect_log_format(log_sample: str) -> str:
    """
    로그 샘플의 형식을 감지합니다.
    
    Args:
        log_sample: 로그 샘플 텍스트
        
    Returns:
        str: 감지된 로그 형식 ('json', 'csv', 'text')
    """
    # JSON 형식 감지
    try:
        # 첫 줄이 JSON 객체인지 확인
        first_line = log_sample.strip().split('\n')[0].strip()
        json.loads(first_line)
        return 'json'
    except (json.JSONDecodeError, IndexError):
        pass
    
    # CSV 형식 감지
    if ',' in log_sample and log_sample.count('\n') > 0:
        comma_count = log_sample.count(',')
        line_count = log_sample.count('\n')
        if comma_count / line_count >= 3:  # 평균적으로 라인당 3개 이상의 쉼표가 있으면 CSV로 간주
            return 'csv'
    
    # 기본적으로 텍스트 로그로 간주
    return 'text'

def process_text_logs(log_content: str) -> pd.DataFrame:
    """
    텍스트 형식 로그를 구조화된 DataFrame으로 변환합니다.
    
    Args:
        log_content: 텍스트 로그 내용
        
    Returns:
        pandas DataFrame: 구조화된 로그 데이터
    """
    lines = log_content.strip().split('\n')
    parsed_logs = []
    
    for line in lines:
        if not line.strip():
            continue
            
        log_entry = {'raw_log': line}
        
        # 정규 표현식으로 필드 추출
        for field, pattern in LOG_PATTERNS.items():
            match = re.search(pattern, line)
            if match:
                log_entry[field] = match.group(0)
        
        # 타임스탬프 처리
        if 'timestamp' in log_entry:
            try:
                # 다양한 타임스탬프 형식 처리 시도
                formats = [
                    '%Y-%m-%d %H:%M:%S',
                    '%Y-%m-%dT%H:%M:%S',
                    '%Y-%m-%d %H:%M:%S.%f',
                    '%Y-%m-%dT%H:%M:%S.%f'
                ]
                
                for fmt in formats:
                    try:
                        ts_str = log_entry['timestamp']
                        # 밀리초나 타임존 부분 제거
                        ts_str = re.sub(r'(\.\d+)?(?:Z|[+-]\d{2}:?\d{2})?$', '', ts_str)
                        log_entry['timestamp'] = datetime.strptime(ts_str, fmt)
                        break
                    except ValueError:
                        continue
            except Exception:
                # 파싱 실패 시 원래 문자열 유지
                pass
                
        parsed_logs.append(log_entry)
    
    # DataFrame 생성
    df = pd.DataFrame(parsed_logs)
    
    # 필수 컬럼이 없는 경우 추가
    for col in ['timestamp', 'log_level', 'error_code']:
        if col not in df.columns:
            df[col] = None
    
    return df

def process_json_logs(log_content: str) -> pd.DataFrame:
    """
    JSON 형식 로그를 구조화된 DataFrame으로 변환합니다.
    
    Args:
        log_content: JSON 로그 내용
        
    Returns:
        pandas DataFrame: 구조화된 로그 데이터
    """
    lines = log_content.strip().split('\n')
    parsed_logs = []
    
    for line in lines:
        if not line.strip():
            continue
            
        try:
            log_entry = json.loads(line)
            log_entry['raw_log'] = line
            parsed_logs.append(log_entry)
        except json.JSONDecodeError:
            # JSON 파싱 실패 시 텍스트 로그로 처리
            log_entry = {'raw_log': line}
            
            # 정규 표현식으로 필드 추출
            for field, pattern in LOG_PATTERNS.items():
                match = re.search(pattern, line)
                if match:
                    log_entry[field] = match.group(0)
                    
            parsed_logs.append(log_entry)
    
    # DataFrame 생성
    df = pd.DataFrame(parsed_logs)
    
    # 필수 컬럼이 없는 경우 추가
    for col in ['timestamp', 'log_level', 'error_code']:
        if col not in df.columns:
            df[col] = None
    
    # 타임스탬프 처리
    if 'timestamp' in df.columns:
        df['timestamp'] = pd.to_datetime(df['timestamp'], errors='coerce')
    
    return df

def process_csv_logs(log_content: str) -> pd.DataFrame:
    """
    CSV 형식 로그를 구조화된 DataFrame으로 변환합니다.
    
    Args:
        log_content: CSV 로그 내용
        
    Returns:
        pandas DataFrame: 구조화된 로그 데이터
    """
    try:
        # CSV 파싱 시도
        df = pd.read_csv(StringIO(log_content))
        
        # 원본 로그 컬럼 추가
        lines = log_content.strip().split('\n')
        if len(lines) > 1:  # 헤더 줄 제외
            raw_logs = lines[1:]
            if len(raw_logs) == len(df):
                df['raw_log'] = raw_logs
        
        # 필수 컬럼이 없는 경우 추가
        for col in ['timestamp', 'log_level', 'error_code']:
            if col not in df.columns:
                df[col] = None
        
        # 타임스탬프 처리
        if 'timestamp' in df.columns:
            df['timestamp'] = pd.to_datetime(df['timestamp'], errors='coerce')
        
        return df
        
    except Exception:
        # CSV 파싱 실패 시 텍스트 로그로 처리
        return process_text_logs(log_content)

def normalize_logs(log_df: pd.DataFrame) -> Dict[str, Any]:
    """
    처리된 로그 DataFrame을 GPT 분석을 위한 정규화된 형식으로 변환합니다.
    
    Args:
        log_df: 처리된 로그 DataFrame
        
    Returns:
        Dict: GPT 분석을 위한 정규화된 로그 데이터
    """
    normalized_data = {
        'log_count': len(log_df),
        'timeframe': {},
        'error_types': {},
        'sample_logs': []
    }
    
    # 시간 범위 설정
    if 'timestamp' in log_df.columns and not log_df['timestamp'].isna().all():
        valid_timestamps = log_df['timestamp'].dropna()
        if not valid_timestamps.empty:
            normalized_data['timeframe'] = {
                'start': valid_timestamps.min().isoformat(),
                'end': valid_timestamps.max().isoformat(),
                'duration_seconds': (valid_timestamps.max() - valid_timestamps.min()).total_seconds()
            }
    
    # 에러 유형 집계
    if 'log_level' in log_df.columns:
        level_counts = log_df['log_level'].value_counts().to_dict()
        normalized_data['log_levels'] = level_counts
    
    if 'error_code' in log_df.columns:
        error_counts = log_df['error_code'].value_counts().to_dict()
        normalized_data['error_types'] = error_counts
    
    # 샘플 로그 추출 (각 로그 레벨별 최대 3개)
    if 'log_level' in log_df.columns and 'raw_log' in log_df.columns:
        for level in log_df['log_level'].dropna().unique():
            level_logs = log_df[log_df['log_level'] == level]['raw_log'].head(3).tolist()
            normalized_data['sample_logs'].extend(level_logs)
    
    # 샘플이 없는 경우 원시 로그에서 최대 10개 추출
    if not normalized_data['sample_logs'] and 'raw_log' in log_df.columns:
        normalized_data['sample_logs'] = log_df['raw_log'].head(10).tolist()
    
    # 시간별 로그 수 집계 (시각화용)
    if 'timestamp' in log_df.columns and not log_df['timestamp'].isna().all():
        try:
            time_series = log_df.set_index('timestamp').resample('10T').size()
            normalized_data['time_series_data'] = {
                'timestamps': time_series.index.astype(str).tolist(),
                'counts': time_series.values.tolist()
            }
        except Exception:
            # 시계열 집계 실패 시 빈 데이터 반환
            normalized_data['time_series_data'] = {'timestamps': [], 'counts': []}
    
    return normalized_data

# 청크 단위 로그 처리 (대용량 로그 파일용)
def process_log_in_chunks(log_file_path: str, chunk_size: int = 10000):
    """
    대용량 로그 파일을 청크 단위로 처리합니다.
    
    Args:
        log_file_path: 로그 파일 경로
        chunk_size: 청크 크기 (라인 수)
        
    Yields:
        processed_chunk: 처리된 로그 청크
    """
    with open(log_file_path, 'r') as f:
        chunk = []
        for i, line in enumerate(f):
            chunk.append(line)
            
            if (i + 1) % chunk_size == 0:
                chunk_content = ''.join(chunk)
                processed_chunk = process_log(chunk_content)
                yield processed_chunk
                chunk = []
        
        # 마지막 청크 처리
        if chunk:
            chunk_content = ''.join(chunk)
            processed_chunk = process_log(chunk_content)
            yield processed_chunk
