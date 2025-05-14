/**
 * 날씨 예보 데이터를 나타내는 엔티티 클래스
 * 
 * <p>이 클래스는 특정 위치의 미래 날씨 예보 정보를 저장합니다.
 * JPA 엔티티로 정의되어 있어 데이터베이스 테이블과 매핑됩니다.</p>
 * 
 * <p>포함된 예보 데이터:</p>
 * <ul>
 *   <li>기본 정보: 위치, 예보 시간</li>
 *   <li>기온 관련: 예상 기온, 최고/최저 기온</li>
 *   <li>강수 관련: 강수 확률, 강수량</li>
 *   <li>기타 기상 요소: 습도, 풍속, 구름량 등</li>
 * </ul>
 * 
 * <p>이 엔티티는 일간/시간별 날씨 예보 API 응답 및 데이터베이스 저장에 사용됩니다.</p>
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

import jakarta.persistence.Entity;
import jakarta.persistence.Id;
import jakarta.persistence.Table;
import java.time.LocalDateTime;

/**
 * 날씨 예보 데이터를 저장하는 엔티티 클래스
 */
@Data               // Lombok: getter/setter, toString, equals, hashCode 자동 생성
@Entity             // JPA: 이 클래스가 데이터베이스 엔티티임을 나타냄
@Builder            // Lombok: 빌더 패턴 구현 자동 생성
@NoArgsConstructor  // Lombok: 파라미터 없는 기본 생성자 자동 생성
@AllArgsConstructor // Lombok: 모든 필드를 파라미터로 받는 생성자 자동 생성
@Table(name = "forecast_data") // JPA: 매핑될 데이터베이스 테이블 이름 지정
public class ForecastData {

    /**
     * 예보 데이터의 고유 식별자
     * 일반적으로 "{locationId}_{forecastTime}" 형식을 사용
     */
    @Id // JPA: 기본 키 필드로 지정
    private String id;
    
    /**
     * 날씨 예보가 속한 위치의
     * 고유 식별자 (예: "SEOUL_1", "BUSAN_2" 등)
     */
    private String locationId;
    
    /**
     * 위치의 이름 (예: "서울", "부산", "뉴욕" 등)
     */
    private String locationName;
    
    /**
     * 예보 시간 (이 시간에 대한 날씨 예보)
     */
    private LocalDateTime forecastTime;
    
    /**
     * 예상 기온 (섭씨 단위)
     */
    private Double temperature;
    
    /**
     * 체감 온도 (섭씨 단위)
     */
    private Double feelsLike;
    
    /**
     * 최저 기온 (섭씨 단위)
     */
    private Double temperatureMin;
    
    /**
     * 최고 기온 (섭씨 단위)
     */
    private Double temperatureMax;
    
    /**
     * 강수 확률 (백분율, 0-100)
     */
    private Integer rainProbability;
    
    /**
     * 강수량 (mm 단위, 예상 시간당)
     */
    private Double rainAmount;
    
    /**
     * 상대 습도 (백분율, 0-100)
     */
    private Integer humidity;
    
    /**
     * 기압 (hPa 단위)
     */
    private Double pressure;
    
    /**
     * 풍속 (m/s 단위)
     */
    private Double windSpeed;
    
    /**
     * 풍향 (도 단위, 0-359)
     */
    private Double windDirection;
    
    /**
     * 구름 양 (백분율, 0-100)
     */
    private Integer cloudiness;
    
    /**
     * 날씨 상태 주요 분류 (예: "Clear", "Rain", "Snow" 등)
     */
    private String weatherMain;
    
    /**
     * 날씨 상태 상세 설명
     */
    private String weatherDescription;
    
    /**
     * 날씨 아이콘 코드
     */
    private String weatherIcon;
    
    /**
     * 가시성 (미터 단위)
     */
    private Double visibility;
    
    /**
     * 자외선 지수 (0-11+)
     */
    private Double uvIndex;
    
    /**
     * 눈 량 (mm 단위, 물 상당량)
     */
    private Double snowAmount;
    
    /**
     * 예보가 마지막으로 업데이트된 시간
     */
    private LocalDateTime lastUpdated;
}
