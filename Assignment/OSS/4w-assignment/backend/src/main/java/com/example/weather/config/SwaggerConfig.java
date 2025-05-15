// package com.example.weather.config;

// import org.springframework.beans.factory.annotation.Value;
// import org.springframework.context.annotation.Bean;
// import org.springframework.context.annotation.Configuration;

// import java.util.List;

// import io.swagger.v3.oas.models.OpenAPI;
// import io.swagger.v3.oas.models.info.Info;
// import io.swagger.v3.oas.models.info.License;

// @Configuration
// public class SwaggerConfig {

//     @Value("${server.servlet.context-path:}")
//     private String contextPath;

//     @Bean
//     public OpenAPI weatherOpenAPI() {
//         return new OpenAPI()
//                 .info(new Info()
//                         .title("날씨 API")
//                         .description("날씨 정보 및 예보 제공 API")
//                         .version("v1.0.0")
//                         .contact(new Contact()
//                                 .name("Nodove 개발팀")
//                                 .email("support@nodove.com")
//                                 .url("https://nodove.com")) 
//                         .license(new License()
//                                 .name("Apache 2.0")
//                                 .url("http://springdoc.org")))
//                 .servers(List.of(
//                         new Server().url(contextPath).description("현재 서버"),
//                         new Server().url("https://api.nodove.com").description("운영 서버")
//                 ));
//     }
// }