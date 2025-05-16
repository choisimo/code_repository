package com.example.weather.model;

import lombok.AllArgsConstructor;
import lombok.Data;

@Data
@AllArgsConstructor
public class GeocodeResponse {
    private double latitude;
    private double longitude;
    private String address;
}
