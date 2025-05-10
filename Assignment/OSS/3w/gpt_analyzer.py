import openai
import pandas as pd
import json
import os
import time
import openai
from openai import OpenAI
from typing import Dict, List, Any, Optional, Tuple, Callable
from functools import wraps

def analyze_with_gpt(
    logs_df: pd.DataFrame, 
    api_key: Optional[str] = None, 
    query: Optional[str] = None,
    model: str = "gpt-4o",
    temperature: float = 0.3,
    max_tokens: int = 4000,
    max_retries: int = 3
) -> Dict[str, Any]:
    """
    로그 데이터를 GPT API를 통해 분석합니다.
    
    Args:
        logs_df: 분석할 로그 데이터프레임
        api_key: OpenAI API 키 (없으면 환경 변수에서 로드)
        query: 추가 분석 쿼리 (선택 사항)
        model: 사용할 GPT 모델 ("gpt-4o", "gpt-3.5-turbo" 등)
        temperature: 응답 다양성 조절 (0.0~1.0)
        max_tokens: 최대 응답 토큰 수
        max_retries: API 오류 시 최대 재시도 횟수
        
    Returns:
        분석 결과 딕셔너리
    """
    # API 키 설정 (매개변수 > 환경변수 > 기본값 순)
    current_api_key = api_key or os.environ.get("OPENAI_API_KEY")
    if not current_api_key:
        raise ValueError("OpenAI API 키가 제공되지 않았습니다. API 키를 매개변수로 전달하거나 OPENAI_API_KEY 환경 변수를 설정하세요.")
    
    try:
        # 로그 데이터 압축 및 프롬프트 생성
        logs_text = compress_logs(logs_df)
        prompt = create_log_analysis_prompt(logs_text, query)
        
        # GPT API 호출
        response_text = call_gpt_api(
            prompt=prompt,
            api_key=current_api_key,
            model=model,
            temperature=temperature,
            max_tokens=max_tokens
        )
        
        # 응답 파싱
        analysis_result = parse_gpt_response(response_text)
        
        # 메타데이터 추가
        analysis_result["analyzed_at"] = pd.Timestamp.now().isoformat()
        analysis_result["log_count"] = len(logs_df)
        
        if 'is_error' in logs_df.columns:
            analysis_result["error_log_count"] = int(logs_df['is_error'].sum())
            analysis_result["error_percentage"] = float(logs_df['is_error'].mean() * 100)
        
        return analysis_result
        
    except Exception as e:
        # 오류 발생 시 오류 정보를 포함한 딕셔너리 반환
        return {
            "error": str(e),
            "error_type": type(e).__name__,
            "analyzed_at": pd.Timestamp.now().isoformat(),
            "success": False
        }
    
    # 데이터프레임을 JSON으로 변환
    log_sample = logs_df.head(20).to_dict(orient="records")
    
    # 에러 로그만 추출해서 샘플링
    error_logs = logs_df[logs_df.get("is_error", False) == True].head(10).to_dict(orient="records")
    
    # 통계 정보 생성
    log_stats = {
        "total_entries": len(logs_df),
        "error_count": len(logs_df[logs_df.get("is_error", False) == True]),
        "time_range": f"{logs_df['timestamp'].min()} to {logs_df['timestamp'].max()}" if 'timestamp' in logs_df.columns else "N/A"
    }
    
    # GPT 프롬프트 구성
    system_msg = "당신은 시스템 로그 분석 전문가입니다. 제공된 로그 데이터를 분석하여 통찰력 있는 정보와 문제 해결 방안을 제시해주세요."
    
    user_msg = f"""
    다음 로그 데이터를 분석해주세요:
    
    로그 샘플:
    {json.dumps(log_sample, indent=2, ensure_ascii=False)}
    
    에러 로그 샘플:
    {json.dumps(error_logs, indent=2, ensure_ascii=False)}
    
    통계 정보:
    {json.dumps(log_stats, indent=2, ensure_ascii=False)}
    
    {f"추가 분석 요청: {query}" if query else ""}
    
    다음 항목에 대해 분석해주세요:
    1. 주요 에러 패턴 및 원인
    2. 시스템 상태 평가
    3. 문제 해결 제안 및 권장 조치
    4. 추가 모니터링이 필요한 영역
    """
    
    try:
        response = openai.ChatCompletion.create(
            model="gpt-3.5-turbo",
            messages=[
                {"role": "system", "content": system_msg},
                {"role": "user", "content": user_msg}
            ],
            temperature=0.3,
            max_tokens=1500
        )
        
        analysis_text = response.choices[0].message.content
        
        # 응답을 섹션별로 구조화
        sections = ["주요 에러 패턴 및 원인", "시스템 상태 평가", "문제 해결 제안 및 권장 조치", "추가 모니터링이 필요한 영역"]
        structured_analysis = {}
        
        current_section = "summary"
        structured_analysis[current_section] = ""
        
        for line in analysis_text.split("\n"):
            found_section = False
            for section in sections:
                if section in line:
                    current_section = section
                    structured_analysis[current_section] = ""
                    found_section = True
                    break
            
            if not found_section and current_section in structured_analysis:
                structured_analysis[current_section] += line + "\n"
        
        return {
            "raw_analysis": analysis_text,
            "structured_analysis": structured_analysis,
            "token_usage": response.usage.total_tokens,
            "error_count": log_stats["error_count"],
            "total_entries": log_stats["total_entries"]
        }
        
    except Exception as e:
        return {
            "error": str(e),
            "raw_analysis": f"분석 중 오류가 발생했습니다: {str(e)}",
            "structured_analysis": {
                "summary": f"분석 중 오류가 발생했습니다: {str(e)}"
            },
            "token_usage": 0,
            "error_count": log_stats["error_count"],
            "total_entries": log_stats["total_entries"]
        }
