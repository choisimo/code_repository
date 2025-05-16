package com.example.weather.client;

import com.example.weather.model.GeocodeResponse;
import com.fasterxml.jackson.databind.JsonNode;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpMethod;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Component;
import org.springframework.web.client.RestTemplate;
import org.springframework.web.server.ResponseStatusException;
import org.springframework.http.HttpStatus;
import org.springframework.web.util.UriComponentsBuilder;

@Component
@RequiredArgsConstructor
public class NaverGeocodeClient {

    private final RestTemplate restTemplate;

    @Value("${naver.maps.geocode-url}")
    private String geocodeUrl;

    /**
     * 주소로부터 좌표를 얻어옵니다.
     */
    public GeocodeResponse geocode(String address, String apiKey) {
        try {
            HttpHeaders headers = new HttpHeaders();
            headers.set("X-NAVER-MAPS-API-KEY", apiKey);
            HttpEntity<?> entity = new HttpEntity<>(headers);

            String uri = UriComponentsBuilder
                    .fromHttpUrl(geocodeUrl)
                    .queryParam("query", address)
                    .toUriString();

            ResponseEntity<JsonNode> resp = restTemplate.exchange(
                uri, HttpMethod.GET, entity, JsonNode.class);
            JsonNode body = resp.getBody();
            JsonNode addrs = body.path("addresses");
            if (addrs.isArray() && addrs.size() > 0) {
                JsonNode first = addrs.get(0);
                double lat = first.path("y").asDouble();
                double lng = first.path("x").asDouble();
                String road = first.path("roadAddress").asText();
                return new GeocodeResponse(lat, lng, road);
            }
            throw new ResponseStatusException(HttpStatus.BAD_REQUEST, "No geocode result");
        } catch (ResponseStatusException e) {
            throw e;
        } catch (Exception e) {
            throw new ResponseStatusException(
                HttpStatus.INTERNAL_SERVER_ERROR,
                "Geocoding failed: " + e.getMessage(), e);
        }
    }
}
