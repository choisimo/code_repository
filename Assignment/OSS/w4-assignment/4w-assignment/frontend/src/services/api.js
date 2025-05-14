import axios from 'axios';

// 기본 API 클라이언트 인스턴스 생성
const apiClient = axios.create({
  baseURL: process.env.VUE_APP_API_BASE_URL || '/api',
  timeout: 10000,
  headers: {
    'Content-Type': 'application/json',
    'Accept': 'application/json'
  }
});

// 날씨 관련 API 호출
export const weatherApi = {
  /**
   * 특정 좌표의 현재 날씨 정보를 가져옵니다.
   * @param {number} lat - 위도
   * @param {number} lng - 경도
   * @returns {Promise} 날씨 정보 객체
   */
  getCurrentWeather(lat, lng) {
    return apiClient.get('/weather', {
      params: { lat, lng }
    });
  },

  /**
   * 특정 좌표의 날씨 예보를 가져옵니다.
   * @param {number} lat - 위도
   * @param {number} lng - 경도
   * @param {number} days - 예보 일수 (기본값: 5)
   * @returns {Promise} 예보 정보 객체
   */
  getForecast(lat, lng, days = 5) {
    return apiClient.get('/forecast', {
      params: { lat, lng, days }
    });
  },

  /**
   * 주소를 좌표로 변환합니다 (지오코딩).
   * @param {string} address - 변환할 주소
   * @returns {Promise} 좌표 정보 객체
   */
  getCoordinates(address) {
    return apiClient.get('/geocode', {
      params: { address }
    });
  }
};

export default apiClient;
