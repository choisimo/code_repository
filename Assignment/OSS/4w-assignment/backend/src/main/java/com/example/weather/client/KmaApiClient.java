package com.example.weather.client;

import com.example.weather.model.ForecastData;
import com.example.weather.model.WeatherData;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpMethod;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Component;
import org.springframework.web.client.RestTemplate;
import org.springframework.web.util.UriComponentsBuilder;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;

@Slf4j
@Component
@RequiredArgsConstructor
public class KmaApiClient {

    private final RestTemplate restTemplate;
    private final ObjectMapper objectMapper;

    @Value("${weather.api.key}")
    private String apiKey;

    @Value("${weather.api.base-url}")
    private String baseUrl;

    /**
     * 기상청 단기예보 조회 API를 호출하여 현재 날씨 데이터를 가져옵니다.
     *
     * @param latitude 위도
     * @param longitude 경도
     * @return 날씨 데이터
     */
    public WeatherData getCurrentWeather(double latitude, double longitude) {
        try {
            // 좌표를 기상청 격자 좌표로 변환
            GridCoordinate grid = convertToGridCoordinate(latitude, longitude);
            
            // 현재 시간 기준으로 API 요청 파라미터 생성
            LocalDateTime now = LocalDateTime.now();
            String baseDate = now.format(DateTimeFormatter.ofPattern("yyyyMMdd"));
            String baseTime = getBaseTime(now);
            
            // API URL 구성
            String url = UriComponentsBuilder.fromHttpUrl(baseUrl + "/getVilageFcst")
                    .queryParam("serviceKey", apiKey)
                    .queryParam("numOfRows", 100)
                    .queryParam("pageNo", 1)
                    .queryParam("dataType", "JSON")
                    .queryParam("base_date", baseDate)
                    .queryParam("base_time", baseTime)
                    .queryParam("nx", grid.getX())
                    .queryParam("ny", grid.getY())
                    .build()
                    .toUriString();
            
            // API 호출
            log.info("Requesting URL: {}", url);
            HttpHeaders headers = new HttpHeaders();
            HttpEntity<String> entity = new HttpEntity<>(headers);
            ResponseEntity<String> response = restTemplate.exchange(url, HttpMethod.GET, entity, String.class);
            log.info("API 응답: {}", response.getBody());
            
            // 응답 파싱
            JsonNode root = objectMapper.readTree(response.getBody());
            JsonNode items = root.path("response").path("body").path("items").path("item");
            
            // 날씨 데이터 생성
            return parseWeatherData(items, latitude, longitude);
        } catch (Exception e) {
            log.error("날씨 데이터 조회 중 오류 발생: {}", e.getMessage(), e);
            throw new RuntimeException("날씨 데이터를 가져오는 중 오류가 발생했습니다: " + e.getMessage(), e);
        }
    }

    /**
     * 기상청 중기예보 조회 API를 호출하여 예보 데이터를 가져옵니다.
     *
     * @param latitude 위도
     * @param longitude 경도
     * @param days 예보 일수
     * @return 예보 데이터 목록
     */
    public List<ForecastData> getForecast(double latitude, double longitude, int days) {
        try {
            // 좌표를 기상청 격자 좌표로 변환
            GridCoordinate grid = convertToGridCoordinate(latitude, longitude);
            
            // 현재 시간 기준으로 API 요청 파라미터 생성
            LocalDateTime now = LocalDateTime.now();
            String baseDate = now.format(DateTimeFormatter.ofPattern("yyyyMMdd"));
            String baseTime = getBaseTime(now);
            
            // API URL 구성
            String url = UriComponentsBuilder.fromHttpUrl(baseUrl + "/getMidLandFcst")
                    .queryParam("serviceKey", apiKey)
                    .queryParam("numOfRows", 100)
                    .queryParam("pageNo", 1)
                    .queryParam("dataType", "JSON")
                    .queryParam("regId", getRegionCode(grid))
                    .build()
                    .toUriString();
            
            // API 호출
            log.info("Requesting URL: {}", url);
            HttpHeaders headers = new HttpHeaders();
            HttpEntity<String> entity = new HttpEntity<>(headers);
            ResponseEntity<String> response = restTemplate.exchange(url, HttpMethod.GET, entity, String.class);
            log.info("API 응답: {}", response.getBody());
            
            // 응답 파싱
            JsonNode root = objectMapper.readTree(response.getBody());
            JsonNode items = root.path("response").path("body").path("items").path("item");
            
            // 예보 데이터 생성
            return parseForecastData(items, latitude, longitude, days);
        } catch (Exception e) {
            log.error("예보 데이터 조회 중 오류 발생: {}", e.getMessage(), e);
            throw new RuntimeException("예보 데이터를 가져오는 중 오류가 발생했습니다: " + e.getMessage(), e);
        }
    }

