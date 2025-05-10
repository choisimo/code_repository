import streamlit as st
import pandas as pd
from io import StringIO
import time
import json
from datetime import datetime
import os

from log_processor import process_log, normalize_logs
from gpt_analyzer import analyze_with_gpt
from visualization.charts import plot_log_frequency, plot_error_distribution
from utils import format_timestamp, export_results
from config import SIDEBAR_STYLE, THEME_COLORS

# 페이지 설정\
st.set_page_config(
    page_title="AI 로그 분석기",
    page_icon="📊",
    layout="wide",
    initial_sidebar_state="expanded"
)

# CSS 스타일 적용
st.markdown(SIDEBAR_STYLE, unsafe_allow_html=True)

# 세션 상태 초기화
if 'logs' not in st.session_state:
    st.session_state.logs = None
if 'analysis_results' not in st.session_state:
    st.session_state.analysis_results = None
if 'is_analyzing' not in st.session_state:
    st.session_state.is_analyzing = False
if 'last_analyzed' not in st.session_state:
    st.session_state.last_analyzed = None

# 사이드바 구성
with st.sidebar:
    st.title("🔍 AI 로그 분석기")
    st.divider()
    
    st.subheader("로그 소스 선택")
    source_option = st.radio(
        "데이터 소스를 선택하세요:",
        ["파일 업로드", "텍스트 입력", "샘플 데이터"]
    )
    
    st.divider()
    st.subheader("분석 설정")
    temperature = st.slider("GPT 창의성 수준", 0.0, 1.0, 0.7, 0.1)
    model_option = st.selectbox(
        "GPT 모델 선택",
        ["gpt-3.5-turbo", "gpt-4"]
    )
    
    st.divider()
    st.subheader("시각화 설정")
    chart_type = st.selectbox(
        "차트 유형",
        ["막대 차트", "라인 차트", "파이 차트", "히트맵"]
    )
    
    st.divider()
    if st.session_state.analysis_results:
        if st.button("결과 내보내기"):
            timestamp = format_timestamp(datetime.now())
            filename = export_results(st.session_state.analysis_results, timestamp)
            st.success(f"결과를 {filename}에 저장했습니다.")

# 메인 화면
st.title("AI 로그 분석기 및 GPT 지원 시스템")
st.markdown("로그 파일을 업로드하거나 텍스트를 입력하여 AI 기반 분석을 시작하세요.")

# 데이터 입력 처리
if source_option == "파일 업로드":
    uploaded_file = st.file_uploader("로그 파일 업로드", type=["txt", "log", "csv"])
    if uploaded_file is not None:
        content = StringIO(uploaded_file.getvalue().decode("utf-8")).read()
        st.session_state.logs = content
        st.success("파일을 성공적으로 업로드했습니다.")
        
elif source_option == "텍스트 입력":
    log_input = st.text_area("분석할 로그를 입력하세요", height=200)
    if st.button("로그 제출") and log_input:
        st.session_state.logs = log_input
        st.success("로그를 성공적으로 제출했습니다.")
        
elif source_option == "샘플 데이터":
    if st.button("샘플 데이터 로드"):
        try:
            with open("data/sample_logs.txt", "r") as f:
                st.session_state.logs = f.read()
            st.success("샘플 데이터를 성공적으로 로드했습니다.")
        except Exception as e:
            st.error(f"샘플 데이터 로드 중 오류가 발생했습니다: {e}")

