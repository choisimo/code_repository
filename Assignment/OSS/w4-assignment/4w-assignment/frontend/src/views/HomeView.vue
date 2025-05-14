<template>
  <div class="home">
    <div class="glass-card p-6 rounded-xl shadow-xl backdrop-blur-md bg-white/30 max-w-xl mx-auto">
      <h1 class="text-3xl font-bold mb-4 text-center text-gray-800">날씨 정보 서비스</h1>
      <p class="mb-6 text-gray-700">
        네이버 지도 API와 공공데이터 포털의 날씨 API를 활용하여 
        현재 위치 및 검색한 지역의 날씨 정보를 확인할 수 있습니다.
      </p>
      <div class="flex justify-center">
        <router-link 
          to="/map" 
          class="px-6 py-3 bg-gradient-to-r from-blue-500 to-indigo-600 text-white rounded-lg shadow-md hover:shadow-lg transition duration-300 ease-in-out transform hover:-translate-y-1">
          지도로 날씨 보기
        </router-link>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'HomeView'
}
</script>

<style scoped>
.home {
  padding: 2rem;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  min-height: 70vh;
}

.glass-card {
  background: rgba(255, 255, 255, 0.25);
  backdrop-filter: blur(10px);
  border: 1px solid rgba(255, 255, 255, 0.3);
}
</style>
