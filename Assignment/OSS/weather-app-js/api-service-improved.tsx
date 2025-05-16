"use client"

// 개선된 API 요청을 처리하는 서비스 클래스
class ApiService {
  // 기본 API URL
  private baseUrl = "/api"

  // HTML 감지를 위한 정규식 패턴
  private htmlPattern = /<\s*(!DOCTYPE|html|head|body|div|p|h[1-6])/i

  // 응답 유효성 검사
  private async validateResponse(response: Response): Promise<any> {
    // 1. 응답 상태 확인
    if (!response.ok) {
      // 응답 본문 가져오기
      const responseText = await response.text()

      // HTML 응답 감지 (Content-Type 헤더와 본문 내용 모두 확인)
      const contentType = response.headers.get("content-type") || ""
      const isHtmlResponse = contentType.includes("text/html") || this.htmlPattern.test(responseText)

      if (isHtmlResponse) {
        console.error("HTML 응답 감지:", responseText.substring(0, 200) + "...")
        throw new Error("서버에서 HTML 응답을 반환했습니다. API 엔드포인트 또는 서버 설정을 확인해주세요.")
      }

      // JSON 파싱 시도
      try {
        const errorData = JSON.parse(responseText)
        throw new Error(errorData?.message || errorData?.error || `API 응답 오류: ${response.status}`)
      } catch (e) {
        // JSON 파싱 실패 시 원본 텍스트 사용
        throw new Error(`API 응답 오류 (${response.status}): ${responseText.substring(0, 100)}`)
      }
    }

    // 2. 응답 타입 확인
    const contentType = response.headers.get("content-type") || ""
    if (!contentType.includes("application/json")) {
      // 응답 본문 가져오기
      const responseText = await response.text()

      // HTML 감지
      if (this.htmlPattern.test(responseText)) {
        console.error("HTML 응답 감지 (성공 상태코드):", responseText.substring(0, 200) + "...")
        throw new Error("서버가 성공 상태코드와 함께 HTML을 반환했습니다. API 엔드포인트를 확인해주세요.")
      }

      // JSON 파싱 시도 (Content-Type이 잘못 설정된 경우를 위한 대비책)
      try {
        return JSON.parse(responseText)
      } catch (e) {
        console.error("응답을 JSON으로 파싱할 수 없습니다:", responseText.substring(0, 200) + "...")
        throw new Error(`예상치 못한 응답 타입: ${contentType}. 응답을 JSON으로 파싱할 수 없습니다.`)
      }
    }

    // 3. JSON 응답 파싱
    try {
      return await response.json()
    } catch (e) {
      const responseText = await response.text()
      console.error("JSON 파싱 오류:", e, "응답:", responseText.substring(0, 200) + "...")
      throw new Error("응답을 JSON으로 파싱하는 중 오류가 발생했습니다.")
    }
  }

  // 날씨 데이터 가져오기
  async getWeather(lat: number, lng: number): Promise<any> {
    try {
      // 로컬스토리지에서 API 키 가져오기
      const apiKey = localStorage.getItem("weatherApiKey")
      if (!apiKey) {
        throw new Error("날씨 API 키가 설정되지 않았습니다.")
      }

      // API 요청 준비
      const url = `${this.baseUrl}/weather?lat=${lat}&lng=${lng}&apiKey=${apiKey}`

      // 요청 로깅
      console.log(`날씨 데이터 요청: ${url}`)

      // API 요청 실행
      const response = await fetch(url, {
        method: "GET",
        headers: {
          Accept: "application/json",
          "X-Requested-With": "XMLHttpRequest", // AJAX 요청임을 명시
        },
      })

      // 응답 검증 및 파싱
      return await this.validateResponse(response)
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

      // API 요청 준비
      const url = `${this.baseUrl}/forecast?lat=${lat}&lng=${lng}&days=${days}&apiKey=${apiKey}`

      // 요청 로깅
      console.log(`날씨 예보 데이터 요청: ${url}`)

      // API 요청 실행
      const response = await fetch(url, {
        method: "GET",
        headers: {
          Accept: "application/json",
          "X-Requested-With": "XMLHttpRequest", // AJAX 요청임을 명시
        },
      })

      // 응답 검증 및 파싱
      return await this.validateResponse(response)
    } catch (error) {
      console.error("날씨 예보 가져오기 오류:", error)
      throw error
    }
  }

  // API 연결 테스트 (개선된 버전)
  async testApiConnection(): Promise<{ success: boolean; message: string; details?: string }> {
    try {
      // 헬스체크 엔드포인트 호출
      const response = await fetch(`${this.baseUrl}/health`, {
        method: "GET",
        headers: {
          Accept: "application/json",
          "X-Requested-With": "XMLHttpRequest",
        },
      })

      // 응답 상태 확인
      if (!response.ok) {
        const responseText = await response.text()
        const contentType = response.headers.get("content-type") || ""
        const isHtmlResponse = contentType.includes("text/html") || this.htmlPattern.test(responseText)

        if (isHtmlResponse) {
          return {
            success: false,
            message: "API 서버가 HTML 응답을 반환했습니다. 서버 설정을 확인해주세요.",
            details: responseText.substring(0, 200) + "...",
          }
        }

        return {
          success: false,
          message: `API 서버 연결 실패: ${response.status} ${response.statusText}`,
          details: responseText.substring(0, 200),
        }
      }

      // 응답 타입 확인
      const contentType = response.headers.get("content-type") || ""
      if (!contentType.includes("application/json")) {
        const responseText = await response.text()
        return {
          success: false,
          message: `API 서버가 JSON이 아닌 응답을 반환했습니다: ${contentType}`,
          details: responseText.substring(0, 200),
        }
      }

      // 성공 응답
      return {
        success: true,
        message: "API 서버에 성공적으로 연결되었습니다.",
      }
    } catch (error) {
      return {
        success: false,
        message: `API 서버 연결 오류: ${error.message}`,
        details: error.stack,
      }
    }
  }

  // CORS 테스트
  async testCorsConfiguration(): Promise<{ success: boolean; message: string }> {
    try {
      // OPTIONS 요청으로 CORS 설정 테스트
      const response = await fetch(`${this.baseUrl}/weather`, {
        method: "OPTIONS",
        headers: {
          Origin: window.location.origin,
          "Access-Control-Request-Method": "GET",
          "Access-Control-Request-Headers": "Content-Type, X-Requested-With",
        },
      })

      // CORS 헤더 확인
      const allowOrigin = response.headers.get("Access-Control-Allow-Origin")
      const allowMethods = response.headers.get("Access-Control-Allow-Methods")
      const allowHeaders = response.headers.get("Access-Control-Allow-Headers")

      if (!allowOrigin || !allowMethods || !allowHeaders) {
        return {
          success: false,
          message: "서버의 CORS 설정이 올바르지 않습니다. 서버 설정을 확인해주세요.",
        }
      }

      return {
        success: true,
        message: "CORS 설정이 올바르게 구성되어 있습니다.",
      }
    } catch (error) {
      return {
        success: false,
        message: `CORS 테스트 중 오류 발생: ${error.message}`,
      }
    }
  }
}

// 싱글톤 인스턴스 생성 및 내보내기
const apiService = new ApiService()
export default apiService
