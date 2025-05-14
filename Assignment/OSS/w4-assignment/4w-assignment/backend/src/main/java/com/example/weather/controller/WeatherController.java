package com.example.weather.controller;

import com.example.weather.model.RegionWeather;
import com.example.weather.model.WeatherResponse;
import com.example.weather.service.WeatherService;
import lombok.RequiredArgsConstructor;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;
import reactor.core.publisher.Flux;

import java.util.List;

@RestController
@RequestMapping("/api/weather")
@RequiredArgsConstructor
public class WeatherController {

    private final WeatherService weatherService;

    @GetMapping
    public ResponseEntity<WeatherResponse> getWeather(
            @RequestParam double lat,
            @RequestParam double lng) {
        return ResponseEntity.ok(weatherService.getWeatherByCoordinates(lat, lng));
    }

    @GetMapping("/current-temp")
    public ResponseEntity<Double> getCurrentTemp(
            @RequestParam double lat,
            @RequestParam double lng) {
        return ResponseEntity.ok(weatherService.getCurrentTemp(lat, lng));
    }

    @GetMapping("/rain-probability")
    public ResponseEntity<Integer> getRainProbability(
            @RequestParam double lat,
            @RequestParam double lng) {
        return ResponseEntity.ok(weatherService.getRainProbability(lat, lng));
    }

    @GetMapping("/wind")
    public ResponseEntity<Double> getWindSpeed(
            @RequestParam double lat,
            @RequestParam double lng) {
        return ResponseEntity.ok(weatherService.getWindSpeed(lat, lng));
    }

    @GetMapping("/regions")
    public ResponseEntity<List<RegionWeather>> getRegionWeather() {
        return ResponseEntity.ok(weatherService.getRegionWeather());
    }

    @GetMapping(value = "/stream", produces = MediaType.TEXT_EVENT_STREAM_VALUE)
    public Flux<WeatherResponse> streamWeather() {
        return weatherService.streamWeatherUpdates();
    }
}
