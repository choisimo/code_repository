"use client"

import apiService from "./api-service"
import mockApiService from "./mock-api-service"

// API 서비스 팩토리
const getApiService = () => {
  // 브라우저 환경인지 확인 (SSR 대응)
  if (typeof window === "undefined") {
    // 서버 사이드 렌더링 환경에서는 모의 API 반환
    return mockApiService
  }

  // 로컬스토리지에서 모의 API 사용 여부 확인
  const useMock = localStorage.getItem("useMockApi") === "true"

  // 모의 API 또는 실제 API 서비스 반환
  return useMock ? mockApiService : apiService
}

export default getApiService
