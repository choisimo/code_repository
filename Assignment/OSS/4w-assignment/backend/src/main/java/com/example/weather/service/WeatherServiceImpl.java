package com.example.weather.service;

import com.example.weather.model.ForecastData;
import com.example.weather.model.ForecastResponse;
import com.example.weather.model.RegionWeather;
import com.example.weather.model.WeatherData;
import com.example.weather.model.WeatherResponse;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.cache.annotation.Cacheable;
import org.springframework.http.HttpStatus;
import org.springframework.stereotype.Service;
import org.springframework.web.server.ResponseStatusException;
import reactor.core.publisher.Flux;
import reactor.core.publisher.Sinks;

import com.example.weather.client.KmaApiClient;
import java.time.Duration;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

@Service
@RequiredArgsConstructor
public class WeatherServiceImpl implements WeatherService {

    private final KmaApiClient kmaApiClient;
    
    // SSE를 위한 Sink 생성
    private final Sinks.Many<WeatherResponse> weatherSink = Sinks.many().multicast().onBackpressureBuffer();
    
    @Value("${weather.api.base-url}")
    private String weatherApiBaseUrl;

    @Override
    @Cacheable(value = "weatherData", key = "#latitude + '-' + #longitude")
    public WeatherResponse getWeatherByCoordinates(double latitude, double longitude) {
        WeatherData data = kmaApiClient.getCurrentWeather(latitude, longitude);
        return WeatherResponse.builder()
                .temperature(data.getTemperature())
                .rainProbability(data.getPrecipitation() != null ? data.getPrecipitation().intValue() : 0)
                .windSpeed(data.getWindSpeed())
                .windDirection(data.getWindDirection().toString())
                .humidity(data.getHumidity() != null ? data.getHumidity().doubleValue() : null)
                .weatherCondition(data.getWeatherMain())
                .timestamp(data.getObservationTime())
                .locationName(data.getLocationName())
                .latitude(latitude)
                .longitude(longitude)
                .build();
    }

    @Override
    @Cacheable(value = "temperatureData", key = "#latitude + '-' + #longitude")
    public Double getCurrentTemp(double latitude, double longitude) {
        return getWeatherByCoordinates(latitude, longitude).getTemperature();
    }

    @Override
    @Cacheable(value = "rainData", key = "#latitude + '-' + #longitude")
    public Integer getRainProbability(double latitude, double longitude) {
        return getWeatherByCoordinates(latitude, longitude).getRainProbability();
    }

    @Override
    @Cacheable(value = "windData", key = "#latitude + '-' + #longitude")
    public Double getWindSpeed(double latitude, double longitude) {
        return getWeatherByCoordinates(latitude, longitude).getWindSpeed();
    }

    @Override
    @Cacheable(value = "regionWeatherData")
    public List<RegionWeather> getRegionWeather() {
        // 실제 구현에서는 여러 지역의 날씨 데이터를 가져옵니다.
        // 여기서는 샘플 데이터를 반환합니다.
        List<RegionWeather> regions = new ArrayList<>();
        
        // 서울
        regions.add(createRegionWeather("11", "서울", 37.5665, 126.9780));
        // 부산
        regions.add(createRegionWeather("21", "부산", 35.1796, 129.0756));
        // 인천
        regions.add(createRegionWeather("22", "인천", 37.4563, 126.7052));
        // 대구
        regions.add(createRegionWeather("23", "대구", 35.8714, 128.6014));
        // 광주
        regions.add(createRegionWeather("24", "광주", 35.1595, 126.8526));
        // 대전
        regions.add(createRegionWeather("25", "대전", 36.3504, 127.3845));
        
        return regions;
    }

    @Override
    public Flux<WeatherResponse> streamWeatherUpdates() {
        return weatherSink.asFlux()
                .mergeWith(Flux.interval(Duration.ofSeconds(10))
                        .map(tick -> {
                            WeatherResponse update = getWeatherByCoordinates(37.5665, 126.9780);
                            weatherSink.tryEmitNext(update);
                            return update;
                        }));
    }

    @Override
    public ForecastResponse getForecastByCoordinates(double latitude, double longitude, int days) {
        List<ForecastData> daily = kmaApiClient.getForecast(latitude, longitude, days);
        String locationName = daily.isEmpty() ? "" : daily.get(0).getLocationName();
        return ForecastResponse.builder()
                .locationId("loc_" + latitude + "_" + longitude)
                .locationName(locationName)
                .latitude(latitude)
                .longitude(longitude)
                .hourlyForecast(Collections.emptyList())
                .dailyForecast(daily)
                .forecastTime(LocalDateTime.now())
                .forecastDays(days)
                .provider("KMA")
                .lastUpdated(LocalDateTime.now())
                .build();
    }

    @Override
    public ForecastResponse getHourlyForecast(double latitude, double longitude, int hours) {
        int days = (int) Math.ceil(hours / 24.0);
        return getForecastByCoordinates(latitude, longitude, days);
    }

    @Override
    public ForecastResponse getWeeklyForecast(double latitude, double longitude) {
        return getForecastByCoordinates(latitude, longitude, 7);
    }

    private RegionWeather createRegionWeather(String code, String name, double lat, double lng) {
        return RegionWeather.builder()
                .regionCode(code)
                .regionName(name)
                .temperature(getCurrentTemp(lat, lng))
                .rainProbability(getRainProbability(lat, lng))
                .windSpeed(getWindSpeed(lat, lng))
                .latitude(lat)
                .longitude(lng)
                .build();
    }
}
