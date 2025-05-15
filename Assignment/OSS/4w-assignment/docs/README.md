# Weather Information Web App

A web application displaying weather data on a Naver Map interface.

## Tech Stack

### Frontend (Vue.js)
- Vue 3 + Vue CLI
- Vue Router
- Pinia (state management) or Vuex
- Axios for HTTP requests
- Naver Maps JavaScript API
- SCSS / Tailwind CSS
- Glassmorphism UI design

### Backend (Spring Boot)
- Java 17
- Spring Boot 3.x
- spring-boot-starter-web
- spring-boot-starter-validation
- spring-boot-starter-cache (Caffeine)
- Lombok
- Spring Data JPA
- Redis
- REST API
- SSE
- Jackson
- Gradle build system

## Project Structure
```
4w-assignment/
├── weather-project/        # Spring Boot backend
│   ├── build.gradle
│   ├── settings.gradle
│   └── src/main/java/com/example/weather  # controllers, services, models
│   └── src/main/resources/
│       ├── application.yml   # 환경 설정
│       └── static            # 정적 리소스
├── frontend/               # Vue.js frontend (to create)
│   ├── public/
│   └── src/
│       ├── assets/
│       ├── components/
│       ├── router/
│       ├── store/
│       └── main.js
├── .gitignore
└── README.md
```

## Prerequisites
- JDK 17+
- Node.js 14+ and npm/yarn
- Naver Cloud API credentials
- Public Data Portal weather API key

## Configuration

### Backend: `weather-project/src/main/resources/application.yml`
```yaml
server:
  port: 8080

spring:
  application:
    name: weather-app
  cache:
    type: caffeine

weather:
  api:
    base-url: https://api.data.go.kr
    key: ${WEATHER_API_KEY}

naver:
  map:
    client-id: ${NAVER_CLIENT_ID}
    client-secret: ${NAVER_CLIENT_SECRET}
```

### Frontend: `frontend/.env`
```bash
VITE_NAVER_CLIENT_ID=your_client_id
VITE_NAVER_CLIENT_SECRET=your_client_secret
VITE_API_BASE_URL=http://localhost:8080
```

## Run the Application

### Backend
```bash
cd weather-project
./gradlew bootRun
```

### Frontend
```bash
cd frontend
npm install
npm run serve
```

## 네이버 지도 API를 활용한 날씨 정보 웹앱 구현 가이드

이 가이드는 네이버 지도 API와 날씨 데이터를 활용하여 웹 애플리케이션을 구현하는 방법을 안내합니다.

## 프로젝트 설정 및 구성

### 네이버 지도 API 설정

1. **네이버클라우드 플랫폼 가입 및 설정**
   - 네이버클라우드에 가입하고 결제수단을 등록해야 합니다.
   - Maps 서비스: https://www.ncloud.com/product/applicationService/maps 접속

2. **애플리케이션 등록**
   - 콘솔에서 애플리케이션 추가 시 'Web Dynamic Map' 옵션을 체크
   - 개발 단계에서는 Web 서비스 URL에 'localhost:8080'을 등록
   - 발급된 클라이언트 ID를 안전하게 보관(유출 주의)

3. **HTML에 네이버 맵 스크립트 추가**
```html
var mapOptions = {
  center: new naver.maps.LatLng(37.3595704, 127.105399),
  zoom: 10
};
var map = new naver.maps.Map('map', mapOptions);
```

## 프로젝트 구조 설계

### 프론트엔드 구조 (Vue.js)

1. **컴포넌트 구조**
```bash
src/
├── components/
│   ├── Header.vue        // 메뉴 네비게이션
│   ├── WeatherMap.vue    // 지도 컴포넌트
│   ├── WeatherInfo.vue   // 날씨 정보 표시
│   ├── CurrentTemp.vue   // 현재 기온
│   ├── RainProbability.vue // 강수 확률
│   └── WindVolume.vue    // 풍량
├── services/
│   └── api.js            // 백엔드 API 모듈
├── store/
│   └── index.js          // Pinia/Vuex 상태 관리
└── App.vue               // 메인 컴포넌트
```

