# 애플리케이션 스타일 및 설정

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