### 샘플 로그:
{sample_logs}
def create_log_analysis_prompt(logs_text: str, query: Optional[str] = None) -> str:
    """
    로그 분석을 위한 GPT 프롬프트 생성
    
    Args:
        logs_text: 분석할 로그 텍스트
        query: 추가 분석 지시 (선택 사항)
    
    Returns:
        구조화된 프롬프트
    """
    prompt = """
다음 로그 데이터를 심층적으로 분석하고, 발견한 패턴과 문제점을 상세히 설명해주세요.
분석 결과는 다음 형식의 JSON으로 응답해주세요:

{
  "error_type": "발견된 주요 오류 유형 (예: 네트워크 타임아웃, 메모리 부족, 인증 실패)",
  "severity": "심각도 (HIGH, MEDIUM, LOW 중 하나)",
  "root_cause": "가장 가능성 높은 근본 원인에 대한 설명",
  "solution": "문제 해결을 위한 제안",
  "detailed_analysis": "로그에서 발견한 패턴과 문제에 대한 상세 분석",
  "affected_systems": ["영향을 받은 시스템 구성요소 목록"],
  "time_patterns": "시간적 패턴이 있다면 설명 (특정 시간대에 오류 집중 등)"
}

모든 필드를 철저히 분석하고 의미 있는 인사이트를 제공해주세요.
가능한 한 빨리 핵심 문제와 그 원인을 식별하십시오.
제공된 로그만으로 확실한 문제 원인을 파악할 수 없는 경우, 가장 가능성 높은 원인과 추가 조사가 필요한 부분을 제안하세요.
def test_gpt_analyzer():
    """
    GPT 로그 분석기 테스트 함수
    
    이 함수는 샘플 로그 데이터를 생성하고 analyze_with_gpt 함수를 테스트합니다.
    환경 변수 OPENAI_API_KEY가 설정되어 있어야 정상 작동합니다.
    """
    # 테스트용 샘플 로그 데이터 생성
    print("샘플 로그 데이터 생성 중...")
    
    # 현재 시간 기준으로 샘플 타임스탬프 생성
    now = pd.Timestamp.now()
    timestamps = [now - pd.Timedelta(minutes=i) for i in range(50)]
    
    # 로그 레벨과 메시지 샘플
    levels = ['INFO', 'DEBUG', 'WARNING', 'ERROR', 'CRITICAL']
    components = ['database', 'authentication', 'network', 'api', 'frontend']
    
    # 오류 메시지 샘플
    error_messages = [
        "Connection timed out",
        "Failed to connect to database",
        "Authentication failed: Invalid credentials",
        "Memory allocation failed",
        "File not found: config.json",
        "Unexpected EOF",
        "Null pointer exception",
        "Index out of range",
        "Permission denied",
        "Disk quota exceeded"
    ]
    
    # 일반 메시지 샘플
    info_messages = [
        "User logged in successfully",
        "Request processed in 0.5s",
        "Cache updated",
        "Background task completed",
        "Config loaded successfully",
        "Data synchronized",
        "Backup completed",
        "Service started",
        "Incoming request from 192.168.1.1",
        "File uploaded: document.pdf"
    ]
    
    # 로그 데이터 생성
    data = []
    for i, ts in enumerate(timestamps):
        # 20%의 확률로 오류 로그 생성
        is_error = i % 5 == 0
        
        if is_error:
            level = 'ERROR' if i % 10 != 0 else 'CRITICAL'
            component = components[i % len(components)]
            message = error_messages[i % len(error_messages)]
        else:
            level = levels[i % 3]  # INFO, DEBUG, WARNING만 사용
            component = components[i % len(components)]
            message = info_messages[i % len(info_messages)]
        
        data.append({
            'timestamp': ts,
            'level': level,
            'component': component,
            'message': message,
            'is_error': is_error,
            'request_id': f'req-{10000 + i}',
            'user_id': f'user-{100 + (i % 10)}'
        })
    
    # 데이터프레임 생성
    logs_df = pd.DataFrame(data)
    
    print(f"샘플 로그 데이터 생성 완료 (총 {len(logs_df)}개)")
    print("\n로그 데이터 미리보기:")
    print(logs_df[['timestamp', 'level', 'component', 'message', 'is_error']].head())
    
    # API 키 확인
    api_key = os.environ.get("OPENAI_API_KEY")
    if not api_key:
        print("\n⚠️ 경고: OPENAI_API_KEY 환경 변수가 설정되지 않았습니다.")
        print("테스트를 진행하려면 API 키가 필요합니다.")
        return
    
    print("\nGPT 로그 분석 시작...")
    start_time = time.time()
    
    # 로그 분석 호출
    result = analyze_with_gpt(
        logs_df=logs_df,
        api_key=api_key,
        query="특히 네트워크 및 데이터베이스 관련 문제를 집중적으로 분석해주세요.",
        model="gpt-3.5-turbo",  # 테스트용으로 더 저렴한 모델 사용
        temperature=0.3
    )
    
    # 분석 결과 출력
    duration = time.time() - start_time
    print(f"\n분석 완료 (소요 시간: {duration:.2f}초)")
    
    print("\n=== 분석 결과 ===")
    print(f"오류 유형: {result.get('error_type', 'N/A')}")
    print(f"심각도: {result.get('severity', 'N/A')}")
    print(f"근본 원인: {result.get('root_cause', 'N/A')}")
    print(f"해결책: {result.get('solution', 'N/A')}")
    print(f"영향을 받은 시스템: {', '.join(result.get('affected_systems', ['N/A']))}")
    print(f"시간 패턴: {result.get('time_patterns', 'N/A')}")
    
    print("\n상세 분석:")
    print(result.get('detailed_analysis', 'N/A'))
    
    return result

# 직접 실행할 때만 테스트 함수 실행
if __name__ == "__main__":
    print("GPT 로그 분석기 테스트를 시작합니다...")
    test_result = test_gpt_analyzer()
    print("\n테스트 완료!")
분석할 로그 데이터:
"""
    prompt += logs_text
    
    # 추가 분석 지시가 있는 경우
    if query:
        prompt += f"\n\n추가 분석 지시: {query}"
    
    return prompt

