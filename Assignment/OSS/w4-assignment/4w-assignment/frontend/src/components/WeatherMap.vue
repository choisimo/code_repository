<template>
  <div class="weather-map-container">
    <div v-if="loading" class="loading-overlay">
      <div class="loading-spinner"></div>
      <p>날씨 데이터 로딩 중...</p>
    </div>
    <div v-if="error" class="error-message">
      <p>{{ error }}</p>
      <button @click="fetchWeatherData" class="retry-button">다시 시도</button>
    </div>
    <div id="weather-map" ref="mapContainer" class="map-container"></div>
    <div class="map-controls">
      <button @click="zoomIn" class="map-control-button">+</button>
      <button @click="zoomOut" class="map-control-button">-</button>
      <button @click="resetLocation" class="map-control-button">
        <span class="reset-icon">⟳</span>
      </button>
    </div>
    <div v-if="selectedLocation" class="location-info">
      <h3>{{ selectedLocation.name }}</h3>
      <div class="weather-details">
        <div class="weather-icon">
          <img :src="getWeatherIconUrl(selectedLocation.weather.icon)" :alt="selectedLocation.weather.description" />
        </div>
        <div class="weather-data">
          <p class="temperature">{{ Math.round(selectedLocation.weather.temp) }}°C</p>
          <p class="description">{{ selectedLocation.weather.description }}</p>
          <p class="humidity">습도: {{ selectedLocation.weather.humidity }}%</p>
          <p class="wind">풍속: {{ selectedLocation.weather.windSpeed }} m/s</p>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import { ref, onMounted, onUnmounted } from 'vue';
import axios from 'axios';

export default {
  name: 'WeatherMap',
  props: {
    apiKey: {
      type: String,
      required: true
    },
    initialLat: {
      type: Number,
      default: 37.5665 // 서울의 위도
    },
    initialLng: {
      type: Number,
      default: 126.9780 // 서울의 경도
    },
    zoom: {
      type: Number,
      default: 10
    }
  },
  setup(props) {
    const mapContainer = ref(null);
    const map = ref(null);
    const markers = ref([]);
    const loading = ref(true);
    const error = ref(null);
    const selectedLocation = ref(null);
    const weatherData = ref([]);

    // 지도 초기화
    const initMap = () => {
      if (!window.google) {
        error.value = "Google Maps API가 로드되지 않았습니다.";
        loading.value = false;
        return;
      }

      map.value = new window.google.maps.Map(mapContainer.value, {
        center: { lat: props.initialLat, lng: props.initialLng },
        zoom: props.zoom,
        mapTypeId: window.google.maps.MapTypeId.ROADMAP,
        zoomControl: false,
        streetViewControl: false,
        fullscreenControl: false
      });

      // 지도 로딩 완료 이벤트
      window.google.maps.event.addListenerOnce(map.value, 'idle', () => {
        fetchWeatherData();
      });

      // 지도 클릭 이벤트
      map.value.addListener('click', () => {
        selectedLocation.value = null;
      });
    };

    // 날씨 데이터 가져오기
    const fetchWeatherData = async () => {
      loading.value = true;
      error.value = null;
      
      try {
        // 현재 지도 경계 가져오기
        const bounds = map.value.getBounds();
        const ne = bounds.getNorthEast();
        const sw = bounds.getSouthWest();
        
        // API 호출
        const response = await axios.get('/api/weather', {
          params: {
            north: ne.lat(),
            east: ne.lng(),
            south: sw.lat(),
            west: sw.lng()
          }
        });
        
        weatherData.value = response.data;
        clearMarkers();
        addWeatherMarkers();
      } catch (err) {
        console.error('날씨 데이터를 가져오는 중 오류가 발생했습니다:', err);
        error.value = '날씨 데이터를 가져오는 중 오류가 발생했습니다. 다시 시도해 주세요.';
      } finally {
        loading.value = false;
      }
    };

    // 마커 초기화
    const clearMarkers = () => {
      markers.value.forEach(marker => marker.setMap(null));
      markers.value = [];
    };

    // 날씨 마커 추가
    const addWeatherMarkers = () => {
      weatherData.value.forEach(location => {
        const marker = new window.google.maps.Marker({
          position: { lat: location.lat, lng: location.lng },
          map: map.value,
          title: location.name,
          icon: {
            url: getWeatherIconUrl(location.weather.icon),
            scaledSize: new window.google.maps.Size(50, 50)
          }
        });

        marker.addListener('click', () => {
          selectedLocation.value = location;
        });

        markers.value.push(marker);
      });
    };

    // 날씨 아이콘 URL 가져오기
    const getWeatherIconUrl = (iconCode) => {
      return `https://openweathermap.org/img/wn/${iconCode}@2x.png`;
    };

    // 확대
    const zoomIn = () => {
      if (map.value) {
        map.value.setZoom(map.value.getZoom() + 1);
      }
    };

    // 축소
    const zoomOut = () => {
      if (map.value) {
        map.value.setZoom(map.value.getZoom() - 1);
      }
    };

    // 위치 재설정
    const resetLocation = () => {
      if (map.value) {
        map.value.setCenter({ lat: props.initialLat, lng: props.initialLng });
        map.value.setZoom(props.zoom);
      }
    };

    // 컴포넌트 마운트 시 Google Maps API 로드
    onMounted(() => {
      if (window.google && window.google.maps) {
        initMap();
      } else {
        const script = document.createElement('script');
        script.src = `https://maps.googleapis.com/maps/api/js?key=${props.apiKey}&callback=initWeatherMap`;
        script.async = true;
        script.defer = true;
        
        window.initWeatherMap = () => {
          initMap();
        };
        
        document.head.appendChild(script);
      }
    });

    // 컴포넌트 언마운트 시 정리
    onUnmounted(() => {
      if (window.google && window.google.maps) {
        clearMarkers();
      }
      delete window.initWeatherMap;
    });

    return {
      mapContainer,
      loading,
      error,
      selectedLocation,
      fetchWeatherData,
      getWeatherIconUrl,
      zoomIn,
      zoomOut,
      resetLocation
    };
  }
};
</script>

