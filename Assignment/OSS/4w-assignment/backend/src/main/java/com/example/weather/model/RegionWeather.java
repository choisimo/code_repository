package com.example.weather.model;

import lombok.AllArgsConstructor;
import lombok.Builder;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@Builder
@NoArgsConstructor
@AllArgsConstructor
public class RegionWeather {
    private String regionCode;
    private String regionName;
    private Double temperature;
    private Integer rainProbability;
    private Double windSpeed;
    private Double latitude;
    private Double longitude;
}
