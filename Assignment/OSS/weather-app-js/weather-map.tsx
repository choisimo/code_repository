"use client"

import { useState, useEffect, useRef } from "react"
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from "@/components/ui/card"
import { Input } from "@/components/ui/input"
import { Button } from "@/components/ui/button"
import { Alert, AlertDescription, AlertTitle } from "@/components/ui/alert"
import { AlertCircle, MapPin, Search } from "lucide-react"
import ApiKeyManager from "./api-key-manager"
import getApiService from "./api-factory"

export default function WeatherMap() {
  // API 서비스 가져오기
  const apiService = getApiService()

  // 상태 관리
  const [mapLoaded, setMapLoaded] = useState(false)
  const [mapError, setMapError] = useState("")
  const [searchQuery, setSearchQuery] = useState("")
  const [selectedLocation, setSelectedLocation] = useState(null)
  const [weatherData, setWeatherData] = useState(null)
  const [loading, setLoading] = useState(false)
  const [apiKeysConfigured, setApiKeysConfigured] = useState(false)

  // 지도 및 마커 참조
  const mapRef = useRef(null)
  const mapInstanceRef = useRef(null)
  const markersRef = useRef([])
  const scriptRef = useRef(null) // 스크립트 요소 참조 추가

  // API 키 확인
  useEffect(() => {
    const naverKey = localStorage.getItem("naverMapsApiKey")
    const weatherKey = localStorage.getItem("weatherApiKey")
    const useMock = localStorage.getItem("useMockApi") === "true"

    setApiKeysConfigured((!!naverKey && !!weatherKey) || useMock)

    if (naverKey || useMock) {
      loadNaverMapsScript(naverKey)
    }

    // 컴포넌트 언마운트 시 스크립트 정리
    return () => {
      if (scriptRef.current && document.head.contains(scriptRef.current)) {
        try {
          document.head.removeChild(scriptRef.current)
        } catch (e) {
          console.warn("스크립트 제거 중 오류 발생:", e)
        }
      }
    }
  }, [])

  // 네이버 지도 스크립트 로드
  const loadNaverMapsScript = (clientId) => {
    // 모의 API 사용 여부 확인
    const useMock = localStorage.getItem("useMockApi") === "true"

    if (useMock) {
      // 모의 API 사용 시 지도 초기화 직접 수행
      setTimeout(() => {
        setMapLoaded(true)
        initializeMap()
      }, 500)
      return
    }

    // 이미 로드된 스크립트가 있는지 확인
    const existingScript = document.getElementById("naver-maps-script")
    if (existingScript) {
      // 이미 로드된 경우 스크립트 참조 저장
      scriptRef.current = existingScript

      // 이미 네이버 맵 객체가 있는지 확인
      if (window.naver && window.naver.maps && window.naver.maps.Service) {
        console.log("네이버 지도 API가 이미 로드되어 있습니다.")
        setMapLoaded(true)
        initializeMap()
      } else {
        // 스크립트는 있지만 아직 로드 중인 경우
        console.log("네이버 지도 API 스크립트가 있지만 아직 초기화되지 않았습니다. 로드 완료를 기다립니다.")

        // 로드 이벤트 리스너 추가
        existingScript.addEventListener("load", () => {
          console.log("네이버 지도 API 로드 완료")
          setMapLoaded(true)
          initializeMap()
        })

        // 타임아웃 설정 (10초 후에도 로드되지 않으면 오류 표시)
        setTimeout(() => {
          if (!window.naver || !window.naver.maps) {
            console.error("네이버 지도 API 로드 타임아웃")
            setMapError("네이버 지도 API 로드에 실패했습니다. API 키를 확인하거나 페이지를 새로고침해 주세요.")
          }
        }, 10000)
      }
      return
    }

    // 새 스크립트 생성
    console.log("네이버 지도 API 스크립트 로드 시작")
    const script = document.createElement("script")
    script.id = "naver-maps-script"
    script.src = `https://openapi.map.naver.com/openapi/v3/maps.js?ncpClientId=${clientId}&submodules=geocoder`
    script.async = true
    script.defer = true // defer 속성 추가

    // 스크립트 이벤트 핸들러
    script.onload = () => {
      console.log("네이버 지도 API 스크립트 로드 완료")

      // API 객체가 초기화될 때까지 짧게 대기
      setTimeout(() => {
        if (window.naver && window.naver.maps) {
          setMapLoaded(true)
          initializeMap()
        } else {
          setMapError("네이버 지도 API가 로드되었지만 초기화에 실패했습니다.")
        }
      }, 500)
    }

    script.onerror = (e) => {
      console.error("네이버 지도 API 로드 오류:", e)
      setMapError("네이버 지도 API를 로드하는데 실패했습니다. API 키를 확인해주세요.")
    }

    // 스크립트 참조 저장 및 DOM에 추가
    scriptRef.current = script
    document.head.appendChild(script)

    // 타임아웃 설정 (15초 후에도 로드되지 않으면 오류 표시)
    setTimeout(() => {
      if (!window.naver || !window.naver.maps) {
        console.error("네이버 지도 API 로드 타임아웃")
        setMapError(
          "네이버 지도 API 로드에 시간이 너무 오래 걸립니다. 네트워크 연결을 확인하거나 페이지를 새로고침해 주세요.",
        )
      }
    }, 15000)
  }

  // 지도 초기화
  const initializeMap = () => {
    // 모의 API 사용 여부 확인
    const useMock = localStorage.getItem("useMockApi") === "true"

    if (useMock) {
      // 모의 지도 UI 표시
      if (mapRef.current) {
        mapRef.current.innerHTML = `
        <div class="flex items-center justify-center h-full bg-gray-100">
          <div class="text-center p-4">
            <div class="text-4xl mb-2">🗺️</div>
            <h3 class="text-lg font-semibold mb-1">모의 지도</h3>
            <p class="text-sm text-gray-600">모의 API 모드에서는 실제 지도가 표시되지 않습니다.</p>
            <p class="text-sm text-gray-600 mt-2">주소 검색을 통해 날씨 정보를 확인할 수 있습니다.</p>
          </div>
        </div>
      `
      }
      setMapError("")
      return
    }

    // 지도 컨테이너가 없거나 네이버 맵 API가 로드되지 않은 경우
    if (!mapRef.current) {
      console.warn("지도 컨테이너가 없습니다.")
      return
    }

    if (!window.naver || !window.naver.maps) {
      console.error("네이버 지도 API가 로드되지 않았습니다.")
      setMapError("네이버 지도 API가 로드되지 않았습니다. API 키를 확인하거나 페이지를 새로고침해 주세요.")
      return
    }

    // 이미 지도 인스턴스가 있는 경우 재사용
    if (mapInstanceRef.current) {
      console.log("지도 인스턴스가 이미 존재합니다.")
      return
    }

    try {
      console.log("지도 인스턴스 생성 시작")
      // 서울 좌표 (기본값)
      const defaultCenter = new window.naver.maps.LatLng(37.5665, 126.978)

      // 지도 생성
      mapInstanceRef.current = new window.naver.maps.Map(mapRef.current, {
        center: defaultCenter,
        zoom: 10,
        zoomControl: true,
        zoomControlOptions: {
          position: window.naver.maps.Position.TOP_RIGHT,
        },
      })

      // 지도 클릭 이벤트
      window.naver.maps.Event.addListener(mapInstanceRef.current, "click", (e) => {
        const latlng = e.coord

        // 클릭한 위치에 마커 생성
        addMarker(latlng)

        // 클릭한 위치의 날씨 정보 가져오기
        fetchWeatherData(latlng.lat(), latlng.lng())
      })

      console.log("지도 인스턴스 생성 완료")
      setMapError("")
    } catch (error) {
      console.error("지도 초기화 오류:", error)
      setMapError("지도를 초기화하는데 실패했습니다: " + error.message)
    }
  }

  // 마커 추가
  const addMarker = (latlng) => {
    // 모의 API 사용 여부 확인
    const useMock = localStorage.getItem("useMockApi") === "true"
    if (useMock) return

    if (!mapInstanceRef.current || !window.naver) return

    // 기존 마커 제거
    clearMarkers()

    // 새 마커 생성
    const marker = new window.naver.maps.Marker({
      position: latlng,
      map: mapInstanceRef.current,
    })

    markersRef.current.push(marker)

    // 지도 중심 이동
    mapInstanceRef.current.setCenter(latlng)
  }

  // 마커 제거
  const clearMarkers = () => {
    // 모의 API 사용 여부 확인
    const useMock = localStorage.getItem("useMockApi") === "true"
    if (useMock) return

    if (markersRef.current.length > 0) {
      markersRef.current.forEach((marker) => marker.setMap(null))
      markersRef.current = []
    }
  }

  // 주소 검색
  const searchLocation = async () => {
    if (!searchQuery.trim()) return

    try {
      setLoading(true)
      setMapError("")

      // 모의 API 사용 여부 확인
      const useMock = localStorage.getItem("useMockApi") === "true"

      // 실제 API 모드에서 네이버 지도 API 로드 여부 확인
      if (!useMock && (!window.naver || !window.naver.maps || !window.naver.maps.Service)) {
        throw new Error("네이버 지도 API가 로드되지 않았습니다. API 키를 확인하거나 페이지를 새로고침해 주세요.")
      }

      // 주소 검색
      const result = await apiService.searchAddress(searchQuery)

      if (!useMock && window.naver && window.naver.maps && mapInstanceRef.current) {
        // 실제 네이버 지도 API 사용 시
        const latlng = new window.naver.maps.LatLng(result.lat, result.lng)

        // 검색 결과 위치에 마커 추가
        addMarker(latlng)

        // 지도 줌 레벨 조정
        mapInstanceRef.current.setZoom(13)
      }

      // 검색 결과 위치의 날씨 정보 가져오기
      fetchWeatherData(result.lat, result.lng)

      setLoading(false)
    } catch (error) {
      setLoading(false)
      setMapError("주소 검색 중 오류가 발생했습니다: " + error.message)
    }
  }

  // 날씨 데이터 가져오기
  const fetchWeatherData = async (lat, lng) => {
    try {
      setLoading(true)

      // API 연결 테스트
      const connectionTest = await apiService.testApiConnection()
      if (!connectionTest.success) {
        setMapError(`${connectionTest.message} 백엔드 서버가 실행 중인지 확인해주세요.`)
        setLoading(false)
        return
      }

      try {
        // 날씨 데이터 가져오기
        const data = await apiService.getWeather(lat, lng)
        setWeatherData(data)
        setSelectedLocation({ lat, lng, address: data.locationName || "선택한 위치" })
      } catch (error) {
        console.error("날씨 데이터 가져오기 오류:", error)
        setMapError(`날씨 정보를 가져오는데 실패했습니다: ${error.message}`)
      } finally {
        setLoading(false)
      }
    } catch (error) {
      console.error("날씨 데이터 가져오기 오류:", error)
      setMapError(`날씨 정보를 가져오는데 실패했습니다: ${error.message}`)
      setLoading(false)
    }
  }

  // 컴포넌트 언마운트 시 정리
  useEffect(() => {
    return () => {
      // 지도 인스턴스 정리
      if (mapInstanceRef.current) {
        // 이벤트 리스너 제거 (필요한 경우)
        if (window.naver && window.naver.maps) {
          try {
            window.naver.maps.Event.clearListeners(mapInstanceRef.current, "click")
          } catch (e) {
            console.warn("지도 이벤트 리스너 제거 중 오류 발생:", e)
          }
        }

        // 마커 정리
        clearMarkers()

        // 지도 인스턴스 참조 제거
        mapInstanceRef.current = null
      }
    }
  }, [])

  return (
    <div className="space-y-6">
      {!apiKeysConfigured && (
        <div className="mb-6">
          <Alert className="mb-4">
            <AlertCircle className="h-4 w-4" />
            <AlertTitle>API 키 설정 필요</AlertTitle>
            <AlertDescription>네이버 지도와 날씨 정보를 사용하기 위해 API 키를 설정해주세요.</AlertDescription>
          </Alert>
          <ApiKeyManager />
        </div>
      )}

      {apiKeysConfigured && (
        <>
          <Card>
            <CardHeader>
              <CardTitle className="flex items-center gap-2">
                <MapPin className="h-5 w-5" />
                날씨 지도
              </CardTitle>
              <CardDescription>
                지도에서 위치를 선택하거나 주소를 검색하여 해당 지역의 날씨를 확인하세요.
              </CardDescription>
            </CardHeader>
            <CardContent>
              <div className="flex flex-col gap-4">
                <div className="flex gap-2">
                  <Input
                    placeholder="주소 검색 (예: 서울시 강남구)"
                    value={searchQuery}
                    onChange={(e) => setSearchQuery(e.target.value)}
                    onKeyDown={(e) => e.key === "Enter" && searchLocation()}
                  />
                  <Button onClick={searchLocation} disabled={loading}>
                    <Search className="h-4 w-4 mr-2" />
                    검색
                  </Button>
                </div>

                {mapError && (
                  <Alert variant="destructive">
                    <AlertCircle className="h-4 w-4" />
                    <AlertTitle>오류</AlertTitle>
                    <AlertDescription>{mapError}</AlertDescription>
                  </Alert>
                )}

                <div
                  ref={mapRef}
                  className="w-full h-[400px] rounded-md border border-gray-200 bg-gray-50"
                  style={{ position: "relative" }}
                >
                  {!mapLoaded && !mapError && (
                    <div className="absolute inset-0 flex items-center justify-center bg-gray-100 bg-opacity-80">
                      <div className="flex flex-col items-center">
                        <div className="w-8 h-8 border-4 border-t-blue-500 border-b-blue-500 rounded-full animate-spin"></div>
                        <p className="mt-2 text-sm text-gray-600">지도 로딩 중...</p>
                      </div>
                    </div>
                  )}
                </div>
              </div>
            </CardContent>
          </Card>

          {weatherData && (
            <Card>
              <CardHeader>
                <CardTitle>{selectedLocation?.address} 날씨</CardTitle>
                <CardDescription>{new Date().toLocaleDateString()} 기준 날씨 정보</CardDescription>
              </CardHeader>
              <CardContent>
                <div className="flex flex-col md:flex-row gap-6">
                  <div className="flex items-center gap-4">
                    <div className="text-5xl font-bold">{Math.round(weatherData.temperature)}°C</div>
                    <div className="text-lg">{weatherData.weatherCondition}</div>
                  </div>

                  <div className="grid grid-cols-2 gap-4">
                    <div>
                      <p className="text-sm text-muted-foreground">습도</p>
                      <p className="text-lg">{weatherData.humidity}%</p>
                    </div>
                    <div>
                      <p className="text-sm text-muted-foreground">강수확률</p>
                      <p className="text-lg">{weatherData.rainProbability}%</p>
                    </div>
                    <div>
                      <p className="text-sm text-muted-foreground">풍속</p>
                      <p className="text-lg">{weatherData.windSpeed} m/s</p>
                    </div>
                    <div>
                      <p className="text-sm text-muted-foreground">풍향</p>
                      <p className="text-lg">{weatherData.windDirection}</p>
                    </div>
                  </div>
                </div>
              </CardContent>
            </Card>
          )}
        </>
      )}
    </div>
  )
}
