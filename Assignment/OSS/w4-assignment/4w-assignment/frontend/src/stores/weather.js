import { defineStore } from 'pinia';
import { weatherApi } from '@/services/api';

export const useWeatherStore = defineStore('weather', {
  state: () => ({
    currentWeather: null,
    forecast: [],
    loading: false,
    error: null,
    selectedLocation: {
      lat: 37.5665, // 서울 기본 좌표
      lng: 126.9780,
      address: '서울특별시'
    },
    locations: [], // 최근 조회한 위치 목록
  }),

  getters: {
    // 날씨 상태에 따른 아이콘 코드 반환
    getWeatherIcon: (state) => {
      if (!state.currentWeather) return 'unknown';
      
      const weatherState = state.currentWeather.sky?.code || 'CLEAR';
      const isNight = state.currentWeather.isNight;
      
      switch (weatherState) {
        case 'CLEAR':
          return isNight ? 'clear-night' : 'clear-day';
        case 'PARTLY_CLOUDY':
          return isNight ? 'partly-cloudy-night' : 'partly-cloudy-day';
        case 'MOSTLY_CLOUDY':
        case 'CLOUDY':
          return 'cloudy';
        case 'RAIN':
          return 'rain';
        case 'SNOW':
          return 'snow';
        case 'SLEET':
          return 'sleet';
        case 'THUNDERSTORM':
          return 'thunderstorm';
        default:
          return 'unknown';
      }
    },
    
    // 온도에 따른 색상 반환
    getTemperatureColor: (state) => {
      if (!state.currentWeather) return '#888888';
      
      const temp = state.currentWeather.temperature?.current;
      if (temp === undefined) return '#888888';
      
      if (temp <= 0) return '#4287f5';  // 매우 추움 (파란색)
      if (temp <= 10) return '#42adf5'; // 추움 (하늘색)
      if (temp <= 20) return '#42f59e'; // 시원함 (연두색)
      if (temp <= 25) return '#f5d442'; // 따뜻함 (노란색)
      if (temp <= 30) return '#f59442'; // 더움 (주황색)
      return '#f54242';                // 매우 더움 (빨간색)
    }
  },

  actions: {
    /**
     * 현재 선택된 위치의 날씨 정보를 가져옵니다.
     */
    async fetchCurrentWeather() {
      if (!this.selectedLocation) return;
      
      const { lat, lng } = this.selectedLocation;
      this.loading = true;
      this.error = null;
      
      try {
        const response = await weatherApi.getCurrentWeather(lat, lng);
        this.currentWeather = response.data;
        
        // 최근 위치 목록에 추가
        this.addToRecentLocations({
          ...this.selectedLocation,
          weather: this.currentWeather
        });
      } catch (err) {
        this.error = err.message || '날씨 정보를 가져오는데 실패했습니다.';
        console.error('날씨 정보 조회 오류:', err);
      } finally {
        this.loading = false;
      }
    },
    
    /**
     * 현재 선택된 위치의 날씨 예보를 가져옵니다.
     * @param {number} days - 예보 일수
     */
    async fetchForecast(days = 5) {
      if (!this.selectedLocation) return;
      
      const { lat, lng } = this.selectedLocation;
      this.loading = true;
      this.error = null;
      
      try {
        const response = await weatherApi.getForecast(lat, lng, days);
        this.forecast = response.data.forecastList || [];
      } catch (err) {
        this.error = err.message || '날씨 예보를 가져오는데 실패했습니다.';
        console.error('날씨 예보 조회 오류:', err);
      } finally {
        this.loading = false;
      }
    },
    
    /**
     * 주소를 검색하여 좌표로 변환하고 해당 위치의 날씨를 가져옵니다.
     * @param {string} address - 검색할 주소
     */
    async searchByAddress(address) {
      if (!address) return;
      
      this.loading = true;
      this.error = null;
      
      try {
        const response = await weatherApi.getCoordinates(address);
        const { lat, lng, formattedAddress } = response.data;
        
        this.selectedLocation = {
          lat,
          lng,
          address: formattedAddress
        };
        
        // 날씨 정보 및 예보 가져오기
        await this.fetchCurrentWeather();
        await this.fetchForecast();
      } catch (err) {
        this.error = err.message || '주소 검색에 실패했습니다.';
        console.error('주소 검색 오류:', err);
      } finally {
        this.loading = false;
      }
    },
    
    /**
     * 좌표로 날씨 정보를 가져옵니다.
     * @param {number} lat - 위도
     * @param {number} lng - 경도
     * @param {string} address - 주소 (선택사항)
     */
    async setLocation(lat, lng, address = '선택한 위치') {
      this.selectedLocation = { lat, lng, address };
      await this.fetchCurrentWeather();
      await this.fetchForecast();
    },
    
    /**
     * 최근 위치 목록에 위치를 추가합니다.
     * @param {Object} location - 추가할 위치 정보
     */
    addToRecentLocations(location) {
      // 중복 위치 체크
      const exists = this.locations.some(loc => 
        loc.lat === location.lat && loc.lng === location.lng
      );
      
      if (!exists) {
        // 최대 5개까지만 저장
        if (this.locations.length >= 5) {
          this.locations.pop();
        }
        
        this.locations.unshift(location);
      }
    },
    
    /**
     * 모든 날씨 정보를 새로고침합니다.
     */
    async refreshAll() {
      await this.fetchCurrentWeather();
      await this.fetchForecast();
    }
  }
});
