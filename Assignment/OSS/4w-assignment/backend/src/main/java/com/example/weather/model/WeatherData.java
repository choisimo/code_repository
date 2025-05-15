/**
 * 날씨 데이터를 나타내는 엔티티 클래스
 * 
 * <p>이 클래스는 특정 위치의 날씨 상태를 나타내는 모든 관련 정보를 저장합니다.
 * JPA 엔티티로 정의되어 있어 데이터베이스 테이블과 매핑됩니다.</p>
 * 
 * <p>포함된 날씨 데이터:</p>
 * <ul>
 *   <li>기본 정보: 위치, 관측 시간</li>
 *   <li>기온 관련: 현재 기온, 체감 온도</li>
 *   <li>습도 및 구름: 습도, 구름 양</li>
 *   <li>바람 관련: 풍속, 풍향</li>
 *   <li>기상 상태: 날씨 설명, 아이콘</li>
 *   <li>기타: 기압, 가시성, 일출/일몰 시간</li>
 * </ul>
 * 
 * <p>이 엔티티는 API 응답 및 데이터베이스 저장 둘 다에 사용됩니다.</p>
 * 
 * @author Nodove 개발팀
 * @version 1.0
 * @since 2023-05-01
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
 * 날씨 데이터를 저장하는 엔티티 클래스
 */
@Data               // Lombok: getter/setter, toString, equals, hashCode 자동 생성
@Entity             // JPA: 이 클래스가 데이터베이스 엔티티임을 나타냄
@Builder            // Lombok: 빌더 패턴 구현 자동 생성
@NoArgsConstructor  // Lombok: 파라미터 없는 기본 생성자 자동 생성
@AllArgsConstructor // Lombok: 모든 필드를 파라미터로 받는 생성자 자동 생성
@Table(name = "weather_data") // JPA: 매핑될 데이터베이스 테이블 이름 지정
public class WeatherData {

    /**
     * 날씨 데이터의 고유 식별자
     * 일반적으로 "{locationId}_{timestamp}" 형식을 사용
     */
    @Id // JPA: 기본 키 필드로 지정
    private String id;
    
    /**
     * 날씨 데이터가 속한 위치의
     * 고유 식별자 (예: "SEOUL_1", "BUSAN_2" 등)
     */
    private String locationId;
    
    /**
     * 위치의 이름 (예: "서울", "부산", "뉴욕" 등)
     */
    private String locationName;
    
    /**
     * 현재 기온 (섭씨 단위)
     */
    private Double temperature;
    
    /**
     * 체감 온도 (섭씨 단위)
     * 기온, 습도, 풍속 등을 고려하여 계산된 값
     */
    private Double feelsLike;
    
    /**
     * 상대 습도 (백분율, 0-100)
     */
    private Integer humidity;
    
    /**
     * 풍속 (m/s 단위)
     */
    private Double windSpeed;
    
    /**
     * 풍향 (도 단위, 0-359)
     * 0은 북쪽, 90은 동쪽, 180은 남쪽, 270은 서쪽을 의미
     */
    private Double windDirection;
    
    /**
     * 구름 양 (백분율, 0-100)
     * 0은 맑음, 100은 완전히 흐림을 의미
     */
    private Integer cloudiness;
    
    /**
     * 강수량 (mm 단위, 지난 1시간)
     */
    private Double precipitation;
    
    /**
     * 날씨 상태 주요 분류 (예: "Clear", "Rain", "Snow" 등)
     * OpenWeatherMap API의 weather.main 필드에 해당
     */
    private String weatherMain;
    
    /**
     * 날씨 상태 상세 설명
     * OpenWeatherMap API의 weather.description 필드에 해당
     */
    private String weatherDescription;
    
    /**
     * 날씨 아이콘 코드
     * 프론트엔드에서 해당 날씨에 맞는 아이콘을 표시할 때 사용
     */
    private String weatherIcon;
    
    /**
     * 기압 (hPa 단위)
     */
    private Double pressure;
    
    /**
     * 가시성 (미터 단위)
     */
    private Double visibility;
    
    /**
     * 날씨 관측 시간
     */
    private LocalDateTime observationTime;
    
    /**
     * 일출 시간
     */
    private LocalDateTime sunriseTime;
    
    /**
     * 일몰 시간
     */
    private LocalDateTime sunsetTime;
    
    /**
     * 데이터가 마지막으로 업데이트된 시간
     * 캐시 관리 및 데이터 신선도 확인에 사용
     */
    private LocalDateTime lastUpdated;
}
