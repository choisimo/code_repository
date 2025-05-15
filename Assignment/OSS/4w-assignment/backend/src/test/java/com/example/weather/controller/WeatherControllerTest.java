package com.example.weather.controller;

import static org.mockito.ArgumentMatchers.anyDouble;
import static org.mockito.Mockito.when;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;

import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.web.servlet.WebMvcTest;
import org.springframework.boot.test.mock.mockito.MockBean;
import org.springframework.test.web.servlet.MockMvc;

import com.example.weather.model.WeatherResponse;
import com.example.weather.service.WeatherService;

import java.time.LocalDateTime;

@WebMvcTest(WeatherController.class)
public class WeatherControllerTest {

    @Autowired
    private MockMvc mockMvc;

    @MockBean
    private WeatherService weatherService;

    @Test
    public void testGetWeather() throws Exception {
        // Mock 데이터 설정
        WeatherResponse mockResponse = WeatherResponse.builder()
                .temperature(25.0)
                .rainProbability(10)
                .windSpeed(5.0)
                .windDirection("북동")
                .humidity(60.0)
                .weatherCondition("맑음")
                .timestamp(LocalDateTime.now())
                .locationName("테스트 위치")
                .latitude(37.5665)
                .longitude(126.9780)
                .build();

        when(weatherService.getWeatherByCoordinates(anyDouble(), anyDouble())).thenReturn(mockResponse);

        // API 호출 테스트
        mockMvc.perform(get("/api/weather")
                .param("lat", "37.5665")
                .param("lng", "126.9780"))
                .andExpect(status().isOk());
    }
}