    /**
     * 위도/경도를 기상청 격자 좌표로 변환합니다.
     */
    private GridCoordinate convertToGridCoordinate(double latitude, double longitude) {
        // 기상청 격자 좌표 변환 알고리즘 (LCC 투영법)
        double RE = 6371.00877; // 지구 반경(km)
        double GRID = 5.0; // 격자 간격(km)
        double SLAT1 = 30.0; // 표준위도 1
        double SLAT2 = 60.0; // 표준위도 2
        double OLON = 126.0; // 기준점 경도
        double OLAT = 38.0; // 기준점 위도
        double XO = 43; // 기준점 X좌표
        double YO = 136; // 기준점 Y좌표
        
        double DEGRAD = Math.PI / 180.0;
        double re = RE / GRID;
        double slat1 = SLAT1 * DEGRAD;
        double slat2 = SLAT2 * DEGRAD;
        double olon = OLON * DEGRAD;
        double olat = OLAT * DEGRAD;
        
        double sn = Math.tan(Math.PI * 0.25 + slat2 * 0.5) / Math.tan(Math.PI * 0.25 + slat1 * 0.5);
        sn = Math.log(Math.cos(slat1) / Math.cos(slat2)) / Math.log(sn);
        double sf = Math.tan(Math.PI * 0.25 + slat1 * 0.5);
        sf = Math.pow(sf, sn) * Math.cos(slat1) / sn;
        double ro = Math.tan(Math.PI * 0.25 + olat * 0.5);
        ro = re * sf / Math.pow(ro, sn);
        
        double ra = Math.tan(Math.PI * 0.25 + latitude * DEGRAD * 0.5);
        ra = re * sf / Math.pow(ra, sn);
        double theta = longitude * DEGRAD - olon;
        if (theta > Math.PI) theta -= 2.0 * Math.PI;
        if (theta < -Math.PI) theta += 2.0 * Math.PI;
        theta *= sn;
        
        int nx = (int) Math.floor(ra * Math.sin(theta) + XO + 0.5);
        int ny = (int) Math.floor(ro - ra * Math.cos(theta) + YO + 0.5);
        
        return new GridCoordinate(nx, ny);
    }

    /**
     * 현재 시간에 맞는 API 요청 기준 시간을 반환합니다.
     */
    private String getBaseTime(LocalDateTime dateTime) {
        int hour = dateTime.getHour();
        int minute = dateTime.getMinute();
        
        // 기상청 API는 3시간 단위로 업데이트됨 (02:00, 05:00, 08:00, 11:00, 14:00, 17:00, 20:00, 23:00)
        if (hour < 2) return "2300";
        else if (hour < 5) return "0200";
        else if (hour < 8) return "0500";
        else if (hour < 11) return "0800";
        else if (hour < 14) return "1100";
        else if (hour < 17) return "1400";
        else if (hour < 20) return "1700";
        else if (hour < 23) return "2000";
        else return "2300";
    }

