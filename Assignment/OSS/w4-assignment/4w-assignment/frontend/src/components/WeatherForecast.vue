<template>
  <div class="weather-forecast">
    <div v-if="isLoading" class="loading-container">
      <div class="spinner"></div>
      <p>날씨 정보를 불러오는 중...</p>
    </div>
    
    <div v-else-if="error" class="error-container">
      <p class="error-message">{{ error }}</p>
      <button class="retry-button" @click="refreshData">다시 시도</button>
    </div>
    
    <div v-else-if="forecast.length" class="forecast-container">
      <h3 class="forecast-title">
        {{ selectedLocation.address }} 일기 예보
      </h3>
      
      <div class="forecast-cards">
        <div 
          v-for="(item, index) in forecast" 
          :key="index" 
          class="forecast-card"
          :class="{ 'current-day': index === 0 }"
        >
          <div class="forecast-day">{{ formatDay(item.date) }}</div>
          <div class="forecast-icon">
            <img :src="getWeatherIconUrl(item.skyCode)" :alt="item.skyText" />
          </div>
          <div class="forecast-temp">
            <span class="high">{{ item.temperatureMax }}°</span>
            <span class="low">{{ item.temperatureMin }}°</span>
          </div>
          <div class="forecast-desc">{{ item.skyText }}</div>
          <div v-if="item.precipitation > 0" class="forecast-rain">
            <span class="rain-icon">💧</span>
            <span class="rain-percent">{{ item.precipitation }}%</span>
          </div>
        </div>
      </div>
      
      <!-- 상세 날씨 정보 -->
      <div class="weather-details" v-if="currentWeather">
        <div class="detail-row">
          <div class="detail-item">
            <div class="detail-label">습도</div>
            <div class="detail-value">{{ currentWeather.humidity || 0 }}%</div>
          </div>
          <div class="detail-item">
            <div class="detail-label">기압</div>
            <div class="detail-value">{{ currentWeather.pressure || 0 }} hPa</div>
          </div>
        </div>
        <div class="detail-row">
          <div class="detail-item">
            <div class="detail-label">풍속</div>
            <div class="detail-value">{{ currentWeather.windSpeed || 0 }} m/s</div>
          </div>
          <div class="detail-item">
            <div class="detail-label">체감온도</div>
            <div class="detail-value">{{ currentWeather.temperatureFeelsLike || 0 }}°C</div>
          </div>
        </div>
      </div>
    </div>
    
    <div v-else class="empty-state">
      <p>날씨 정보가 없습니다.</p>
      <p>지도에서 위치를 선택하거나 주소를 검색해주세요.</p>
    </div>
  </div>
</template>

<script>
import { computed, onMounted } from 'vue';
import { useWeatherStore } from '@/stores/weather';

export default {
  name: 'WeatherForecast',
  
  props: {
    // 날씨 예보를 표시할 위치 (선택적)
    location: {
      type: Object,
      default: null
    }
  },
  
  setup(props) {
    const weatherStore = useWeatherStore();
    
    // 날씨 정보와 상태 가져오기
    const forecast = computed(() => weatherStore.forecast || []);
    const currentWeather = computed(() => weatherStore.currentWeather);
    const isLoading = computed(() => weatherStore.loading);
    const error = computed(() => weatherStore.error);
    const selectedLocation = computed(() => weatherStore.selectedLocation);
    
    // 현재 위치에 대한 날씨 데이터 가져오기
    const fetchData = async () => {
      // props.location이 있으면 해당 위치로, 없으면 선택된 위치 사용
      if (props.location) {
        await weatherStore.setLocation(
          props.location.lat, 
          props.location.lng, 
          props.location.address
        );
      } else {
        await weatherStore.fetchCurrentWeather();
        await weatherStore.fetchForecast();
      }
    };
    
    // 날씨 데이터 새로고침
    const refreshData = async () => {
      await weatherStore.refreshAll();
    };
    
    // 날짜 포맷팅 (요일 표시)
    const formatDay = (dateString) => {
      if (!dateString) return '';
      
      const days = ['일', '월', '화', '수', '목', '금', '토'];
      const date = new Date(dateString);
      
      // 오늘인지 확인
      const today = new Date();
      if (date.setHours(0, 0, 0, 0) === today.setHours(0, 0, 0, 0)) {
        return '오늘';
      }
      
      return days[date.getDay()] + '요일';
    };
    
    // 날씨 상태에 따른 아이콘 URL 반환
    const getWeatherIconUrl = (skyCode) => {
      if (!skyCode) return '/icons/weather/unknown.svg';
      
      // 날씨 코드에 따른 아이콘 매핑
      const iconMap = {
        'CLEAR': 'clear-day',
        'PARTLY_CLOUDY': 'partly-cloudy-day',
        'MOSTLY_CLOUDY': 'cloudy',
        'CLOUDY': 'cloudy',
        'RAIN': 'rain',
        'SNOW': 'snow',
        'SLEET': 'sleet',
        'THUNDERSTORM': 'thunderstorm'
      };
      
      const iconName = iconMap[skyCode] || 'unknown';
      return `/icons/weather/${iconName}.svg`;
    };
    
    // 컴포넌트 마운트 시 데이터 로드
    onMounted(() => {
      fetchData();
    });
    
    return {
      forecast,
      currentWeather,
      isLoading,
      error,
      selectedLocation,
      refreshData,
      formatDay,
      getWeatherIconUrl
    };
  }
};
</script>

