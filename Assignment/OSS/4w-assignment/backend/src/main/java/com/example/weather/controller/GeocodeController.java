package com.example.weather.controller;

import com.example.weather.model.GeocodeResponse;
import com.example.weather.client.NaverGeocodeClient;
import lombok.RequiredArgsConstructor;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestHeader;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/api")
@RequiredArgsConstructor
public class GeocodeController {

    private final NaverGeocodeClient naverGeocodeClient;

    /**
     * 주소로부터 좌표(위도, 경도)와 도로명 주소를 반환합니다.
     */
    @GetMapping("/geocode")
    public ResponseEntity<GeocodeResponse> geocode(
            @RequestParam String address,
            @RequestHeader("X-NAVER-MAPS-API-KEY") String apiKey) {
        GeocodeResponse result = naverGeocodeClient.geocode(address, apiKey);
        return ResponseEntity.ok(result);
    }
}