    /**
     * 격자 좌표에 해당하는 지역 코드를 반환합니다.
     */
    private String getRegionCode(GridCoordinate grid) {
        // 격자 좌표를 기반으로 지역 코드 매핑
        // 실제 구현에서는 좀 더 정확한 매핑 로직이 필요합니다
        int x = grid.getX();
        int y = grid.getY();
        
        if (x >= 55 && x <= 66 && y >= 123 && y <= 131) return "11B00000"; // 서울, 인천, 경기도
        else if (x >= 73 && x <= 106 && y >= 76 && y <= 122) return "11D10000"; // 강원도 영서
        else if (x >= 92 && x <= 106 && y >= 123 && y <= 140) return "11D20000"; // 강원도 영동
        else if (x >= 48 && x <= 70 && y >= 108 && y <= 119) return "11C20000"; // 충청북도
        else if (x >= 44 && x <= 65 && y >= 87 && y <= 107) return "11C10000"; // 충청남도
        else if (x >= 60 && x <= 89 && y >= 59 && y <= 76) return "11F20000"; // 전라북도
        else if (x >= 50 && x <= 72 && y >= 33 && y <= 58) return "11F10000"; // 전라남도
        else if (x >= 86 && x <= 103 && y >= 46 && y <= 75) return "11H10000"; // 경상북도
        else if (x >= 74 && x <= 101 && y >= 31 && y <= 45) return "11H20000"; // 경상남도
        else if (x >= 52 && x <= 56 && y >= 33 && y <= 39) return "11G00000"; // 제주도
        else return "11B00000"; // 기본값: 서울
    }

    /**
     * API 응답에서 날씨 데이터를 파싱합니다.
     */
    private WeatherData parseWeatherData(JsonNode items, double latitude, double longitude) {
        // 필요한 날씨 요소 초기화
        Double temperature = null;
        Double feelsLike = null;
        Integer humidity = null;
        Double windSpeed = null;
        Double windDirection = null;
        Integer cloudiness = null;
        Double precipitation = null;
        String weatherMain = "맑음"; // 기본값
        String weatherDescription = "맑음";
        String weatherIcon = "01d";
        Double pressure = null;
        Double visibility = null;
        
        // 위치 정보
        String locationId = "loc_" + latitude + "_" + longitude;
        String locationName = getLocationNameByCoordinates(latitude, longitude);
        
        // API 응답 파싱
        if (items.isArray()) {
            for (JsonNode item : items) {
                String category = item.path("category").asText();
                String fcstValue = item.path("fcstValue").asText();
                
                switch (category) {
                    case "T1H": // 기온
                        temperature = Double.parseDouble(fcstValue);
                        break;
                    case "REH": // 습도
                        humidity = Integer.parseInt(fcstValue);
                        break;
                    case "WSD": // 풍속
                        windSpeed = Double.parseDouble(fcstValue);
                        break;
                    case "VEC": // 풍향
                        windDirection = Double.parseDouble(fcstValue);
                        break;
                    case "SKY": // 하늘상태
                        int skyCode = Integer.parseInt(fcstValue);
                        if (skyCode == 1) {
                            weatherMain = "맑음";
                            weatherDescription = "맑음";
                            weatherIcon = "01d";
                        } else if (skyCode == 3) {
                            weatherMain = "구름많음";
                            weatherDescription = "구름많음";
                            weatherIcon = "03d";
                        } else if (skyCode == 4) {
                            weatherMain = "흐림";
                            weatherDescription = "흐림";
                            weatherIcon = "04d";
                        }
                        break;
                    case "PTY": // 강수형태
                        int ptyCode = Integer.parseInt(fcstValue);
                        if (ptyCode == 1) {
                            weatherMain = "비";
                            weatherDescription = "비";
                            weatherIcon = "09d";
                        } else if (ptyCode == 2) {
                            weatherMain = "비/눈";
                            weatherDescription = "비/눈";
                            weatherIcon = "13d";
                        } else if (ptyCode == 3) {
                            weatherMain = "눈";
                            weatherDescription = "눈";
                            weatherIcon = "13d";
                        } else if (ptyCode == 4) {
                            weatherMain = "소나기";
                            weatherDescription = "소나기";
                            weatherIcon = "09d";
                        }
                        break;
                    case "RN1": // 1시간 강수량
                        precipitation = Double.parseDouble(fcstValue);
                        break;
                }
            }
        }
        
        // 체감온도 계산 (간단한 계산식)
        if (temperature != null && windSpeed != null) {
            feelsLike = temperature - (windSpeed * 0.5);
        } else {
            feelsLike = temperature;
        }
        
        // 날씨 데이터 생성
        return WeatherData.builder()
                .id(locationId + "_" + LocalDateTime.now())
                .locationId(locationId)
                .locationName(locationName)
                .temperature(temperature)
                .feelsLike(feelsLike)
                .humidity(humidity)
                .windSpeed(windSpeed)
                .windDirection(windDirection)
                .cloudiness(cloudiness)
                .precipitation(precipitation)
                .weatherMain(weatherMain)
                .weatherDescription(weatherDescription)
                .weatherIcon(weatherIcon)
                .pressure(pressure)
                .visibility(visibility)
                .observationTime(LocalDateTime.now())
                .lastUpdated(LocalDateTime.now())
                .build();
    }

