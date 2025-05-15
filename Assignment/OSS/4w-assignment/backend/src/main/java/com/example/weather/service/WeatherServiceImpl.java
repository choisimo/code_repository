package com.example.weather.service;

import com.example.weather.model.ForecastData;
import com.example.weather.model.ForecastResponse;
import com.example.weather.model.RegionWeather;
import com.example.weather.model.WeatherResponse;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.cache.annotation.Cacheable;
import org.springframework.http.HttpStatus;
import org.springframework.stereotype.Service;
import org.springframework.web.server.ResponseStatusException;
import reactor.core.publisher.Flux;
import reactor.core.publisher.Sinks;

import java.time.Duration;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

@Service
@RequiredArgsConstructor
public class WeatherServiceImpl implements WeatherService {

    private final Random random = new Random();
    
    // SSE를 위한 Sink 생성
    private final Sinks.Many<WeatherResponse> weatherSink = Sinks.many().multicast().onBackpressureBuffer();
    
    @Value("${weather.api.base-url}")
    private String weatherApiBaseUrl;
    
    @Value("${weather.api.key}")
    private String weatherApiKey;

    @Override
    @Cacheable(value = "weatherData", key = "#latitude + '-' + #longitude")
    public WeatherResponse getWeatherByCoordinates(double latitude, double longitude) {
        // 실제 구현에서는 공공데이터 포털 API를 호출하여 날씨 데이터를 가져옵니다.
        // 여기서는 샘플 데이터를 반환합니다.
        try {
            return WeatherResponse.builder()
                    .temperature(generateRandomTemperature())
                    .rainProbability(generateRandomRainProbability())
                    .windSpeed(generateRandomWindSpeed())
                    .windDirection(getRandomWindDirection())
                    .humidity(random.nextDouble() * 50 + 30)
                    .weatherCondition(getRandomWeatherCondition())
                    .timestamp(LocalDateTime.now())
                    .locationName("샘플 위치")
                    .latitude(latitude)
                    .longitude(longitude)
                    .build();
        } catch (Exception e) {
            throw new ResponseStatusException(HttpStatus.INTERNAL_SERVER_ERROR, "날씨 데이터를 가져오는 중 오류가 발생했습니다.", e);
        }
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
    public ForecastResponse getHourlyForecast(double latitude, double longitude, int hours) {
        try {
            // 실제 구현에서는 외부 API를 호출하여 시간별 예보 데이터를 가져옵니다.
            // 여기서는 샘플 데이터를 생성합니다.
            List<ForecastData> hourlyForecast = new ArrayList<>();
            LocalDateTime now = LocalDateTime.now();
            
            for (int i = 0; i < hours; i++) {
                LocalDateTime forecastTime = now.plusHours(i);
                
                ForecastData forecast = ForecastData.builder()
                    .id("hourly_forecast_" + latitude + "_" + longitude + "_" + i)
                    .locationId("loc_" + latitude + "_" + longitude)
                    .locationName("샘플 위치")
                    .forecastTime(forecastTime)
                    .temperature(generateRandomTemperature())
                    .feelsLike(generateRandomTemperature() - 2)
                    .rainProbability(generateRandomRainProbability())
                    .rainAmount(random.nextDouble() * 5)
                    .humidity(random.nextInt(40) + 40)
                    .pressure(1000 + random.nextDouble() * 30)
                    .windSpeed(generateRandomWindSpeed())
                    .windDirection(random.nextDouble() * 360)
                    .cloudiness(random.nextInt(100))
                    .weatherMain(getRandomWeatherCondition())
                    .weatherDescription(getRandomWeatherCondition() + " 상태입니다")
                    .weatherIcon("01d")
                    .visibility(random.nextDouble() * 10000)
                    .lastUpdated(LocalDateTime.now())
                    .build();
                    
                hourlyForecast.add(forecast);
            }
            
            return ForecastResponse.builder()
                .locationId("loc_" + latitude + "_" + longitude)
                .locationName("샘플 위치")
                .latitude(latitude)
                .longitude(longitude)
                .hourlyForecast(hourlyForecast) // 시간별 예보 데이터 설정
                .forecastTime(LocalDateTime.now())
                .provider("샘플 데이터 제공자")
                .lastUpdated(LocalDateTime.now())
                .build();
        } catch (Exception e) {
            throw new RuntimeException("시간별 날씨 예보 데이터를 가져오는 중 오류가 발생했습니다.", e);
        }
    }
    
    @Override
    public ForecastResponse getWeeklyForecast(double latitude, double longitude) {
        return getForecastByCoordinates(latitude, longitude, 7);
    }
    

    
    private RegionWeather createRegionWeather(String code, String name, double lat, double lng) {
        return RegionWeather.builder()
                .regionCode(code)
                .regionName(name)
                .temperature(generateRandomTemperature())
                .rainProbability(generateRandomRainProbability())
                .windSpeed(generateRandomWindSpeed())
                .latitude(lat)
                .longitude(lng)
                .build();
    }
    
    private double generateRandomTemperature() {
        return Math.round((random.nextDouble() * 30 - 5) * 10) / 10.0;
    }
    
    private int generateRandomRainProbability() {
        return random.nextInt(101);
    }
    
    private double generateRandomWindSpeed() {
        return Math.round(random.nextDouble() * 10 * 10) / 10.0;
    }
    
    private String getRandomWindDirection() {
        String[] directions = {"북", "북동", "동", "남동", "남", "남서", "서", "북서"};
        return directions[random.nextInt(directions.length)];
    }
    
    private String getRandomWeatherCondition() {
        String[] conditions = {"맑음", "구름 조금", "구름 많음", "흐림", "비", "눈", "비/눈"};
        return conditions[random.nextInt(conditions.length)];
    }

    @Override
    public ForecastResponse getForecastByCoordinates(double latitude, double longitude, int days) {
        try {
            List<ForecastData> dailyForecast = new ArrayList<>();
            LocalDateTime now = LocalDateTime.now();
            for (int i = 0; i < days; i++) {
                LocalDateTime forecastTime = now.plusDays(i);
                ForecastData forecast = ForecastData.builder()
                    .id("daily_forecast_" + latitude + "_" + longitude + "_" + i)
                    .locationId("loc_" + latitude + "_" + longitude)
                    .locationName("샘플 위치")
                    .forecastTime(forecastTime)
                    .temperature(generateRandomTemperature())
                    .feelsLike(generateRandomTemperature())
                    .rainProbability(generateRandomRainProbability())
                    .rainAmount(random.nextDouble() * 5)
                    .humidity(random.nextInt(40) + 40)
                    .pressure(1000 + random.nextDouble() * 30)
                    .windSpeed(generateRandomWindSpeed())
                    .windDirection(random.nextDouble() * 360)
                    .cloudiness(random.nextInt(100))
                    .weatherMain(getRandomWeatherCondition())
                    .weatherDescription(getRandomWeatherCondition() + " 상태입니다")
                    .weatherIcon("01d")
                    .visibility(random.nextDouble() * 10000)
                    .lastUpdated(LocalDateTime.now())
                    .build();
                dailyForecast.add(forecast);
            }
            return ForecastResponse.builder()
                .locationId("loc_" + latitude + "_" + longitude)
                .locationName("샘플 위치")
                .latitude(latitude)
                .longitude(longitude)
                .dailyForecast(dailyForecast)
                .forecastDays(days)
                .forecastTime(LocalDateTime.now())
                .provider("샘플 데이터 제공자")
                .lastUpdated(LocalDateTime.now())
                .build();
        } catch (Exception e) {
            throw new RuntimeException("주간 날씨 예보 데이터를 가져오는 중 오류가 발생했습니다.", e);
        }
    }
}