<style scoped>
.weather-map-container {
  position: relative;
  width: 100%;
  height: 500px;
  border-radius: 8px;
  overflow: hidden;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
}

.map-container {
  width: 100%;
  height: 100%;
}

.loading-overlay {
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background-color: rgba(255, 255, 255, 0.8);
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
  z-index: 10;
}

.loading-spinner {
  width: 50px;
  height: 50px;
  border: 5px solid #f3f3f3;
  border-top: 5px solid #3498db;
  border-radius: 50%;
  animation: spin 1s linear infinite;
  margin-bottom: 10px;
}

@keyframes spin {
  0% { transform: rotate(0deg); }
  100% { transform: rotate(360deg); }
}

.error-message {
  position: absolute;
  top: 50%;
  left: 50%;
  transform: translate(-50%, -50%);
  background-color: white;
  padding: 20px;
  border-radius: 8px;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
  text-align: center;
  z-index: 10;
}

.retry-button {
  background-color: #3498db;
  color: white;
  border: none;
  padding: 8px 16px;
  border-radius: 4px;
  cursor: pointer;
  margin-top: 10px;
}

.retry-button:hover {
  background-color: #2980b9;
}

.map-controls {
  position: absolute;
  top: 10px;
  right: 10px;
  display: flex;
  flex-direction: column;
  gap: 5px;
  z-index: 5;
}

.map-control-button {
  width: 36px;
  height: 36px;
  background-color: white;
  border: 1px solid #ccc;
  border-radius: 4px;
  display: flex;
  justify-content: center;
  align-items: center;
  font-size: 18px;
  cursor: pointer;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
}

.map-control-button:hover {
  background-color: #f5f5f5;
}

.reset-icon {
  font-size: 16px;
}

.location-info {
  position: absolute;
  bottom: 20px;
  left: 20px;
  background-color: white;
  border-radius: 8px;
  padding: 15px;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
  max-width: 280px;
  z-index: 5;
}

.location-info h3 {
  margin-top: 0;
  margin-bottom: 10px;
  font-size: 18px;
  font-weight: 600;
}

.weather-details {
  display: flex;
  align-items: center;
}

.weather-icon {
  flex-shrink: 0;
  margin-right: 10px;
}

.weather-icon img {
  width: 60px;
  height: 60px;
}

.weather-data {
  flex-grow: 1;
}

.temperature {
  font-size: 24px;
  font-weight: 700;
  margin: 0;
}

.description {
  text-transform: capitalize;
  margin: 5px 0;
}

.humidity, .wind {
  font-size: 14px;
  margin: 3px 0;
  color: #555;
}
</style>
