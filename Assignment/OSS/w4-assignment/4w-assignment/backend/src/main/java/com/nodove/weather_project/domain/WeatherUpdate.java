package com.nodove.weather_project;

import jakarta.persistence.*;
import lombok.*;
import java.time.LocalDateTime;

@Entity
@Table(name = "weather_update")
@Data
@NoArgsConstructor
@AllArgsConstructor
public class WeatherUpdate {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;

    private double lat;
    private double lng;
    private double temperature;
    private int rainProbability;
    private double windVolume;
    private LocalDateTime timestamp;
}
