/**
 * 애플리케이션의 캐싱 설정을 담당하는 구성 클래스
 * 
 * <p>이 클래스는 Spring의 캐시 추상화 계층을 설정하고, Caffeine 캐시 구현체를 사용합니다.
 * Caffeine은 고성능 Java 캐싱 라이브러리로 Google Guava 캐시의 개선된 버전입니다.</p>
 * 
 * <p>주요 특징:</p>
 * <ul>
 *   <li>메모리 기반 캐시 설정</li>
 *   <li>캐시 만료 정책 설정 (TTL, 시간 기반 만료)</li>
 *   <li>최대 캐시 항목 수 제한</li>
 *   <li>캐시 통계 활성화</li>
 * </ul>
 * 
 * <p>이 설정은 날씨 API 호출과 같은 자주 변경되지 않는 데이터를 캐싱하여
 * 외부 API 호출 횟수를 줄이고 응답 시간을 개선하는 데 사용됩니다.</p>
 * 
 * @author Nodove 개발팀
 * @version 1.0
 * @since 2023-06-01
 */
package com.example.weather.config;

import com.github.benmanes.caffeine.cache.Caffeine;
import org.springframework.cache.CacheManager;
import org.springframework.cache.annotation.EnableCaching;
import org.springframework.cache.caffeine.CaffeineCacheManager;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.cache.caffeine.CaffeineCache;
import org.springframework.cache.support.SimpleCacheManager;

import java.util.Arrays;
import java.util.concurrent.TimeUnit;

@Configuration
@EnableCaching
public class CacheConfig {

    @Bean
    public CacheManager cacheManager() {
        SimpleCacheManager cacheManager = new SimpleCacheManager();
        cacheManager.setCaches(Arrays.asList(
            // 현재 날씨 데이터 캐시 (5분 TTL)
            new CaffeineCache("weatherData", 
                Caffeine.newBuilder()
                    .maximumSize(500)
                    .expireAfterWrite(5, TimeUnit.MINUTES)
                    .recordStats()
                    .build()),
                    
            // 일반 예보 데이터 캐시 (30분 TTL)
            new CaffeineCache("forecasts", 
                Caffeine.newBuilder()
                    .maximumSize(300)
                    .expireAfterWrite(30, TimeUnit.MINUTES)
                    .recordStats()
                    .build()),
                    
            // 시간별 예보 데이터 캐시 (15분 TTL)
            new CaffeineCache("hourlyForecasts", 
                Caffeine.newBuilder()
                    .maximumSize(200)
                    .expireAfterWrite(15, TimeUnit.MINUTES)
                    .recordStats()
                    .build()),
                    
            // 주간 예보 데이터 캐시 (1시간 TTL)
            new CaffeineCache("weeklyForecasts", 
                Caffeine.newBuilder()
                    .maximumSize(100)
                    .expireAfterWrite(1, TimeUnit.HOURS)
                    .recordStats()
                    .build()),
                    
            // 지역 날씨 데이터 캐시 (10분 TTL)
            new CaffeineCache("regionWeather", 
                Caffeine.newBuilder()
                    .maximumSize(50)
                    .expireAfterWrite(10, TimeUnit.MINUTES)
                    .recordStats()
                    .build())
        ));
        return cacheManager;
    }
}
