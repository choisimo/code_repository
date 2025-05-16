"use client"

import { useState, useEffect } from "react"
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from "@/components/ui/card"
import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs"
import { Alert, AlertDescription, AlertTitle } from "@/components/ui/alert"
import { AlertCircle, CloudRain, Thermometer } from "lucide-react"
import ApiKeyManager from "./api-key-manager"
import getApiService from "./api-factory"

export default function WeatherController() {
  // API 서비스 가져오기
  const apiService = getApiService()

  // 상태 관리
  const [currentWeather, setCurrentWeather] = useState(null)
  const [forecast, setForecast] = useState([])
  const [loading, setLoading] = useState(false)
  const [error, setError] = useState("")
  const [apiKeysConfigured, setApiKeysConfigured] = useState(false)
  const [location, setLocation] = useState({
    lat: 37.5665, // 서울 기본 좌표
    lng: 126.978,
    address: "서울특별시",
  })

  // API 키 확인
  useEffect(() => {
    const weatherKey = localStorage.getItem("weatherApiKey")
    const useMock = localStorage.getItem("useMockApi") === "true"

    // 모의 API를 사용하거나 API 키가 있으면 날씨 데이터 로드
    setApiKeysConfigured(!!weatherKey || useMock)
    if (weatherKey || useMock) {
      fetchWeatherData()
    }
  }, [])

  // 날씨 데이터 가져오기
  const fetchWeatherData = async () => {
    try {
      setLoading(true)
      setError("")

      // API 연결 테스트
      const connectionTest = await apiService.testApiConnection()
      if (!connectionTest.success) {
        setError(`${connectionTest.message} 백엔드 서버가 실행 중인지 확인해주세요.`)
        setLoading(false)
        return
      }

      // 현재 날씨 가져오기
      try {
        const weatherData = await apiService.getWeather(location.lat, location.lng)
        setCurrentWeather(weatherData)
      } catch (error) {
        console.error("현재 날씨 데이터 가져오기 오류:", error)
        setError(`현재 날씨 정보를 가져오는데 실패했습니다: ${error.message}`)
        setLoading(false)
        return
      }

      // 날씨 예보 가져오기
      try {
        const forecastData = await apiService.getForecast(location.lat, location.lng, 7)
        setForecast(forecastData.dailyForecast || [])
      } catch (error) {
        console.error("날씨 예보 데이터 가져오기 오류:", error)
        // 현재 날씨는 성공했으므로 예보 오류는 경고만 표시
        console.warn("날씨 예보를 가져오는데 실패했습니다:", error.message)
      }

      setLoading(false)
    } catch (error) {
      console.error("날씨 데이터 가져오기 오류:", error)
      setError(error.message || "날씨 정보를 가져오는데 실패했습니다.")
      setLoading(false)
    }
  }

  // 날씨 아이콘 URL 가져오기
  const getWeatherIconUrl = (iconCode) => {
    if (!iconCode) return "/placeholder.svg?height=50&width=50"
    return `/icons/weather/${iconCode}.svg`
  }

  // 날짜 포맷팅
  const formatDate = (dateString) => {
    if (!dateString) return ""

    const date = new Date(dateString)
    const today = new Date()

    // 오늘인지 확인
    if (date.toDateString() === today.toDateString()) {
      return "오늘"
    }

    // 요일 반환
    const days = ["일", "월", "화", "수", "목", "금", "토"]
    return `${date.getMonth() + 1}/${date.getDate()} (${days[date.getDay()]})`
  }

  return (
    <div className="space-y-6">
      {!apiKeysConfigured && (
        <div className="mb-6">
          <Alert className="mb-4">
            <AlertCircle className="h-4 w-4" />
            <AlertTitle>API 키 설정 필요</AlertTitle>
            <AlertDescription>날씨 정보를 사용하기 위해 API 키를 설정해주세요.</AlertDescription>
          </Alert>
          <ApiKeyManager />
        </div>
      )}

      {apiKeysConfigured && (
        <Card>
          <CardHeader>
            <CardTitle className="flex items-center gap-2">
              <Thermometer className="h-5 w-5" />
              {location.address} 날씨
            </CardTitle>
            <CardDescription>{new Date().toLocaleDateString()} 기준 날씨 정보</CardDescription>
          </CardHeader>
          <CardContent>
            {loading ? (
              <div className="flex justify-center py-8">
                <div className="flex flex-col items-center">
                  <div className="w-8 h-8 border-4 border-t-blue-500 border-b-blue-500 rounded-full animate-spin"></div>
                  <p className="mt-2 text-sm text-gray-600">날씨 정보 로딩 중...</p>
                </div>
              </div>
            ) : error ? (
              <Alert variant="destructive">
                <AlertCircle className="h-4 w-4" />
                <AlertTitle>오류</AlertTitle>
                <AlertDescription>{error}</AlertDescription>
              </Alert>
            ) : (
              <Tabs defaultValue="current">
                <TabsList className="grid w-full grid-cols-2">
                  <TabsTrigger value="current">현재 날씨</TabsTrigger>
                  <TabsTrigger value="forecast">주간 예보</TabsTrigger>
                </TabsList>

                <TabsContent value="current" className="pt-4">
                  {currentWeather && (
                    <div className="flex flex-col md:flex-row items-center gap-6">
                      <div className="flex items-center gap-4">
                        <div className="text-6xl font-bold">{Math.round(currentWeather.temperature)}°C</div>
                      </div>

                      <div className="grid grid-cols-2 gap-4">
                        <div>
                          <p className="text-sm text-muted-foreground">습도</p>
                          <p className="text-lg">{currentWeather.humidity}%</p>
                        </div>
                        <div>
                          <p className="text-sm text-muted-foreground">강수확률</p>
                          <p className="text-lg">{currentWeather.rainProbability}%</p>
                        </div>
                        <div>
                          <p className="text-sm text-muted-foreground">풍속</p>
                          <p className="text-lg">{currentWeather.windSpeed} m/s</p>
                        </div>
                        <div>
                          <p className="text-sm text-muted-foreground">풍향</p>
                          <p className="text-lg">{currentWeather.windDirection}</p>
                        </div>
                      </div>
                    </div>
                  )}
                </TabsContent>

                <TabsContent value="forecast" className="pt-4">
                  {forecast.length > 0 ? (
                    <div className="grid grid-cols-2 md:grid-cols-4 lg:grid-cols-7 gap-4">
                      {forecast.map((day, index) => (
                        <div
                          key={index}
                          className="flex flex-col items-center p-3 rounded-lg border bg-card text-card-foreground shadow-sm"
                        >
                          <p className="font-medium">{formatDate(day.forecastTime)}</p>
                          <div className="my-2">
                            <CloudRain className="h-8 w-8 text-blue-500" />
                          </div>
                          <div className="flex gap-2">
                            <span className="text-red-500">{Math.round(day.temperatureMax)}°</span>
                            <span className="text-blue-500">{Math.round(day.temperatureMin)}°</span>
                          </div>
                          <p className="text-sm mt-1">{day.rainProbability}%</p>
                        </div>
                      ))}
                    </div>
                  ) : (
                    <p className="text-center py-4 text-muted-foreground">예보 데이터가 없습니다.</p>
                  )}
                </TabsContent>
              </Tabs>
            )}
          </CardContent>
        </Card>
      )}
    </div>
  )
}
