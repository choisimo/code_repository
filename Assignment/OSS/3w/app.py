import streamlit as st
import pandas as pd
from io import StringIO
import time
import json
import logging
from datetime import datetime
import os
import streamlit.components.v1 as components

# 로깅 설정
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s'
)

# 세션 상태 초기화
if "openai_api_key" not in st.session_state:
    st.session_state.openai_api_key = ""
logger = logging.getLogger("log-analyzer-app")

# 페이지 설정
st.set_page_config(
    page_title="AI 로그 분석기",
    page_icon="📊",
    layout="wide",
    initial_sidebar_state="expanded"
)

# CSS 스타일 적용
st.markdown("""
    <style>
    /* CSS 스타일 정의 */
    </style>
""", unsafe_allow_html=True)

# --- 로컬스토리지에서 API 키 로드/저장용 JS 삽입 ---
components.html(
    '''<script>
      const key = localStorage.getItem('OPENAI_API_KEY');
      if (key) window.parent.postMessage({ type:'loadKey', key }, '*');
      window.addEventListener('message', e => {
        if (e.data?.type==='saveKey') localStorage.setItem('OPENAI_API_KEY', e.data.key);
      });
    </script>''',
    height=0,
)
# 로드된 키를 세션에 반영
msg = st.query_params.get('message')
if msg:
    try:
        data = json.loads(msg[0])
        if data.get('type')=='loadKey':
            st.session_state.openai_api_key = data['key']
    except:
        pass

from log_processor import process_log, normalize_logs, process_text_logs
from gpt_analyzer import GPTAnalyzer
from visualization.charts import plot_log_frequency, plot_error_distribution
from utils import format_timestamp, export_results
from config import SIDEBAR_STYLE, THEME_COLORS, get_api_key

# 세션 상태 초기화
if 'logs' not in st.session_state:
    st.session_state.logs = None
if 'analysis_results' not in st.session_state:
    st.session_state.analysis_results = None
if 'is_analyzing' not in st.session_state:
    st.session_state.is_analyzing = False
if 'last_analyzed' not in st.session_state:
    st.session_state.last_analyzed = None
if 'response_language' not in st.session_state:
    st.session_state.response_language = "한국어"
if 'detailed_analysis' not in st.session_state:
    st.session_state.detailed_analysis = None
if 'uploaded_files' not in st.session_state:
    st.session_state.uploaded_files = []
if 'detailed_analysis_history' not in st.session_state:
    st.session_state.detailed_analysis_history = []
if 'previous_source_option' not in st.session_state:
    st.session_state.previous_source_option = None
if 'clear_on_source_change' not in st.session_state:
    st.session_state.clear_on_source_change = True

# 모델 옵션 반영: 분석 시점에 GPTAnalyzer 생성

# 사이드바 구성
with st.sidebar:
    st.title("🔍 AI 로그 분석기")
    st.divider()

    st.subheader("로그 소스 선택")
    source_option = st.radio(
        "데이터 소스를 선택하세요:",
        ["파일 업로드", "텍스트 입력", "샘플 데이터"]
    )
    # 상세 분석 이력 자동 초기화 옵션
    st.checkbox(
        "새 로그 소스 선택 시 상세 분석 초기화",
        value=st.session_state.clear_on_source_change,
        key="clear_on_source_change"
    )
    # 소스 변경 감지 후 이력 초기화
    if st.session_state.previous_source_option is not None and source_option != st.session_state.previous_source_option and st.session_state.clear_on_source_change:
        st.session_state.detailed_analysis_history = []
    st.session_state.previous_source_option = source_option

    st.divider()
    st.subheader("분석 설정")
    temperature = st.slider("GPT 창의성 수준", 0.0, 1.0, 0.7, 0.1)
    model_option = st.selectbox(
        "GPT 모델 선택",
        ["gpt-3.5-turbo", "gpt-4"]
    )

    st.subheader("응답 언어")
    language_option = st.selectbox("응답 언어", ["한국어", "English"], index=0, key="response_language")

    st.divider()
    st.subheader("시각화 설정")
    chart_type = st.selectbox(
        "차트 유형",
        ["막대 차트", "라인 차트", "파이 차트", "히트맵"]
    )

    st.divider()
    st.subheader("API 설정")
    api_key_input = st.text_input("OpenAI API 키", type="password", value=st.session_state.openai_api_key)
    if api_key_input:
        st.session_state.openai_api_key = api_key_input
        # 브라우저 로컬스토리지에 저장 요청
        components.html(
            f'''<script>
               window.parent.postMessage({{type:'saveKey',key:'{api_key_input}'}}, '*');
             </script>''',
            height=0,
        )
    st.divider()

    if st.session_state.analysis_results:
        # 결과를 다운로드할 CSV 생성
        timestamp = format_timestamp(datetime.now())
        df = normalize_logs(process_log(st.session_state.logs))
        buffer = export_results(df, st.session_state.analysis_results)
        st.download_button(
            label="CSV 다운로드", data=buffer,
            file_name=f"analysis_{timestamp}.csv", mime="text/csv"
        )

