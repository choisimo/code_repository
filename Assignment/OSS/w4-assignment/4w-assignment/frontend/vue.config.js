const { defineConfig } = require('@vue/cli-service')
const { defineConfig } = require('@vue/cli-service')

module.exports = defineConfig({
  transpileDependencies: true,
  
  // 개발 서버 설정
  devServer: {
    port: 3000,
    proxy: {
      '/api': {
        target: 'http://localhost:8080',
        changeOrigin: true
      }
    }
  },
  
  // 프로덕션 빌드 설정
  outputDir: 'dist',
  assetsDir: 'assets',
  
  // CSS 설정
  css: {
    loaderOptions: {
      scss: {
        additionalData: `@import "@/assets/styles/variables.scss";`
      }
    }
  }
})
module.exports = defineConfig({
  transpileDependencies: true,
  devServer: {
    port: 8081,
    proxy: {
      '^/api': {
        target: 'http://localhost:8080',
        changeOrigin: true
      }
    }
  }
})
