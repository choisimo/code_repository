<template>
  <div class="map-container">
    <ErrorAlert 
      :message="errorMessage" 
      :show="showError" 
      @close="showError = false" 
    />
    
    <div class="glass-card p-4 mb-4 rounded-lg">
      <h2 class="text-2xl font-bold mb-2 text-gray-800">지도 날씨</h2>
      <p class="text-gray-700 mb-4">지도에서 원하는 위치를 클릭하여 해당 지역의 날씨 정보를 확인하세요.</p>
      
      <div class="search-box mb-4">
        <input 
          type="text" 
          v-model="searchQuery" 
          @keyup.enter="searchLocation"
          placeholder="지역 검색" 
          class="w-full p-2 border border-gray-300 rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500"
        />
        <button 
          @click="searchLocation" 
          class="mt-2 px-4 py-2 bg-blue-500 text-white rounded-md hover:bg-blue-600 transition-colors"
        >
          검색
        </button>
      </div>
    </div>
    
    <WeatherMap 
      :initialLat="initialLocation.lat" 
      :initialLng="initialLocation.lng" 
      :zoom="zoom"
      :naverMapsClientId="naverMapsClientId"
    />
    
    <WeatherForecast v-if="selectedLocation" :location="selectedLocation" class="mt-4" />
  </div>
</template>

<script>
import { ref, computed } from 'vue';
import { useWeatherStore } from '@/stores/weather';
import { useApiKeyStore } from '@/stores/apiKey';
import WeatherMap from '@/components/WeatherMap.vue';
import WeatherForecast from '@/components/WeatherForecast.vue';
import ErrorAlert from '@/components/ErrorAlert.vue';

export default {
  name: 'MapView',
  components: {
    WeatherMap,
    WeatherForecast,
    ErrorAlert
  },
  setup() {
    const weatherStore = useWeatherStore();
    const apiKeyStore = useApiKeyStore();
    const searchQuery = ref('');
    const initialLocation = ref({
      lat: 37.5665, // 서울의 위도
      lng: 126.9780 // 서울의 경도
    });
    const zoom = ref(10);
    const naverMapsClientId = computed(() => apiKeyStore.naverMapsKey);
    
    const selectedLocation = ref(weatherStore.selectedLocation);
    const errorMessage = ref('');
    const showError = ref(false);
    
    // 위치 검색
    const searchLocation = async () => {
      if (!searchQuery.value.trim()) return;
      
      try {
        await weatherStore.searchByAddress(searchQuery.value);
        
        // 검색 성공 시 초기 위치 업데이트
        if (weatherStore.selectedLocation) {
          initialLocation.value = {
            lat: weatherStore.selectedLocation.lat,
            lng: weatherStore.selectedLocation.lng
          };
          selectedLocation.value = weatherStore.selectedLocation;
          zoom.value = 13; // 검색 시 줌 레벨 조정
        }
      } catch (err) {
        console.error('위치 검색 오류:', err);
        errorMessage.value = '위치 검색 중 오류가 발생했습니다: ' + (err.message || '알 수 없는 오류');
        showError.value = true;
      }
    };
    
    return {
      searchQuery,
      initialLocation,
      zoom,
      selectedLocation,
      searchLocation,
      naverMapsClientId,
      errorMessage,
      showError
    };
  }
};
</script>

<style scoped>
.map-container {
  padding: 1rem;
}

.glass-card {
  background: rgba(255, 255, 255, 0.25);
  backdrop-filter: blur(10px);
  border: 1px solid rgba(255, 255, 255, 0.3);
}

.search-box {
  display: flex;
  flex-direction: column;
}

@media (min-width: 640px) {
  .search-box {
    flex-direction: row;
    gap: 8px;
  }
  
  .search-box button {
    margin-top: 0;
  }
}
</style>