"use client"

// 모의 API 데이터 및 서비스
class MockApiService {
  // 모의 응답 지연 시간 (ms)
  private getDelay() {
    return Number.parseInt(localStorage.getItem("mockApiDelay") || "500", 10)
  }

  // 모의 날씨 데이터 생성
  private generateMockWeatherData(lat: number, lng: number) {
    // 위치에 따라 약간 다른 데이터 생성
    const seed = (lat * 10 + lng) % 100
    const temp = Math.round(15 + (seed % 20))
    const humidity = Math.round(40 + (seed % 40))
    const rainProb = Math.round(seed % 100)
    const windSpeed = Math.round((5 + (seed % 10)) * 10) / 10

    // 날씨 상태 결정
    let weatherCondition = "맑음"
    if (rainProb > 70) weatherCondition = "비"
    else if (rainProb > 40) weatherCondition = "흐림"
    else if (rainProb > 20) weatherCondition = "구름 조금"

    return {
      temperature: temp,
      rainProbability: rainProb,
      windSpeed: windSpeed,
      windDirection: this.getWindDirection(seed),
      humidity: humidity,
      weatherCondition: weatherCondition,
      timestamp: new Date().toISOString(),
      locationName: this.getLocationName(lat, lng),
      latitude: lat,
      longitude: lng,
    }
  }

  // 모의 예보 데이터 생성
  private generateMockForecastData(lat: number, lng: number, days: number) {
    const dailyForecast = []
    const now = new Date()

    for (let i = 0; i < days; i++) {
      const date = new Date(now)
      date.setDate(date.getDate() + i)

      // 날짜에 따라 약간 다른 데이터 생성
      const seed = (lat * 10 + lng + i) % 100
      const tempMax = Math.round(18 + (seed % 15))
      const tempMin = Math.round(tempMax - 5 - (seed % 5))
      const rainProb = Math.round((seed * i) % 100)

      dailyForecast.push({
        forecastTime: date.toISOString(),
        temperature: (tempMax + tempMin) / 2,
        temperatureMax: tempMax,
        temperatureMin: tempMin,
        rainProbability: rainProb,
        weatherMain: this.getWeatherMain(rainProb),
        weatherDescription: this.getWeatherDescription(rainProb),
        weatherIcon: this.getWeatherIcon(rainProb),
        humidity: Math.round(40 + (seed % 40)),
        windSpeed: Math.round((3 + (seed % 8)) * 10) / 10,
        windDirection: Math.round(seed * 3.6) % 360,
      })
    }

    return {
      locationId: `loc_${lat}_${lng}`,
      locationName: this.getLocationName(lat, lng),
      latitude: lat,
      longitude: lng,
      dailyForecast: dailyForecast,
      forecastDays: days,
      forecastTime: now.toISOString(),
      provider: "모의 데이터 제공자",
      lastUpdated: now.toISOString(),
    }
  }

  // 풍향 텍스트 반환
  private getWindDirection(seed: number) {
    const directions = ["북", "북동", "동", "남동", "남", "남서", "서", "북서"]
    return directions[seed % directions.length]
  }

  // 위치에 따른 지역명 반환
  private getLocationName(lat: number, lng: number) {
    // 서울 근처
    if (lat > 37.4 && lat < 37.7 && lng > 126.8 && lng < 127.2) {
      return "서울특별시"
    }
    // 부산 근처
    else if (lat > 35.0 && lat < 35.3 && lng > 128.9 && lng < 129.3) {
      return "부산광역시"
    }
    // 대전 근처
    else if (lat > 36.2 && lat < 36.5 && lng > 127.2 && lng < 127.5) {
      return "대전광역시"
    }
    // 기타 지역
    else {
      return "선택한 위치"
    }
  }

  // 강수확률에 따른 날씨 상태
  private getWeatherMain(rainProb: number) {
    if (rainProb > 70) return "비"
    else if (rainProb > 40) return "흐림"
    else if (rainProb > 20) return "구름 조금"
    else return "맑음"
  }