2. **지도 구현**
```javascript
// WeatherMap.vue
watch: {
  zoom(newZoom) {
    const scale = 156543.03392 * Math.cos(this.center.lat * Math.PI/180) / Math.pow(2, newZoom);
    const kmScale = scale * 1000;
    if (kmScale < 5000) {
      // 현재 위치 요약 팝업
    } else {
      // 행정구역별 색상 시각화
    }
  }
}
```

### 백엔드 구조 (Spring Boot)

```java
@RestController
@RequestMapping("/api/weather")
public class WeatherController {
    @GetMapping("/current-temp")
    public ResponseEntity<Double> getCurrentTemp(@RequestParam double lat, @RequestParam double lng) {
        return ResponseEntity.ok(weatherService.getCurrentTemp(lat, lng));
    }
    @GetMapping("/rain-probability")
    public ResponseEntity<Integer> getRainProbability(@RequestParam double lat, @RequestParam double lng) {
        return ResponseEntity.ok(weatherService.getRainProbability(lat, lng));
    }
    @GetMapping("/wind")
    public ResponseEntity<Double> getWindVolume(@RequestParam double lat, @RequestParam double lng) {
        return ResponseEntity.ok(weatherService.getWindVolume(lat, lng));
    }
}
```

## 실시간 상호작용 (SSE 및 WebSocket)

### Backend (Spring Boot)
- 의존성: spring-boot-starter-webflux, spring-boot-starter-websocket
- SSE 예시 컨트롤러:
```java
@GetMapping(value = "/stream/weather", produces = MediaType.TEXT_EVENT_STREAM_VALUE)
public Flux<WeatherUpdate> streamWeather() {
    return weatherService.streamWeatherUpdates();
}
```
- WebSocket 구성:
```java
@Configuration
@EnableWebSocket
public class WebSocketConfig implements WebSocketConfigurer {
    @Override
    public void registerWebSocketHandlers(WebSocketHandlerRegistry registry) {
        registry.addHandler(new WeatherWebSocketHandler(), "/ws/weather")
                .setAllowedOrigins("*");
    }
}
```

### Frontend (Vue.js)
- SSE:
```javascript
const es = new EventSource(`${import.meta.env.VITE_API_BASE_URL}/api/weather/stream`);
es.onmessage = ({ data }) => {
  const update = JSON.parse(data);
  // 실시간 UI 업데이트
};
```
- WebSocket:
```javascript
const ws = new WebSocket(`ws://${window.location.host}/ws/weather`);
ws.onmessage = (e) => {
  const update = JSON.parse(e.data);
  // 실시간 UI 업데이트
};
```

## 개발 단계별 가이드

### 1. 기술명세서 작성
- 서비스 구조도: 프론트·백 간 데이터 흐름 다이어그램
- 컴포넌트·API 명세: 요청/응답 형식 정의

### 2. 프론트엔드 개발
```bash
vue create weather-app
```
- 네이버 Maps JS API 통합 및 env 관리
- Pinia/Vuex 상태 관리

### 3. 백엔드 개발
```bash
spring init --dependencies=web,lombok weather-backend
```
- 공공데이터 포털 API 연동 (WeatherService.java)
- 캐싱: Caffeine/Redis, 재시도(backoff)

### 4. 연동 테스트
- 로컬 서버: `npm run serve`, `./gradlew bootRun`
- API 검사: Postman/Insomnia

## 구현 시 주의사항
1. 네이버 지도 API 키 보안 및 도메인 등록
2. 반응형 디자인 및 모바일 UX
3. 상태 관리: 메뉴 전환·지도 상태
4. 성능 최적화: 캐싱·불필요 호출 최소화

## 제출 준비
1. GitHub repo: 프론트·백 폴더 분리, README 업데이트
2. 화면 녹화: 30초 이내 주요 기능 시연
3. 기술명세서 검토: 구조·API 정확성
