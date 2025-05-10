import pandas as pd
import re
from datetime import datetime
import json
from io import StringIO
import time
from typing import Dict, List, Any, Union

LOG_PATTERNS = {
    # 기존 패턴 개선
    'apache': r'^(?P<ip>\S+) \S+ \S+ \[(?P<timestamp>[^\]]+)\] "(?P<method>\S+)(?: +(?P<uri>[^"]*?)(?: +(?P<protocol>\S+))?)?" (?P<status>\d+) (?P<bytes>\d+|-)',
    'nginx': r'^(?P<remote_addr>\S+) - (?P<remote_user>\S+) \[(?P<time_local>[^\]]+)\] "(?P<request>\S+ \S+ \S+)" (?P<status>\d+) (?P<body_bytes_sent>\d+) "(?P<http_referer>[^"]*)" "(?P<http_user_agent>[^"]*)"',
    
    # 신규 시스템 로그 포맷
    'syslog_bsd': r'^<(?P<pri>\d+)>(?P<timestamp>\w{3} \d{2} \d{2}:\d{2}:\d{2}) (?P<hostname>\S+) (?P<tag>\w+)(?:\[(?P<pid>\d+)\])?: (?P<message>.*)',
    'syslog_rfc5424': r'^<(?P<pri>\d+)>1 (?P<timestamp>\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}(?:\.\d+)?Z) (?P<hostname>\S+) (?P<app>\S+) (?P<procid>\S+) (?P<msgid>\S+) (?P<structured_data>\[.*?\]) (?P<message>.*)',
    
    # Windows 이벤트 로그
    'windows_event': r'^(?P<timestamp>\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}\.\d+Z) (?P<event_id>\d+) (?P<level>\w+) (?P<source>\S+) (?P<message>.*)',
    
    # CEF 포맷
    'cef': r'CEF:(?P<cef_version>\d+)\|(?P<device_vendor>[^|]*)\|(?P<device_product>[^|]*)\|(?P<device_version>[^|]*)\|(?P<signature_id>[^|]*)\|(?P<name>[^|]*)\|(?P<severity>[^|]*)\|(?P<extension>.*)',
    
    # GELF 포맷 (JSON 구조)
    'gelf': r'^{"version":"1\.1", "host":"(?P<host>\S+)", "short_message":"(?P<short_message>.*?)", ',
    
    # AWS CloudTrail
    'aws_cloudtrail': r'^(?P<event_time>\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}Z) (?P<event_source>\S+) (?P<event_name>\S+)',
    
    # IIS W3C 확장 형식
    'iis_w3c': r'^(?P<date>\d{4}-\d{2}-\d{2}) (?P<time>\d{2}:\d{2}:\d{2}) (?P<s_ip>\S+) (?P<cs_method>\S+) (?P<cs_uri_stem>\S+) (?P<cs_uri_query>\S+) (?P<s_port>\d+)',
    
    # Docker JSON 로그
    'docker_json': r'\{"log":"(?P<message>.*?)","stream":"(?P<stream>stdout|stderr)","time":"(?P<timestamp>.*?)"\}',

    # 커스텀 애플리케이션 로그
    'custom_app': r'^\[(?P<timestamp>\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2},\d{3})\] (?P<level>\w+) (?P<thread>\S+) (?P<logger>\S+) - (?P<message>.*)',
    
    # 네트워크 장비 로그 (Cisco)
    'cisco_asa': r'^%ASA-(?P<level>\d+)-(?P<message_id>\d+): (?P<message>.*)',
    
    # Kubernetes Pod 로그
    'k8s_pod': r'^(?P<timestamp>\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}\.\d+Z) \[(?P<level>\w+)\] (?P<message>.*)',
    
    # ELF(Extended Log Format)
    'elf': r'^#Version: (?P<version>\d+\.\d+)\n#Fields: (?P<fields>.*?)\n(?P<data>.*)',
    
    # W3C 확장 로그
    'w3c_extended': r'^(?P<date>\d{4}-\d{2}-\d{2}) (?P<time>\d{2}:\d{2}:\d{2}) (?P<s_ip>\S+) (?P<cs_method>\S+) (?P<cs_uri_stem>\S+) (?P<cs_uri_query>\S+) (?P<s_port>\d+)',
    
    # Heroku 로그
    'heroku': r'^(?P<timestamp>\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}\.\d+Z) (?P<source>\S+) (?P<dyno>\S+) (?P<message>.*)',
    
    # CloudWatch 로그
    'aws_cloudwatch': r'^\[(?P<timestamp>\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}\.\d+Z)\] (?P<level>\w+) (?P<message>.*)'
}