<style lang="scss" scoped>
.weather-forecast {
  width: 100%;
  padding: 1rem;
  border-radius: 12px;
  background: rgba(255, 255, 255, 0.8);
  backdrop-filter: blur(10px);
  box-shadow: 0 4px 15px rgba(0, 0, 0, 0.1);
  margin-bottom: 1.5rem;
  
  .loading-container {
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    padding: 2rem;
    
    .spinner {
      width: 30px;
      height: 30px;
      border: 3px solid rgba(0, 0, 0, 0.1);
      border-top-color: #3498db;
      border-radius: 50%;
      animation: spin 1s linear infinite;
      margin-bottom: 1rem;
    }
    
    @keyframes spin {
      to { transform: rotate(360deg); }
    }
  }
  
  .error-container {
    text-align: center;
    padding: 1.5rem;
    
    .error-message {
      color: #e74c3c;
      margin-bottom: 1rem;
    }
    
    .retry-button {
      padding: 0.5rem 1rem;
      background: #3498db;
      color: white;
      border: none;
      border-radius: 4px;
      cursor: pointer;
      transition: background 0.2s;
      
      &:hover {
        background: #2980b9;
      }
    }
  }
  
  .forecast-container {
    .forecast-title {
      font-size: 1.2rem;
      font-weight: 600;
      margin-bottom: 1rem;
      color: #2c3e50;
      text-align: center;
    }
    
    .forecast-cards {
      display: flex;
      overflow-x: auto;
      gap: 10px;
      padding: 0.5rem 0;
      
      &::-webkit-scrollbar {
        height: 4px;
      }
      
      &::-webkit-scrollbar-track {
        background: rgba(0, 0, 0, 0.05);
        border-radius: 10px;
      }
      
      &::-webkit-scrollbar-thumb {
        background: rgba(0, 0, 0, 0.2);
        border-radius: 10px;
      }
    }
    
    .forecast-card {
      flex: 0 0 auto;
      width: 90px;
      padding: 0.8rem;
      border-radius: 10px;
      background: rgba(255, 255, 255, 0.7);
      text-align: center;
      transition: transform 0.2s, box-shadow 0.2s;
      
      &:hover {
        transform: translateY(-3px);
        box-shadow: 0 5px 10px rgba(0, 0, 0, 0.1);
      }
      
      &.current-day {
        background: rgba(52, 152, 219, 0.1);
        border: 1px solid rgba(52, 152, 219, 0.3);
      }
      
      .forecast-day {
        font-size: 0.8rem;
        font-weight: 600;
        margin-bottom: 0.5rem;
        color: #34495e;
      }
      
      .forecast-icon {
        margin: 0.5rem 0;
        
        img {
          width: 40px;
          height: 40px;
        }
      }
      
      .forecast-temp {
        margin: 0.5rem 0;
        
        .high {
          font-weight: 600;
          color: #e74c3c;
          margin-right: 0.5rem;
        }
        
        .low {
          color: #3498db;
        }
      }
      
      .forecast-desc {
        font-size: 0.75rem;
        color: #7f8c8d;
        margin-bottom: 0.5rem;
      }
      
      .forecast-rain {
        display: flex;
        align-items: center;
        justify-content: center;
        font-size: 0.75rem;
        color: #3498db;
        
        .rain-icon {
          margin-right: 0.2rem;
        }
      }
    }
  }
  
  .weather-details {
    margin-top: 1.5rem;
    padding-top: 1rem;
    border-top: 1px solid rgba(0, 0, 0, 0.1);
    
    .detail-row {
      display: flex;
      justify-content: space-between;
      margin-bottom: 0.8rem;
    }
    
    .detail-item {
      flex: 1;
      display: flex;
      flex-direction: column;
      align-items: center;
      
      .detail-label {
        font-size: 0.8rem;
        color: #7f8c8d;
        margin-bottom: 0.2rem;
      }
      
      .detail-value {
        font-size: 1rem;
        font-weight: 600;
        color: #2c3e50;
      }
    }
  }
  
  .empty-state {
    text-align: center;
    padding: 2rem;
    color: #7f8c8d;
  }
}

@media (max-width: 768px) {
  .weather-forecast {
    padding: 0.8rem;
    
    .forecast-container {
      .forecast-cards {
        .forecast-card {
          width: 80px;
          padding: 0.6rem;
        }
      }
    }
  }
}
</style>
