<template>
  <div class="app-container">
    <header class="bg-gradient-to-r from-blue-500 to-indigo-600 text-white p-4 shadow-lg">
      <div class="container mx-auto flex justify-between items-center">
        <h1 class="text-2xl font-bold">Nodove 날씨</h1>
        <nav>
          <router-link to="/" class="px-4 py-2 mr-2 hover:bg-blue-600 rounded-md transition-colors">홈</router-link>
          <router-link to="/map" class="px-4 py-2 hover:bg-blue-600 rounded-md transition-colors">지도</router-link>
        </nav>
      </div>
    </header>
    
    <ApiKeyManager />
    <main class="container mx-auto p-4 min-h-screen">
      <router-view />
    </main>
    
    <footer class="bg-gray-800 text-white p-4 mt-8">
      <div class="container mx-auto text-center">
        <p> 2023 Nodove 날씨 애플리케이션</p>
      </div>
    </footer>
  </div>
</template>

<script>
import ApiKeyManager from '@/components/ApiKeyManager.vue';

export default {
  name: 'App',
  components: { ApiKeyManager }
}
</script>

<style lang="scss">
.app-container {
  display: flex;
  flex-direction: column;
  min-height: 100vh;
}

main {
  flex: 1;
}
</style>