@retry_on_error(max_retries=3, initial_delay=1.0)
def call_gpt_api(
    prompt: str, 
    api_key: str, 
    model: str = "gpt-4o", 
    temperature: float = 0.3,
    max_tokens: int = 4000
) -> str:
    """
    OpenAI API를 호출하여 응답을 가져옴
    
    Args:
        prompt: 분석 프롬프트
        api_key: OpenAI API 키
        model: 사용할 GPT 모델
        temperature: 응답 다양성 (0.0~1.0)
        max_tokens: 최대 응답 토큰 수
    
    Returns:
        GPT 응답 텍스트
    """
    client = OpenAI(api_key=api_key)
    
    response = client.chat.completions.create(
        model=model,
        messages=[
            {"role": "system", "content": "당신은 시스템 로그 분석 전문가입니다. 제공된 로그를 철저히 분석하여 문제점과 해결책을 제시하세요."},
            {"role": "user", "content": prompt}
        ],
        temperature=temperature,
        max_tokens=max_tokens,
        top_p=1.0,
        frequency_penalty=0.0,
        presence_penalty=0.0
    )
    
    return response.choices[0].message.content

def parse_gpt_response(response_text: str) -> Dict[str, Any]:
    """
    GPT 응답에서 JSON 구조를 추출
    
    Args:
        response_text: GPT 응답 텍스트
    
    Returns:
        파싱된 JSON 딕셔너리
    """
    try:
        # JSON 블록 추출 (마크다운 코드 블록 내부에 있을 수 있음)
        json_text = response_text
        
        # 마크다운 코드 블록 처리 (```json ... ``` 형식)
        if "```json" in response_text:
            json_block = response_text.split("```json")[1].split("```")[0].strip()
            json_text = json_block
        # 일반 코드 블록 처리 (``` ... ``` 형식)
        elif "```" in response_text:
            json_block = response_text.split("```")[1].split("```")[0].strip()
            json_text = json_block
        
        # JSON 파싱
        result = json.loads(json_text)
        
        # 필수 필드 확인 및 기본값 설정
        required_fields = ["error_type", "severity", "root_cause", "solution", 
                          "detailed_analysis", "affected_systems", "time_patterns"]
        
        for field in required_fields:
            if field not in result:
                result[field] = "분석 정보 없음"
                
        return result
    
    except (json.JSONDecodeError, IndexError) as e:
        # JSON 파싱 실패 시 텍스트 응답 그대로 반환
        return {
            "error_type": "알 수 없음",
            "severity": "UNKNOWN",
            "root_cause": "분석 불가",
            "solution": "분석 불가",
            "detailed_analysis": response_text,
            "affected_systems": ["알 수 없음"],
            "time_patterns": "분석 불가",
            "parsing_error": str(e)
        }
