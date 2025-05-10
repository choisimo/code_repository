import logging
import json
import openai
import os
import pandas as pd
from typing import Dict, List, Any, Optional, Union
import re
import time
from config import get_api_key

# 로깅 설정
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s'
)

logger = logging.getLogger("gpt-analyzer")

# OpenAI API 키 설정 (환경 변수에서 로드)
OPENAI_API_KEY = os.environ.get("OPENAI_API_KEY") or get_api_key()

class GPTAnalyzer:
    """
    GPT API를 사용하여 로그 데이터를 분석하는 클래스
    """
    
    def __init__(self, api_key: Optional[str] = None, model: str = "gpt-4", language: str = "한국어"):
        """
        GPT 분석기 초기화
        
        Args:
            api_key (str, optional): OpenAI API 키. 기본값은 환경 변수에서 로드됩니다.
            model (str): 사용할 GPT 모델. 기본값은 'gpt-4'
            language (str): 사용할 언어. 기본값은 '한국어'
        """
        self.language = language
        self.api_key = api_key or OPENAI_API_KEY
        if not self.api_key:
            logger.warning("OpenAI API 키가 설정되지 않았습니다. GPT 분석이 작동하지 않을 수 있습니다.")
        else:
            openai.api_key = self.api_key
        
        self.model = model
        logger.info(f"GPT 분석기 초기화 완료 (모델: {model}, 언어: {language})")
    
    def prepare_log_data(self, df: pd.DataFrame, max_logs: int = 50) -> str:
        """
        GPT API 요청을 위해 로그 데이터를 준비합니다.
        
        Args:
            df (pd.DataFrame): 로그 데이터프레임
            max_logs (int): 포함할 최대 로그 수
        
        Returns:
            str: GPT API로 전송할 형식의 로그 데이터
        """
        # 데이터가 너무 많은 경우 샘플링 (처음, 마지막, 에러 로그 위주)
        if len(df) > max_logs:
            # 에러 로그가 있으면 우선적으로 포함
            if 'is_error' in df.columns:
                error_logs = df[df['is_error']]
                if len(error_logs) > max_logs // 2:
                    error_logs = error_logs.sample(max_logs // 2)
                
                # 나머지는 비에러 로그에서 추출
                non_error_logs = df[~df['is_error']]
                remaining = max_logs - len(error_logs)
                # 처음과 마지막 로그 포함
                head_count = min(remaining // 2, 10)
                tail_count = min(remaining - head_count, 10)
                sample_count = remaining - head_count - tail_count
                
                if sample_count > 0 and len(non_error_logs) > head_count + tail_count:
                    middle_logs = non_error_logs.iloc[head_count:-tail_count].sample(sample_count)
                    sampled_df = pd.concat([
                        non_error_logs.head(head_count),
                        middle_logs,
                        non_error_logs.tail(tail_count),
                        error_logs
                    ])
                else:
                    sampled_df = pd.concat([
                        non_error_logs.head(head_count),
                        non_error_logs.tail(tail_count),
                        error_logs
                    ])
            else:
                # 에러 표시가 없는 경우 균등하게 샘플링
                head_count = min(max_logs // 3, 20)
                tail_count = min(max_logs // 3, 20)
                sample_count = max_logs - head_count - tail_count
                
                if sample_count > 0 and len(df) > head_count + tail_count:
                    middle_logs = df.iloc[head_count:-tail_count].sample(sample_count)
                    sampled_df = pd.concat([
                        df.head(head_count),
                        middle_logs,
                        df.tail(tail_count)
                    ])
                else:
                    # 데이터가 충분하지 않은 경우
                    sampled_df = df.head(max_logs)
            
            # 시간순 정렬
            if 'timestamp' in sampled_df.columns:
                sampled_df = sampled_df.sort_values('timestamp')
        else:
            sampled_df = df
        
        # 데이터프레임을 문자열로 변환
        log_text = sampled_df.to_string(index=False)
        
        # 로그 데이터 통계 추가
        stats = {
            "전체 로그 수": len(df),
            "샘플 로그 수": len(sampled_df)
        }
        
        if 'is_error' in df.columns:
            stats["에러 로그 수"] = df['is_error'].sum()
            stats["에러 비율"] = f"{df['is_error'].mean() * 100:.2f}%"
        
        if 'level' in df.columns:
            level_counts = df['level'].value_counts().to_dict()
            stats["로그 레벨 분포"] = level_counts
        
        # 최종 입력 텍스트 생성
        input_text = f"로그 데이터 통계:\n{json.dumps(stats, ensure_ascii=False, indent=2)}\n\n"
        input_text += f"로그 샘플 데이터:\n{log_text}\n"
        
        return input_text
    
    def analyze_logs(self, df: pd.DataFrame, specific_query: Optional[str] = None) -> Dict[str, Any]:
        """
        로그 데이터를 GPT API를 사용하여 분석합니다.
        
        Args:
            df (pd.DataFrame): 로그 데이터프레임
            specific_query (str, optional): 특정 분석 질문
        
        Returns:
            Dict[str, Any]: 구조화된 분석 결과
        """
        if not self.api_key:
            logger.error("API 키가 설정되지 않아 분석을 진행할 수 없습니다.")
            return generate_mock_analysis({"error": "API 키 없음", "data": df.head(5).to_dict()})
        
        # 로그 데이터 준비
        log_data = self.prepare_log_data(df)
        
        # 시스템 프롬프트 생성 (언어별)
        if self.language == "English":
            system_prompt = """You are an expert log analysis assistant. Given the log data, return a JSON object with the following keys:
1. error_type: summary of error or issue type
2. severity: severity level (1-5)
3. root_cause: analysis of possible cause
4. solution: suggested fix
5. patterns: list of notable patterns or trends
Return only JSON."""
        else:
            system_prompt = """당신은 전문적인 로그 분석 시스템입니다. 주어진 로그 데이터를 분석하여 다음 정보를 JSON 형식으로 반환해주세요:
1. 주요 에러 또는 이슈 타입
2. 심각도 수준 (1-5, 5가 가장 심각)
3. 발생 원인 분석
4. 해결 방안 제안
5. 주목할 패턴이나 추세

반환 포맷:
{
    "error_type": "...",
    "severity": ...,
    "root_cause": "...",
    "solution": "...",
    "patterns": [...],
    "summary": "..."
}

응답은 항상 JSON 형식으로만 제공해야 합니다."""
        
        user_prompt = log_data
        if specific_query:
            user_prompt += f"\n\n추가 질문: {specific_query}"
        
        # API 요청
        try:
            logger.info("GPT API에 분석 요청 전송 중...")
            response = openai.chat.completions.create(
                model=self.model,
                messages=[
                    {"role": "system", "content": system_prompt},
                    {"role": "user", "content": user_prompt}
                ],
                temperature=0.1,
                max_tokens=2000
            )
            
            # 응답 파싱
            response_text = response.choices[0].message.content
            logger.debug(f"원본 응답: {response_text}")
            
            try:
                # JSON 응답 추출
                json_match = re.search(r'({.*})', response_text, re.DOTALL)
                if json_match:
                    response_text = json_match.group(1)
                
                analysis_result = json.loads(response_text)
                
                # 기본 구조 확인 및 보완
                if not isinstance(analysis_result, dict):
                    raise ValueError("응답이 딕셔너리 형식이 아닙니다.")
                
                required_keys = ["error_type", "severity", "root_cause", "solution", "patterns", "summary"]
                for key in required_keys:
                    if key not in analysis_result:
                        analysis_result[key] = "분석 불가"
                
                return analysis_result
                
            except json.JSONDecodeError as e:
                logger.error(f"JSON 파싱 오류: {e}")
                logger.debug(f"파싱 시도한 텍스트: {response_text}")
                return generate_mock_analysis({"error": "JSON 파싱 오류", "raw_response": response_text})
                
        except Exception as e:
            logger.error(f"GPT API 요청 중 오류 발생: {e}")
            return generate_mock_analysis({"error": str(e), "data": log_data[:500]})
    
    def analyze_with_retry(self, df: pd.DataFrame, specific_query: Optional[str] = None, 
                         max_retries: int = 3, backoff_factor: float = 2.0) -> Dict[str, Any]:
        """
        재시도 로직이 포함된 로그 분석 함수
        
        Args:
            df (pd.DataFrame): 로그 데이터프레임
            specific_query (str, optional): 특정 분석 질문
            max_retries (int): 최대 재시도 횟수
            backoff_factor (float): 재시도 간격 증가 계수
            
        Returns:
            Dict[str, Any]: 구조화된 분석 결과
        """
        retry_count = 0
        last_error = None
        
        while retry_count < max_retries:
            try:
                return self.analyze_logs(df, specific_query)
            except Exception as e:
                last_error = e
                retry_count += 1
                if retry_count < max_retries:
                    sleep_time = backoff_factor ** retry_count
                    logger.warning(f"분석 실패 ({retry_count}/{max_retries}), {sleep_time}초 후 재시도: {e}")
                    time.sleep(sleep_time)
                else:
                    logger.error(f"최대 재시도 횟수 초과, 분석 실패: {e}")
        
        # 모든 재시도 실패 시 더미 데이터 반환
        return generate_mock_analysis({"error": str(last_error), "retries_exceeded": True})
    
    def parse_logs(self, raw_logs: str) -> List[Dict[str, Any]]:
        """
        AI 기반 로그 파서. 원본 로그 텍스트를 구조화된 dict list로 변환합니다.
        """
        try:
            # 시스템 프롬프트 생성 (언어별)
            if self.language == "English":
                system_prompt = (
                    "You are a log parsing assistant. Given raw log entries, extract timestamp, level, component, and message. "
                    "Return as a JSON array of objects with keys: timestamp, level, component, message."
                )
            else:
                system_prompt = (
                    "당신은 로그 파싱 어시스턴트입니다. 주어진 원시 로그 엔트리에서 timestamp, log level, component, message를 추출하세요. "
                    "timestamp, level, component, message 키를 가진 JSON 배열로 반환해주세요."
                )
            response = openai.ChatCompletion.create(
                model=self.model,
                messages=[
                    {"role": "system", "content": system_prompt},
                    {"role": "user", "content": raw_logs}
                ],
                temperature=0,
                max_tokens=1500
            )
            response_text = response.choices[0].message.content
            # JSON 배열 추출
            json_match = re.search(r'(\[.*\])', response_text, re.DOTALL)
            if json_match:
                response_text = json_match.group(1)
            parsed = json.loads(response_text)
            return parsed
        except Exception as e:
            logger.error(f"AI 로그 파싱 오류: {e}")
            return []


def generate_mock_analysis(log_data: Dict[str, Any]) -> Dict[str, Any]:
    """
    테스트 목적으로 더미 분석 결과를 생성합니다.
    
    Args:
        log_data (Dict[str, Any]): 원본 로그 데이터
        
    Returns:
        Dict[str, Any]: 더미 분석 결과
    """
    # 기본 더미 결과
    mock_result = {
        "error_type": "리소스 부족 오류",
        "severity": 3,
        "root_cause": "애플리케이션이 사용 가능한 메모리보다 더 많은 메모리를 할당하려고 시도했습니다.",
        "solution": "애플리케이션의 메모리 사용량을 모니터링하고 필요한 경우 메모리 제한을 늘리거나 메모리 누수를 확인하세요.",
        "patterns": [
            "일정 시간 간격으로 에러가 반복됨",
            "메모리 사용량이 계속 증가하는 패턴",
            "특정 사용자 작업 후 발생하는 경향"
        ],
        "summary": "이 로그는 메모리 부족으로 인한 애플리케이션 오류를 보여줍니다. 에러는 주기적으로 발생하며, 특히 사용자 트래픽이 많은 시간대에 집중됩니다. 메모리 최적화 및 리소스 관리 개선이 필요합니다.",
        "_note": "이 결과는 API 연결 실패로 생성된 더미 데이터입니다. 실제 분석이 아닙니다."
    }
    
    # 오류 정보가 있으면 포함
    if isinstance(log_data, dict) and "error" in log_data:
        mock_result["_error_info"] = log_data["error"]
    
    return mock_result