    /**
     * API 응답에서 예보 데이터를 파싱합니다.
     */
    private List<ForecastData> parseForecastData(JsonNode items, double latitude, double longitude, int days) {
        List<ForecastData> forecastList = new ArrayList<>();
        String locationId = "loc_" + latitude + "_" + longitude;
        String locationName = getLocationNameByCoordinates(latitude, longitude);
        LocalDateTime now = LocalDateTime.now();
        
        // 중기예보 데이터 파싱
        if (items.isArray() && items.size() > 0) {
            JsonNode item = items.get(0);
            
            // 최대 days일까지의 예보 생성
            for (int i = 0; i < Math.min(days, 10); i++) {
                LocalDateTime forecastTime = now.plusDays(i);
                String dayKey = "rnSt" + (i + 3) + "Am"; // 강수확률 키 (3일 후부터 시작)
                String tempHighKey = "taMax" + (i + 3); // 최고기온 키
                String tempLowKey = "taMin" + (i + 3); // 최저기온 키
                
                // 해당 일자의 예보 데이터가 없으면 건너뜀
                if (!item.has(dayKey)) continue;
                
                Integer rainProbability = item.has(dayKey) ? item.path(dayKey).asInt() : null;
                Double tempHigh = item.has(tempHighKey) ? item.path(tempHighKey).asDouble() : null;
                Double tempLow = item.has(tempLowKey) ? item.path(tempLowKey).asDouble() : null;
                
                // 예보 데이터 생성
                ForecastData forecast = ForecastData.builder()
                        .id("daily_forecast_" + latitude + "_" + longitude + "_" + i)
                        .locationId(locationId)
                        .locationName(locationName)
                        .forecastTime(forecastTime)
                        .temperature((tempHigh + tempLow) / 2) // 평균 기온
                        .temperatureMax(tempHigh)
                        .temperatureMin(tempLow)
                        .rainProbability(rainProbability)
                        .weatherMain(getWeatherMainByRainProbability(rainProbability))
                        .weatherDescription(getWeatherDescriptionByRainProbability(rainProbability))
                        .weatherIcon(getWeatherIconByRainProbability(rainProbability))
                        .lastUpdated(LocalDateTime.now())
                        .build();
                
                forecastList.add(forecast);
            }
        }
        
        return forecastList;
    }

    /**
     * 강수확률에 따른 날씨 상태를 반환합니다.
     */
    private String getWeatherMainByRainProbability(Integer rainProbability) {
        if (rainProbability == null) return "맑음";
        if (rainProbability >= 60) return "비";
        if (rainProbability >= 30) return "구름많음";
        return "맑음";
    }

    /**
     * 강수확률에 따른 날씨 설명을 반환합니다.
     */
    private String getWeatherDescriptionByRainProbability(Integer rainProbability) {
        if (rainProbability == null) return "맑음";
        if (rainProbability >= 60) return "비가 내릴 확률이 높습니다";
        if (rainProbability >= 30) return "구름이 많고 비가 내릴 수 있습니다";
        return "맑은 날씨가 예상됩니다";
    }

    /**
     * 강수확률에 따른 날씨 아이콘을 반환합니다.
     */
    private String getWeatherIconByRainProbability(Integer rainProbability) {
        if (rainProbability == null) return "01d";
        if (rainProbability >= 60) return "09d";
        if (rainProbability >= 30) return "03d";
        return "01d";
    }

    /**
     * 좌표에 해당하는 위치 이름을 반환합니다.
     */
    private String getLocationNameByCoordinates(double latitude, double longitude) {
        // 실제 구현에서는 좌표를 기반으로 위치 이름을 조회하는 로직이 필요합니다
        // 여기서는 간단히 서울로 고정
        return "서울";
    }

    /**
     * 기상청 격자 좌표를 나타내는 내부 클래스
     */
    @lombok.Data
    private static class GridCoordinate {
        private final int x;
        private final int y;
    }
}