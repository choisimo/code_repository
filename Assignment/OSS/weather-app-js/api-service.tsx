"use client"

// API 요청을 처리하는 서비스 클래스
class ApiService {
  // 기본 API URL
  private baseUrl = "/api"

  // 날씨 데이터 가져오기
  async getWeather(lat: number, lng: number): Promise<any> {
    try {
      // 로컬스토리지에서 API 키 가져오기
      const apiKey = localStorage.getItem("weatherApiKey")
      if (!apiKey) {
        throw new Error("날씨 API 키가 설정되지 않았습니다.")
      }

      // API 요청
      const response = await fetch(`${this.baseUrl}/weather?lat=${lat}&lng=${lng}&apiKey=${apiKey}`)

      // 응답 확인
      if (!response.ok) {
        if (response.headers.get("content-type")?.includes("text/html")) {
          throw new Error("서버에서 HTML 응답을 반환했습니다. API 엔드포인트를 확인해주세요.")
        }
        const errorData = await response.json().catch(() => null)
        throw new Error(errorData?.message || `API 응답 오류: ${response.status}`)
      }

      // 응답 타입 확인
      const contentType = response.headers.get("content-type")
      if (!contentType || !contentType.includes("application/json")) {
        throw new Error(`예상치 못한 응답 타입: ${contentType}`)
      }

      return await response.json()
    } catch (error) {
      console.error("날씨 데이터 가져오기 오류:", error)
      throw error
    }
  }

  // 날씨 예보 가져오기
  async getForecast(lat: number, lng: number, days = 5): Promise<any> {
    try {
      // 로컬스토리지에서 API 키 가져오기
      const apiKey = localStorage.getItem("weatherApiKey")
      if (!apiKey) {
        throw new Error("날씨 API 키가 설정되지 않았습니다.")
      }

      // API 요청
      const response = await fetch(`${this.baseUrl}/forecast?lat=${lat}&lng=${lng}&days=${days}&apiKey=${apiKey}`)

      // 응답 확인
      if (!response.ok) {
        if (response.headers.get("content-type")?.includes("text/html")) {
          throw new Error("서버에서 HTML 응답을 반환했습니다. API 엔드포인트를 확인해주세요.")
        }
        const errorData = await response.json().catch(() => null)
        throw new Error(errorData?.message || `API 응답 오류: ${response.status}`)
      }

      // 응답 타입 확인
      const contentType = response.headers.get("content-type")
      if (!contentType || !contentType.includes("application/json")) {
        throw new Error(`예상치 못한 응답 타입: ${contentType}`)
      }

      return await response.json()
    } catch (error) {
      console.error("날씨 예보 가져오기 오류:", error)
      throw error
    }
  }

  // 주소 검색 (지오코딩)
  async searchAddress(query: string): Promise<any> {
    try {
      // 로컬스토리지에서 네이버 API 키 가져오기
      const naverKey = localStorage.getItem("naverMapsApiKey")
      if (!naverKey) {
        throw new Error("네이버 지도 API 키가 설정되지 않았습니다.")
      }

      // 네이버 지도 API가 로드되었는지 확인
      if (!window.naver || !window.naver.maps || !window.naver.maps.Service) {
        // API가 로드되지 않은 경우 로드 시도
        console.warn("네이버 지도 API가 로드되지 않았습니다. 로드를 시도합니다.")

        // 5초 동안 API 로드 대기
        let attempts = 0
        const maxAttempts = 10

        while ((!window.naver || !window.naver.maps || !window.naver.maps.Service) && attempts < maxAttempts) {
          await new Promise((resolve) => setTimeout(resolve, 500))
          attempts++
        }

        // 최대 시도 횟수 후에도 로드되지 않으면 오류 발생
        if (!window.naver || !window.naver.maps || !window.naver.maps.Service) {
          throw new Error("네이버 지도 API 로드에 실패했습니다. API 키를 확인하거나 페이지를 새로고침해 주세요.")
        }
      }

      // 네이버 지도 API의 지오코딩 서비스 사용
      return new Promise((resolve, reject) => {
        window.naver.maps.Service.geocode(
          {
            query: query,
          },
          (status, response) => {
            if (status !== window.naver.maps.Service.Status.OK) {
              reject(new Error(`주소 검색에 실패했습니다. 상태 코드: ${status}`))
              return
            }

            if (!response || !response.v2 || !response.v2.addresses || response.v2.addresses.length === 0) {
              reject(new Error("검색 결과가 없습니다. 다른 주소를 입력해 보세요."))
              return
            }

            const result = response.v2.addresses[0]
            if (result) {
              resolve({
                lat: Number.parseFloat(result.y),
                lng: Number.parseFloat(result.x),
                address: result.roadAddress || result.jibunAddress,
              })
            } else {
              reject(new Error("검색 결과가 없습니다."))
            }
          },
        )
      })
    } catch (error) {
      console.error("주소 검색 오류:", error)
      throw error
    }
  }

  // 직접 API 요청 테스트
  async testApiConnection(): Promise<{ success: boolean; message: string }> {
    try {
      const response = await fetch(`${this.baseUrl}/health`)

      if (!response.ok) {
        return {
          success: false,
          message: `API 서버 연결 실패: ${response.status} ${response.statusText}`,
        }
      }

      return { success: true, message: "API 서버에 성공적으로 연결되었습니다." }
    } catch (error) {
      return {
        success: false,
        message: `API 서버 연결 오류: ${error.message}`,
      }
    }
  }
}

// 싱글톤 인스턴스 생성 및 내보내기
const apiService = new ApiService()
export default apiService
