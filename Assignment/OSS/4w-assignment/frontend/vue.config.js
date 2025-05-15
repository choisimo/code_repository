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
        additionalData: (content, loaderContext) => {
          if (loaderContext.resourcePath.endsWith('variables.scss')) {
            return content
          }
          return `@import "@/assets/styles/variables.scss";\n${content}`
        }
      }
    }
  }
})
