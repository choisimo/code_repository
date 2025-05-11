# 애플리케이션 스타일 및 설정
import os
import json
import pathlib
from typing import Optional
import streamlit as st

# 스타일 및 테마 설정
SIDEBAR_STYLE = {
    "padding": "1.5rem",
    "background-color": "#f8f9fa",
}

THEME_COLORS = {
    "primary": "#0066cc",
    "secondary": "#6c757d",
    "success": "#28a745",
    "danger": "#dc3545",
    "warning": "#ffc107",
    "info": "#17a2b8",
}

def get_api_key() -> Optional[str]:
    """
    여러 소스에서 OpenAI API 키를 찾아 반환합니다.
    우선순위: Streamlit secrets > 세션 저장소 > 환경 변수 > 프로젝트 루트 로컬 설정 파일 > 로컬 설정 파일
    """
    # 1. Streamlit secrets에서 확인
    if hasattr(st, 'secrets') and st.secrets.get("OPENAI_API_KEY"):
        return st.secrets["OPENAI_API_KEY"]
    # 2. 세션 변수에서 확인
    if "openai_api_key" in st.session_state and st.session_state.openai_api_key:
        return st.session_state.openai_api_key

    # 3. 환경 변수에서 확인
    if os.getenv("OPENAI_API_KEY"):
        return os.getenv("OPENAI_API_KEY")

    # 4. 프로젝트 루트 로컬 설정 파일 확인
    local_config_path = pathlib.Path.cwd() / ".gptlogger_config.json"
    if local_config_path.exists():
        try:
            with open(local_config_path, "r") as f:
                local_cfg = json.load(f)
                if "OPENAI_API_KEY" in local_cfg:
                    return local_cfg["OPENAI_API_KEY"]
        except (json.JSONDecodeError, IOError):
            pass

    # 5. 사용자 홈 디렉토리 로컬 설정 파일 확인
    config_path = pathlib.Path.home() / ".gptlogger_config.json"
    if config_path.exists():
        try:
            with open(config_path, "r") as f:
                config = json.load(f)
                if "OPENAI_API_KEY" in config:
                    return config["OPENAI_API_KEY"]
        except (json.JSONDecodeError, IOError):
            pass
    
    # API 키를 찾지 못함
    return None

def save_api_key_to_file(api_key: str) -> bool:
    """API 키를 로컬 설정 파일에 저장합니다."""
    config_path = pathlib.Path.home() / ".gptlogger_config.json"
    
    # 기존 설정 파일이 있으면 읽습니다
    config = {}
    if config_path.exists():
        try:
            with open(config_path, "r") as f:
                config = json.load(f)
        except (json.JSONDecodeError, IOError):
            pass
    
    # API 키 업데이트
    config["OPENAI_API_KEY"] = api_key
    
    try:
        # 디렉토리가 없으면 생성
        config_path.parent.mkdir(parents=True, exist_ok=True)
        
        # 설정 저장
        with open(config_path, "w") as f:
            json.dump(config, f)
        return True
    except IOError:
        return False
# 사이드바 스타일
SIDEBAR_STYLE = """
<style>
    .sidebar .sidebar-content {
        background-color: #f0f2f6;
        padding: 20px;
    }
    .sidebar .sidebar-content .stButton>button {
        width: 100%;
        border-radius: 5px;
        background-color: #4e8cff;
        color: white;
    }
    .reportview-container .main .block-container {
        padding-top: 2rem;
    }
    .stProgress > div > div > div > div {
        background-color: #4e8cff;
    }
    h1, h2, h3 {
        color: #1e3a8a;
    }
    .stAlert > div {
        padding: 0.75rem 1.5rem;
        border-radius: 4px;
    }
</style>
"""

# 테마 색상
THEME_COLORS = {
    "primary": "#4e8cff",
    "secondary": "#f0f2f6",
    "accent": "#ff6b6b",
    "background": "#ffffff",
    "text": "#333333",
    "success": "#28a745",
    "warning": "#ffc107",
    "error": "#dc3545"
}

# API 설정
DEFAULT_GPT_MODEL = "gpt-3.5-turbo"
MAX_TOKENS = 1500

# 로그 분석 설정
DEFAULT_LOG_FORMAT = "standard"
SAMPLE_SIZE = 1000  # 대용량 로그 파일 샘플링 크기
