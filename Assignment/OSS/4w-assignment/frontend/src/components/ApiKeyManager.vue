<template>
  <div class="api-key-manager p-4 bg-white shadow-md mb-4">
    <h2 class="text-xl font-semibold mb-2">API Key 설정</h2>
    <div class="flex flex-col sm:flex-row gap-4">
      <div>
        <label class="block mb-1">네이버 지도 API Key</label>
        <input
          v-model="naverMapsKey"
          type="text"
          class="border rounded p-2 w-full"
          placeholder="Naver Maps API Key"
        />
      </div>
      <div>
        <label class="block mb-1">날씨 API Key</label>
        <input
          v-model="weatherApiKey"
          type="text"
          class="border rounded p-2 w-full"
          placeholder="Weather API Key"
        />
      </div>
    </div>
  </div>
</template>

<script setup>
import { computed } from 'vue';
import { useApiKeyStore } from '@/stores/apiKey';

const keyStore = useApiKeyStore();

const naverMapsKey = computed({
  get: () => keyStore.naverMapsKey,
  set: (value) => keyStore.setNaverMapsKey(value)
});

const weatherApiKey = computed({
  get: () => keyStore.weatherApiKey,
  set: (value) => keyStore.setWeatherApiKey(value)
});
</script>

<style scoped>
.api-key-manager input {
  outline: none;
}
</style>
