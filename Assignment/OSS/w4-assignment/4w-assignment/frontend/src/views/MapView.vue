<template>
  <div class="map-container">
    <div class="glass-card p-4 mb-4 rounded-lg">
      <h2 class="text-2xl font-bold mb-2 text-gray-800">지도 날씨</h2>
      <p class="text-gray-700 mb-4">지도에서 원하는 위치를 클릭하여 해당 지역의 날씨 정보를 확인하세요.</p>
      
      <div class="search-box mb-4">
        <input 
          type="text" 
          v-model="searchQuery" 
          placeholder="지역 검색" 
          class="w-full p-2 border border-gray-300 rounded-md focus:outline-none focus:ring-2 focus:ring-blue-500"
        />
      </div>
    </div>
    
    <div class="map-wrapper h-96 rounded-lg overflow-hidden shadow-lg">
      <div id="map-container" class="w-full h-full"></div>
    </div>
    
    <div v-if="weatherData" class="weather-info glass-card p-4 mt-4 rounded-lg">
      <h3 class="text-xl font-semibold mb-2">{{ weatherData.location }}</h3>
      <div class="flex flex-wrap">
        <div class="w-1/2 md:w-1/4 p-2">
          <div class="text-gray-600">온도</div>
          <div class="text-xl">{{ weatherData.temperature }}°C</div>
        </div>
        <div class="w-1/2 md:w-1/4 p-2">
          <div class="text-gray-600">습도</div>
          <div class="text-xl">{{ weatherData.humidity }}%</div>
        </div>
        <div class="w-1/2 md:w-1/4 p-2">
          <div class="text-gray-600">날씨</div>
          <div class="text-xl">{{ weatherData.condition }}</div>
        </div>
        <div class="w-1/2 md:w-1/4 p-2">
          <div class="text-gray-600">바람</div>
          <div class="text-xl">{{ weatherData.wind }} m/s</div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'MapView',
  data() {
    return {
      searchQuery: '',
      map: null,
      weatherData: null
    }
  },
  mounted() {
    // 네이버 맵 스크립트가 로드되면 지도 초기화
    this.loadNaverMapScript()
  },
  methods: {
    loadNaverMapScript() {
      // 실제 구현에서는 네이버 맵 API를 로드하여 지도를 표시합니다
      // 이 예시에서는 더미 데이터로 대체합니다
      setTimeout(() => {
        this.initializeMap()
      }, 500)
    },
    initializeMap() {
      // 실제 구현에서는 네이버 맵을 초기화합니다
      // 이 예시에서는 날씨 데이터만 표시합니다
      this.weatherData = {
        location: '서울특별시 강남구',
        temperature: 22,
        humidity: 65,
        condition: '맑음',
        wind: 3.2
      }
    }
  }
}
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
</style>
