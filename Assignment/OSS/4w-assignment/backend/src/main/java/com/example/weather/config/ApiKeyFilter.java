package com.example.weather.config;

import org.springframework.stereotype.Component;
import jakarta.servlet.Filter;
import jakarta.servlet.ServletException;
import jakarta.servlet.ServletRequest;
import jakarta.servlet.ServletResponse;
import jakarta.servlet.FilterChain;
import jakarta.servlet.http.HttpServletRequest;
import java.io.IOException;

@Component
public class ApiKeyFilter implements Filter {

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) throws IOException, ServletException {
        HttpServletRequest httpReq = (HttpServletRequest) request;
        String apiKey = httpReq.getHeader("X-WEATHER-API-KEY");
        
        // API 키가 없는 경우에도 진행할 수 있도록 수정
        if (apiKey != null && !apiKey.isEmpty()) {
            httpReq.setAttribute("weatherApiKey", apiKey);
        }
        
        chain.doFilter(request, response);
    }
}