# 메인 화면
st.title("AI 로그 분석기 및 GPT 지원 시스템")
st.markdown("로그 파일을 업로드하거나 텍스트를 입력하여 AI 기반 분석을 시작하세요.")

# 데이터 입력 처리
if source_option == "파일 업로드":
    uploaded_files = st.file_uploader("로그 파일 업로드", type=["txt", "log", "csv"], accept_multiple_files=True)
    if uploaded_files:
        st.session_state.uploaded_files = uploaded_files
        file_names = [f.name for f in uploaded_files]
        selected_name = st.selectbox("파일 선택", file_names)
        selected_file = next((f for f in uploaded_files if f.name == selected_name), None)
        if selected_file:
            content = StringIO(selected_file.getvalue().decode("utf-8")).read()
            st.session_state.logs = content
            st.success(f"{selected_name} 파일을 성공적으로 업로드했습니다.")

elif source_option == "텍스트 입력":
    log_input = st.text_area("분석할 로그를 입력하세요", height=200)
    if log_input:
        st.session_state.logs = log_input
        st.success("로그를 상태에 저장했습니다.")

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
            st.text_area("로그 내용",
                         value=st.session_state.logs[:2000] + ("\n..." if len(st.session_state.logs) > 2000 else ""),
                         height=400, disabled=True)

    with col2:
        st.subheader("처리된 로그")
        processed_logs = process_log(st.session_state.logs)
        if processed_logs.empty:
            processed_logs = process_text_logs(st.session_state.logs)
            st.warning("기본 파서로 로그를 처리할 수 없어 텍스트 파서로 처리했습니다.")
        with st.expander("처리된 로그 보기", expanded=True):
            st.dataframe(processed_logs, use_container_width=True)

    # 분석 실행 버튼
    analyze_button = st.button("GPT로 로그 분석하기")
    if analyze_button:
        if not st.session_state.openai_api_key:
            st.error("먼저 OpenAI API 키를 등록해주세요.")
        else:
            st.session_state.is_analyzing = True

    # 분석 수행
    if st.session_state.is_analyzing:
        # 처리된 로그가 비어 있으면 에러 표시
        if processed_logs.empty:
            st.error("처리된 로그가 없습니다. 올바른 로그 파일을 업로드하거나 로그 형식을 확인하세요.")
            st.session_state.is_analyzing = False
        else:
            normalized_logs = normalize_logs(processed_logs)
            with st.spinner("AI가 로그를 분석 중입니다..."):
                # 프로그레스 바 표시
                progress_bar = st.progress(0)
                for i in range(100):
                    time.sleep(0.05)  # 실제 API 호출 시 이 부분은 제거
                    progress_bar.progress(i + 1)

                # 모델 옵션 반영: 새로운 GPTAnalyzer 인스턴스 생성
                analyzer = GPTAnalyzer(api_key=st.session_state.openai_api_key, model=model_option,
                                       language=st.session_state.response_language)
                gpt_results = analyzer.analyze_with_retry(normalized_logs)

                # 결과 저장
                st.session_state.analysis_results = gpt_results
                st.session_state.detailed_analysis_history = []  # 새로운 메인 분석 시 기존 상세 분석 이력 초기화
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

        # 메인 분석 결과 다운로드 준비
        timestamp_download = datetime.now().strftime("%Y%m%d_%H%M%S")
        df_main_download = normalize_logs(process_log(st.session_state.logs))
        buffer_main_csv = export_results(df_main_download, results)

        # 탭으로 결과 표시
        tab1, tab2, tab3, tab4 = st.tabs(["요약", "상세 분석", "시각화", "JSON"])

        with tab1:
            st.subheader("주요 발견사항")
            st.markdown(f"**문제 유형:** {results['error_type']}")
            try:
                severity_int = int(results.get('severity', 1))
            except ValueError:
                severity_int = 1
            st.markdown(f"**위험도:** {'🔴' * severity_int} ({severity_int}/5)")
            st.markdown(f"**근본 원인:** {results['root_cause']}")
            st.markdown(f"**해결 방안:** {results['solution']}")

            # 메인 분석 결과 CSV 다운로드
            st.download_button(
                label="메인 분석 결과 다운로드 (CSV)",
                data=buffer_main_csv,
                file_name=f"main_analysis_{timestamp_download}.csv",
                mime="text/csv"
            )

        with tab2:
            st.subheader("상세 분석")
            # 기본 분석 요약 표시
            st.markdown(f"**요약:** {results.get('summary', '')}")

            # 추가 로그 통합 (기존 업로드 파일 선택 + 새 파일 업로드 + 텍스트 붙여넣기)
            existing_files = st.session_state.get("uploaded_files", [])
            if existing_files:
                # 기본으로 모든 업로드된 파일 선택
                selected_existing = st.multiselect(
                    "기존 업로드된 로그 선택",
                    options=[f.name for f in existing_files],
                    default=[f.name for f in existing_files],
                    key="detail_existing"
                )
            else:
                selected_existing = []
            new_files = st.file_uploader("새로운 로그 파일 업로드", type=["txt", "log", "csv"], accept_multiple_files=True,
                                         key="detail_new_files")
            new_text = st.text_area("추가 로그 복사/붙여넣기", height=150, key="detail_text")

            # 추가 세부 분석 요청 UI
            st.markdown("#### 추가 분석 요청")
            detail_query = st.text_input("세부 질문을 입력하세요", key="detail_query")
            if st.button("세부 분석 요청", key="detail_analyze"):
                if not st.session_state.openai_api_key:
                    st.error("먼저 OpenAI API 키를 등록해주세요.")
                else:
                    # 세부 로그 및 원본 로그 통합 후 분석
                    analyzer = GPTAnalyzer(api_key=st.session_state.openai_api_key, model=model_option,
                                           language=st.session_state.response_language)
                    raw_combined = st.session_state.logs or ""
                    # 기존 업로드된 파일 내용 추가
                    for name in selected_existing:
                        f = next((f for f in existing_files if f.name == name), None)
                        if f:
                            raw_combined += "\n" + f.getvalue().decode("utf-8")
                    # 새로운 파일 내용 추가
                    if new_files:
                        for f in new_files:
                            raw_combined += "\n" + f.getvalue().decode("utf-8")
                    # 텍스트 입력 추가
                    if new_text:
                        raw_combined += "\n" + new_text
                    # AI 파서로 먼저 구조화 시도
                    parsed_entries = analyzer.parse_logs(raw_combined)
                    if parsed_entries:
                        df_combined = pd.DataFrame(parsed_entries)
                    else:
                        # 기본/텍스트 파서 폴백
                        df_combined = process_log(raw_combined)
                        if df_combined.empty:
                            df_combined = process_text_logs(raw_combined)
                    df_combined = normalize_logs(df_combined)
                    detail_res = analyzer.analyze_with_retry(df_combined, specific_query=detail_query)
                    # 결과를 이력에 추가하여 점진적 분석 지원
                    st.session_state.detailed_analysis_history.append({'query': detail_query, 'result': detail_res})

            # 세부 분석 결과 표시 (페이지별)
            if st.session_state.detailed_analysis_history:
                detail_tabs = st.tabs([f"{i + 1}. {entry.get('query', '')}" for i, entry in
                                       enumerate(st.session_state.detailed_analysis_history)])
                for tab, entry in zip(detail_tabs, st.session_state.detailed_analysis_history):
                    with tab:
                        da = entry['result']
                        st.markdown(f"**문제 유형:** {da.get('error_type', '')}")
                        try:
                            sev = int(da.get('severity', 1))
                        except:
                            sev = 1
                        st.markdown(f"**위험도:** {'🔴' * sev} ({sev}/5)")
                        st.markdown(f"**근본 원인:** {da.get('root_cause', '')}")
                        st.markdown(f"**해결 방안:** {da.get('solution', '')}")
                        patterns = da.get('patterns', [])
                        if patterns:
                            st.markdown("**주목할 패턴:**")
                            for p in patterns:
                                st.markdown(f"- {p}")
                        summary = da.get('summary', '')
                        if summary:
                            st.markdown("**요약:**")
                            st.markdown(summary)
                # 세부 분석 전체 결과 JSON 다운로드
                hist_json = json.dumps(st.session_state.detailed_analysis_history, ensure_ascii=False, indent=2)
                st.download_button(
                    label="세부 분석 결과 다운로드 (JSON)",
                    data=hist_json,
                    file_name=f"detailed_analysis_{timestamp_download}.json",
                    mime="application/json"
                )
            else:
                st.write("세부 분석 이력이 없습니다.")

            # 기존 영향 시스템 정보
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
    "© 2025 AI 로그 분석 시스템 | 개발자에게 [연락하기](mailto:admin@nodove.com) | "
    "[문서 보기]('')"
)
