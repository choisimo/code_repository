import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import io
import streamlit as st
from typing import Tuple, Dict, List, Any, Optional

def plot_log_frequency(df: pd.DataFrame, time_unit: str = "hour") -> plt.Figure:
    """
    로그 발생 빈도를 시간대별로 시각화합니다.
    
    Args:
        df: 로그 데이터프레임
        time_unit: 시간 단위 (hour, day, minute)
        
    Returns:
        Matplotlib 그림 객체
    """
    if 'timestamp' not in df.columns:
        fig, ax = plt.subplots(figsize=(10, 6))
        ax.text(0.5, 0.5, "타임스탬프 정보가 없습니다", ha='center', va='center', fontsize=12)
        ax.set_axis_off()
        return fig
    
    # 원본 데이터 보존을 위한 복사
    temp_df = df.copy()
    
    if time_unit == "hour":
        temp_df['time_bin'] = temp_df['timestamp'].dt.hour
        x_label = "시간 (24시간)"
        title = "시간대별 로그 발생 빈도"
        
    elif time_unit == "day":
        temp_df['time_bin'] = temp_df['timestamp'].dt.day
        x_label = "일"
        title = "일별 로그 발생 빈도"
        
    elif time_unit == "minute":
        temp_df['time_bin'] = temp_df['timestamp'].dt.minute
        temp_df = temp_df.groupby([(temp_df['timestamp'].dt.hour), 'time_bin']).size().reset_index(name='count')
        temp_df['full_minute'] = temp_df.apply(lambda x: f"{int(x[0]):02d}:{int(x['time_bin']):02d}", axis=1)
        freq_data = temp_df.set_index('full_minute')['count']
        
        fig, ax = plt.subplots(figsize=(12, 6))
        freq_data.plot(kind='line', ax=ax, marker='o', color='royalblue')
        ax.set_xlabel("시간:분")
        ax.set_ylabel("로그 수")
        ax.set_title("분 단위 로그 발생 빈도")
        ax.grid(True, alpha=0.3)
        plt.xticks(rotation=45)
        plt.tight_layout()
        return fig
    
    else:
        raise ValueError(f"지원하지 않는 시간 단위: {time_unit}")
    
    # 시간대별 로그 빈도 계산
    freq_data = temp_df.groupby('time_bin').size()
    
    # 에러 로그 빈도 계산 (에러 플래그가 있는 경우)
    if 'is_error' in temp_df.columns:
        error_freq = temp_df[temp_df['is_error']].groupby('time_bin').size()
    else:
        error_freq = None
    
    # 그래프 생성
    fig, ax = plt.subplots(figsize=(10, 6))
    
    # 전체 로그 플롯
    freq_data.plot(kind='bar', ax=ax, alpha=0.7, color='royalblue', label='전체 로그')
    
    # 에러 로그 플롯 (있는 경우)
    if error_freq is not None:
        error_freq.plot(kind='bar', ax=ax, alpha=0.7, color='red', label='에러 로그')
        ax.legend()
    
    ax.set_xlabel(x_label)
    ax.set_ylabel("로그 수")
    ax.set_title(title)
    ax.grid(True, alpha=0.3, axis='y')
    
    plt.tight_layout()
    
    return fig

def plot_error_distribution(df: pd.DataFrame) -> plt.Figure:
    """
    에러 유형 분포를 시각화합니다.
    
    Args:
        df: 로그 데이터프레임
        
    Returns:
        Matplotlib 그림 객체
    """
    # 에러 로그가 없는 경우 처리
    if 'is_error' not in df.columns or df['is_error'].sum() == 0:
        fig, ax = plt.subplots(figsize=(10, 6))
        ax.text(0.5, 0.5, "에러 로그가 없거나 식별할 수 없습니다", ha='center', va='center', fontsize=12)
        ax.set_axis_off()
        return fig
    
    # 에러 로그만 추출
    error_df = df[df['is_error']]
    
    # 에러 코드 또는 레벨에 따른 분류
    if 'error_code' in error_df.columns and error_df['error_code'].notna().any():
        # 에러 코드별 빈도
        error_counts = error_df['error_code'].value_counts().head(10)
        title = "상위 10개 에러 코드 분포"
        
    elif 'level' in error_df.columns:
        # 에러 레벨별 빈도
        error_counts = error_df['level'].value_counts()
        title = "에러 레벨 분포"
        
    else:
        # 컴포넌트별 에러 빈도
        if 'component' in error_df.columns:
            error_counts = error_df['component'].value_counts().head(10)
            title = "컴포넌트별 에러 분포"
        else:
            fig, ax = plt.subplots(figsize=(10, 6))
            ax.text(0.5, 0.5, "에러 분류 정보가 없습니다", ha='center', va='center', fontsize=12)
            ax.set_axis_off()
            return fig
    
    # 그래프 생성
    fig, ax = plt.subplots(figsize=(10, 6))
    
    # 파이 차트와 바 차트 결합
    plt.subplot(1, 2, 1)
    error_counts.plot(kind='pie', autopct='%1.1f%%', startangle=90, 
                      colors=plt.cm.Reds(np.linspace(0.4, 0.8, len(error_counts))))
    plt.axis('equal')
    plt.title(title)
    plt.ylabel('')
    
    plt.subplot(1, 2, 2)
    error_counts.plot(kind='bar', color=plt.cm.Reds(np.linspace(0.4, 0.8, len(error_counts))))
    plt.xlabel('에러 유형')
    plt.ylabel('발생 횟수')
    plt.title('에러 유형별 발생 빈도')
    plt.xticks(rotation=45)
    plt.grid(axis='y', alpha=0.3)
    
    plt.tight_layout()
    
    return fig