  // 강수확률에 따른 날씨 설명
  private getWeatherDescription(rainProb: number) {
    if (rainProb > 70) return "비가 내릴 확률이 높습니다"
    else if (rainProb > 40) return "흐린 날씨가 예상됩니다"
    else if (rainProb > 20) return "구름이 조금 있는 날씨입니다"
    else return "맑은 날씨가 예상됩니다"
  }

  // 강수확률에 따른 날씨 아이콘
  private getWeatherIcon(rainProb: number) {
    if (rainProb > 70) return "09d"
    else if (rainProb > 40) return "04d"
    else if (rainProb > 20) return "03d"
    else return "01d"
  }

  // 모의 지연 함수
  private async delay(ms: number) {
    return new Promise((resolve) => setTimeout(resolve, ms))
  }

  // 날씨 데이터 가져오기
  async getWeather(lat: number, lng: number): Promise<any> {
    await this.delay(this.getDelay())
    return this.generateMockWeatherData(lat, lng)
  }

  // 날씨 예보 가져오기
  async getForecast(lat: number, lng: number, days = 5): Promise<any> {
    await this.delay(this.getDelay())
    return this.generateMockForecastData(lat, lng, days)
  }

  // 주소 검색 (지오코딩)
  async searchAddress(query: string): Promise<any> {
    await this.delay(this.getDelay())

    // 간단한 주소 매핑
    const addressMap = {
      서울: { lat: 37.5665, lng: 126.978, address: "서울특별시" },
      부산: { lat: 35.1796, lng: 129.0756, address: "부산광역시" },
      대전: { lat: 36.3504, lng: 127.3845, address: "대전광역시" },
      대구: { lat: 35.8714, lng: 128.6014, address: "대구광역시" },
      광주: { lat: 35.1595, lng: 126.8526, address: "광주광역시" },
      인천: { lat: 37.4563, lng: 126.7052, address: "인천광역시" },
      울산: { lat: 35.5384, lng: 129.3114, address: "울산광역시" },
      세종: { lat: 36.48, lng: 127.289, address: "세종특별자치시" },
      제주: { lat: 33.4996, lng: 126.5312, address: "제주특별자치도" },
      강남: { lat: 37.4959854, lng: 127.0664091, address: "서울특별시 강남구" },
      강북: { lat: 37.6396099, lng: 127.0256575, address: "서울특별시 강북구" },
      강서: { lat: 37.5657617, lng: 126.8226561, address: "서울특별시 강서구" },
      강동: { lat: 37.5492077, lng: 127.1464824, address: "서울특별시 강동구" },
      마포: { lat: 37.5622906, lng: 126.9087803, address: "서울특별시 마포구" },
      용산: { lat: 37.5311008, lng: 126.9810742, address: "서울특별시 용산구" },
      종로: { lat: 37.5990998, lng: 126.9861493, address: "서울특별시 종로구" },
      중구: { lat: 37.5579452, lng: 126.9941904, address: "서울특별시 중구" },
      송파: { lat: 37.5048534, lng: 127.1144822, address: "서울특별시 송파구" },
      영등포: { lat: 37.5263891, lng: 126.8957717, address: "서울특별시 영등포구" },
    }

    // 검색어에 해당하는 주소 찾기
    for (const [key, value] of Object.entries(addressMap)) {
      if (query.includes(key)) {
        return value
      }
    }

    // 검색어에 숫자가 포함된 경우 (상세 주소로 간주)
    const hasNumber = /\d/.test(query)
    if (hasNumber) {
      // 검색어에서 주요 지역명 추출 시도
      for (const [key, value] of Object.entries(addressMap)) {
        if (query.includes(key)) {
          // 상세 주소를 포함한 결과 반환
          return {
            ...value,
            address: `${value.address} ${query.replace(key, "").trim()}`,
          }
        }
      }
    }

    // 기본값 반환 (서울시 중심)
    return {
      lat: 37.5665,
      lng: 126.978,
      address: `검색 결과: ${query}`,
    }
  }

  // API 연결 테스트
  async testApiConnection(): Promise<{ success: boolean; message: string }> {
    await this.delay(this.getDelay())
    return {
      success: true,
      message: "모의 API 서버에 성공적으로 연결되었습니다.",
    }
  }
}

// 싱글톤 인스턴스 생성 및 내보내기
const mockApiService = new MockApiService()
export default mockApiService
