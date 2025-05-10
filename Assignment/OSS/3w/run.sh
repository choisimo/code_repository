#!/bin/bash

# 이 스크립트는 Streamlit 앱을 쉽게 실행하기 위한 도우미입니다.
echo "Streamlit 애플리케이션 시작 중..."

# Streamlit 명령어 실행
streamlit run app.py

# 오류 처리
if [ $? -ne 0 ]; then
    echo "오류: Streamlit 실행에 실패했습니다."
    echo "Streamlit이 설치되어 있는지 확인하세요: pip install streamlit"
fi