# 패턴 확장성: 구조화 데이터 파싱 및 동적 필드 매핑
FIELD_MAPPINGS = {
    'syslog_rfc5424': {
        'pri': lambda x: {'facility': int(x)//8, 'severity': int(x)%8}
    }
}

def parse_structured_data(sd: str) -> Dict[str, str]:
    """ structured_data 필드에서 key="value" 쌍을 파싱합니다."""
    return dict(re.findall(r'(\S+)="([^"]*)"', sd))

def detect_log_format(sample: str) -> str:
    """ 샘플 텍스트 기반으로 로그 형식을 감지합니다. """
    try:
        json.loads(sample)
        return 'json'
    except:
        pass
    if ',' in sample and '\n' in sample:
        return 'csv'
    for fmt, pattern in LOG_PATTERNS.items():
        if re.search(pattern, sample, flags=re.MULTILINE):
            return fmt
    return 'standard'

def process_log(content: str, log_format: str = "standard") -> pd.DataFrame:
    # 패턴 확장성: 감지된 형식 사용 or 파라미터 우선
    fmt = detect_log_format(content[:1000]) if log_format == "standard" else log_format
    if fmt == 'json':
        return process_json_logs(content)
    elif fmt == 'csv':
        return process_csv_logs(content)
    # MM/DD HH:MM:SS 형식 우선 처리
    alt_pattern = r'(\d{2}/\d{2} \d{2}:\d{2}:\d{2})\s+(\w+)\s+(:[^:]+:)\s+(.*)'
    alt_logs = []
    for line in content.splitlines():
        m = re.match(alt_pattern, line)
        if m:
            ts, lvl, comp, msg = m.groups()
            alt_logs.append({
                "timestamp": ts,
                "level": lvl,
                "component": comp.strip(':'),
                "message": msg
            })
    if alt_logs:
        return pd.DataFrame(alt_logs)
    pattern = LOG_PATTERNS.get(fmt)
    if not pattern:
        raise ValueError(f"지원하지 않는 로그 형식: {fmt}")
    
    logs = []
    for line in content.splitlines():
        match = re.match(pattern, line)
        if match:
            if fmt == "standard":
                timestamp, level, component, message = match.groups()
                logs.append({
                    "timestamp": timestamp,
                    "level": level,
                    "component": component,
                    "message": message
                })
            elif fmt == "apache":
                ip, timestamp, request, status, size = match.groups()
                logs.append({
                    "ip": ip,
                    "timestamp": timestamp,
                    "request": request,
                    "status": status,
                    "size": size if size != "-" else "0"
                })
            elif fmt == "nginx":
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
            elif fmt == "custom":
                timestamp, level, component, message = match.groups()
                logs.append({
                    "timestamp": timestamp,
                    "level": level,
                    "component": component,
                    "message": message
                })
            else:
                # 기타 패턴: groupdict 구조화 + structured_data 파싱
                entry = match.groupdict()
                sd = entry.pop('structured_data', None)
                if sd:
                    entry.update(parse_structured_data(sd))
                logs.append(entry)
    # 동적 필드 매핑 적용
    for entry in logs:
        for fld, mapper in FIELD_MAPPINGS.get(fmt, {}).items():
            if fld in entry:
                entry.update(mapper(entry[fld]))
    df = pd.DataFrame(logs)
    if not df.empty:
        return df

    # AI 기반 폴백 파싱
    from gpt_analyzer import GPTAnalyzer
    analyzer = GPTAnalyzer()
    try:
        parsed = analyzer.parse_logs(content)
        if parsed:
            return pd.DataFrame(parsed)
    except Exception:
        pass

    return pd.DataFrame()

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
            # Ensure only dict entries are added to avoid DataFrame errors
            if isinstance(log_entry, dict):
                log_entry['raw_log'] = line
                parsed_logs.append(log_entry)
            else:
                logger.warning(f"JSON 파싱 결과가 dict가 아닙니다. 스킵: {line[:100]}")
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
import pandas as pd
import json
import re
from datetime import datetime
import logging

# 로깅 설정
logger = logging.getLogger(__name__)

def process_log(log_content, log_format='json'):
    """
    로그 콘텐츠를 처리하고 분석 가능한 데이터프레임으로 변환합니다.
    
    Args:
        log_content (str): 로그 파일의 내용
        log_format (str): 로그 형식 (json, csv, text 등)
        
    Returns:
        pandas.DataFrame: 처리된 로그 데이터프레임
    """
    logger.info(f"로그 처리 시작: 형식 {log_format}")
    
    try:
        if log_format == 'json':
            return _process_json_log(log_content)
        elif log_format == 'csv':
            return _process_csv_log(log_content)
        elif log_format == 'text':
            return _process_text_log(log_content)
        else:
            logger.warning(f"지원되지 않는 로그 형식: {log_format}")
            raise ValueError(f"지원되지 않는 로그 형식: {log_format}")
    except Exception as e:
        logger.error(f"로그 처리 중 오류 발생: {str(e)}")
        raise

def _process_json_log(log_content):
    """JSON 로그 처리"""
    logs = []
    for line in log_content.splitlines():
        if line.strip():
            try:
                log_entry = json.loads(line)
                # Ensure only dict entries are added to avoid DataFrame errors
                if isinstance(log_entry, dict):
                    logs.append(log_entry)
                else:
                    logger.warning(f"JSON 파싱 결과가 dict가 아닙니다. 스킵: {line[:100]}")
            except json.JSONDecodeError:
                logger.warning(f"JSON 파싱 오류: {line[:100]}")
    
    if not logs:
        return pd.DataFrame()
    
    return pd.DataFrame(logs)

def _process_csv_log(log_content):
    """CSV 로그 처리"""
    try:
        # CSV 파싱 시도
        df = pd.read_csv(pd.StringIO(log_content))
        return df
    except Exception as e:
        logger.error(f"CSV 로그 처리 오류: {str(e)}")
        return pd.DataFrame()

def _process_text_log(log_content):
    """텍스트 로그 처리 - 일반적인 로그 패턴 검색"""
    # 일반적인 로그 패턴: [시간] [레벨] [컴포넌트] - 메시지
    pattern = r'(\d{4}-\d{2}-\d{2}\s\d{2}:\d{2}:\d{2}(?:\.\d+)?)\s+(\w+)\s+(?:\[([^\]]+)\])?\s*-?\s*(.*)'
    logs = []
    
    for line in log_content.splitlines():
        match = re.match(pattern, line)
        if match:
            timestamp, level, component, message = match.groups()
            logs.append({
                'timestamp': timestamp,
                'level': level,
                'component': component or '',
                'message': message
            })
        elif line.strip():
            # 패턴에 맞지 않는 줄은 이전 로그의 연속으로 처리
            if logs:
                logs[-1]['message'] += '\n' + line
    
    return pd.DataFrame(logs)

def normalize_logs(df):
    """
    로그 데이터프레임을 정규화하고 표준 형식으로 변환합니다.
    
    Args:
        df (pandas.DataFrame): 원시 로그 데이터프레임
        
    Returns:
        pandas.DataFrame: 정규화된 로그 데이터프레임
    """
    logger.info("로그 정규화 시작")
    
    if df.empty:
        return df
    
    # 필수 열 확인 및 추가
    required_columns = ['timestamp', 'level', 'message']
    result_df = df.copy()
    
    # 타임스탬프 열 처리
    timestamp_columns = [col for col in df.columns if 'time' in col.lower() or 'date' in col.lower()]
    if 'timestamp' not in df.columns and timestamp_columns:
        result_df['timestamp'] = df[timestamp_columns[0]]
    elif 'timestamp' not in df.columns:
        result_df['timestamp'] = datetime.now().isoformat()
    
    # 로그 레벨 열 처리
    level_columns = [col for col in df.columns if 'level' in col.lower() or 'severity' in col.lower()]
    if 'level' not in df.columns and level_columns:
        result_df['level'] = df[level_columns[0]]
    elif 'level' not in df.columns:
        result_df['level'] = 'INFO'
    
    # 메시지 열 처리
    message_columns = [col for col in df.columns if 'message' in col.lower() or 'msg' in col.lower()]
    if 'message' not in df.columns and message_columns:
        result_df['message'] = df[message_columns[0]]
    elif 'message' not in df.columns:
        # 메시지가 없는 경우, 다른 컬럼들을 합쳐서 메시지 생성
        other_cols = [col for col in df.columns if col not in ['timestamp', 'level']]
        if other_cols:
            result_df['message'] = df[other_cols].apply(lambda row: ' '.join(str(val) for val in row), axis=1)
        else:
            result_df['message'] = ''
    
    # 타임스탬프 형식 표준화
    if 'timestamp' in result_df.columns:
        result_df['timestamp'] = standardize_timestamps(result_df['timestamp'])
    
    # 로그 레벨 표준화
    if 'level' in result_df.columns:
        result_df['level'] = standardize_log_levels(result_df['level'])
    
    logger.info("로그 정규화 완료")
    return result_df

def standardize_timestamps(timestamp_series):
    """타임스탬프를 표준 형식으로 변환"""
    def convert_timestamp(ts):
        if pd.isna(ts):
            return None
        
        ts = str(ts)
        
        # Unix 타임스탬프 (초 단위)
        if ts.isdigit() and len(ts) == 10:
            return datetime.fromtimestamp(int(ts)).isoformat()
        
        # Unix 타임스탬프 (밀리초 단위)
        if ts.isdigit() and len(ts) == 13:
            return datetime.fromtimestamp(int(ts) / 1000).isoformat()
        
        # 다양한 날짜 형식 처리 시도
        formats = [
            '%Y-%m-%d %H:%M:%S',
            '%Y-%m-%dT%H:%M:%S',
            '%Y-%m-%dT%H:%M:%S.%f',
            '%Y/%m/%d %H:%M:%S',
            '%d/%b/%Y:%H:%M:%S'
        ]
        
        for fmt in formats:
            try:
                return datetime.strptime(ts, fmt).isoformat()
            except ValueError:
                continue
        
        return ts

def standardize_log_levels(level_series):
    """로그 레벨을 표준 형식으로 변환"""
    level_map = {
        'debug': 'DEBUG',
        'info': 'INFO',
        'information': 'INFO',
        'warn': 'WARNING',
        'warning': 'WARNING',
        'error': 'ERROR',
        'err': 'ERROR',
        'critical': 'CRITICAL',
        'fatal': 'CRITICAL'
    }
    
    def map_level(level):
        if pd.isna(level):
            return 'INFO'
        
        level_str = str(level).lower()
        return level_map.get(level_str, level)
    
    return level_series.apply(map_level)

def extract_error_patterns(df):
    """로그에서 오류 패턴을 추출"""
    if 'level' not in df.columns or 'message' not in df.columns:
        return {}
    
    error_df = df[df['level'].str.upper().isin(['ERROR', 'CRITICAL', 'FATAL'])]
    if error_df.empty:
        return {}
    
    # 간단한 오류 패턴 추출
    error_patterns = {}
    for message in error_df['message']:
        # 오류 코드 추출 (예: ERR-1234)
        code_match = re.search(r'(ERR|ERROR|E)-\d+', message)
        if code_match:
            code = code_match.group(0)
            error_patterns[code] = error_patterns.get(code, 0) + 1
            continue
            
        # Exception 유형 추출
        exception_match = re.search(r'([A-Za-z]+Exception|[A-Za-z]+Error)', message)
        if exception_match:
            exception = exception_match.group(0)
            error_patterns[exception] = error_patterns.get(exception, 0) + 1
            continue
            
        # 일반적인 오류 메시지 키워드
        keywords = ['failed', 'failure', 'error', 'cannot', 'unable to', 'timeout']
        for keyword in keywords:
            if keyword in message.lower():
                error_patterns[keyword] = error_patterns.get(keyword, 0) + 1
                break
    
    return error_patterns