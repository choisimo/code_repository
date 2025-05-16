.example.weather.model.ForecastData
\
.example.weather.model.WeatherData
.example.weather.model.WeatherResponse
.fasterxml.jackson.databind.JsonNode
.fasterxml.jackson.databind.ObjectMapper
.RequiredArgsConstructor
.extern.slf4j.Slf4j
.springframework.beans.factory.annotation.Value
.springframework.http.HttpEntity
.springframework.http.HttpHeaders
.springframework.http.HttpMethod
.springframework.http.ResponseEntity
.springframework.stereotype.Component
.springframework.web.client.RestTemplate
.springframework.web.util.UriComponentsBuilder
.time.LocalDateTime
.time.format.DateTimeFormatter
.util.ArrayList
.util.List

@Slf4j
@Component
@RequiredArgsConstructor
public
class KmaApiClient {
  private final
  RestTemplate
  restTemplate
  private final
  ObjectMapper
  objectMapper

  @Value("${weather.api.base-url}")
  private String
  baseUrl

  /**
   * 기상청 단기예보 조회 API를 호출하여 현재 날씨 데이터를 가져옵니다.
   *
   * @param latitude 위도
   * @param longitude 경도
   * @param apiKey API 키 (클라이언트에서 전달받은 키 또는 서버 설정 키)
   * @return 날씨 데이터
   */
  public WeatherData
  getCurrentWeather(double latitude, double longitude, String apiKey) {
        try {
            // 좌표를 기상청 격자 좌표로 변환
            GridCoordinate grid = convertToGridCoordinate(latitude, longitude);
            
            // 현재 시간 기준으로 API 요청 파라미터 생성
            LocalDateTime now = LocalDateTime.now();
            String baseDate = now.format(DateTimeFormatter.ofPattern("yyyyMMdd"));
            String baseTime = getBaseTime(now);
            
            // API URL 구성 (API 키를 파라미터로 전달)
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
        } catch (Exception e) 
            log.error("날씨 데이터 조회 중 오류 발생: {}", e.getMessage(), e);
            throw new RuntimeException("날씨 데이터를 가져오는 중 오류가 발생했습니다: " + e.getMessage(), e);
    }

  /**
   * 현재 날씨 데이터를 WeatherResponse 형태로 변환하여 반환합니다.
   */
  public WeatherResponse
  getCurrentWeatherAsResponse(double latitude, double longitude, String apiKey) {
        WeatherData data = getCurrentWeather(latitude, longitude, apiKey);
        
        return WeatherResponse.builder()
                .temperature(data.getTemperature())
                .rainProbability(getRainProbabilityFromWeatherData(data))
                .windSpeed(data.getWindSpeed())
                .windDirection(getWindDirectionText(data.getWindDirection()))
                .humidity(data.getHumidity() != null ? data.getHumidity().doubleValue() : null)
                .weatherCondition(data.getWeatherDescription())
                .timestamp(LocalDateTime.now())
                .locationName(data.getLocationName())
                .latitude(latitude)
                .longitude(longitude)
                .build();
    }

  /**
   * 날씨 데이터에서 강수확률을 추출합니다.
   */
  private Integer
  getRainProbabilityFromWeatherData(WeatherData data) {
        // 날씨 상태에 따른 강수확률 추정
        String weatherMain = data.getWeatherMain();
        if (weatherMain == null) return 0;
        
        if (weatherMain.contains("비") || weatherMain.contains("소나기")) {
            return 80;
        } else if (weatherMain.contains("구름많음")) {
            return 30;
        } else if (weatherMain.contains("흐림")) {
            return 50;
        }
        
        return 0;
    }

  /**
   * 풍향 각도를 텍스트로 변환합니다.
   */
  private String
  getWindDirectionText(Double windDirection) {
        if (windDirection == null) return "무풍";
        
        // 풍향 각도를 16방위로 변환
        String[] directions = {"북", "북북동", "북동", "동북동", "동", "동남동", "남동", "남남동", 
                              "남", "남남서", "남서", "서남서", "서", "서북서", "북서", "북북서"};
        
        int index = (int) Math.round(windDirection / 22.5) % 16;
        return directions[index];
    }

  // 나머지 메서드는 동일...
}
