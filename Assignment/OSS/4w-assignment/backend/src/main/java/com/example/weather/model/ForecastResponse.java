/**
 * 날씨 예보 API 응답을 위한 DTO 클래스
 * 
 * <p>이 클래스는 클라이언트에게 날씨 예보 정보를 반환하기 위한 응답 객체입니다.
 * 여러 시간대의 예보 데이터와 함께 위치 정보 및 요약 정보를 포함합니다.</p>
 * 
 * @author Nodove 개발팀
 * @version 1.0
 * @since 2023-07-01
 */
package com.example.weather.model;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.time.LocalDateTime;
import java.util.List;

@Data
@Builder
@NoArgsConstructor
@AllArgsConstructor
public class ForecastResponse {
    
    /**
     * 위치 식별자
     */
    private String locationId;
    
    /**
     * 위치 이름
     */
    private String locationName;
    
    /**
     * 위도
     */
    private Double latitude;
    
    /**
     * 경도
     */
    private Double longitude;
    
    /**
     * 시간별 예보 데이터 목록
     */
    private List<ForecastData> hourlyForecast;
    
    /**
     * 일별 예보 데이터 목록
     */
    private List<ForecastData> dailyForecast;
    
    /**
     * 예보 발표 시간
     */
    private LocalDateTime forecastTime;
    
    /**
     * 예보 유효 기간 (일 수)
     */
    private Integer forecastDays;
    
    /**
     * 데이터 제공자 정보
     */
    private String provider;
    
    /**
     * 마지막 업데이트 시간
     */
    private LocalDateTime lastUpdated;
}
