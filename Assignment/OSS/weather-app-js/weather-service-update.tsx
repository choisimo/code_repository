.example.weather.client.KmaApiClient
.example.weather.model.ForecastData
.example.weather.model.ForecastResponse
.example.weather.model.RegionWeather
.example.weather.model.WeatherResponse
.example.weather.service.WeatherService
.RequiredArgsConstructor
.springframework.beans.factory.annotation.Value
.springframework.cache.annotation.Cacheable
.springframework.http.HttpStatus
.springframework.stereotype.Service
.springframework.web.server.ResponseStatusException
.core.publisher.Flux
.core.publisher.Sinks
.servlet.http.HttpServletRequest
.time.Duration
.time.LocalDateTime
.util.ArrayList
.util.List
.util.Random

@Service
@RequiredArgsConstructor
public
class WeatherServiceImpl implements WeatherService {
  private final
  Random
  random = new Random()
  private final
  KmaApiClient
  kmaApiClient
  private final
  HttpServletRequest
  request

  // SSE를 위한 Sink 생성
  private final
  Sinks;
  .
  Many<WeatherResponse>
  weatherSink = Sinks.many().multicast().onBackpressureBuffer()

  @Value("${weather.api.base-url}")
  private String
  weatherApiBaseUrl

  @Override
  @Cacheable((value = "weatherData"), (key = "#latitude + '-' + #longitude"))
  public WeatherResponse
  getWeatherByCoordinates(double latitude, double longitude) {
        // 실제 구현에서는 공공데이터 포털 API를 호출하여 날씨 데이터를 가져옵니다.
        try {
            // 요청에서 API 키 가져오기 (미들웨어에서 설정됨)
            String apiKey = (String) request.getAttribute("apiKey");
            
            // KMA API 클라이언트를 통해 날씨 데이터 가져오기
            return kmaApiClient.getCurrentWeatherAsResponse(latitude, longitude, apiKey);
        } catch (Exception e) 
            throw new ResponseStatusException(HttpStatus.INTERNAL_SERVER_ERROR, "날씨 데이터를 가져오는 중 오류가 발생했습니다: " + e.getMessage(), e);
    }

  // 나머지 메서드도 동일하게 수정...
}
