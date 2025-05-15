package com.example.weather.model;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.time.LocalDateTime;

@Data
@Builder
@NoArgsConstructor
@AllArgsConstructor
public class WeatherResponse {
    private Double temperature;
    private Integer rainProbability;
    private Double windSpeed;
    private String windDirection;
    private Double humidity;
    private String weatherCondition;
    private LocalDateTime timestamp;
    private String locationName;
    private Double latitude;
    private Double longitude;
}