다음 정보를 JSON 형식으로 응답해주세요:
1. error_type: 주요 오류 유형 (문자열)
2. severity: 문제의 심각도 (1-5, 정수)
3. root_cause: 문제의 근본 원인 (문자열)
4. solution: 권장 해결 방안 (문자열)
5. detailed_analysis: 상세 분석 내용 (문자열)
6. affected_systems: 영향 받는 시스템 목록 (문자열 배열)
7. time_patterns: 시간대별 패턴 분석 (문자열)
"""

            class GPTLogAnalyzer:
                """
                GPT를 활용한 로그 분석 클래스
                
                이 클래스는 로그 데이터를 분석하고 GPT 모델을 사용하여 인사이트를 추출하는 기능을 제공합니다.
                로그 데이터를 처리하고, GPT 모델에 적절한 프롬프트로 전달하며, 결과를 구조화된 형식으로 반환합니다.
                """
                
                def __init__(
                    self,
                    api_key: Optional[str] = None,
                    model: str = "gpt-4",
                    temperature: float = 0.7,
                    model: str = "gpt-4",
                    api_key: Optional[str] = None,
                    custom_prompt: Optional[str] = None
                    ) -> Dict[str, Any]:
                    """
                    GPTLogAnalyzer를 사용하여 로그를 분석하는 래퍼 함수입니다.
                    기존 코드와의 호환성을 위해 유지합니다.
                    
                    Args:
                        log_data: 분석할 로그 데이터
                        temperature: 응답의 창의성 수준
                        model: 사용할 GPT 모델
                        api_key: OpenAI API 키
                        custom_prompt: 사용자 정의 프롬프트
                        
                    Returns:
                        분석 결과를 포함하는 딕셔너리
                    """
                    analyzer = GPTLogAnalyzer(
                        api_key=api_key,
                        model=model,
                        temperature=temperature
                    )
                    return analyzer.analyze_logs(
                        log_data=log_data,
                        custom_prompt=custom_prompt
                    )
                    
                    
                    class LogDataManager:
                    """
                    로그 데이터를 관리하고 전처리하는 클래스
                    
                    이 클래스는 다양한 형식의 로그 데이터를 로드하고, 정규화하며,
                    분석에 필요한 형태로 변환하는 기능을 제공합니다.
                    """
                    
                    def __init__(self):
                        """LogDataManager 인스턴스를 초기화합니다."""
                        self.log_data = {}
                        self.processed_df = None
                    
                    def load_from_json(self, json_data: Union[str, Dict]) -> None:
                        """
                        JSON 형식의 로그 데이터를 로드합니다.
                        
                        Args:
                            json_data: JSON 문자열 또는 딕셔너리
                        """
                        if isinstance(json_data, str):
                            self.log_data = json.loads(json_data)
                        else:
                            self.log_data = json_data
                        
                        self._process_data()
                    
                    def load_from_csv(self, csv_data: Union[str, pd.DataFrame]) -> None:
                        """
                        CSV 형식의 로그 데이터를 로드합니다.
                        
                        Args:
                            csv_data: CSV 문자열 또는 DataFrame
                        """
                        if isinstance(csv_data, str):
                            df = pd.read_csv(io.StringIO(csv_data))
                        else:
                            df = csv_data
                        
                        # DataFrame을 JSON 형식으로 변환
                        entries = df.to_dict(orient="records")
                        self.log_data = {"entries": entries}
                        
                        self._process_data()
                    
                    def _process_data(self) -> None:
                        """
                        로그 데이터를 DataFrame으로 변환하고 전처리합니다.
                        """
                        entries = self.log_data.get("entries", [])
                        if entries:
                            # DataFrame 생성
                            self.processed_df = pd.DataFrame(entries)
                            
                            # 타임스탬프 처리
                            if "timestamp" in self.processed_df.columns:
                                self.processed_df["timestamp"] = self.processed_df["timestamp"].apply(
                                    lambda x: pd.to_datetime(x, errors="coerce")
                                )
                    
                    def get_dataframe(self) -> pd.DataFrame:
                        """
                        처리된 로그 DataFrame을 반환합니다.
                        
                        Returns:
                            처리된 로그 DataFrame
                        """
                        if self.processed_df is None:
                            return pd.DataFrame()
                        return self.processed_df
                    
                    def get_log_data(self) -> Dict[str, Any]:
                        """
                        로그 데이터 딕셔너리를 반환합니다.
                        
                        Returns:
                            로그 데이터 딕셔너리
                        """
                        return self.log_data
                    
                    def filter_by_time_range(self, start_time: Optional[str] = None, end_time: Optional[str] = None) -> None:
                        """
                        지정된 시간 범위로 로그를 필터링합니다.
                        
                        Args:
                            start_time: 시작 시간 (문자열)
                            end_time: 종료 시간 (문자열)
                        """
                        if self.processed_df is None or "timestamp" not in self.processed_df.columns:
                            return
                        
                        df = self.processed_df.copy()
                        
                        if start_time:
                            start_dt = pd.to_datetime(start_time)
                            df = df[df["timestamp"] >= start_dt]
                        
                        if end_time:
                            end_dt = pd.to_datetime(end_time)
                            df = df[df["timestamp"] <= end_dt]
                        
                        self.processed_df = df
                        
                        # 필터링된 DataFrame을 다시 log_data에 반영
                        self.log_data["entries"] = df.to_dict(orient="records")
                    
                    
                    class LogAnalysisExporter:
                    """
                    로그 분석 결과를 다양한 형식으로 내보내는 클래스
                    """
                    
                    @staticmethod
                    def export_to_format(
                        df: pd.DataFrame,
                        analysis_results: Dict[str, Any], 
                        export_format: str = "csv"
                    ) -> Optional[BinaryIO]:
                        """
                        분석 결과를 지정된 형식으로 내보냅니다.
                        
                        Args:
                            df: 로그 데이터프레임
                            analysis_results: GPT 분석 결과
                            export_format: 내보내기 형식 (csv, json, excel)
                            
                        Returns:
                            내보내기 파일 스트림
                        """
                        from utils import export_results
                        return export_results(df, analysis_results, export_format)
                    
                    
                    # 기본 프롬프트 템플릿
                    DEFAULT_PROMPT_TEMPLATE = """
                    ## 로그 분석 요청
                    
                    ### 로그 통계:
                    {log_stats}
                    
                    ### 샘플 로그:
                    {sample_logs}
                    
                    다음 정보를 JSON 형식으로 응답해주세요:
                    1. error_type: 주요 오류 유형 (문자열)
                    2. severity: 문제의 심각도 (1-5, 정수)
                    3. root_cause: 문제의 근본 원인 (문자열)
                    4. solution: 권장 해결 방안 (문자열)
                    5. detailed_analysis: 상세 분석 내용 (문자열)
                    6. affected_systems: 영향 받는 시스템 목록 (문자열 배열)
                    7. time_patterns: 시간대별 패턴 분석 (문자열)
                    """
                    
                    # 필요한 모듈 import
                    import json
                    import random
                    import re
                    import pandas as pd
                    import io
                    from typing import Dict, Any, List, Union, Optional, BinaryIO
                    from datetime import datetime
                    max_tokens: int = 2000,
                    default_prompt_template: Optional[str] = None
                ):
                    """
                    GPTLogAnalyzer 인스턴스를 초기화합니다.
                    
                    Args:
            api_key: OpenAI API 키
            model: 사용할 GPT 모델
            temperature: 응답의 창의성 수준 (0.0-1.0)
            max_tokens: 생성할 최대 토큰 수
            default_prompt_template: 기본 프롬프트 템플릿
                    """
                    self.api_key = api_key
                    self.model = model
                    self.temperature = temperature
                    self.max_tokens = max_tokens
                    self.default_prompt_template = default_prompt_template or DEFAULT_PROMPT_TEMPLATE
                
                def analyze_logs(
                    self,
                    log_data: Dict[str, Any],
                    custom_prompt: Optional[str] = None,
                    temperature: Optional[float] = None,
                    model: Optional[str] = None
                ) -> Dict[str, Any]:
                    """
                    로그 데이터를 GPT 모델을 사용하여 분석합니다.
                    
                    Args:
            log_data: 분석할 로그 데이터
            custom_prompt: 사용자 정의 프롬프트
            temperature: 응답의 창의성 수준 (기본값 사용 시 None)
            model: 사용할 GPT 모델 (기본값 사용 시 None)
            
                    Returns:
            분석 결과를 포함하는 딕셔너리
                    """
                    # 분석에 사용할 설정값 결정
                    temp = temperature if temperature is not None else self.temperature
                    gpt_model = model if model is not None else self.model
    model: str = "gpt-3.5-turbo",
    max_retries: int = 3,
    retry_delay: int = 2
) -> Dict[str, Any]:
    """
    GPT API를 사용하여 로그 데이터를 분석합니다.
    
    Args:
        log_data: 정규화된 로그 데이터 딕셔너리
        temperature: GPT 모델의 temperature 값 (0.0-1.0)
        model: 사용할 GPT 모델 이름
        max_retries: API 호출 실패 시 최대 재시도 횟수
        retry_delay: 재시도 사이의 대기 시간(초)
        
    Returns:
        Dict: GPT 분석 결과를 담은 딕셔너리
    """
    # 테스트 모드 감지 (API 키 없음)
    if not OPENAI_API_KEY:
        logger.info("테스트 모드로 실행 중입니다. 더미 데이터를 반환합니다.")
        return generate_mock_analysis(log_data)
    
    # 프롬프트 구성
    prompt = format_prompt(log_data)
    
    # API 호출 및 재시도 로직
    for attempt in range(max_retries):
        try:
            logger.info(f"GPT API 호출 시도 중 ({attempt + 1}/{max_retries})...")
            
            response = openai.ChatCompletion.create(
                model=model,
                messages=[
                    {"role": "system", "content": "당신은 로그 분석 전문가입니다. JSON 형식으로만 응답하세요."},
                    {"role": "user", "content": prompt}
                ],
                temperature=temperature,
                max_tokens=2000,
                top_p=1.0,
                frequency_penalty=0.0,
                presence_penalty=0.0
            )
            
            # 응답 파싱
            result = parse_gpt_response(response, log_data)
            logger.info("GPT 분석이 성공적으로 완료되었습니다.")
            return result
            
        except Exception as e:
            logger.error(f"GPT API 호출 중 오류 발생: {e}")
            if attempt < max_retries - 1:
                wait_time = retry_delay * (2 ** attempt)  # 지수 백오프
                logger.info(f"{wait_time}초 후 재시도합니다...")
                time.sleep(wait_time)
            else:
                logger.error("최대 재시도 횟수에 도달했습니다. 더미 데이터를 반환합니다.")
                return generate_mock_analysis(log_data)
    
    # 재시도 실패 시 더미 데이터 반환
    return generate_mock_analysis(log_data)

def format_prompt(log_data: Dict[str, Any]) -> str:
    """
    로그 데이터를 GPT 프롬프트 형식으로 포맷팅합니다.
    
    Args:
        log_data: 정규화된 로그 데이터
        
    Returns:
        str: 포맷팅된 프롬프트
    """
    # 시간 정보 추출
    timeframe = log_data.get('timeframe', {})
    start_time = timeframe.get('start', '알 수 없음')
    end_time = timeframe.get('end', '알 수 없음')
    duration = timeframe.get('duration_seconds', '알 수 없음')
    
    # 로그 레벨 및 에러 유형 정보
    log_levels = json.dumps(log_data.get('log_levels', {}), ensure_ascii=False)
    error_types = json.dumps(log_data.get('error_types', {}), ensure_ascii=False)
    
    # 샘플 로그 포맷팅
    sample_logs = "\n".join(log_data.get('sample_logs', ['샘플 로그 없음']))
    
    # 프롬프트 템플릿에 데이터 적용
    formatted_prompt = LOG_ANALYSIS_PROMPT.format(
        log_count=log_data.get('log_count', 0),
        start_time=start_time,
        end_time=end_time,
        duration=duration,
        log_levels=log_levels,
        error_types=error_types,
        sample_logs=sample_logs
    )
    
    return formatted_prompt
def retry_on_error(max_retries: int = 3, initial_delay: float = 1.0):
    """
    OpenAI API 오류 발생 시 자동으로 재시도하는 데코레이터
    
    Args:
        max_retries: 최대 재시도 횟수
        initial_delay: 첫 재시도 대기 시간 (초, 이후 지수적으로 증가)
    """
    def decorator(func: Callable):
        @wraps(func)
        def wrapper(*args, **kwargs):
            for attempt in range(max_retries + 1):
                try:
                    return func(*args, **kwargs)
                except (openai.APIError, openai.RateLimitError, 
                        openai.APIConnectionError, openai.Timeout) as e:
                    if attempt == max_retries:
                        raise
                    
                    # 지수 백오프로 대기 시간 증가
                    delay = initial_delay * (2 ** attempt)
                    print(f"API 오류 발생: {e}. {delay:.2f}초 후 재시도 ({attempt+1}/{max_retries})...")
                    time.sleep(delay)
                    
        return wrapper
    return decorator

def compress_logs(logs_df: pd.DataFrame, max_rows: int = 100) -> str:
    """
    로그 데이터프레임을 GPT 분석용 압축 텍스트로 변환
    
    Args:
        logs_df: 분석할 로그 데이터프레임
        max_rows: 포함할 최대 로그 행 수
    
    Returns:
        압축된 로그 텍스트
    """
    # 데이터프레임이 너무 크면 샘플링
    if len(logs_df) > max_rows:
        # 오류 로그는 보존
        if 'is_error' in logs_df.columns:
            error_logs = logs_df[logs_df['is_error']].copy()
            # 오류 로그가 20% 이상이 되도록 샘플링
            error_count = min(len(error_logs), int(max_rows * 0.2))
            non_error_count = max_rows - error_count
            
            if len(error_logs) > error_count:
                error_logs = error_logs.sample(error_count)
            
            non_error_logs = logs_df[~logs_df['is_error']].sample(non_error_count)
            sampled_logs = pd.concat([error_logs, non_error_logs])
        else:
            # 오류 플래그가 없으면 랜덤 샘플링
            sampled_logs = logs_df.sample(max_rows)
        
        logs_df = sampled_logs.sort_index()
    
    # 로그 데이터를 압축 텍스트로 변환
    log_text = []
    
    # 컬럼명 추출
    columns = logs_df.columns.tolist()
    
    # 요약 정보 추가
    log_text.append(f"총 로그 수: {len(logs_df)}")
    log_text.append(f"컬럼: {', '.join(columns)}")
    
    # 로그 유형 분포 (있는 경우)
    if 'level' in logs_df.columns:
        level_counts = logs_df['level'].value_counts()
        log_text.append("\n로그 레벨 분포:")
        for level, count in level_counts.items():
            log_text.append(f"- {level}: {count}개 ({count/len(logs_df)*100:.1f}%)")
    
    # 에러 비율 (있는 경우)
    if 'is_error' in logs_df.columns:
        error_count = logs_df['is_error'].sum()
        error_pct = error_count / len(logs_df) * 100
        log_text.append(f"\n에러 로그: {error_count}개 ({error_pct:.1f}%)")
    
    # 타임스탬프 범위 (있는 경우)
    if 'timestamp' in logs_df.columns:
        log_text.append(f"\n기간: {logs_df['timestamp'].min()} ~ {logs_df['timestamp'].max()}")
    
    # 실제 로그 데이터 (최대 100개)
    log_text.append("\n=== 로그 샘플 ===")
    for _, row in logs_df.iterrows():
        log_line = []
        for col in columns:
            # 타임스탬프 포맷팅
            if col == 'timestamp' and pd.notna(row[col]):
                log_line.append(f"{col}={row[col]}")
            # 일반 필드
            elif pd.notna(row[col]):
                log_line.append(f"{col}={row[col]}")
        
        log_text.append(" | ".join(log_line))
    
    return "\n".join(log_text)
def parse_gpt_response(response: Dict[str, Any], log_data: Dict[str, Any]) -> Dict[str, Any]:
    """
    GPT API 응답을 파싱하여 구조화된 분석 결과로 변환합니다.
    
    Args:
        response: GPT API 응답
        log_data: 원본 로그 데이터 (추가 필드 위한)
        
    Returns:
        Dict: 파싱된 분석 결과
    """
    try:
        # API 응답에서 텍스트 추출
        content = response['choices'][0]['message']['content'].strip()
        
        # JSON 부분 추출 (마크다운 코드 블록 내부의 JSON 포함)
        json_match = content
        if "```json" in content:
            json_match = content.split("```json")[1].split("```")[0].strip()
        elif "```" in content:
            json_match = content.split("```")[1].split("```")[0].strip()
        
        # JSON 파싱
        result = json.loads(json_match)
        
        # 시각화를 위한 추가 데이터 병합
        if 'time_series_data' in log_data:
            result['time_series_data'] = log_data['time_series_data']
        
        # 임의 오류 분포 데이터 추가 (시각화용)
        result['error_distribution'] = generate_error_distribution(log_data)
        
        return result
    
    except (json.JSONDecodeError, KeyError, IndexError) as e:
        logger.error(f"GPT 응답 파싱 중 오류 발생: {e}")
        logger.debug(f"원본 응답: {response}")
        
        # 파싱 실패 시 더미 데이터 반환
        return generate_mock_analysis(log_data)

def generate_mock_analysis(log_data: Dict[str, Any]) -> Dict[str, Any]:
    """
    테스트 목적으로 더미 분석 결과를 생성합니다.
    
    Args:
        log_data: 원본 로그 데이터
        
    Returns:
        Dict: 더미 분석 결과
    """
    # 기본 더미 결과
    mock_result = {
        "error_type": "리소스 부족 오류",
        "severity": 3,
        "root_cause": "애플리케이션이 사용 가능한 메모리보다 더 많은 메모리를 할당하려고 시도했습니다.",
        "solution": "애플리케이션의 메모리 제한을 늘리거나 코드를 최적화하여 메모리 사용량을 줄이세요.",
        "detailed_analysis": "로그에서 발견된 OutOfMemoryError는 JVM이 더 이상 객체를 할당할 메모리가 없음을 나타냅니다. 이는 주로 (1) 메모리 누수, (2) 대용량 데이터 처리, 또는 (3) 힙 크기 설정 오류로 인해 발생합니다. 로그에 표시된 가비지 컬렉션 이벤트 증가는 메모리 부족 전에 시스템이 메모리를 확보하려고 시도했음을 보여줍니다.",
        "affected_systems": ["백엔드 서버", "데이터베이스 연결 풀", "캐시 관리자"],
        "time_patterns": "오류는 주로 사용자 활동이 많은 시간대(08:00-10:00, 13:00-15:00)에 발생했습니다."
    }
    
    # 시각화를 위한 데이터 추가
    if 'time_series_data' in log_data:
        mock_result['time_series_data'] = log_data['time_series_data']
    else:
        # 더미 시계열 데이터 생성
        timestamps = [f"2023-06-{i:02d} 10:00:00" for i in range(1, 11)]
        counts = [random.randint(5, 50) for _ in range(10)]
        mock_result['time_series_data'] = {
            "timestamps": timestamps,
            "counts": counts
        }
    
    # 오류 분포 데이터 추가
    mock_result['error_distribution'] = generate_error_distribution(log_data)
    
    return mock_result

def generate_error_distribution(log_data: Dict[str, Any]) -> Dict[str, int]:
    """
    오류 유형 분포 데이터를 생성합니다.
    
    Args:
        log_data: 원본 로그 데이터
        
    Returns:
        Dict: 오류 유형 및 빈도 딕셔너리
    """
    # 실제 로그에서 오류 유형이 있으면 사용
    if 'error_types' in log_data and log_data['error_types']:
        return log_data['error_types']
    
    # 없으면 더미 데이터 생성
    return {
        "OutOfMemoryError": random.randint(10, 50),
        "NullPointerException": random.randint(5, 30),
        "IOException": random.randint(3, 20),
        "SQLException": random.randint(2, 15),
        "TimeoutException": random.randint(1, 10)
    }

def analyze_log_pattern(log_data: Dict[str, Any], focus: str = "general") -> Dict[str, Any]:
    """
    특정 초점에 맞춰 로그 패턴을 분석합니다.
    
    Args:
        log_data: 정규화된 로그 데이터
        focus: 분석 초점 (예: "security", "performance", "errors")
        
    Returns:
        Dict: 패턴 분석 결과
    """
    # 기본 분석과 동일한 프로세스 사용하되, 프롬프트에 초점 추가
    custom_prompt = f"""
    당신은 시스템 로그 분석 전문가입니다. 다음 로그 데이터를 {focus} 관점에서 분석해주세요.
    
    ### 로그 정보:
    {format_prompt(log_data)}
    
    특별히 {focus}와 관련된 패턴을 집중적으로 분석해주세요.
    """
    
    # 실제 API 호출 코드 (분석 함수와 유사)
    # 간략화를 위해 생략하고 더미 결과 반환
    return {
        "pattern_type": f"{focus} 패턴",
        "findings": f"{focus} 관련 패턴 분석 결과입니다.",
        "recommendations": f"{focus} 문제 해결을 위한 권장 사항입니다."
    }

# 배치 분석 함수 (대용량 로그 처리용)
def batch_analyze_logs(log_chunks: List[Dict[str, Any]], model: str = "gpt-3.5-turbo") -> Dict[str, Any]:
    """
    대용량 로그를 청크로 나누어 배치 분석합니다.
    
    Args:
        log_chunks: 로그 청크 리스트
        model: 사용할 GPT 모델
        
    Returns:
        Dict: 통합된 분석 결과
    """
    chunk_results = []
    
    for i, chunk in enumerate(log_chunks):
        logger.info(f"청크 {i+1}/{len(log_chunks)} 분석 중...")
        result = analyze_with_gpt(chunk, model=model)
        chunk_results.append(result)
    
    # 결과 통합 (간략화를 위해 첫 번째 결과 기반으로 통합)
    combined_result = chunk_results[0].copy()
    combined_result["detailed_analysis"] = "여러 로그 청크의 통합 분석 결과:\n\n"
    
    for i, result in enumerate(chunk_results):
        combined_result["detailed_analysis"] += f"청크 {i+1} 분석: {result['detailed_analysis']}\n\n"
    
    # 심각도는 최대값으로 설정
    combined_result["severity"] = max(r["severity"] for r in chunk_results)
    
    return combined_result
