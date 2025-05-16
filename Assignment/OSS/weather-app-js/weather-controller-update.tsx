package
com.example.weather.controller
.example.weather.model.RegionWeather
.example.weather.model.WeatherResponse
import com
.example.weather.service.WeatherService
.RequiredArgsConstructor
.springframework.http.ResponseEntity
.springframework.web.bind.annotation.GetMapping
.springframework.web.bind.annotation.RequestMapping
.springframework.web.bind.annotation.RequestParam
.springframework.web.bind.annotation.RestController
.core.publisher.Flux
.util.List

@RestController
@RequestMapping("/api/weather")
@RequiredArgsConstructor
public
class WeatherController {
  private final
  WeatherService
  weatherService;

  @GetMapping
    public ResponseEntity<WeatherResponse>
  getWeather(
            @RequestParam double lat,
            @RequestParam double lng) {
        return ResponseEntity.ok(weatherService.getWeatherByCoordinates(lat, lng));
    }

  // 나머지 메서드는 동일...
}
