<template>
    <div v-if="show" class="error-alert" :class="{ 'error-alert-show': show }">
      <div class="error-content">
        <div class="error-icon">⚠️</div>
        <div class="error-message">{{ message }}</div>
      </div>
      <button class="close-button" @click="close">×</button>
    </div>
  </template>
  
  <script>
  import { ref, watch } from 'vue';
  
  export default {
    name: 'ErrorAlert',
    props: {
      message: {
        type: String,
        required: true
      },
      duration: {
        type: Number,
        default: 5000 // 5초 후 자동으로 닫힘
      },
      show: {
        type: Boolean,
        default: false
      }
    },
    emits: ['close'],
    setup(props, { emit }) {
      const timer = ref(null);
      
      const close = () => {
        emit('close');
      };
      
      watch(() => props.show, (newValue) => {
        if (newValue) {
          // 이전 타이머가 있으면 제거
          if (timer.value) {
            clearTimeout(timer.value);
          }
          
          // 새 타이머 설정
          timer.value = setTimeout(() => {
            close();
          }, props.duration);
        }
      });
      
      return {
        close
      };
    }
  };
  </script>
  
  <style scoped>
  .error-alert {
    position: fixed;
    top: 20px;
    right: 20px;
    background-color: #f8d7da;
    color: #721c24;
    border: 1px solid #f5c6cb;
    border-radius: 4px;
    padding: 12px 16px;
    display: flex;
    align-items: center;
    justify-content: space-between;
    box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
    z-index: 1000;
    max-width: 400px;
    transform: translateX(120%);
    transition: transform 0.3s ease-in-out;
  }
  
  .error-alert-show {
    transform: translateX(0);
  }
  
  .error-content {
    display: flex;
    align-items: center;
  }
  
  .error-icon {
    margin-right: 12px;
    font-size: 20px;
  }
  
  .error-message {
    font-size: 14px;
  }
  
  .close-button {
    background: none;
    border: none;
    font-size: 20px;
    cursor: pointer;
    margin-left: 16px;
    color: #721c24;
  }
  </style>