# 로그 표시 및 분석 시작
if st.session_state.logs:
    col1, col2 = st.columns([1, 1])
    
    with col1:
        st.subheader("원본 로그")
        with st.expander("원본 로그 보기", expanded=True):
            st.text_area("로그 내용", value=st.session_state.logs[:2000] + ("\n..." if len(st.session_state.logs) > 2000 else ""), height=400, disabled=True)
    
    with col2:
        st.subheader("처리된 로그")
        processed_logs = process_log(st.session_state.logs)
        with st.expander("처리된 로그 보기", expanded=True):
            st.dataframe(processed_logs, use_container_width=True)
    
    # 분석 실행 버튼
    analyze_button = st.button("GPT로 로그 분석하기")
    if analyze_button:
        st.session_state.is_analyzing = True
        
    # 분석 수행
    if st.session_state.is_analyzing:
        normalized_logs = normalize_logs(processed_logs)
        
        with st.spinner("AI가 로그를 분석 중입니다..."):
            # 프로그레스 바 표시
            progress_bar = st.progress(0)
            for i in range(100):
                time.sleep(0.05)  # 실제 API 호출 시 이 부분은 제거
                progress_bar.progress(i + 1)
                
            # GPT 분석 호출
            gpt_results = analyze_with_gpt(
                normalized_logs, 
                temperature=temperature,
                model=model_option
            )
            
            # 결과 저장
            st.session_state.analysis_results = gpt_results
            st.session_state.is_analyzing = False
            st.session_state.last_analyzed = datetime.now()
            
            # 프로그레스 바 완료 후 제거
            progress_bar.empty()
            
            st.success("분석이 완료되었습니다!")
    
    # 분석 결과 표시
    if st.session_state.analysis_results:
        st.header("분석 결과")
        st.markdown(f"*마지막 분석: {format_timestamp(st.session_state.last_analyzed)}*")
        
        results = st.session_state.analysis_results
        
        # 탭으로 결과 표시
        tab1, tab2, tab3, tab4 = st.tabs(["요약", "상세 분석", "시각화", "JSON"])
        
        with tab1:
            st.subheader("주요 발견사항")
            st.markdown(f"**문제 유형:** {results['error_type']}")
            st.markdown(f"**위험도:** {'🔴' * results['severity']} ({results['severity']}/5)")
            st.markdown(f"**근본 원인:** {results['root_cause']}")
            st.markdown(f"**해결 방안:** {results['solution']}")
        
        with tab2:
            st.subheader("상세 분석")
            st.write(results.get('detailed_analysis', '상세 분석이 제공되지 않았습니다.'))
            
            if 'affected_systems' in results:
                st.subheader("영향 받는 시스템")
                for system in results['affected_systems']:
                    st.markdown(f"- {system}")
        
        with tab3:
            st.subheader("로그 시각화")
            
            # 시각화 컴포넌트 사용
            if 'time_series_data' in results:
                st.subheader("시간대별 로그 발생 빈도")
                fig = plot_log_frequency(results['time_series_data'], chart_type)
                st.plotly_chart(fig, use_container_width=True)
            
            if 'error_distribution' in results:
                st.subheader("오류 유형 분포")
                fig = plot_error_distribution(results['error_distribution'], chart_type)
                st.plotly_chart(fig, use_container_width=True)
        
        with tab4:
            st.subheader("원시 JSON 데이터")
            st.json(results)

# 도움말 섹션
with st.expander("사용 도움말", expanded=False):
    st.markdown("""
    ### 사용 방법
    1. 왼쪽 사이드바에서 데이터 소스와 분석 설정을 선택하세요.
    2. 로그 파일을 업로드하거나 로그 텍스트를 입력하세요.
    3. '분석 실행' 버튼을 클릭하여 GPT 기반 분석을 시작하세요.
    4. 분석 결과를 확인하고 필요한 경우 내보내기를 실행하세요.
    
    ### 지원되는 로그 형식
    - 기본 텍스트 로그
    - syslog 형식
    - JSON 형식 로그
    - CSV 형식 로그
    
    ### 분석 결과 해석
    - **요약 탭**: 주요 문제와 해결 방안을 빠르게 확인합니다.
    - **상세 분석 탭**: 심층적인 문제 진단 정보를 제공합니다.
    - **시각화 탭**: 로그 패턴과 오류 분포를 시각적으로 확인합니다.
    - **JSON 탭**: 분석 결과의 원시 데이터를 확인합니다.
    """)

# 푸터
st.markdown("---")
st.markdown(
    "© 2023 AI 로그 분석 시스템 | 개발자에게 [연락하기](mailto:example@example.com) | "
    "[문서 보기](https://github.com/username/ai-log-analyzer)"
)
