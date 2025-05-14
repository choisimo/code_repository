package com.example.weather.service;

import com.example.weather.model.ForecastResponse;
import com.example.weather.model.RegionWeather;
import com.example.weather.model.WeatherResponse;
import reactor.core.publisher.Flux;

import java.util.List;

public interface WeatherService {
    
    WeatherResponse getWeatherByCoordinates(double latitude, double longitude);
    
    Double getCurrentTemp(double latitude, double longitude);
    
    Integer getRainProbability(double latitude, double longitude);
    
    Double getWindSpeed(double latitude, double longitude);
    
    List<RegionWeather> getRegionWeather();
    
    Flux<WeatherResponse> streamWeatherUpdates();
    
    /**
     * 특정 좌표에 대한 날씨 예보를 조회합니다.
     * 
     * @param latitude 위도
     * @param longitude 경도
     * @param days 예보 일수 (1-7)
     * @return 날씨 예보 응답 객체
     */
    ForecastResponse getForecastByCoordinates(double latitude, double longitude, int days);
    
    /**
     * 특정 좌표에 대한 시간별 예보를 조회합니다.
     * 
     * @param latitude 위도
     * @param longitude 경도
     * @param hours 예보 시간 (1-48)
     * @return 날씨 예보 응답 객체
     */
    ForecastResponse getHourlyForecast(double latitude, double longitude, int hours);
    
    /**
     * 특정 좌표에 대한 주간 예보를 조회합니다.
     * 
     * @param latitude 위도
     * @param longitude 경도
     * @return 날씨 예보 응답 객체
     */
    ForecastResponse getWeeklyForecast(double latitude, double longitude);
}
