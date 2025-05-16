import { defineStore } from 'pinia';

export const useApiKeyStore = defineStore('apiKey', {
  state: () => ({
    naverMapsKey: localStorage.getItem('naverMapsKey') || '',
    weatherApiKey: localStorage.getItem('weatherApiKey') || ''
  }),
  actions: {
    setNaverMapsKey(key) {
      this.naverMapsKey = key;
      localStorage.setItem('naverMapsKey', key);
    },
    setWeatherApiKey(key) {
      this.weatherApiKey = key;
      localStorage.setItem('weatherApiKey', key);
    }
  }
});
