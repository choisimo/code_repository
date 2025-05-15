package com.example.weather.controller;

import com.example.weather.model.ForecastResponse;
import com.example.weather.service.WeatherService;
import lombok.RequiredArgsConstructor;
import org.springframework.cache.annotation.Cacheable;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

/**
 * 날씨 예보 API를 제공하는 컨트롤러
 * 
 * <p>이 컨트롤러는 시간별, 일별, 주간 예보 등 다양한 형태의 날씨 예보 정보를 제공하는 API를 포함합니다.</p>
 * 
 * @author Nodove 개발팀
 * @version 1.0
 * @since 2023-07-01
 */
@RestController
@RequestMapping("/api/forecast")
@RequiredArgsConstructor
public class ForecastController {

    private final WeatherService weatherService;

    /**
     * 특정 좌표에 대한 날씨 예보를 제공합니다.
     * 
     * @param lat 위도
     * @param lng 경도
     * @param days 예보 일수 (기본값: 5)
     * @return 날씨 예보 응답
     */
    @GetMapping
    @Cacheable(value = "forecasts", key = "{#lat, #lng, #days}")
    public ResponseEntity<ForecastResponse> getForecast(
            @RequestParam double lat,
            @RequestParam double lng,
            @RequestParam(defaultValue = "5") int days) {
        return ResponseEntity.ok(weatherService.getForecastByCoordinates(lat, lng, days));
    }

    /**
     * 시간별 날씨 예보를 제공합니다.
     * 
     * @param lat 위도
     * @param lng 경도
     * @param hours 예보 시간 (기본값: 24)
     * @return 시간별 날씨 예보 응답
     */
    @GetMapping("/hourly")
    @Cacheable(value = "hourlyForecasts", key = "{#lat, #lng, #hours}")
    public ResponseEntity<ForecastResponse> getHourlyForecast(
            @RequestParam double lat,
            @RequestParam double lng,
            @RequestParam(defaultValue = "24") int hours) {
        return ResponseEntity.ok(weatherService.getHourlyForecast(lat, lng, hours));
    }

    /**
     * 주간 날씨 예보를 제공합니다.
     * 
     * @param lat 위도
     * @param lng 경도
     * @return 주간 날씨 예보 응답
     */
    @GetMapping("/weekly")
    @Cacheable(value = "weeklyForecasts", key = "{#lat, #lng}")
    public ResponseEntity<ForecastResponse> getWeeklyForecast(
            @RequestParam double lat,
            @RequestParam double lng) {
        return ResponseEntity.ok(weatherService.getWeeklyForecast(lat, lng));
    }
}
